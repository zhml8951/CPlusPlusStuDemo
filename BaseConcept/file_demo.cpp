#include "file_demo.h"
#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <fstream>

// C++ 文件操作大体分两类(ansi_C语言方式和modern_C++方式)

namespace file_operator
{
	constexpr int kBufferSize = 1024;

	// ANSI C 方式操作文件。
	// 数据流 <fopen打开数据流，fclose关闭数据流> 程序与数据之间以流的方式进行。
	// 缓冲区<Buffer>
	void FileOpDemo01()
	{
		constexpr auto file_in = "d:\\temp\\Test01.ini";
		//constexpr auto file_in = "d:\\temp\\test.xml";
		constexpr auto file_out = "d:\\temp\\c_file_out.txt";

		FILE* file_ptr = nullptr;
		const auto stat = fopen_s(&file_ptr, file_in, "rt");
		if (stat != 0) {
			printf_s("\n!!Read file error. \n");
		}

		printf("seek: %ld\n", ftell(file_ptr));
		fpos_t fpos;
		auto ch = '\n';
		while (true) {
			ch = fgetc(file_ptr);
			if (feof(file_ptr)) break;
			//printf("%c", ch);
		}
		printf("read end.\n");
		printf("fgetpos: %d\n fpos: %lld", fgetpos(file_ptr, &fpos), fpos);
		fseek(file_ptr, 0, SEEK_END);
		const auto file_length = ftell(file_ptr);
		fseek(file_ptr, 0, SEEK_SET);
		const auto buf_size = file_length + 1;
		const auto p_buf = static_cast<char*>(calloc(buf_size, sizeof(char)));

		const auto read_size = fread_s(p_buf, buf_size, sizeof(char), file_length, file_ptr);
		std::string str(p_buf);
		printf("\n");
		std::cout << "\n...out content...\n " << str << "\n";
		std::cout << "\n" << "read_size:  " << read_size << "\n";
		free(p_buf);
		fclose(file_ptr);
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

	void ReadFileStream()
	{
		auto print_state = [](const std::ios& stream) -> void {
			using std::cout;
			cout << "good()=" << stream.good() << "\n";
			cout << "eof()=" << stream.eof() << "\n";
			cout << "fail()=" << stream.fail() << "\n";
			cout << "bad()=" << stream.bad() << "\n";
		};

		void(*print_point)(const std::ios&) ;// = [](const std::ios& stream) -> void {
			
		//}


		std::string content;
		std::ifstream ifs("d:\\temp\\csdn.sql");
		if (ifs.is_open()) {
			while (bool(std::getline(ifs, content))) {
			}
		}
		else {
			throw "Read file error";
		}
	}
}

int main(int argc, char* argv[])
{
	file_operator::FileOpDemo01();
}
