//#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

constexpr auto kReadNumber = 10;
constexpr auto kSqlFile = "d:\\temp\\csdn.sql";

class CsdnUser
{
public:
	std::string username_;
	std::string password_;
	std::string email_;
};

typedef std::vector<CsdnUser> VecUsers;

int read_in(const int read_of_number, VecUsers& user_data)
{
	using namespace std;
	ifstream ifs(kSqlFile, ios::in);
	if (!ifs) {
		cout << "data sql file read error.\n";
		return 0;
	}
	char ch;
	for (auto i = 0; i < read_of_number; i++) {
		if (!ifs.get(ch)) {
			printf("Read file end.\n");
			return 0;
		}
		while (ch != '#') {
			if (ch == ' ') {
				ifs.get(ch);
				continue;
			}
			user_data[i].username_ += ch;
			ifs.get(ch);
		}
		ifs.get(ch);
		while (ch != '#') {
			if (ch == ' ') {
				ifs.get(ch);
				continue;
			}
			user_data[i].password_ += ch;
			ifs.get(ch);
		}

		ifs.get(ch);
		while (ch != '\n') {
			if (ch == ' ') {
				ifs.get(ch);
				continue;
			}
			user_data[i].email_ += ch;
			ifs.get(ch);
		}
	}
	ifs.close();
	return 1;
}

void output(const CsdnUser& pr_user)
{
	std::cout << pr_user.username_ << ",  " << pr_user.password_ << ",  " << pr_user.email_ << "\n\n";
}

void my_find(const CsdnUser& f_user)
{
	if (f_user.username_ == "Arnni") {
		std::cout << "-----------user found.\n";
		std::cout << f_user.username_ << "  " << f_user.password_ << " " << f_user.email_ << "\n";
	}
}

bool write_file(VecUsers& w_users)
{
	using namespace std;
	ofstream ofs(kSqlFile, ios::app | ios::out);
	if (!ofs) {
		cout << "Open file failed.\n";
		return false;
	}
	for (auto iter_user = w_users.begin(); iter_user != w_users.end(); ++iter_user) {
		ofs << iter_user->username_ << " # " << iter_user->password_ << " # " << iter_user->email_ << "\n";
	}
	ofs.close();
	return true;
}

namespace point_ref_demo
{
	// C++ 基本概念( 指针，引用，值) 传递方面的使用。 这些基本的东西都忘，  尴尬......
	using namespace std;

	void value_transfer(int value)
	{
		printf("\n%p", &value);
		printf("\n%d", value);
		value = 0xff;
	}

	void test_value_transfer()
	{
		auto const a = 0x100;
		printf("\n%p", &a);
		printf("\n%d", a);
		value_transfer(a);
		printf("\n%d", a);
	}

	void reference_transfer(int& ref)
	{
		printf("\n%d", ref);
		printf("\n%p", &ref);
		ref = 0xff;
	}

	void test_ref_transfer()
	{
		auto num = 0x10;
		printf("\n%d", num);
		printf("\n%p", &num);
		reference_transfer(num);
		printf("\n%d", num);
	}

	void point_transfer(int* point)
	{
		printf("\n%d", *point);
		printf("\n%p", point);
		printf("\n%p", &point);
		*point = 0xfff;
	}

	void test_point_transfer()
	{
		auto num = 0x11;
		printf("\n%d", num);
		printf("\n%p", &num);
		point_transfer(&num);
		printf("\n%d", num);
	}
}

int csdn_main(int argc, char* argv[])
{
	using namespace std;
	VecUsers users(kReadNumber);
	if (!read_in(kReadNumber, users)) {
		std::cout << "\n Error.\n";
		return -1;
	}

	std::for_each(users.begin(), users.end(), output);
	std::for_each(users.begin(), users.end(), my_find);

	write_file(users);
	cout << "\n\n";
	for_each(users.begin(), users.end(), output);

	//point_ref_demo::test_value_transfer();
	//point_ref_demo::test_ref_transfer();
	//point_ref_demo::test_point_transfer();
	return 0;
}