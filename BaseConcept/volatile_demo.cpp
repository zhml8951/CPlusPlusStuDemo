#include <cstdio>

// ReSharper disable CppUseAuto

// ### volatile �����ڱ���׶Σ����������ڳ�������ִ�н׶εġ� 
void value_without_volatile()
{
	// 
	int i = 10;
	int a = i;
	printf("a = %d\n", a);

	__asm { // vs ������෽ʽ������__asm  GCC��Ƕ���ʹ��asm("mov...) 
		mov dword ptr [ebp-4], 20h
	} // ʹ��asmֱ��ʹ����iֵ�� ��i ���32; �޸���i���ڴ��е�ֵ���ֲ��ñ�����֪����
	// c��c++ ��Ƕ��࣬ ������ֱ�ӱ��͵�����׶Σ� �����������д���������__asm{....} ��������룬 
	// __asm{... } ֱ���޸���i��ֵ�� ���Ա��������޸�֪�ġ� 

	int b = i;
	printf("b = %d\n", b);
	// ʹ��cl����
	//##  cl /Od volatile_demo.cpp, ��Debugģʽ�� ������a=10, b=32; 
	//##  cl /O2 volatile_demo.cpp, ��Releaseģʽ�� ������ a=10, b=10;
	// ����i��ֵ�ɻ��ֱ���޸ĶԱ��������޸�֪�ģ� ��Debugģʽ�£��������Դ��벻�����Ż��� ʹ��i��ʱ�򶼻�ȥi���ڴ��ַȥȡֵ���ʿɵ��޸ĺ��i=32;
	// ��Release ģʽ�£� ��������Դ�������Ż��� ����i�ı仯�Ա������޸�֪�����Ż���b=iֱ��ȡaֵ������Ϊ����ͬ�ġ�
	// ���������volatile�����ã� ������i:   volatile int i = 10; ÿ��ȡiֵʱ����ȥӲ���Ĵ���ȥȡ����Ϊvolatile����i��ÿ�ζ�ȡʱ�����в�ͬ���塣 
	// ��Ҳ��volatile ���ױ�ĺ��塣
}

void value_with_volatile()
{
	// ����volatile(�ױ�) volatile �ؼ����������η��������ı�����ʾ���ܱ�һЩδ֪���ض�����(�������Ż���Ӳ�������ϵͳ���أ� �����߳�)�� 
	// volatile ����ı����� �ṩһ�����ַ��֤�ȶ����ʡ�
	volatile int i = 10;
	int a = i;
	printf("a = %d\n", a);
	__asm {
		mov dword ptr [ebp-4], 20h
	} // asm�޸���iֵ�����������޸�
	int b = i;
	printf("b = %d\n", b);
}

// volatile,  const �����Ӧ��ʹ�÷���Ҳ���ơ� 
// const char* cp_char;
// volatile char* vp_char;
// char* const pc_char, 
// char* volatile char* pv_char;   
// ��Щ��������ⷽʽͬconst�����Ƶġ� 
// const char* ���Ϊ<����ָ��>��ָ��ָ������ ָ�����ָ������������ volatile char* 
// char* const ��<ָ�볣��>��ָ ָ�뱾���ǳ�����ָ�벻����ָ������������ char* volatile 

// volatile ������<���߳�>�У��������̶߳�Ҫ�õ�ĳһ����������������������ܻᱻĳ�߳��޸ģ� volatile�������Ƿ�ֹ�Ż��������ѱ������ڴ�װ��CPU�Ĵ�����
// ����������Ĵ������ڴ�ֵ�ͼĴ�����ֵ���ܱ��߳��޸ģ�����ͬ�߳̿����еĴ��ڴ��ȡ��������߳�ֱ�ӴӼĴ�����ȡ�������ɳ���Ĵ���ִ�С� 
// volatile ���ñ�����ÿ�β����ñ�����һ��Ҫ���ڴ�������ȡ���� ����ʹ�üĴ����е�ֵ �� 


int main(int argc, char* argv[])
{
	value_without_volatile();
	value_with_volatile();
}
