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

	/*
	 *  ini 项目。单项item由key, value构成，注释可以在value后面(right_comment)，也可独立成行，注释采用#开头
	 */
	struct IniItem
	{
		string key;
		string value;
		string comment;
		string right_comment;
	};

	/*
	 *  Section(章节)，ini文件里以[Section], 独立成行. 一个Section包含1个或多个item; 
	 *  采用vector<IniItem> 存储为items;
	 */
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

		int GetValues(const string& section, const string& key, vector<string>* values);

		int GetStringValue(const string& section, const string& key, string* value);

		int GetIntValue(const string& section, const string& key, int* value);

		int GetDoubleValue(const string& section, const string& key, double* value);

		int GetBoolValue(const string& section, const string& key, bool* value);

		int GetComment(const string& section, const string& key, string* comment);

		int GetRightComment(const string& section, const string& key, string* right_comment);

		int GetSections(vector<string>* sections);

		int GetSectionCount();

		bool HasSection(const string& section);

		bool HasKey(const string& section, const string& key);

		IniSection* GetSection(const string& section = "");

	private:
		static void Trim(string& str);

		static bool StartWith(const string& str, const string& prefix);

		static bool Split(const string& str, const string& sep, string* left_content, string* right_comment);

		static bool StrCmpIgnoreCase(const string& str1, const string& str2);

		static bool Parse(const string& content, string* key, string* value);

		bool IsCommentLine(const string& str) const;

		int UpdateSection(const string& clean_line, const string& comment, const string& right_comment,
			IniSection** section);

		int AddKeyValuePair(const string& clean_line, const string& comment, const string& right_comment,
			IniSection* section);

		int GetValues(const string& section, const string& key, vector<string>* values, vector<string>* comments);

		int GetValue(const string& section, const string& key, string* value);

		int GetValue(const string& section, const string& key, string* value, string* comment);

		void Print();

		void Release();

	private:
		typedef vector<IniSection*>::iterator IniSectionIter;
		vector<IniSection*> sections_vec_; //用于存储段(section)集合
		string ini_filepath_;
		string comment_delimiter_;
		string err_msg_;
	};
}
