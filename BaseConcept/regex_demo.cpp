#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <cassert>

/*
 *	C++正则表达式的使用：	{ 规则  ->   匹配    ->   结果 ｝
 *	typedef basic_regex<char> regex;
 *	typedef basic_regex<wchar_t> wregex;
 *
 *	  一. 定义正则表达式： 常用样式： regex r("[a-z0-9]+");
 *	    ^\d+$  ==>  ^[0-9]\d*|0$											非负整数
 *	    ^-[1-9]\d*|0$  ==>  ^((-\d+)|(0+))$							        非正整数
 *	    ^\d+(\.\d+)?$ ==> ^[1-9]\d*\.\d*|0\.d*[1-9]\d*|0?\.0+|0$			非负浮点数
 *
 *    二. 元字符和特殊符号：
 *	    \ 转义符， C++里使用\\转义;^ $ * + ? {n} {n,} {n, m}
 *		  *?  +? 匹配模式为非贪婪模式， 没有？则贪婪模式
 *		...|... 多项选择；
 *	    (...) 子表达式， regex语句中可以使用\(1\)来引用，在匹配结果中sm中通过array下标来使用sm[i];
 *	    (?:pattern) 匹配pattern,但不取匹配结果,也就是不能通过sm[i]来取得这个结果
 *		(?=pattern)  正向肯定预查, 非获取匹配： W\\w+(?=98|NT|10) 能匹配Win10的Win,但不匹配Win32的Win;
 *		(?!pattern)  正向否定预查,
 *		(?<=pattern) 反向肯定预查,
 *		(!<!pattern) 反向否定预查,
 *
 *		  \\b(单词边界) \\B(非单词边界)
 *		  \\d 数字字符， \\D 非数字字符
 *		  \\s 空白符   \\S 非空白符
 *		  \\w 字母、数字、下划线、[a-zA-Z0-9_]	\\W ==> [^a-zA-Z0-9_]
 *	  三. 操作函数：
 *	    regex_search： 目标序列中的某些子序列是否与正则式规则匹配, 表达式在目标序列中的任何位置找到即返回true;
 *	    regex_match:  在整个目标序列与正则表达式规则匹配才返回true, 目标序列前后都不能有符加字符: 即完全匹配
 *	    regex_replace:
 *	  四. match_results<string::const_iterator>  => smatch;
 *	  
 *	  五. 正则迭代器, 通过迭代器遍历字符串所有目标:
 *		regex_iterator,   regex_token_iterator; 
 *
 *
 */

namespace regex_simple_demo
{
	void Demo01()
	{
		std::string str = "hhh112233";
		std::regex r("[a-z0-9]+");
		bool flag = std::regex_match(str, r);
		std::cout << "regex match flag: ";
		std::cout << std::boolalpha << flag << "\n";
	}

	void Demo02()
	{
		std::string target = "@abc def--";
		std::regex e("(\\w+)\\W+(\\w+)");
		std::smatch sm;
		std::regex_search(target, sm, e);

		std::cout << "sm.prefix: " << sm.prefix() << "\n";
		for (int i = 0; i < sm.size(); i++) {
			std::cout << "sm[" << i << "]: " << sm[i] << "\n";
		}
		std::cout << "sm.suffix: " << sm.suffix() << "\n";
	}

	void match_email()
	{
		std::regex re("\\b[\\w.+-]+@[\\w._-]+\\.[a-zA-z]{2,6}\\b");
		std::string info = "My Email is ming-li.zheng@sina.com.cn, and name: zhml8951";
		std::smatch sm;
		std::regex_search(info, sm, re);
		std::cout << "sm.size: " << sm.size() << "\n";
		for (int i = 0; i < sm.size(); i++) {
			std::cout << "sm[ " << i << "]: " << sm[i] << "\n";
		}
		std::cout << "sm.prefix: " << sm.prefix() << "\n";
		std::cout << "sm.suffix: " << sm.suffix() << "\n";
	}

	void regex_iter()
	{
		std::string str = "Hello 2018, Bye 2017";
		std::smatch results;
		std::regex pattern("\\d{4}");

		std::string::const_iterator iter_start = str.begin();
		std::string::const_iterator iter_end = str.end();
		while (std::regex_search(iter_start, iter_end, results, pattern)) {
			std::string tmp = results[0];
			std::cout << tmp << "  ";
			iter_start = results[0].second;
		}

		std::regex_search(str, results, pattern);
		std::cout << "results.size: " << results.size() << "\n";
		std::cout << "results[0]:  " << results[0] << ", ";
		std::cout << "sizeof(results[0].first: " << typeid(results[0].first).name() << "\n";
		std::cout << std::regex_search("123,", std::regex("\\d")) << "\n";
		std::cout << std::regex_match("123,", std::regex("\\d")) << "\n";
		str = "Hello_2020!";
		pattern = std::regex("\\w*_");
		std::cout << std::regex_replace(str, pattern, "Hi, ") << "\n";
	}

