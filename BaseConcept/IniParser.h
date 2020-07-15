#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

using std::string;
using std::map;
using std::vector;

class IniParser
{
private:
	map<string, map<string, string>> map_ini_;
	static string& replace_all(string& str, const string& old_value, const string& new_value);
	static string& replace_all_distinct(string& str, const string& new_value, const string& old_value);

public:
	~IniParser() { clear(); }

	static string get_string_from_file(const string& ini_file);

	size_t get_size() const;

	void clear() { map_ini_.clear(); }
};
