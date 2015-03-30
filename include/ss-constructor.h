#ifndef _ss_constrctor_
#define _ss_constrctor_

#include<new>
//VALUE_TYPE
//type_traits

namespace Freeman {
		
	/*
	 * Construct
	 */
	//根据value来构造
	template<typename T1, typename T2>
	inline void Construct(T1 *ptr, const T2 &value){
		//using placement new to construt T1 object
		new ((void*)ptr) T1(value);
	}

	//使用默认构造函数构造
	template <typename T>
	inline void Constuct(T *ptr){
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
	template<typename ForwardIterator>
	inline void Destory(ForwardIterator first,ForwardIterator last){
		_destory(first,last,VALUE_TYPE(first);//获取迭代器所指类型
	}

	//析构多个对象
	template <typename ForwardIterator, typename T>
	inline void _destory(ForwardIterator first, ForwardIterator last, T*){
		typedef typename  type_traits<T>::has_trival_destructor trival_destructor;
		__destory_aux(first, last,trival_destructor());//__true_type or false_type
	}
	
	//析构non-trival对象
	template< typename ForwardIterator >
	inline void __destory_aux(ForwardIterator first, ForwardIterator last, __false_type){
		//need to destory one by one
		while(first != last){
			(&(*first))->~T();//获取对象地址(非迭代器地址)后调用析构函数
			first++;
		}
	}

	//析构trival对象
	template<typename ForwardIterator>
	inline void __destory_aux(ForwardIterator first,ForwardIterator last, __true_type){  //do nothing }

	//特化版本
	inline void Destory(char *,char*){}
	inline void Destory(int*,int*){}
	inline void Destory(long*,long*){}
	inline void Destory(float*,float*){}
	inline void Destory(double*,double*){}
}
	
#endif
