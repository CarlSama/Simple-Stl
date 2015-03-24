#pragma once

#include<cnew>

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

//析构单个对象
template <typename T>
inline void destory(T *pointer){
	pointer->~T();
}

//析构多个对象
template <typename ForwardIterator>
inline void destory(ForwardIterator first, ForwardIterator last){
	__Destory(first,last);
}

//析构non-trival对象
template< typename ForwardIterator >
inline void __destory_aux(ForwardIterator first, ForwardIterator last, __false_type){
	//need to destory one by one
	while(first != last){
		destory(&(*first));
		first++;
	}
}

//析构trival对象
template <typename ForwardIterator>
inline void __destory_aux(ForwardIterator first,ForwardIterator last, __true_type){
	//do nothing
}

//析构多个对象
template <typename ForwardIterator, typename T>
inline void __destory(ForwardIterator first, ForwardIterator last, T*){
	//什么意思???
	typedef typename  __type_traits<T>::has_trival_destructor trival_destructor;
	__destory_aux(first, last,trival_destructor);
}

/*
 * 使用value_type来获取迭代器所指向对象的类型
 * 利用type_traits<T>判断该类型所包含函数的trival ?
 */
template<typename ForwardIterator>
inline void __Destory(ForwardIterator first,ForwardIterator last){
	//value_type
	__destory(first,last,VALUE_TYPE(first);
}

//特化版本
inline void _destory(char *,char*){}
inline void _destory(int*,int*){}
inline void _destory(long*,long*){}
inline void _destory(float*,float*){}
inline void _destory(double*,double*){}
