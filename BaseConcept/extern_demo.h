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
// #ifdef __cplusplus ��C++ ����Ԥ����꣬
// extern "C" {   // ��Ҫԭ����C++ ֧�����ػ��ƣ� ��C�ǲ�֧�ֵġ� C++�ڱ�������ʱ��Ժ���������һЩ����(�������ı�)��
// ���ݺ������������ͷ������ͽ��в�ͬ��ʽ���� 
// objdump 
