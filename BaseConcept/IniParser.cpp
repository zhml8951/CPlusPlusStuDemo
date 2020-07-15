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
	FILE* fp = nullptr;
	fopen_s(&fp, ini_file.c_str(), "rt");
	if (fp == nullptr) { return ""; }

	// 判断文件大小。也就是有多少字，开辟多少buff.
	fseek(fp, 0, SEEK_END);
	const auto size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	uint8_t* buffer = nullptr;
	const auto buff_size = sizeof(uint8_t) * (size + 1);
	buffer = static_cast<uint8_t*>(malloc(buff_size));
	buffer[size] = '\0';
	auto read_size = fread_s(buffer, buff_size, sizeof(uint8_t), size, fp);
	fclose(fp);
	
	if (read_size < size) buffer[read_size] = '\n';

	//std::string str(reinterpret_cast<char*>(buffer));
	free(buffer);
	return "ok";
}


int main(int argc, char* argv[])
{
}
