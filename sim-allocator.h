#pragma once

#include<new>
#include<cstddef>
#include<climits>
#include<iostream>

namespace Freeman{
	template< typename T>
		inline T* _allocate(ptrdiff_t size, T*){
			set_new_handler(0);
			try{
				//use golbal operator new to allocate heap memory 
				//Doesn't call constructor
				T *tmp = (T*)(::opertor new((size_t)(size * sizeof(T))));//global
				return tmp;
			}catch(std::bad_alloc& ba){
				std::cerr<<"bad_alloc catched : "<<ba.what()<<endl;
			}
		}

	template< typename T>
		inline void _deallocate(T *buffer){
			::operator delete(buffer);
		}

	template< typename T>
		inline void _construct(T1 *p, const T2& value){
			//using placement new to constrct object on given memory
			//1)	use T2 object to constuct a T1 object(temporary object)
			//2)	construct object on given memory
			// Am I right ?
			new (p) T1(value);
		}

	template< typename T>
		inline void _destory(T* ptr){
			ptr->~T();
		}

	template <typename T>
		class Allocator{
			public:
				//essential interface
				typedef T			value_type;
				typedef T*			pointer;
				typedef const T*	const_pointer;
				typedef size_t		size_type;
				typedef ptrdiff_t	difference_type;
				typedef T&			reference;
				typedef const T&	const_reference;

				template<typename U>
					struct rebind {
						typedef Allocator<U> other;
					};

				pointer allocate(size_type n ,const void* hint = 0){
					return _allocate((difference_type)(n), (pointer)(0));
				}

				void deallocate(pointer p,size_type n){
					_deallocate(p);
				}

				void construct(pointer p ,const T& value){
					_construct(p, value);
				}

				void destory(pointer p){
					_destory(p);
				}

				pointer address(reference x){
					return (pointer)(&x);
				}

				const_pointer const_address(const_reference x){
					return (const_pointer)(&x);
				}

				size_type max_size() const{
					return (size_type)(UINT_MAX / sizeofA(T));
				}

		};//end of class Allocator

}// end of namespace 
				






