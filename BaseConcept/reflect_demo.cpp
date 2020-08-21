#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <mutex>
#include <functional>

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
#define REFLECT_STRING(name) REFLECT_ATTR(std::string, name)

namespace reflect_stu
{
	using std::string;
	constexpr int kBuffSize = 16;

	enum class OpType { };

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

	// 字符串计算hash值,  C++ switch... case... 只支持整型常量或enum类型， 可采用将字符串转hash方式，可采用标准库std::hash(不能做常量)
	//-------------------------------------------------------------------------------------------------------

	constexpr uint64_t kPrime = 0x100000001B3ull;
	constexpr uint64_t kBasis = 0xCBF29CE484222325ull;

	static constexpr uint64_t Hash(char const* str)
	{
		auto rst{kBasis};
		while (*str) {
			rst ^= *str; // 位异或运算;
			rst *= kPrime;
			str++;
		}
		return rst;
	}

	//采用递归调用产生字符串的Hash值, 这里主要用switch... case 的整型常量；
	static constexpr uint64_t HashCompileRecur(char const* str, const uint64_t last_value = kBasis)
	{
		return *str ? HashCompileRecur(str + 1, (*str ^ last_value) * kPrime) : last_value;
		// 这里的递归调用需要注意理解, 通过last_value保存最后hash值. 使用方法妙; 在当前轮次进行*str^las)*kP保存至last_va,传指针str+1,循环调用;
	}

	// operator""_hash 重载 operator""_hash, 后面size_t不能少

	static constexpr uint64_t operator""_hash(char const* str, size_t)
	{
		return Hash(str);
	}

	// -------------------------------------------------------------------------------------------------------

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
			const auto dest = static_cast<char*>(obj) + offset_;
			if (type_ == nullptr || type_ == "object") return false;

			switch (Hash(type_)) {
			case Hash("int"):
				*reinterpret_cast<int*>(dest) = val;
				break;
			case "double"_hash:
				*reinterpret_cast<double*>(dest) = val;
				break;
			case "bool"_hash:
				*reinterpret_cast<bool*>(dest) = val < -0.00001 || val > 0.00001;
				break;
			case "float"_hash:
				*reinterpret_cast<float*>(dest) = val;
				break;
			case "std::string"_hash:
				*reinterpret_cast<string*>(dest) = std::to_string(val);
				break;
			case "string"_hash:
				*reinterpret_cast<string*>(dest) = std::to_string(val);
				break;
			default:
				return false;
			}
			return true;
		}

		bool Set(void* obj, const char* val) const
		{
			const auto dest = static_cast<char*>(obj) + offset_;
			if (val == nullptr || type_ == nullptr || type_ == "object") return false;

			if (type_ == "string" || type_ == "std::string")
				*reinterpret_cast<string*>(dest) = val;
			else {
				if (*val == 0) return true;

				switch (Hash(type_)) {
				case "int"_hash:
					*reinterpret_cast<int*>(dest) = atoi(val);
					break;
				case "bool"_hash:
					*reinterpret_cast<bool*>(dest) = (string{val} == "true");
					break;
				case "float"_hash:
					*reinterpret_cast<float*>(dest) = atof(val);
					break;
				case "double"_hash:
					*reinterpret_cast<double*>(dest) = atof(val);
					break;
				default:
					printf("Unknown type or value.\n");
				}
			}
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

	public:
		string ToString() const
		{
			string msg;
			std::vector<ReflectItem> vec = ReflectHelper::GetList(this->GetClassName());

			for (auto& item : vec) {
				if (item.GetType() == "string") {
					msg += ".\"" + item.GetName() + "\":\"" + item.Get(this) + "\"";
				}
				else {
					msg += ",\"" + item.GetName() + "\":" + item.Get(this);
				}
			}
			if (msg.empty()) return "{}";
			return "{" + msg.substr(1) + "}";
		}
	};

	void reflect_test_main()
	{
		//std::unique_ptr<Object> obj(new Json());
		Object* obj = new Json;
		std::vector<ReflectItem> vec = ReflectHelper::GetList(obj->GetClassName());
		puts("Class member variables defined: ");
		puts("------------------------------------------");
		for(auto& item: vec)
			std::cout << " " << item.GetType() << "  " << item.GetName() << ";" << "\n";

		ReflectHelper::GetItem("Json", "intval_").Set(obj, 100);
		ReflectHelper::GetItem("Json", "boolval_").Set(obj, true );

		delete obj;
	}
}

int main(int argc, char* argv[]) {}
