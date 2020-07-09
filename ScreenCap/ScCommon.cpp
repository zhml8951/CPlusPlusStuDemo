#include "pch.h"
#include "ScCommon.h"
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

namespace sc
{
	void sanitize_rects(std::vector<ImageRect>& rects, const Image& img)
	{
		for (auto& r : rects) {
			if (r.right_i > width(img))
				r.right_i = width(img);
			if (r.bottom_i > height(img))
				r.bottom_i = height(img);
		}
	}

	template <typename Block>
	class BitMap
	{
		static_assert(std::is_unsigned<Block>::value);
		static const size_t bits_per_block_ = sizeof(Block) * 8;

	public:
		BitMap(const size_t width, const size_t height) :
			width_(width), height_(height), blocks_((width * height) / bits_per_block_ + 1, 0)
		{
		}

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
	bool BitMap<Block>::get(size_t x, size_t y) const
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

	static void merge(std::vector<ImageRect>& rects)
	{
		if (rects.size() <= 2) {
			return;
		}
		std::vector<ImageRect> out_rects;
		out_rects.reserve(rects.size());
		out_rects.push_back(rects[0]);
		for (size_t i = 1; i < rects.size(); i++) {
			if (out_rects.back().right_i == rects[i].left_i && out_rects.back().bottom_i == rects[i].bottom_i) {
				out_rects.back().right_i = rects[i].right_i;
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
			auto found = std::find_if(rects.rbegin(), rects.rend(), [=](const ImageRect& rect)
			{
				return rect.bottom_i == ot.top_i && rect.left_i == ot.left_i && rect.right_i == ot.right_i;
			});

			if (found == rects.rend()) {
				rects.push_back(ot);
			}
			else { found->bottom_i = ot.bottom_i; }
		}
	}

	constexpr auto kMaxDist = 256;

	static std::vector<ImageRect> get_rects(const BitMap<uint64_t>& map)
	{
		std::vector<ImageRect> rects;
		rects.reserve(map.width() * map.height());
		for (decltype(map.height()) y = 0; y < map.height(); ++y) {
			for (decltype(map.width()) x = 0; x < map.width(); ++x) {
				if (map.get(y, x)) {
					ImageRect rect;
					rect.top_i = y * kMaxDist;
					rect.bottom_i = (y + 1) * kMaxDist;
					rect.left_i = x * kMaxDist;
					rect.right_i = (x + 1) * kMaxDist;

					rects.push_back(rect);
				}
			}
		}
		return rects;
	}

	std::vector<ImageRect> get_difs(const Image& old_image, const Image& new_image)
	{
		auto old_ptr = reinterpret_cast<const int*>(start_src(old_image));
		auto new_ptr = reinterpret_cast<const int*>(start_src(new_image));
		const auto width = sc::width(new_image);
		const auto height = sc::height(new_image);

		const auto width_chunks = width / kMaxDist;
		const auto height_chunks = height / kMaxDist;

		const auto line_rem = width % kMaxDist;
		const auto bottom_rem = height % kMaxDist;

		BitMap<uint64_t> changes{static_cast<size_t>(width_chunks) + 1, static_cast<size_t>(height_chunks) + 1};

		const auto compare = [&](size_t x, size_t y, size_t n_pixels)
		{
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
		auto rects = get_rects(changes);
		merge(rects);
		sanitize_rects(rects, new_image);
		return rects;
	}

	Monitor create_monitor(const int index, const int id, const int h, const int w, const int ox, const int oy,
	                       const std::string& s, const float scaling)
	{
		Monitor ret{};
		ret.index_i = index;
		ret.id_i = id;
		assert(s.size() +1 < sizeof(ret.name_c));
		memcpy(ret.name_c, s.c_str(), s.size() + 1);
		ret.original_offset_x_i = ret.offset_x_i = ox;
		ret.original_offset_y_i = ret.offset_y_i = oy;
		ret.original_width_i = ret.width_i = w;
		ret.original_height_i = ret.height_i = h;
		return ret;
	}

	Monitor create_monitor(const int index, const int id, const int adapter, const int h, const int w, const int ox,
	                       const int oy, const std::string& n, const float scaling)
	{
		auto ret = create_monitor(index, id, h, w, ox, oy, n, scaling);
		ret.adapter_i = adapter;
		return ret;
	}

	Image create_image(const ImageRect& rect, int row_padding, const ImageBgra* data)
	{
		Image ret;
		ret.bounds = rect;
		ret.data = data;
		ret.bytes_to_next_row = row_padding;
		return ret;
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
