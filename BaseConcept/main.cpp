#include <QtCore/QCoreApplication>
#include <string>
// #include "Stl01.h"
#include "cast_demo.h"
#include "winapi_demo01.h"
#include "extern_demo.h"


void test_extern();

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);
	//vector_demo::accumulate_demo();
	//vector_demo::accumulate_stu::test_main();
	//stl_china_demo::compare_no_stl();
	//stl_china_demo::compare_stl();
	//stl_china_demo::io_iterator();
	//const std::string input_file = "D:\\temp\\test_input.txt";
	//const std::string output_file = "D:\\temp\\test_output.txt";
	//auto rst = stl_china_demo::iter_copy_file(input_file, output_file);
	//demo02();
	//template_demo::test_main();
	//win_api_demo_main();
	test_extern();
	return a.exec();
}

void test_extern()
{
	const auto str003 = int_to_string(int01);
	printf_s("str001: %s\n", str003);
	printf_s("str02_from_extern: %s\n", str02);
	printf_s("str01_from extern_demo: %s\n", str01);
	for (auto iter : vec01) {
		printf("vec_value: %s,   ", iter.c_str());
	}
}
