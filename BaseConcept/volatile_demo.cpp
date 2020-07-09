#include <cstdio>

// ReSharper disable CppUseAuto

// ### volatile 作用于编译阶段，并不作用于程序真正执行阶段的。 
void value_without_volatile()
{
	// 
	int i = 10;
	int a = i;
	printf("a = %d\n", a);

	__asm { // vs 内联汇编方式，采用__asm  GCC内嵌汇编使用asm("mov...) 
		mov dword ptr [ebp-4], 20h
	} // 使用asm直接使改了i值， 将i 变成32; 修改了i在内存中的值，又不让编译器知道。
	// c，c++ 内嵌汇编， 汇编代码直接被送到编译阶段， 编译器不进行处理，故这里__asm{....} 不进入编译， 
	// __asm{... } 直接修改了i的值， 但对编译器是无感知的。 

	int b = i;
	printf("b = %d\n", b);
	// 使用cl编译
	//##  cl /Od volatile_demo.cpp, 即Debug模式， 输出结果a=10, b=32; 
	//##  cl /O2 volatile_demo.cpp, 即Release模式， 输出结果 a=10, b=10;
	// 由于i的值由汇编直接修改对编译器是无感知的， 在Debug模式下，编译器对代码不进行优化， 使用i的时候都会去i的内存地址去取值，故可得修改后的i=32;
	// 而Release 模式下， 编译器会对代码进行优化， 这里i的变化对编译器无感知，故优化后b=i直接取a值，它认为是相同的。
	// 这里就引入volatile的作用， 当定义i:   volatile int i = 10; 每次取i值时都会去硬件寄存器去取，因为volatile表明i在每次读取时可能有不同意义。 
	// 这也是volatile 的易变的含义。
}

void value_with_volatile()
{
	// 加入volatile(易变) volatile 关键字类型修饰符，声明的变量表示可能被一些未知因素而更改(编译器优化，硬件或操作系统因素， 其它线程)， 
	// volatile 定义的变量， 提供一特殊地址保证稳定访问。
	volatile int i = 10;
	int a = i;
	printf("a = %d\n", a);
	__asm {
		mov dword ptr [ebp-4], 20h
	} // asm修改了i值，但编译器无感
	int b = i;
	printf("b = %d\n", b);
}

// volatile,  const 两相对应。使用方法也类似。 
// const char* cp_char;
// volatile char* vp_char;
// char* const pc_char, 
// char* volatile char* pv_char;   
// 这些声明的理解方式同const是相似的。 
// const char* 理解为<常量指针>即指针指向常量， 指针可再指向其它变量。 volatile char* 
// char* const 即<指针常量>即指 指针本身是常量，指针不可再指向其它变量。 char* volatile 

// volatile 多用于<多线程>中，当两个线程都要用到某一个变量，而且这个变量可能会被某线程修改， volatile的作用是防止优化编译器把变量从内存装入CPU寄存器，
// 变量被载入寄存器后，内存值和寄存器内值可能被线程修改，而不同线程可能有的从内存读取，而别的线程直接从寄存器读取，这会造成程序的错误执行。 
// volatile 是让编译器每次操作该变量都一定要从内存中真正取出， 而不使用寄存器中的值 。 


int main(int argc, char* argv[])
{
	value_without_volatile();
	value_with_volatile();
}
