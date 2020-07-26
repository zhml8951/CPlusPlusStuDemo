#pragma once
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
	
	static bool replace_all_distinct(string& str, const string& new_value, const string& old_value);

public:
	~IniParser() { clear(); }

	static void ReplaceAllDistinct(string& content, const string& old_val, const string& new_val);

	static string get_string_from_file(const string& ini_file);

	static bool read_file_content(string& rst, const string& file_name);

	size_t get_size() const { return this->map_ini_.size(); };

	bool read_ini(const std::string& filename);

	void view() const;

	string get_string(const string& root, const string& key, const string& def) const;

	int get_int(const string& root, const string& key, int def) const;

	double get_double(const string& root, const string& key, double def) const;

	bool get_bool(const string& root, const string& key, bool def) const ;

	bool write_ini(const string& path);

	void set_value(const string& root, const string& key, const string& value);

	void set_string(const string& root, const string& key, const string& value);

	void set_int(const string& root, const string& key, int value);

	void set_double(const string& root, const string& key, double value);

	void set_bool(const string& root, const string& key, bool value);

	void clear() { map_ini_.clear(); }
};
