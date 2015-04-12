#ifndef _SS_ITERATOR_PRIVATE_
#define _SS_ITERATOR_PRIVATE_

#include <cstddef>

/*
 * 定义5种iterator类型
 * 为了编译时确定函数,编译器需要class type做overload resolution
 * 继承的方式可以表示迭代器能力的包含
 */
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

/*
 * 5种迭代器及其内部别名
 */
template<typename Tp,typename Distance>
struct input_iterator {
	typedef Tp value_type;
	typedef Distance difference_type;
	typedef Tp* pointer;
	typedef Tp& reference;
	typedef input_iterator_tag iterator_category;
};

template<typename Tp,typename Distance>
struct output_iterator {
	typedef Tp					value_type;
	typedef Distance			difference_type;
	typedef Tp*					pointer;
	typedef Tp&					reference;
	typedef output_iterator_tag iterator_category;
};

template<typename Tp,typename Distance>
struct forward_iterator {
	typedef Tp						value_type;
	typedef Distance				difference_type;
	typedef Tp*						pointer;
	typedef Tp&						reference;
	typedef forward_iterator_tag	iterator_category;
};

template<typename Tp,typename Distance>
struct bidirectional_iterator {
	typedef Tp value_type;
	typedef Distance difference_type;
	typedef Tp* pointer;
	typedef Tp& reference;
	typedef bidirectional_iterator_tag iterator_category;
};

template<typename Tp,typename Distance>
struct random_access_iterator {
	typedef Tp value_type;
	typedef Distance difference_type;
	typedef Tp* pointer;
	typedef Tp& reference;
	typedef random_access_iterator_tag iterator_category;
};

/*
 * 在迭代器中定义有别名
 * "萃取"的过程就是获得别名的过程
 */
template<typename Category, typename Tp, typename Distance = ptrdiff_t, typename Pointer = Tp*, typename Reference = Tp&>
struct iterator {
	typedef Tp			value_type;
	typedef Distance	difference_type;
	typedef Pointer		pointer;
	typedef Reference	reference;
	typedef Category	iterator_category;
};

/*
 * 萃取器获得iterator中的别名
 * 增加这一层，是为了对built-in type类型的指针做partial specialization
 */
template<typename Iterator>
struct iterator_traits {
	/*
	 * typename 指明_Iterator::iterator_category为类型名
	 * 因为iterator是template参数,在实例化前,
	 * 编译器无法确定iterator_category是type,member function or data member
	 * typename 告诉编译器这个一个type
	 */
	typedef typename Iterator::iterator_category	iterator_category;
	typedef typename Iterator::value_type			value_type;
	typedef typename Iterator::difference_type		difference_type;
	typedef typename Iterator::pointer				pointer;
	typedef typename Iterator::reference			reference;
};

//partial specialization for pointer
template <typename Tp>
struct iterator_traits<Tp*> {
	//原生指针是random_access_iterator
	typedef random_access_iterator_tag	iterator_category;
	typedef Tp							value_type;
	typedef ptrdiff_t					difference_type;
	typedef Tp*							pointer;
	typedef Tp&							reference;
};

//partial specialization for const pointer
template <typename Tp>
struct iterator_traits<const Tp*> {
	typedef random_access_iterator_tag	iterator_category;
	typedef Tp							value_type;
	typedef ptrdiff_t					difference_type;
	typedef Tp*							pointer;
	typedef Tp&							reference;
};

/*
 * Good idea here, makes me didn't have the heart to delete this :)
 * 未直接返回类型 -> 在面对较大类型时存在大量拷贝
 * 未返回引用类型 -> 引用不能对应no object,所以仍然需要建立局部对象
 * 返回指针类型   -> 1) 可能放置直接返回带来的大量拷贝
 *					 2) 指针可以指向no object,不需要建立对象
 */
/*template<typename Iterator>//通过参数来推断Iterator类型
inline typename Iterator::value_type* value_type(const Iterator&){
	return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}
*/

/*
 * 迭代器函数
 *
 * distance & advance
 */

//distance
template<typename InputIterator, typename Distance>
inline void __distance(InputIterator first, InputIterator last, Distance &n,input_iterator_tag tag){
	while(first != last){
		++n; ++first;
	}
}
template<typename RandomAccessIterator, typename Distance>
inline void __distance(RandomAccessIterator first, RandomAccessIterator last,Distance &n,random_access_iterator_tag tag){
	n = last - first;
}
template<typename InputIterator, typename Distance>
inline void distance(InputIterator first, InputIterator last, Distance &n){
	__distance(first,last,n,iterator_traits<InputIterator>(first));
}

template<typename InputIterator>
typename iterator_traits<InputIterator>::difference_type __distance(InputIterator begin,InputIterator end,input_iterator_tag) {
	typename iterator_traits<InputIterator>::difference_type len = 0;
	while(begin != end){
		++begin; ++len;
	}
	return len;
}
template<typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type __distance(RandomAccessIterator begin,RandomAccessIterator end) {
	return end - begin;
}
template<typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type distance(InputIterator begin,InputIterator end) {
	typename iterator_traits<InputIterator>::iterator_category category;
	return __distance(begin,end,category());
}

//advance
template<typename InputIterator, typename Distance>
inline void __advance(InputIterator curr, Distance n, input_iterator_tag) {
	while(n--){
		++curr;
	}
}

template<typename BidrectionalIterator, typename Distance>
inline void __advance(BidrectionalIterator curr, Distance n, bidirectional_iterator_tag) {
	if(n >= 0 ){//might be negative
		while(n -- ) 
			++curr;
	}else{
		while(n ++ ) 
			++curr;
	}
}

template<typename RandomAccessIterator,typename Distance>
inline void __advance(RandomAccessIterator curr, Distance n,random_access_iterator_tag){
	curr += n;
}

template<typename InputIterator,typename Distance>
inline void advance(InputIterator& iter,Distance n){
	__advance(iter, n,iterator_category(iter));
}

#endif
