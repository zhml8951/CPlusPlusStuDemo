#include "ini_file.h"
#include <fstream>
#include <cstring>
#include <algorithm>

namespace ini_file
{
	IniFile::IniFile() : comment_delimiter_("#") { }

	int IniFile::Load(const string& file_name)
	{
		string line;
		string clean_line;
		string comment;
		string right_comment;

		int error_no;

		Release();

		this->ini_filepath_ = file_name;
		std::ifstream ifs(ini_filepath_);
		if (!ifs.is_open()) {
			err_msg_ = string("open") + ini_filepath_ + string(" file failed.");
			return kErrorOpenFileFailed;
		}

		auto cur_section = new IniSection();
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
				error_no = UpdateSection(clean_line, comment, right_comment, &cur_section);
			}
			else {
				error_no = AddKeyValuePair(clean_line, comment, right_comment, cur_section);
			}
			if (error_no != 0) {
				ifs.close();
				return error_no;
			}
		}
		ifs.close();
		return kRetOk;
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

	int IniFile::Save()
	{
		return SaveAs(this->ini_filepath_);
	}

	int IniFile::SaveAs(const string& file_name)
	{
		string data = "";
		for (auto sec_it = sections_vec_.begin(); sec_it != sections_vec_.end(); ++sec_it) {
			if ((*sec_it)->comment != "") {
				data += (*sec_it)->comment;
			}
			if ((*sec_it)->name != "") {
				data += string("[") + (*sec_it)->name + string("]");
				data += kDelimit;
			}
			if ((*sec_it)->right_comment != "") {
				data += " " + this->comment_delimiter_ + (*sec_it)->right_comment;
			}

			for (auto item_it = (*sec_it)->items.begin(); item_it != (*sec_it)->items.end(); ++item_it) {
				if (item_it->comment != "") {
					data += item_it->comment;
					if (data[data.length() - 1] != '\n') data += kDelimit;
				}

				data += item_it->key + "=" + item_it->value;
				if (item_it->right_comment != "") {
					data += " " + this->comment_delimiter_ + item_it->right_comment;
				}
				if (data[data.length() - 1] != '\n') data += kDelimit;
			}
		}
		std::ofstream ofs;
		ofs << data;
		ofs.close();
		return kRetOk;
	}

	int IniFile::GetStringValue(const string& section, const string& key, string* value)
	{
		return GetValue(section, key, value);
	}

	int IniFile::GetIntValue(const string& section, const string& key, int* value)
	{
		string str_value;
		const auto error_no = GetValue(section, key, &str_value);
		*value = atoi(str_value.c_str());
		return error_no;
	}

	int IniFile::GetDoubleValue(const string& section, const string& key, double* value)
	{
		string str_value;
		const auto error_no = GetValue(section, key, &str_value);
		*value = atof(str_value.c_str());
		return error_no;
	}

	int IniFile::GetBoolValue(const string& section, const string& key, bool* value)
	{
		string str_value;
		const auto err = GetValue(section, key, &str_value);

		if (StrCmpIgnoreCase(str_value, "true") || StrCmpIgnoreCase(str_value, "1")) *value = true;

		if (StrCmpIgnoreCase(str_value, "false") || StrCmpIgnoreCase(str_value, "0")) *value = false;

		return err;
	}

	int IniFile::GetComment(const string& section, const string& key, string* comment)
	{
		auto sect = GetSection(section);
		if (sect == nullptr) {
			err_msg_ = string("not found the section. ") + section;
			return kErrorNotFoundSection;
		}
		if (key == "") {
			*comment = sect->comment;
			return kRetOk;
		}

		// ReSharper disable once CppUseAuto
		for (IniSection::IniItemIter it = sect->begin(); it != sect->end(); ++it) {
			if (it->key == key) {
				*comment = it->comment;
				return kRetOk;
			}
		}
		this->err_msg_ = string("not found the key") + section;
		return kErrorNotFoundKey;
	}

	int IniFile::GetRightComment(const string& section, const string& key, string* right_comment)
	{
		IniSection* sect = GetSection(section);
		if (sect == nullptr) {
			err_msg_ = string("not found the section.") + section;
			return kErrorNotFoundSection;
		}
		if (key == "") {
			*right_comment = sect->right_comment;
			return kRetOk;
		}

		for (auto it = sect->items.begin(); it != sect->items.end(); ++it) {
			if (it->key == key) {
				*right_comment = it->right_comment;
				return kRetOk;
			}
		}

		this->err_msg_ = string("not found the key.") + key;
		return kErrorNotFoundKey;
	}

	int IniFile::GetSections(vector<string>* sections)
	{
		for (auto it = this->sections_vec_.begin(); it != this->sections_vec_.end(); ++it) {
			sections->push_back((*it)->name);
			//*(*it)->name
		}
		// TODO ADD Sections.
	}

	IniSection* IniFile::GetSection(const string& section)
	{
		for (auto it = this->sections_vec_.begin(); it != this->sections_vec_.end(); ++it) {
			if ((*it)->name == section) { return *it; }
		}
		return nullptr;
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

	bool IniFile::Split(const string& str, const string& sep, string* left_content, string* right_comment)
	{
		const auto pos = str.find(sep);
		string left, right;
		if (pos != string::npos) {
			left = string(str, 0, pos);
			right = string(str, pos + 1);
			Trim(left);
			Trim(right);
			*left_content = left;
			*right_comment = right;
			return true;
		}
		else {
			left = str;
			right = "";
			Trim(left);
			*left_content = left;
			*right_comment = right;
			return false;
		}
	}

	bool IniFile::StrCmpIgnoreCase(const string& str1, const string& str2)
	{
		auto first_str{ str1 };
		auto last_str{ str2 };
		std::transform(first_str.begin(), first_str.end(), first_str.begin(), tolower);
		std::transform(last_str.begin(), last_str.end(), last_str.begin(), tolower);

		return (first_str == last_str);
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
		if (!Parse(clean_line, &key, &value)) {
			err_msg_ = string("parse line failed:  ") + clean_line;
			return kErrorParseKeyValueFailed;
		}
		IniItem item;
		item.key = key;
		item.value = value;
		item.comment = comment;
		item.right_comment = right_comment;
		section->items.push_back(item);
		return 0;
	}

	bool IniFile::Parse(const string& content, string* key, string* value)
	{
		return Split(content, "=", key, value);
	}

	int IniFile::GetValues(const string& section, const string& key, vector<string>* values, vector<string>* comments)
	{
		values->clear();
		comments->clear();
		auto sect = GetSection(section);
		if (sect == nullptr) {
			err_msg_ = string("Not found the section. ") + section;
			return kErrorNotFoundSection;
		}

		for (auto it = sect->begin(); it != sect->end(); ++it) {
			if (it->key == key) {
				auto value = it->value;
				auto comment = it->comment;
				values->push_back(value);
				comments->push_back(comment);
			}
		}
		if (values->size() == 0) {
			err_msg_ = string("Not found the key  ") + key;
			return kErrorNotFoundKey;
		}
		return kRetOk;
	}

	int IniFile::GetValues(const string& section, const string& key, vector<string>* values)
	{
		vector<string> comments;
		return GetValues(section, key, values, &comments);
	}

	int IniFile::GetValue(const string& section, const string& key, string* value, string* comment)
	{
		auto sect = GetSection(section);
		if (sect == nullptr) {
			err_msg_ = string("not found the section. ") + section;
			return kErrorNotFoundSection;
		}
		for (auto it = sect->begin(); it != sect->end(); ++it) {
			if (it->key == key) {
				*value = it->value;
				*comment = it->comment;
				return kRetOk;
			}
		}
		this->err_msg_ = string("not fond the key. ") + key;
		return kErrorNotFoundKey;
	}

	int IniFile::GetValue(const string& section, const string& key, string* value)
	{
		string comment;
		return GetValue(section, key, value, &comment);
	}

	void IniFile::Print()
	{
		printf("\n################# print start ################\n\n");
		printf("file path: [%s] \n", this->ini_filepath_.c_str());
		printf("comment delimiter: [%s]\n", this->comment_delimiter_.c_str());

		for (auto it = sections_vec_.begin(); it != sections_vec_.end(); ++it) {
			printf("comment:  [%s]\n", (*it)->comment.data());
			printf("section:  [%s]\n", (*it)->name.c_str());
			if ((*it)->right_comment != "") {
				printf("right comment: [%s]\n", (*it)->right_comment.c_str());
			}
			for (auto item_it = (*it)->items.begin(); item_it != (*it)->items.end(); ++item_it) {
				printf("    comment  :  [%s]\n", item_it->comment.c_str());
				printf("    param    :  [%s=%s]\n", item_it->key.c_str(), item_it->value.c_str());

				if (item_it->right_comment != "") {
					printf("     right comment:  [%s]\n", item_it->right_comment.c_str());
				}
			}
		}
		printf("\n\n################# print start ################\n");
	}

	// string 两端空格去除，实现方式：从0坐标开始while循环，使用isspace判断是否是空格，直到不是空格，从不是空格处创建新的str, string(str, i, len-i);
	// 新的str再次从尾部开始for循环，直到不是空格，得到尾部非空格i的偏移位置，再次创建新string， 这里涉及两次拷贝。

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

int main(int argc, char* argv[])
{
	const std::string config_file = "d:\\temp\\Config.ini";
	auto ini_config = ini_file::IniFile();
	ini_file::IniFile config_ini;
	ini_config.Load(config_file);
	auto sect = ini_config.GetSection("HotKey");
	for (auto it = sect->begin(); it != sect->end(); ++it) {
		printf("item: [%s = %s]\n", it->key.c_str(), it->value.c_str());
	}
}