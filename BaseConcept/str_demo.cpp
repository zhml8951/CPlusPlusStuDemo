#include "str_demo.h"
#include <iostream>
#include <string>

namespace string_stu
{
	void FindTest01()
	{
		using namespace std;
		string s1("dada4994befb934493e8f6f70128c0df");
		string flag = "e8";
		auto pos = s1.find("e8");
		if (pos != string::npos) {
			cout << "find flags at:  " << pos << "\n";
		}
		else {
			cout << "Not found the flag(e8)";
		}
		flag = string("cf");
		pos = s1.find(flag);
		cout << "find flag(cf) at: " << pos << "\n";
		std::cout << "pos==string::npos:  " << std::boolalpha << (pos == string::npos) << "\n";

		flag = {"a"};
		auto i{0};
		pos = 0;
		while ((pos = s1.find_first_of(flag, pos)) != string::npos) {
			cout << "position: " << i << pos << "\n";
			pos++;
			++i;
		}
	}

	void FindFirstDemo()
	{
		using std::string;
		string str("look for non-alphabetic characters.....");
		string str2{"469903 string character."};
		const string find_str{"abcedfghijklmnopqrstuvwxyz"};
		auto found = str.find_first_not_of(find_str);
		printf("str:  %s", str.data());
		if (found != string::npos) {
			//str.replace(found, 1, "*");
			std::cout << "found pos:  " << found << "\n";
			str[found] = '*';
			std::cout << ", found : " << str[found] << "\n";
			std::cout << str << "\n";
		}

		found = 0;
		found = str2.find_first_of(find_str);
		printf("str2_data:  %s", str2.data());
		if (found != string::npos) {
			std::cout << "Found pos: " << found << "\n";
			//str2.replace(found, 1, "x");
			str2.insert(found-1, "X");
			printf("str2.replaced:  %s\n", str2.c_str());
		}
	}

	void StrModify()
	{
		
	}
}

int StrDemoMain()
{
	string_stu::FindTest01();
	string_stu::FindFirstDemo();
	return 0;
}

int main(int argc, char* argv[])
{
	StrDemoMain();
}
