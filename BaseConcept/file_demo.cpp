#include "file_demo.h"
#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>

constexpr int kBufferSize = 1024;

// C-style read file 
void file_read(const char* file_name)
{
	char buff[kBufferSize];
	FILE* pf;
	auto state = fopen_s(&pf, file_name, "r");

	std::cout << "file open state: " << state << "\n";
	std::cout << "errno:  " << errno << "\n";
	//int count, err_no = 0;
	const auto read_cnt = fread(buff, sizeof(buff), kBufferSize, pf);
	std::cout << "read count: " << read_cnt << "\n";
	fclose(pf);
}

void write_file(const char* file_name)
{
	const auto data_ptr = static_cast<unsigned int*>(malloc(sizeof(int) * kBufferSize));

	for (unsigned int i = 0; i < kBufferSize; i++) {
		data_ptr[i] = i; //»º³åÇø³õÊ¼»¯¡£
	}
	FILE* pf;
	auto state = fopen_s(&pf, file_name, "w");

	std::cout << "file state: " << state << "\n";

	fwrite(data_ptr, sizeof(int), kBufferSize, pf);
	fclose(pf);
	free(data_ptr);
}

//int main(int argc, char* argv[])
//{
//	write_file("write_out");
//	//file_read("zhihu_01.cpp");
//}
