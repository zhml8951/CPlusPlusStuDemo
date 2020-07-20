#include "IniParser.h"
#include <sstream>
#include <cstdio>
#include <iostream>
#include <fstream>

static void trim_string(string& str)
{
	str.erase(0, str.find_first_not_of(' '));
	str.erase(str.find_last_not_of(" ") + 1);
}

string& IniParser::replace_all(string& str, const string& old_value, const string& new_value)
{
	while (true) {
		string::size_type pos = 0;
		pos = str.find(old_value);
		if (pos == string::npos) { break; }
		str.replace(pos, old_value.length(), new_value);
	}
	return str;
}

// 主要目的确保文件换行符为\r\n(CRLF), 当文本由mac导出时，换行符是\r(CR),读取文档是最长文档叠加到一起。

bool IniParser::replace_all_distinct(string& str, const string& new_value, const string& old_value)
{
	try {
		for (size_t pos = 0; pos != string::npos; pos += new_value.length()) {
			pos = str.find(old_value, pos);
			if (pos == string::npos) break;

			str.replace(pos, old_value.length(), new_value);
		}
		return true;
	}
	catch (...) {
		return false;
	}
}

bool IniParser::read_file_content(string& rst, const string& file_name)
{
	try {
		const std::ifstream file_in(file_name);
		std::ostringstream tmp;
		tmp << file_in.rdbuf();
		auto lvalue = new std::string(tmp.str());

		memmove(&rst, lvalue, sizeof(*lvalue)); // std::string(tmp.str());
		return true;
	}
	catch (...) {
		return false;
	}
}

void IniParser::ReplaceAllDistinct(string& content, const string& old_val, const string& new_val)
{
	auto begin = -1;
	while ((begin = content.find(old_val, begin + 1)) != string::npos) {
		content.replace(begin, old_val.length(), new_val);
		begin += old_val.length();
	}
}

string IniParser::get_string_from_file(const string& ini_file)
{
	FILE* fp = nullptr;
	fopen_s(&fp, ini_file.c_str(), "rt");
	if (fp == nullptr) { return ""; }

	// 判断文件大小。也就是有多少字，开辟多少buff.
	fseek(fp, 0, SEEK_END);
	const auto size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	uint8_t* buffer = nullptr;
	const auto buff_size = sizeof(uint8_t) * (size + 1);
	std::cout << "buff_size: " << buff_size << "\n";
	buffer = static_cast<uint8_t*>(malloc(buff_size));
	memset(buffer, 0, buff_size);
	buffer[size] = '\0';
	const auto read_size = fread_s(buffer, buff_size, sizeof(uint8_t), buff_size, fp);
	fclose(fp);

	if (read_size < size) buffer[read_size] = '\n';

	std::string str(reinterpret_cast<char*>(buffer));
	std::cout << "str:  " << str << "\n";
	free(buffer);
	return str;
}

bool IniParser::read_ini(const std::string& filename)
{
	using std::string;
	std::stringstream conf_file_in;
	const auto content = new string;
	std::cout << "content.address: " << content << "\n";
	try {
		//auto content = get_string_from_file(filename);
		auto success = read_file_content(*content, filename);
		std::cout << content << "\n";

		// 主要目的确保文件换行符为\r\n(CRLF), 当文本由mac导出时，换行符是\r(CR),读取文档是最长文档叠加到一起。
		if (!(replace_all_distinct(*content, "\r", "\r\n"))) {
			return false;
		}

		conf_file_in << content;
		string str_line = "";
		std::map<string, string>* kv_node = nullptr;
		size_t left_pos;
		size_t right_pos;
		size_t equal_div_pos;

		while (std::getline(conf_file_in, str_line)) {
			if (str_line.empty()) continue;
			if ((string::npos != (left_pos = str_line.find("["))) &&
				(str_line.npos != (right_pos = str_line.find("]")))) {
				auto root = str_line.substr(left_pos + 1, right_pos - left_pos - 1);
				trim_string(root);
				if (!root.empty()) {
					this->map_ini_[root] = std::map<string, string>{};
					kv_node = &(this->map_ini_[root]);
				}
			}
			else if (string::npos != (equal_div_pos = str_line.find("="))) {
				auto key = str_line.substr(0, equal_div_pos);
				const auto note_pos = str_line.find(";");
				string value;
				if (string::npos != note_pos) {
					value = str_line.substr(equal_div_pos + 1, note_pos - equal_div_pos - 1);
				}
				else {
					value = str_line.substr(equal_div_pos + 1, str_line.size() - 1);
				}

				trim_string(key);
				trim_string(value);
				if (kv_node != nullptr && !key.empty()) {
					(*kv_node)[key] = value;
				}
			}
		}

		delete content;
		return true;
	}
	catch (...) {
		return false;
	}
}

int main(int argc, char* argv[])
{
	std::string file_name = "d:\\temp\\Config.ini";

	auto rst = IniParser().read_ini(file_name);
}