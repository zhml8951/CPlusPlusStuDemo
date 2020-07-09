#include "BitMapAlgo.h"

#include <iostream>

void bit_map::BitMap::set(const size_t num)
{
	const auto index = num >> 5; //�൱�� index = num / 32;
	const auto pos = num % 32; // pos = num % 32;
	this->data_[index] |= (1 << pos); //1λ����posλ��data_[index]����<λ��> ��ָ��λ��1��
}

void bit_map::BitMap::reset(const size_t num)
{
	const auto index = num >> 5;
	const auto pos = num % 32;
	this->data_[index] &= ~(1 << pos); // 1λ����posλ����
}

bool bit_map::BitMap::has_existed(const size_t num)
{
	const auto index = num >> 5;
	const auto pos = num % 32;
	auto flag = false;
	if (this->data_[index] & (1 << pos)) {
		flag = true;
	}
	return flag;
}

int main(int argc, char* argv[])
{
	size_t num = 888;
	std::cout << "num: " << num << ", num>>5:  " << (num >> 5) << "\n";
	num = 88;
	std::cout << "num: " << num << ", num>>5:  " << (num >> 5) << "\n";
	num = 32;
	std::cout << "num: " << num << ", num>>5:  " << (num >> 5) << "\n";
	std::cout << "(size_t)-1: " << size_t(-1) << "\n";
	std::cout << "static_cast<size_t>(-1): " << (static_cast<size_t>(-1)) << "\n";
	std::cout << "uint8(-1): " << std::hex << (+uint8_t(-1)) << "\n";
	std::cout << "uint8(-2): " << std::dec << unsigned(uint8_t(-2)) << "\n";
}