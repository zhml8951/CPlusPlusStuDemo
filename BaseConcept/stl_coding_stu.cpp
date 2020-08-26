#include <iostream>
#include <new>

namespace allocator_demo
{
	/*
	 * allocator 允许将内存分配和初始化分离, allocator内部使用::operator new管理内存。
	 */
	template <typename Ty>
	class Allocator ///空间(memory)配置器, STL标准库默认使用的allocator;
	{
		using ValueType = Ty;
		typedef Ty* Pointer;
		typedef const Ty* ConstPointer;
		typedef Ty& Reference;
		typedef const Ty& ConstReference;

		template <typename Oth>
		struct Rebind
		{
			using Other = Allocator<Oth>;
		};

		constexpr Allocator() noexcept
		{
			/* Default constructor Allocator. (do nothing.)*/
		}

		constexpr Allocator(const Allocator&) noexcept = default;

		template <typename Other>
		constexpr Allocator(const Allocator<Other>&) noexcept
		{
			/*Constructor from related Allocator*/
		}

		/*
		 * operator new 与 new operator 是非常大的区别的。 new operator 是C++内建的，分配空间和调用构造函数一起完成的
		 *	同类还有::operator new[], ::operator delete,  ::operator delete[]
		 */

		void Deallocate(Ty* const ty_ptr, const size_t size)	/// 理清内存
		{
			::operator delete(ty_ptr);	/// 实质调用::operator
		} 

		Ty* Allocate(const size_t count)
		{
			std::set_new_handler(nullptr);
			Ty* tmp = static_cast<Ty*>(::operator new(static_cast<size_t>(count * sizeof(Ty))));
			if (tmp ==nullptr) {
				std::cerr << "Out of Memory." << std::endl;
				exit(1);
			}
			return tmp;
		}

	};

	/*
	 * STL随处可以的traits, 
	 */
}

int main(int argc, char* argv[]) { }