	void JudgeNumber()
	{
		std::regex nonNegativeFloat("^\\d+(\\.\\d+)?$");
		std::regex r("^[1-9]\\d{0,2}\\.\\d*|^[1-9]\\d{0,2}|^0\\.\\d{1}|0$");
		//std::regex r("^[1-9]*$");
		std::cout << "Input number: " << "\n";
		std::string number;
		while (true) {
			std::cin >> number;
			//auto flag = std::regex_match(number, nonNegativeFloat);
			auto flag = std::regex_match(number, r);
			std::cout << std::boolalpha;
			std::cout << "Inputted is number: " << flag << "\n";
		}
	}

	void SplitString(const std::string file)
	{
		std::cout << file << "\n";
		std::ifstream ifs;
		ifs.open(file.c_str());

		assert(ifs.is_open());

		if (ifs.fail()) {
			std::string error = "Unable to read " + file;
			throw error;
		}

		std::string line;

		if (ifs) {
			while (!ifs.eof()) {
				std::getline(ifs, line);
				std::regex pattern("[\\S]+");
				std::sregex_token_iterator end;
				for (std::sregex_token_iterator i(line.begin(), line.end(), pattern); i != end; ++i) {
					std::cout << *i << "\n";
				}

			}
			ifs.close();
		}
		else {
			std::cout << file << " can not read." << "\n";
		}
	}

	void regex_iterator_demo01()
	{
		std::string str("This is subject has a submarine as a subSequence.");
		std::regex rgx("\\b(sub)([^ ]*)");
		std::regex_token_iterator<std::string::iterator> rend;
		std::regex_token_iterator<std::string::iterator> it(str.begin(), str.end(), rgx, 0);
		while (it != rend) {
			std::cout << it->str() << "\n";
			++it;
		}
		std::regex_iterator<std::string::const_iterator> it_s;
	}

	void regex_iterator_demo02()
	{
		std::string pattern("[^c]ei");
		pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
		std::regex r(pattern, std::regex::icase);
		std::string str("Ruby CarolineI biubiubiu Weindy SpikeI Winnceiy ");
		for (std::sregex_iterator it(str.begin(), str.end(), r), end_it; it != end_it; ++it) {
			std::cout << it->str() << "\n";
		}
		std::cout << std::endl;

		std::cout << "str: " << str << "\n";
		for (std::sregex_iterator it(str.begin(), str.end(), r), end_it; it != end_it; ++it) {
			auto pos = it->prefix().length();
			pos = pos > 40 ? pos - 40 : 0;
			std::cout << it->prefix().str().substr(pos) << "\n\t\t>>>" << it->str() << "<<<\n";
			std::cout << it->suffix().str().substr(0, 40) << "\n";
		}
	}

	// regex iterator 在对于大段文字进行遍历, 选择需要的匹配。 

	// 使用regex_iterator 提取html中的注释

	std::string html_string =
		"<html><body>Login Successful!</body><!-- EXTRACT-THIS --> \
		<p>Test</p> <!-- some documents --> <h2>Test2</h2> <!-- about documents --> </html>";

	void regex_iterator_demo03()
	{
		std::regex r("(<\\!--[^>]* -->)");

		std::sregex_iterator it(html_string.begin(), html_string.end(), r);
		std::sregex_iterator end_it;
		while (it != end_it) {
			std::smatch rst = *it;
			std::cout << rst.str() << "\n";
			++it;
		}
	}

	// 使用iterator_iterator 删除html中的注释 
	void regex_iterator_demo04()
	{
		std::regex r("(<\\!--[^>]*-->)");
		std::sregex_token_iterator it =
			std::sregex_token_iterator(html_string.begin(), html_string.end(), r, -1);
		std::sregex_token_iterator end;
		for (; it != end; ++it) {
			std::cout << "<TOKEN_ITERATOR>:  " << static_cast<std::string>(*it) << "\n";
		}
	}
}

int main(int argc, char* argv[])
{
	//regex_simple_demo::Demo01();
	//regex_simple_demo::Demo02();
	//regex_simple_demo::match_email();
	//regex_simple_demo::regex_iter();
	//regex_simple_demo::JudgeNumber();
	try {
		regex_simple_demo::SplitString("d:\\temp\\StudentInfo2.txt");
		//regex_simple_demo::SplitString("d:\\temp\\Config.ini");
	}
	catch (std::string& err) {
		std::cout << "function exec error: " << err << "\n";
	}

	//regex_simple_demo::regex_iterator_demo01();
	//regex_simple_demo::regex_iterator_demo02();
	regex_simple_demo::regex_iterator_demo03();
	regex_simple_demo::regex_iterator_demo04();
}
