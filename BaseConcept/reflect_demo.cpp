#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <mutex>

/*
 *  反射(reflect)是指程序在运行时动态获取对象属性与方法的一种机制，Java
 *  在C++中即 编译器需要将类型信息(属性类型与偏移地址以及成员函数的地址等信息)编译到程序文件中, 做到运行时只根据对象的地址和引用就可以获得对象的类型信息；
 *
 */

#define REFLECT_ATTR(type, name) \
type name;	\
const ReflectHelper ref_##name = ReflectHelper(this, &this->name, #type, #name)

#define REFLECT_INT(name) REFLECT_ATTR(int, name)
#define REFLECT_BOOL(name) REFLECT_ATTR(bool, name)
#define REFLECT_FLOAT(name) REFLECT_ATTR(float, name)
#define REFLECT_DOUBLE(name) REFLECT_ATTR(double, name)
#define REFLECT_STRING(name) REFLECT_ATTR(string, name)

namespace reflect_stu
{
	using std::string;
	constexpr int kBuffSize = 16;

	class Object
	{
	public:
		virtual ~Object() {}

		virtual string ToString() const
		{
			char buffer[kBuffSize];
			sprintf_s(buffer, kBuffSize, "%p", this);
			return buffer;
		}

		virtual string GetClassName() const
		{
#ifdef _MSC_VER
			return typeid(*this).name() + 6;
#else
			const char* name = typeid(*this).name();
			while (*name >= '0' && *name <= '9') name++;
			return name;
#endif
		}
	};


	class ReflectItem
	{
	protected:
		int offset_;
		string name_;
		const char* type_;
	public:
		ReflectItem() : offset_(0), type_(nullptr) {}

		ReflectItem(const int offset, const char* type, const string& name) : offset_(offset), name_(name),
		                                                                      type_(type) {}

		bool CanUse() const { return type_ ? true : false; }
		bool operator<(const ReflectItem& other) const { return name_ < other.name_; }
		bool operator==(const ReflectItem& other) const { return this->name_ == other.name_; }

		int GetOffset() const { return this->offset_; }
		string GetName() const { return type_ ? this->name_ : ""; }
		string GetType() const { return type_ ? this->type_ : ""; }

		string Get(const void* obj) const
		{
			const auto dst = static_cast<char*>(const_cast<void*>(obj)) + offset_;
			if (type_ == nullptr || type_ == "object") return "";
			if (type_ == "int") return std::to_string(*(reinterpret_cast<int*>(dst)));
			if (type_ == "bool") return *reinterpret_cast<bool*>(dst) ? "true" : "false";
			if (type_ == "float") return std::to_string(*reinterpret_cast<float*>(dst));
			if (type_ == "double") return std::to_string(*reinterpret_cast<double*>(dst));
			return *reinterpret_cast<string*>(dst);
		}

		bool Set(void* obj, const int val) const
		{
			auto const dst = static_cast<char*>(obj);
			if (type_ == nullptr || type_ == "object")
				return false;

			if (type_ == "int")
				*reinterpret_cast<int*>(dst) = val;
			else if (type_ == "bool")
				*reinterpret_cast<bool*>(dst) = (val ? true : false);
			else if (type_ == "float")
				*reinterpret_cast<float*>(dst) = val;
			else if (type_ == "double")
				*reinterpret_cast<double*>(dst) = val;
			else
				*reinterpret_cast<string*>(dst) = std::to_string(val);
			return true;
		}

		bool Set(void* obj, const double val) const
		{
			return true;
		}

		bool Set(void* obj, const char* val) const
		{
			return true;
		}

		bool Set(void* obj, const string& val) const { return Set(obj, val.c_str()); }
		bool Set(void* obj, const bool val) const { return Set(obj, val ? 1 : 0); }
		bool Set(void* obj, const float val) const { return Set(obj, static_cast<double>(val)); }
	};

	class ReflectHelper
	{
	protected:
		static std::mutex& GetMutex()
		{
			static std::mutex m;
			return m;
		}

		static std::map<string, std::set<ReflectItem>>& GetMap()
		{
			static std::map<string, std::set<ReflectItem>> map;
			return map;
		}

	public:
		ReflectHelper(Object* self, void* data, const char* type, const char* name)
		{
			static auto& attr_map = GetMap();

			std::lock_guard<std::mutex> lk(GetMutex());
			auto const key = self->GetClassName();

			if (type == "int" || type == "bool" || type == "float" || type == "double" || type == "string") {
				attr_map[key].insert(ReflectItem(static_cast<char*>(data) - reinterpret_cast<char*>(self), type, name));
			}
			else {
				attr_map[key].insert(ReflectItem(static_cast<char*>(data) - reinterpret_cast<char*>(self),
				                                 "object", name));
			}
		}

		static std::vector<ReflectItem> GetList(const string& key)
		{
			static auto& attr_map = GetMap();
			std::vector<ReflectItem> vec;
			std::lock_guard<std::mutex> lk(GetMutex());

			auto it = attr_map.find(key);
			if (it == attr_map.end()) return {};
			for (auto& item : it->second)
				vec.push_back(item);
			std::sort(vec.begin(), vec.end(), [](const ReflectItem& a, const ReflectItem& b) {
				return a.GetOffset() < b.GetOffset();
			});

			return std::move(vec);
		}

		static ReflectItem GetItem(const string& key, const string& name)
		{
			// ReSharper disable once CppUseAuto
			static std::map<string, std::set<ReflectItem>>& attr_map = GetMap();
			ReflectItem item(0, nullptr, name);
			std::lock_guard<std::mutex> lk(GetMutex());

			auto it = attr_map.find(key);
			if (it == attr_map.end()) return item;
			auto const tmp = it->second.find(item);
			if (tmp == it->second.end()) return item;
			return *tmp;
		}
	};


	class Json final : public Object
	{
	private:
		REFLECT_INT(intval_);
		REFLECT_BOOL(boolval_);
		REFLECT_STRING(stringval_);
	};
}
