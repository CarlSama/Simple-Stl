#ifndef _ss_constrctor_
#define _ss_constrctor_

#include<new>
#include"iterator-ext.h"
#include "iterator.h"
#include "type-traits.h"

namespace Freeman {
		
	/*
	 * Construct
	 */
	//根据value来构造
	template<typename T1, typename T2>
    inline void construct(T1 *ptr, const T2 &value){
		//using placement new to construt T1 object
		new ((void*)ptr) T1(value);
	}

	//使用默认构造函数构造
	template <typename T>
    inline void constuct(T *ptr){
		//using the default constructor of T
		new ((void*)ptr) T();
	}



	/*
	 * Destory
	 */
	/*
	 * 使用value_type来获取迭代器所指向对象的类型
	 * 析构多个对象,需要读,写->ForwardIterator
	 * 利用type_traits<T>判断该类型所包含函数的trival
	 */
	//析构non-trival对象
	template< typename ForwardIterator >
    inline void __destory(ForwardIterator first, ForwardIterator last, _false_type){
		//need to destory one by one
		while(first != last){
			(&(*first))->~T();//获取对象地址(非迭代器地址)后调用析构函数
			first++;
		}
	}
	//析构trival对象
	template<typename ForwardIterator>
    inline void __destory(ForwardIterator first,ForwardIterator last, _true_type){
		//do nothing 
	}
    template<typename ForwardIterator>
    inline void destory(ForwardIterator first,ForwardIterator last){
        typedef typename iterator_traits<ForwardIterator>::value_type type;
        typename type_traits<type>::has_trival_destructor trival_destructor;
        _destory(first,last,trival_destructor);//获取迭代器所指类型
    }

	//特化版本
	template<>
    inline void destory<char*>(char*,char*){}
	template<>
    inline void destory<int*>(int*,int*){}
	template<>
    inline void destory<long*>(long*,long*){}
	template<>
    inline void destory<float*>(float*,float*){}
	template<>
    inline void destory<double*>(double*,double*){}
}
	
#endif
