#include "IniParser.h"
#include <sstream>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

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

// 文件换行符有3种， windows下是"\r\n"(CRLF), Linux默认 '\n'(LF), Mac默认'\r'(CR), 不同换行符会影响文件读取。
// 此函数主要目的确保文件换行符为\r\n(CRLF), 当文本由mac导出时，换行符是\r(CR),读取文档会叠加到一起。

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
		const std::ifstream file_in(file_name.c_str());
		if (!(file_in.is_open())) {
			printf("Read ini file(%s) error.\n", file_name.c_str());
			return false;
		}
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
		const auto success = read_file_content(*content, filename);
		if (!success) {
			printf("Read ini file(%s), Returned content Failure.\n", filename.c_str());
			return false;
		}

		// 主要目的确保文件换行符为\r\n(CRLF), 当文本由mac导出时，换行符是\r(CR),读取文档是最长文档叠加到一起。
		if (!(replace_all_distinct(*content, "\r", "\r\n"))) {
			return false;
		}

		conf_file_in << (*content);
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
	}
	catch (...) {
		return false;
	}

	delete content;
	return true;
}

void IniParser::view() const
{
	for (auto it = this->map_ini_.begin(); it != map_ini_.end(); ++it) {
		printf(it->first.data());
		printf(":\n");
		for (auto val_it = it->second.begin(); val_it != it->second.end(); ++val_it) {
			printf(val_it->first.c_str());
			printf(" = ");
			printf(val_it->second.c_str());
			printf("\n");
		}
	}
}

string IniParser::get_string(const string& root, const string& key, const string& def) const
{
	auto iter = this->map_ini_.find(root);
	if (iter == map_ini_.end()) return "";
	auto sub_iter = iter->second.find(key);
	if (sub_iter == iter->second.end()) return "";
	if (!(sub_iter->second).empty())
		return sub_iter->second;
	return def;
}

int IniParser::get_int(const string& root, const string& key, int def) const
{
	const auto str = get_string(root, key, "");
	if (str.empty()) return def;
	int rst;
	try {
		std::istringstream is(str);
		is >> rst;
	}
	catch (...) {
		rst = def;
	}
	return rst;
}

double IniParser::get_double(const string& root, const string& key, double def) const
{
	const auto str = get_string(root, key, "");
	if (str.empty()) return def;

	double rst;
	try {
		std::istringstream is(str);
		is >> rst;
	}
	catch (...) {
		rst = def;
	}
	return rst;
}

bool IniParser::get_bool(const string& root, const string& key, bool def) const
{
	auto str = get_string(root, key, "");
	if (str.empty()) return def;

	if (str == "1") return true;

	std::transform(str.begin(), str.end(), str.begin(), tolower);

	if (str == "true") return true;

	return false;
}

bool IniParser::write_ini(const string& path)
{
	std::ofstream out_conf_file(path.c_str());
	if (!out_conf_file) return false;
	for (auto it = this->map_ini_.begin(); it != map_ini_.end(); ++it) {
		out_conf_file << "[" << it->first << "]" << std::endl;
		for (map<string, string>::iterator sub_it = it->second.begin(); sub_it != it->second.end(); ++sub_it) {
			out_conf_file << sub_it->first << " = " << sub_it->second << std::endl;
		}
	}
	out_conf_file.close();
	out_conf_file.clear();
	return true;
}

void IniParser::set_value(const string& root, const string& key, const string& value)
{
	map<string, map<string, string>>::iterator it = this->map_ini_.find(root);
	if (map_ini_.end() != it) {
		map<string, string>::iterator sub_it = it->second.find(key);
		if (sub_it != it->second.end())
			sub_it->second = value;
		else
			it->second[key] = value;
	}
	else {
		map<string, string> m;
		m[key] = value;
		this->map_ini_[root] = std::move(m);
	}
}

void IniParser::set_string(const string& root, const string& key, const string& value)
{
	set_value(root, key, value);
}

void IniParser::set_int(const string& root, const string& key, int value)
{
	const auto str = std::to_string(value);
	set_value(root, key, str);
}

void IniParser::set_double(const string& root, const string& key, double value)
{
	char buff[64];
	sprintf_s(buff, "%lf", value);
	set_value(root, key, buff);
}

void IniParser::set_bool(const string& root, const string& key, bool value)
{
	if (value) set_value(key, key, "1");
	else set_value(root, key, "0");
}

int main(int argc, char* argv[])
{
	const std::string ini_file = "d:\\temp\\Config.ini";
	IniParser parser01;
	auto res = parser01.read_ini(ini_file);
	parser01.view();

	std::cout << std::boolalpha << res << "\n";
}
