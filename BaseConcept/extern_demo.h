#pragma once
#include <string>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
inline int add(const int num1, const int num2)
{
	return num1 + num2;
}

extern const char* str01;
extern char* str02;
extern std::string str03;
extern std::vector<std::string> vec01;
extern int int01;

std::string int_to_string(const int num);

#ifdef __cplusplus
}
#endif
// ===
// #ifdef __cplusplus 是C++ 内置预定义宏，
// extern "C" {   // 主要原因是C++ 支持重载机制， 而C是不支持的。 C++在编译生成时会对函数名进行一些处理(函数名改编)，
// 根据函数名及参数和返回类型进行不同方式处理， 
// objdump 
