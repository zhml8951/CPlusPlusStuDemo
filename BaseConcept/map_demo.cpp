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
		// 使用pair<int, string>(...) 组成键值对。
		student.insert(std::pair<int, const char*>(101, "Student_101"));
		student.insert(std::pair<int,const char*>(102, "Student_102"));

		// 使用insert(map<int, string>::value_type(...)相当于构建一个新对象，直接插入。
		student.insert(std::map<int, const char*>::value_type(103, "Student_103"));
		student.insert(std::map<int, const char*>::value_type(104, "Student_104"));

	}
}


int main(int argc, char* argv[])
{
}
