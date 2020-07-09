#include <iostream>
#include <string>

// ReSharper disable CppUseAuto

/*
 * �������ú�ָ����ָ����һ���� ��Ȼ�����϶���ָ������ ��û�й涨����ָ�������һ���ǳ�����
 * ��ʹָ��ǳ�����Ҳ����ͨ���������ú�ָ����ָ�����޸���ָ�����ֵ������Ϊ������Ϊ�Լ�ָ����ǳ�����
 * �������ú�ָ����ָ����ָ����Ƿǳ������󣬿�ͨ���ǳ������������и�ֵ������
 */

/*
 * ����const, �ײ�const. ////
 * ����һ����������� ֻ�ж���const. ���磺 const string str="lll"; const int i = 0; ������constλ�����⣬
 * �磺 const string str="str", string const str="str" ��һ���ġ�  ���Ƕ���const, û�еײ�const
 * �������õ�const���ǵײ�const, ��Ϊ���ñ����Ƕ��� ���Բ����ڶ���const;
 * ָ��const��Ϊ����const�͵ײ�const, ����const��ʾָ�뱾���ǳ���������ָ�롣 �ײ�const��ʾָ��ָ����ǳ�����ָ������ָ�롣
 */

/*
 * �������ã�ָ����ָ������ں����βΣ��������β�����ҪĿ���Ƿ�ֹ�����޸�ʵ��ֵ����ƣ�Ҳ���ǵײ�constָ��ͳ������ã�
 * void func(const string* str1, const string& str2);
 * ����str1�� str2���Դ�����Ҳ���Դ��ǳ�����Ŀ����Ƿ�ֹ�����޸�ʵ�ε�ֵ��
 * const...* �ײ�const: ָ����ָ��, ...* const ����const��  ����ָ��
 * 
 */


namespace const_demo
{
	const int kA = 100;

	void func(const int* i, const std::string& str)
	{
	}

	void func2(int* const i1, const int* const i2)
	{
	}

	void Test01()
	{
		const int c = 3;
		int* p = const_cast<int*>(&c);
		// ����ͨ��ǿ��ת��const_cast<int*>(&c), (int*)&c ������ת����ָ��ǳ���ָ�룬��ͨ��ָ��ǳ���ָ�����޸�Ŀ������ֵ�ǲ��ܳɹ��ġ�
		auto p2 = &c;
		*p = 4; // ����ͨ��ָ��ǳ���ָ���޸�ֵ��ʵ���ǽ�pָ����һ����������tmp����cֵ�ǲ���ġ�
		printf("c: %d", c);
		printf("*p: %d", *p);

		p = const_cast<int*>(&kA); //��ָͨ��ָ��ȫ�ַǳ���Ч��ͬ�ֲ�������һ���ġ�

		printf("a=*p: %d", *p);
		*p = 200;
		printf("a=*p: %d", kA);

		func2(p, &c);	//
		//func2(&c, &c);		//�����Ǵ���ġ�
	}
}
