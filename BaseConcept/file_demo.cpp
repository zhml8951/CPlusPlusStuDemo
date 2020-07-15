#include "file_demo.h"
#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>

// C++ 文件操作大体分两类(ansi_C语言方式和modern_C++方式)

namespace file_operator
{
	constexpr int kBufferSize = 1024;

	// ANSI C 方式操作文件。
	// 数据流 <fopen打开数据流，fclose关闭数据流> 程序与数据之间以流的方式进行。
	// 缓冲区<Buffer>
	void FileOpDemo01()
	{
		constexpr auto file_in = "d:\\temp\\c_file_operator.txt";
		constexpr auto file_out = "d:\\temp\\c_file_out.txt";

		FILE* p_file = nullptr;
		auto stat = fopen_s(&p_file, file_in, "r");
		if (stat != 0) {
			printf_s("\n!!Read file error. \n");
		}

		printf("seek: %ld\n", ftell(p_file));
		fpos_t fpos;
		auto ch = '\n';
		while (true) {
			ch = fgetc(p_file);
			if (feof(p_file)) break;
			printf("%c", ch);
		}
		printf("fgetpos: %d\n fpos: %lld", fgetpos(p_file, &fpos), fpos);
		fclose(p_file);
	}

	void FileReadDemo1(const char* file_name)
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

	void WriteFileDemo1(const char* file_name)
	{
		const auto data_ptr = static_cast<unsigned int*>(malloc(sizeof(int) * kBufferSize));

		for (unsigned int i = 0; i < kBufferSize; i++) {
			data_ptr[i] = i; //缓冲区初始化。
		}
		FILE* pf;
		auto state = fopen_s(&pf, file_name, "w");

		std::cout << "file state: " << state << "\n";

		fwrite(data_ptr, sizeof(int), kBufferSize, pf);
		fclose(pf);
		free(data_ptr);
	}

	// TODO C语言方式的文件处理看似简单，目前还不知道具体什么情怀下使用及如何使用。 后面再补充吧。 
}

int main(int argc, char* argv[])
{
	file_operator::FileOpDemo01();
}
