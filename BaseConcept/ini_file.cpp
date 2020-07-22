#include "ini_file.h"
#include <fstream>

namespace ini_file
{
	IniFile::IniFile() : comment_delimiter_("#")
	{
	}

	int IniFile::Load(const string& file_name)
	{
		string line;
		string clean_line;
		string comment;
		string right_comment;
		IniSection* cur_section = nullptr;

		int error_no;

		Release();

		this->ini_filepath_ = file_name;
		std::ifstream ifs(ini_filepath_);
		if (!ifs.is_open()) {
			err_msg_ = string("open") + ini_filepath_ + string(" file failed.");
			return kErrorOpenFileFailed;
		}

		cur_section = new IniSection();
		cur_section->name = "";
		sections_vec_.push_back(cur_section);

		while (std::getline(ifs, line)) {
			Trim(line);

			// step 0,
			if (line.length() <= 0) {
				comment += kDelimit;
				continue;
			}

			// step 1,
			if (IsCommentLine(line)) {
				comment += line + kDelimit;
				continue;
			}

			// 检查行尾是否存在注释， 若存在注释则添加到right_comment中
			Split(line, this->comment_delimiter_, &clean_line, &right_comment);

			// step 2.
			if (clean_line[0] == '[') {
				error_no = 0;
			}
			else {
				error_no = 1;
			}
		}
	}

	int IniFile::Save()
	{
	}

	int IniFile::SaveAs(const string& file_name)
	{
	}

	int IniFile::GetStringValue(const string& section, const string& key, string* value)
	{
	}

	IniSection* IniFile::GetSection(const string& section)
	{
		for (IniSectionIter it = this->sections_vec_.begin(); it != this->sections_vec_.end(); ++it) {
			if ((*it)->name == section) { return *it; }
		}
		return nullptr;
	}

	void IniFile::Release()
	{
		this->ini_filepath_ = "";
		// ReSharper disable once CppUseAuto
		for (IniSectionIter it = sections_vec_.begin(); it != sections_vec_.end(); ++it) {
			delete(*it);
		}
		this->sections_vec_.clear();
	}

	// 使用string::erase 去除两端空格
	void IniFile::Trim(string& str)
	{
		str.erase(0, str.find_first_not_of(" "));
		str.erase(str.find_last_not_of(" ") + 1);
	}

	bool IniFile::IsCommentLine(const string& str) const
	{
		return StartWith(str, comment_delimiter_);
	}

	bool IniFile::StartWith(const string& str, const string& prefix)
	{
		if (strncmp(str.c_str(), prefix.c_str(), prefix.size()) == 0)
			return true;
		return false;
	}

	bool IniFile::Split(const string& str, const string& sep, string* p_left, string* p_right)
	{
		const auto pos = str.find(sep);
		string left, right;
		if (pos != string::npos) {
			left = string(str, 0, pos);
			right = string(str, pos + 1);
			Trim(left);
			Trim(right);
			*p_left = left;
			*p_right = right;
			return true;
		}
		else {
			left = str;
			right = "";
			Trim(left);
			*p_left = left;
			*p_right = right;
			return false;
		}
	}

	int IniFile::UpdateSection(const string& clean_line, const string& comment, const string& right_comment,
	                           IniSection** section)
	{
		const auto pos = clean_line.find_first_of(']');
		if (pos == string::npos) {
			err_msg_ = string("No matched ] found.");
			return kErrorUnmatchedBrackets;
		}
		const auto len = pos - 1;
		if (len <= 0) {
			err_msg_ = string("section name is empty.");
			return kErrorSectionEmpty;
		}

		string s(clean_line, 1, len);
		Trim(s);

		if (GetSection(s) != nullptr) {
			err_msg_ = string("section ") + s + string(" already exist");
			return kErrorSectionAlreadyExists;
		}

		auto new_sec = new IniSection();
		new_sec->name = s;
		new_sec->comment = comment;
		new_sec->right_comment = right_comment;
		sections_vec_.push_back(new_sec);
		*section = new_sec;
		return 0;
	}

	int IniFile::AddKeyValuePair(const string& clean_line, const string& comment, const string& right_comment,
	                             IniSection* section)
	{
		string key, value;
	// TODO ....
		return 0;
	}

	// string 两端空格去除，实现方式比较复杂，两次创建string. 这里不再使用，
	void TrimComplex(string& str)
	{
		auto len = str.length();
		auto i = 0;
		while ((i < len) && isspace(str[i]) && (str[i] != '\0')) {
			i++;
		}
		if (i != 0) {
			str = string(str, i, len - i);
		}
		len = str.length();

		for (i = len - 1; i >= 0; --i) {
			if (!isspace(str[i])) { break; }
		}
		str = string(str, 0, i + 1);
	}
}
