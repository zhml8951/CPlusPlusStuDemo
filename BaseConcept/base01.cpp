#include "base01.h"
#include <iostream>
#include <string.h>
#include <string>

namespace base_concept
{
	void point_demo()
	{
		double counts[20]{}; // ==> double counts[20] = {0};
		double count[20] = {1};
		char dog[8]{'d', 'o', 'g', '0'};
		char cat[]{'t', 'o', 'm', '1'};
		char tom[] = "TomCat";
		auto jim = "TomJim";
		strcpy_s(dog, cat);
		std::string str1 = "str--1";
		str1.append(jim);
		std::cout << "str1: " << str1 << "\n";
		std::printf("dog: %s\n", dog);
		std::cout << typeid(jim).name() << "\n";
		const auto len = strlen(tom);
		std::cout << "tom_length: " << len << "\n";
	}

	typedef struct Inflatable
	{
		char name[20];
		float volume;
		double price;

		Inflatable(): name{"000"}, volume(0.0), price(0.00)
		{
			std::printf("Initialized in Inflatable()\n");
		}

		Inflatable(const char* n, const float v, const double p): volume(v), price(p)
		{
			strcpy_s(name, n);
			std::printf("Initialized in Inflatable(...)\n");
		}
	}* InflatableP;

	void test_struct()
	{
		Inflatable inf01{"name01", 8.9, 99.90};
		std::printf("inf01: \n");
		std::cout << "\nname: " << inf01.name << ",  " << inf01.price << "\n";

		auto inf02 = new Inflatable;
		std::printf("inf02: \n");
		std::cout << "\nname: " << inf02->name << ",  " << inf02->price << "\n";
		//inf02->name = {'n','0'};
		strcpy_s(inf02->name, "inf02_name02");
		inf02->price = 188.99;
		inf02->volume = 88.99;
		std::printf("inf02: \n");
		std::cout << "\nname: " << inf02->name << ",  " << inf02->price << "\n";
		const auto inf03 = &inf01;
		std::printf("inf03: \n");
		std::cout << "\nname: " << inf03->name << ",  " << inf03->price << "\n";

		const auto inf04 = new Inflatable("Inflatable_inf04", 34.4, 88);
		std::printf("inf04: \n");
		std::cout << "\nname: " << inf04->name << ",  " << inf04->price << "\n";

		delete inf02;
		delete inf04;
	}


	void enum_demo()
	{
		enum Spectrum { kRed = 1001, kOrange, kYellow, kIndigo };
		const auto band = kRed;
		std::cout << "band: " << band << "\n";
		auto e1 = kOrange;
		std::cout << "e1: " << e1 << '\n';
		e1 = kYellow;
		std::cout << "e1: " << e1 << '\n';
	}
}
/*
int main()
{
	//base_concept::point_demo();
	//base_concept::test_struct();
	base_concept::enum_demo();
	int up = 8;
	int* p_up;
	p_up = &up;
	std::cout << "up_value: " << up << ", *p_up_value: " << *p_up << "\n";
	std::cout << "up_addr: " << &up << ", p_up_value: " << p_up << "\n";
	std::cout << "p_up_addr: " << &p_up << "\n";

	auto *up_new = new int(88);
	std::cout << "up_new.addr: " << up_new  << ", *up_new:  " << *up_new << "\n";
	int* p_new = nullptr;
	std::cout << "p_new=nullptr: " << p_new << "\n";
	p_new = up_new ;
	std::cout << "p_new.value: " << *p_new << ",  p_new.addr: " << p_new << "\n";
	std::cout << "up_new_&add: " << &up_new << ", p_new.&add: " << &p_new << "\n";
	int *psome = new int{30};
	int *psome2 = new int[30]{30};
	printf("\n");
	std::cout <<"sizeof(psome): " << sizeof(psome) << "sizeof(*psome): " <<  sizeof(*psome) << "\n";
	std::cout <<"sizeof(psome2): " << sizeof(psome2) << "sizeof(*psome2): " <<  sizeof(*psome2) << "\n";
}
*/
#include "group_by.hpp"

int main(int argc, char* argv[])
{
	group_demo::TestGroupBy();	
}

