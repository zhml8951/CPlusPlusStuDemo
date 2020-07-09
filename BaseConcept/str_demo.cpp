#include "str_demo.h"
#include <iostream>
#include <string>


void find_test()
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

	flag = {"a"};
	auto i{0};
	pos = 0;
	while ((pos = s1.find_first_of(flag, pos)) != string::npos) {
		cout << "position: " << i << pos << "\n";
		pos++;
		++i;
	}
}

int str_demo_main()
{
	find_test();
	return 0;
}
