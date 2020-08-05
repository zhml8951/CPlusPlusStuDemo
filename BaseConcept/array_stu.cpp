#include <array>
#include <iostream>

/*
 * 1. 数组内存的存储方式：
	*a: 在java中，数组初始是引用类型，存于堆上。 这是固定的。 内存管理由jvm自动处理。
	*b: C++中，数组有静态与动态两类。
	* 静态数组：char ch[NUM]这里的NUM必须是常量。也就是编译时即可确定的。静态数组是在栈上分配， 静态数组使用sizeof可得实际占用内存容量
	* 动态数组：使用new, malloc在堆上分配， 指向数组的指针使用sizeof得到是指针大小。
	* 全局数组：存储于静态区(常量区)
	* 指针数组：本质还是一个数组，数组中每个元素都是一个指针。 char* pArray[10] 这里[] 运算优先级比*高，故pArray先与[]结合，是一个数组， 数组元素都是指针
	* 当数组作为函数的参数传递时，自动退化为同类型指针， 指向第一个元素。
	* 数组名与指针一样，都是指向元素的，通常指针是指针变量，而数组名是指针常量，它始终指向数组第一个元素。
	* 目前的理解是二级指针int** p 并不完全等价于arr[][];

 * 2. 指针与数组：
	# 指针数组， 元素都是指针的数组： int *ptr_arr[10]; 总体是数组,"存储指针的数组",多用于字符串，例如： char* names[] = {"arnni", "John"};
	# 数组指针， 指向数组的指针：   int (*arr_ptr)[10];  总体是指针,"指向数组，数组元素int"; 等同于 int (*)[10] arr_ptr;
	#
	// 指针函数 指针数组， 函数指针 数组指针 这些都有异曲同工之作。
 * 3. 指针与函数
 *  # 指针函数。 返回值为指指针的函数， 经常用到。 int* func(int, char*);
 *  # 函数指针。 指向函数的指针.  int(*func)(int, char*);
 */

// ReSharper disable CppUseAuto
namespace arr01
{
	// 数组指针做为返回类型。
	// 数组指针样式： int (*ptr)[10] 代换进去 int (*func(args...))[10] 这里args: int (&arr)[10] 数组引用
	int (* GetCpyArr(int (&arr)[10]))[10] /* 这就难看了。 换成 ->int(*)[10] 就好理解了。*/
	{
		int (*n)[10] = reinterpret_cast<int(*)[10]>(new int[10]);
		for (int i = 0; i < 10; i++) {
			(*n)[i] = arr[i];
		}
		return n;
	}

	// 方法2, 采用别名的方法来简化代码；(别名的定义方式有2种，typedef, using)
	typedef int IntArrP[10];
	typedef int IntArrR[10];
	using DoubleArrP = double[10];
	using DoubleArrR = double[10];

	IntArrP* GetCpyArr02(IntArrR& arr)
	{
		// ReSharper disable once CppCStyleCast
		// 下面3种定义方式是完全等效
		IntArrP* n = (int(*)[10])new int[10];
		IntArrP* n2 = reinterpret_cast<int(*)[10]>(new int[10]);
		const auto n3 = reinterpret_cast<IntArrP*>(new int[10]);

		for (int i = 0; i < 10; i++) {
			(*n)[i] = arr[i] + 100;
		}
		delete[] n2;
		delete[] n3;
		return n;
	}

	// 方法3 采用 auto func(arg...) -> int (*) [10]

	auto GetCpyArr03(const double (&arr)[10]) -> double(*)[10]
	{
		using DoubleArrPtr = double(*)[10];
		using DoubleArrRef = double(&)[10];

		const DoubleArrPtr n_arr = reinterpret_cast<DoubleArrPtr>(new double[20]);
		for (int i = 0; i < 10; i++) {
			(*n_arr)[i] = arr[i] + 1000.0;
		}
		return n_arr;
	}

	void ArrayPointTest()
	{
		int arr01[20][10];
		for (int y = 0, i = 100; y < 20; y++, i += 10) {
			for (int x = 0; x < 10; x++) {
				arr01[y][x] = i + x;
			}
		}

		int (*p_arr01)[10] = arr01;
		for (int i = 0; i < 10; i++) {
			std::cout << (*p_arr01)[i] << "\n";
		}

		// 使用数组指针编历全部元素
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 10; x++) {
				std::cout << (*(p_arr01 + y))[x] << "  ";
			}
			printf("\n");
		}

		// 指针数组最好理解的用法则在于用数组存储char* 的字符串。
		const char* arr_str[] = {"str01", "str02"};
		for (int i = 0; i < (sizeof(arr_str) / sizeof(arr_str[0])); i++) {
			std::cout << "str:  " << arr_str[i] << "\n";
		}

		int* p_int[20];
		for (int i = 0; i < 20; i++) {
			p_int[i] = arr01[i];
		}

		printf("Use int* p[]. \n");
		int* pi = nullptr;
		for (int i = 0; i < 20; i++) {
			pi = p_int[i];
			for (int j = 0; j < 10; j++) {
				printf("%d  ", *pi);
				pi++;
			}
			printf("\n");
		}

		printf("\nTest GetCpyArr\n");
		const auto p_cpy_arr = GetCpyArr(arr01[10]);
		pi = nullptr;
		for (int i = 0; i < 20; i++) {
			pi = p_cpy_arr[i];
			for (int j = 0; j < 10; j++) {
				printf("%d  ", *pi);
				pi++;
			}
			printf("\n");
		}
	}
}

/*
 *  # 二维数组 行(row)Y, 列(col)X 在一般固定行和固定列的数组再转 指针数组或数组指针 的意义不大。 
 *  # 当行(Y)固定，列长不一定采用指针数组，这时应用指针数组如： const char* names[4] = {"John", "Green", "Arnni", "pic"}. 这里的4是行数，
 *  # 当列(X)固定，行不固定时采用数组指针， int (*Sex)[3]; 主要作用是将数组降级到数组，便于有时传参使用。如下：
  	int ar[][3] = {{3, 4, 5}, {33, 44, 55}};
	int (*sex)[3] = ar;
	int ar2[][3] = {{11, 22, 33}, {44, 55, 66}, {77, 88, 99}};
	sex = ar2; 
 *  
 */

int main(int argc, char* argv[])
{
	arr01::ArrayPointTest();
}
