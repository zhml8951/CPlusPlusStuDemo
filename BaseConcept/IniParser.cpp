#include "IniParser.h"
#include <io.h>

static void trim_string(string& str)
{
	str.erase(0, str.find_first_not_of(' '));
	str.erase(str.find_first_not_of(' ') + 1);
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

string& IniParser::replace_all_distinct(string& str, const string& new_value, const string& old_value)
{
	for (size_t pos = 0; pos != string::npos; pos += new_value.length()) {
		pos = str.find(old_value, pos);
		if (pos == string::npos) break;

		str.replace(pos, old_value.length(), new_value);
	}
	return str;
}

string IniParser::get_string_from_file(const string& ini_file)
{
	unsigned char* buffer = nullptr;
	size_t size = 0;
	size_t read_size;
	FILE* fp = nullptr;
	fopen_s(&fp, ini_file.c_str(), "rt");
	if (fp == nullptr) { return ""; }
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buffer = static_cast<unsigned char*>(malloc(sizeof(unsigned char) * (size + 1)));
	buffer[size] = '\0';
	//read_size = fread_s(buffer, sizeof(unsigned char), size, fp);
	free(buffer);
	return "ok";
}
