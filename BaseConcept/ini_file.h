#pragma once
#include <string>
#include <vector>

namespace ini_file
{
	using std::string;
	using std::vector;

	constexpr auto kRetOk = 0;
	constexpr auto kErrorUnmatchedBrackets = 2; // 没有匹配[,]
	constexpr auto kErrorSectionEmpty = 3; // 段为空
	constexpr auto kErrorSectionAlreadyExists = 4; // 段名已经存在
	constexpr auto kErrorParseKeyValueFailed = 5; // 解析key,value 错误
	constexpr auto kErrorOpenFileFailed = 6; // 文件打开失败
	constexpr auto kErrorNoEnoughMemory = 7; // 内存不足
	constexpr auto kErrorNotFoundKey = 8; // 没有找到对应的key
	constexpr auto kErrorNotFoundSection = 9; // 没有找到对应section(段)

	const char kDelimit[] = "\n";

	struct IniItem
	{
		string key;
		string value;
		string comment;
		string right_comment;
	};

	struct IniSection
	{
		typedef std::vector<IniItem>::iterator IniItemIter;

		IniItemIter begin()
		{
			return items.begin();
		}

		IniItemIter end()
		{
			return items.end();
		}

		string name;
		string comment;
		string right_comment;
		vector<IniItem> items;
	};

	class IniFile
	{
	public:
		IniFile();
		~IniFile() { Release(); }
		int Load(const string& file_name);
		int Save();
		int SaveAs(const string& file_name);

		int GetStringValue(const string& section, const string& key, string* value);
		IniSection* GetSection(const string& section="");

	private:
		void Release();
		static void Trim(string& str);
		bool IsCommentLine(const string& str) const;
		static bool StartWith(const string& str, const string& prefix);
		static bool Split(const string& str, const string& sep, string *p_left, string* p_right);
		int UpdateSection(const string& clean_line, const string& comment, const string& right_comment, IniSection **section);
		int AddKeyValuePair(const string& clean_line, const string& comment, const string& right_comment, IniSection *section);



	private:
		typedef vector<IniSection*>::iterator IniSectionIter;
		vector<IniSection*> sections_vec_; //用于存储段(section)集合
		string ini_filepath_;
		string comment_delimiter_;
		string err_msg_;
	};
}
