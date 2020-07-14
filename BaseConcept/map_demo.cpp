#include <iostream>
#include <map>
#include <string>
#include <cstring>

// map<int, string>(key, value) key-value.键值对. 其中key是不允许重复的。

namespace map_demo
{
	void MapSimple01()
	{
		std::map<int, const char*> student;

		// map通常的3种方式插入数据。

		// 使用pair<int, string>(...) 组成键值对。 <方法1， 使用pair组成键值对>，insert .
		student.insert(std::pair<int, const char*>(101, "Student_101"));
		student.insert(std::pair<int, const char*>(102, "Student_102"));

		typedef std::pair<int, const char*> StuPair; // using 与typedef 新老两种写法。
		using StuPairUse = std::pair<int, const char*>;
		student.insert(StuPair(11, "Student_11"));
		student.insert(StuPair(12, "Student_12"));

		// 使用insert(map<int, string>::value_type(...)相当于构建一个新对象，直接插入。
		student.insert(std::map<int, const char*>::value_type(103, "Student_103"));
		student.insert(std::map<int, const char*>::value_type(104, "Student_104"));

		typedef std::map<int, const char*>::value_type StuMap;
		using StuMapUse = std::map<int, const char*>::value_type;
		student.insert(StuMap(13, "Student_13"));
		student.insert(StuMap(14, "Student_14"));

		// 直接使用下标的方式
		student[105] = "Student_105";
		student[106] = "Student_106";

		// 下面是重复值，会替换前面值。 也就是key相同， value会被替换。
		student[105] = "Student_0105";
		student[101] = "Student_0101";

		printf("student_count: %lld\n", student.count(101));

		// 正向迭代器
		for (auto it = student.begin(); it != student.end(); ++it) {
			printf("student:  %d -- %s\n", it->first, (*it).second);
		}

		typedef std::pair<std::map<int, const char*>::iterator, bool> MapInsertRst;

		MapInsertRst rst = student.insert(StuPair(21, "Student_21"));
		if (rst.second) {
			printf("student insert data success. \n");
		}
		rst = student.insert(StuMapUse(103, "Student0_103"));
		if (rst.second) {
			printf("Student insert data success.\n");
		}
		else {
			printf("Insert data failure.\n");
		}

		printf_s("student_map.size: %lld.\n", student.size());

		std::map<int, const char*>::iterator it01; // 正向迭代器，非常量
		std::map<int, const char*>::const_iterator const_it; // 常量迭代器
		std::map<int, const char*>::reverse_iterator it02; // 反向迭代器

		for (auto it = student.rbegin(); it != student.rend(); ++it) {
			printf("student_reverse: [%d,  %s]\n", it->first, it->second);
		}

		// map 查找元素的方法常用find. count主要判断键位是否有值。 有值返回1，否则返回0；
		const auto find_elem = student.find(101);

		if (find_elem != student.end()) {
			printf("find_element: [%d, %s]\n", find_elem->first, find_elem->second);
		}
	}
}

int main(int argc, char* argv[])
{
	map_demo::MapSimple01();
}
