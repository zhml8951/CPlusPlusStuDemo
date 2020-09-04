#include "pch.h"
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include "ScCommon.h"

namespace sc
{
	void SanitizeRects(std::vector<ImageRect>& rects, const Image& img)
	{
		for (auto& r : rects) {
			if (r.right > Width(img))
				r.right = Width(img);
			if (r.bottom > Height(img))
				r.bottom = Height(img);
		}
	}

	Monitor CreateMonitor(const int index, const int id, const int h, const int w, const int ox, const int oy,
	                      const std::string& n, const float scale)
	{
		Monitor ret{};
		ret.index = index;
		ret.id = id;
		assert(n.size() + 1 < sizeof(ret.name));
		memcpy(ret.name, n.c_str(), n.size() + 1);
		ret.original_offset_x = ret.offset_x = ox;
		ret.original_offset_y = ret.offset_y = oy;
		ret.original_width = ret.width = w;
		ret.original_height = ret.height = h;
		ret.scaling = scale;
		return ret;
	}

	Monitor CreateMonitor(const int index, const int id, const int adapter, const int h, int const w,
	                      int const ox, int const oy, const std::string& n, float const scale)
	{
		auto ret = CreateMonitor(index, id, h, w, ox, oy, n, scale);
		ret.adapter = adapter;
		return ret;
	}

	bool IsMonitorInsideBounds(const std::vector<Monitor>& monitors, const Monitor& monitor)
	{
		using namespace std;
		auto total_width = 0;
		for (auto& m : monitors) {
			total_width += Width(m);
		}
		if (find_if(begin(monitors), end(monitors), [&](auto& m) { return m.id == monitor.id; }) == end(monitors)) {
			return false;
		}

		auto& real_monitor = monitors[Index(monitor)];

		if (Height(real_monitor) < Height(monitor) || total_width < Width(monitor) + OffsetX(monitor) ||
			Width(monitor) > Width(real_monitor)) {
			return false;
		}
		else if (Height(real_monitor) == Height(monitor) && Width(real_monitor) == Width(monitor) &&
			(OffsetX(real_monitor) != OffsetX(monitor) || OffsetY(real_monitor) != OffsetY(monitor))) {
			return false;
		}
		return true;
	}

	Image CreateImage(const ImageRect& img_rect, int row_padding, const ImageBgra* data)
	{
		Image img;
		img.bounds = img_rect;
		img.data = data;
		img.bytes_to_next_row = row_padding;
		return img;
	}

	template <typename Block>
	class BitMap
	{
		static_assert(std::is_unsigned<Block>::value);
		static const size_t bits_per_block_ = sizeof(Block) * 8;

	public:
		BitMap(const size_t width, const size_t height) : width_(width), height_(height),
		                                                  blocks_((width * height) / bits_per_block_ + 1, 0) { }

		bool get(size_t x, size_t y) const;
		void set(size_t x, size_t y);

		size_t width() const
		{
			return width_;
		}

		size_t height() const
		{
			return height_;
		}

		friend std::string get_info(const BitMap<Block>& b)
		{
			auto block_size = b.blocks_.size();
			std::string ret{"bit_map info: <block_size>="};
			ret += std::to_string(block_size) + ", <bits_per_block>" + std::to_string(bits_per_block_);
			return ret;
		}

	private:
		size_t width_;
		size_t height_;
		std::vector<Block> blocks_;
	};

	template <typename Block>
	bool BitMap<Block>::get(const size_t x, const size_t y) const
	{
		const auto index = x * width_ + y;
		const auto n_block = index / bits_per_block_;
		const auto n_bit = index % bits_per_block_;
		return blocks_[n_block] & (Block(1) << n_bit);
	}

	template <typename Block>
	void BitMap<Block>::set(const size_t x, const size_t y)
	{
		const auto index = x * width_ + y;
		const auto n_block = index / bits_per_block_;
		const auto n_bit = index % bits_per_block_;
		blocks_[n_block] |= (Block(1) << n_bit);
	}

