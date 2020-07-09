#include <iostream>
#include <fstream>
#include <string>

#define GET_NAME() getname(__LINE__)
#define NAME() getname(int line)

#define OBJECT [](int line)->std::string \
{\
	ifstream fcin(__FILE__,ios::in); \
 	std::string object_name_str; \
	while(line--) getline(fcin, object_name_str); \
	fcin.close(); \
	object_name_str = object_name_str.substr(0, object_name_str.find("GET_NAME") -1 ); \
	for(int i= object_name_str.size() - 1; i >= 0; --i) \
		if(!isalpha(object_name_str[i]) && !isdigit(object_name_str[i]) && object_name_str[i] != '_') \
			return object_name_str = object_name_str.substr(i + 1); \
	return object_name_str; \
}(line)

using namespace std;

struct Node
{
	string NAME() // ==> string get_name(int line)
	{
		//std::printf("line: %d", line);
		return OBJECT;
	}

	friend ostream& operator<<(const ostream& lhs, const string& cs);
};

std::string obj(int line)
{
	ifstream f_cin(__FILE__, ios::in);
	string obj_name_str;
	while (line--) {
		printf("line: %d.  ", line);
		std::getline(f_cin, obj_name_str);
		printf("line_str: %s \n", obj_name_str.c_str());
	}
	std::cout << "obj_name_str:  \n" << obj_name_str << '\n';
	f_cin.close();
	obj_name_str = obj_name_str.substr(0, obj_name_str.find("get_name") - 1);
	for (int i = obj_name_str.size() - 1; i >= 0; --i)
		if (!isalpha(obj_name_str[i]) && !isdigit(obj_name_str[i]) && obj_name_str[i] != '_')
			return obj_name_str = obj_name_str.substr(i + 1);
	return obj_name_str;
}

struct Node02
{
	static string get_name(const int line) {
		return obj(line);
	}
};

ostream& operator<<(const ostream& lhs, const string& cs)
{
	return lhs << cs << ", test..." << '\n';
}

int main()
{
	Node test1;
	auto &test2 = test1;
	Node test3;
	cout << test1.GET_NAME() << endl;
	cout << test2.GET_NAME() << endl;
	cout << test3.GET_NAME() << endl;

	Node02 n2_test1;
	cout << "\nNode02 test: \n";
	cout << n2_test1.get_name(__LINE__) << endl;
	// fstream getline test...
	// string include header.
}



