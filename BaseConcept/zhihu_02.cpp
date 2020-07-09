#include <iostream>
#include <fstream>

#define GET_NAME() getname(__LINE__)
#define NAME() getname(int line)
#define VALID(ch) (isdigit(ch) || isalpha(ch) || ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '_' || ch == '{' || ch == '}')

#define OBJECT [](int line) -> std::string {                                          \
	ifstream fcin(__FILE__, ios::in);                                                 \
	std::string object_name_str;                                                      \
	while (line--)                                                                    \
		get_line(fcin, object_name_str);                                              \
	fcin.close();                                                                     \
	object_name_str = object_name_str.substr(0, object_name_str.find("GetName") - 1); \
	for (int i = object_name_str.size() - 1; i >= 0; --i)                             \
		if (!VALID(object_name_str[i]))                                               \
		{                                                                             \
			object_name_str = object_name_str.substr(i + 1);                          \
			break;                                                                    \
		}                                                                             \
	while (object_name_str.back() == '(' || object_name_str.back() == ')')            \
		object_name_str.pop_back();                                                   \
	while (object_name_str.front() == '(' || object_name_str.front() == ')')          \
		object_name_str = object_name_str.substr(1, object_name_str.size() - 1);      \
	return object_name_str;                                                           \
}(line)

using namespace std;

void get_line(const ifstream &fc_in, const string &cs)
{
}

struct Node02
{
	string NAME()
	{
		return OBJECT;
	}
	Node02 &operator()() { return *this; }
};

ostream &operator<<(const ostream &lhs, const string &cs)
{
	return lhs << cs << '\n';
}

int main()
{
	Node02 fuck[3];

	std::cout << ((fuck[1]))()()().GET_NAME();
}

uint8_t