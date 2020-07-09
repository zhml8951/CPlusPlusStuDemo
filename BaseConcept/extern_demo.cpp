#include "extern_demo.h"

const char* str01 = "First String for Extern";
char* str02 = const_cast<char*>("Second String no const for Extern");
std::string str03 = "Third std:string for Extern";
std::vector<std::string> vec01{"str01", "str02", "str03"};
int int01 = 888;

std::string int_to_string(const int num)
{
	using std::string;
	auto value = std::to_string(num);
	value += value;
	return value;
}

#define DECLARE_HANDLE(name) struct get_name##__{int unused;}; typedef struct get_name##__ *get_name

int main(int argc, char* argv[])
{
	DECLARE_HANDLE(Dfine01);
	Dfine01__ struct01;
	struct01.unused = 88;
	Dfine01 struct02;
#define FUNC(a) #a
#define LINK(a,b) a##\ b
#define INT_NUM(name) (int #get_name)
#define MAX(x,y) (x)>(y)?(x):(y)
	printf_s("func(a): %s", FUNC(a));
	printf("max(x,y): %d", MAX(88, 99));
}
