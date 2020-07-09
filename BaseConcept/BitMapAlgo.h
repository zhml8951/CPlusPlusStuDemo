#pragma once
#include <vector>

/*
 BitMap ���Bit��Ӧ�����ƣ�Mapͼ�� ������ͼ���������ݴ���������㷨, �����㷨Bloom Filter(��¡������)
	������� ��һ��bitλ�����ĳ��Ԫ�ض�Ӧ��value,��key���Ǹ�Ԫ�أ� �������ÿһ�����ݵ�ÿһ��������λ����ʾһ������
	0��ʾ�����ڣ� 1��ʾ���ڡ�����int 322; ����int����4Byte,Ҳ����32bits, 322>>5(322/32)=10, 322%32=2.
	BitMapλ��10������2λ�� ��vec[10] = 2 ��
 */

namespace bit_map
{
	class BitMap
	{
	public:
		explicit BitMap(const size_t num)
		{
			data_.reserve((num >> 5) + 1);
		}

		void set(size_t num);
		void reset(size_t num);
		bool has_existed(size_t num);
	private:
		std::vector<size_t> data_;
	};
}