	static void Merge(std::vector<ImageRect>& rects)
	{
		if (rects.size() <= 2) {
			return;
		}
		std::vector<ImageRect> out_rects;
		out_rects.reserve(rects.size());
		out_rects.push_back(rects[0]);
		for (size_t i = 1; i < rects.size(); i++) {
			if (out_rects.back().right == rects[i].left &&
				out_rects.back().bottom == rects[i].bottom) {
				out_rects.back().right = rects[i].right;
			}
			else {
				out_rects.push_back(rects[i]);
			}
		}
		if (out_rects.size() <= 2) {
			rects = std::move(out_rects);
			return;
		}
		rects.clear();
		for (auto& ot : out_rects) {
			auto found = std::find_if(rects.rbegin(), rects.rend(), [=](const ImageRect& rect) {
				return rect.bottom == ot.top && rect.left == ot.left &&
					rect.right == ot.right;
			});

			if (found == rects.rend()) {
				rects.push_back(ot);
			}
			else { found->bottom = ot.bottom; }
		}
	}

	constexpr auto kMaxDist = 256;

	static std::vector<ImageRect> GetRects(const BitMap<uint64_t>& map)
	{
		std::vector<ImageRect> rects;
		rects.reserve(map.width() * map.height());
		for (decltype(map.height()) y = 0; y < map.height(); ++y) {
			for (decltype(map.width()) x = 0; x < map.width(); ++x) {
				if (map.get(y, x)) {
					ImageRect rect;
					rect.top = y * kMaxDist;
					rect.bottom = (y + 1) * kMaxDist;
					rect.left = x * kMaxDist;
					rect.right = (x + 1) * kMaxDist;

					rects.push_back(rect);
				}
			}
		}
		return rects;
	}

	std::vector<ImageRect> GetDifs(const Image& old_img, const Image& new_img)
	{
		auto old_ptr = reinterpret_cast<const int*>(StartSrc(old_img));
		auto new_ptr = reinterpret_cast<const int*>(StartSrc(new_img));
		const auto width = sc::Width(new_img);
		const auto height = sc::Height(new_img);

		const auto width_chunks = width / kMaxDist;
		const auto height_chunks = height / kMaxDist;

		const auto line_rem = width % kMaxDist;
		const auto bottom_rem = height % kMaxDist;

		BitMap<uint64_t> changes{static_cast<size_t>(width_chunks) + 1, static_cast<size_t>(height_chunks) + 1};

		const auto compare = [&](size_t x, size_t y, size_t n_pixels) {
			if (!changes.get(x, y)) {
				if (memcmp(old_ptr, new_ptr, n_pixels * sizeof(int))) {
					changes.set(x, y);
				}
			}
			old_ptr += n_pixels;
			new_ptr += n_pixels;
		};

		for (auto x = 0; x < height_chunks; ++x) {
			for (auto i = 0; i < kMaxDist; ++i) {
				for (auto y = 0; y < width_chunks; ++y) {
					compare(x, y, kMaxDist);
				}
				compare(x, width_chunks, line_rem);
			}
		}

		for (auto i = 0; i < bottom_rem; ++i) {
			for (auto y = 0; y < width_chunks; ++y) {
				compare(height_chunks, y, kMaxDist);
			}
			compare(height_chunks, width_chunks, line_rem);
		}
		auto rects = GetRects(changes);
		Merge(rects);
		SanitizeRects(rects, new_img);
		return rects;
	}
}

//int main(int argc, char* argv[])
//{
//	sc::BitMap<uint8_t> bit_map01(33, 33);
//	bit_map01.set(22, 22);
//	auto geted = bit_map01.get(22, 22);
//	std::cout << std::boolalpha << geted << "\n";
//	auto info = get_info(bit_map01);
//	std::cout << "info: " << info << "\n";
//}
