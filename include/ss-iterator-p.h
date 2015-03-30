#pragma once

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template<typename _Category, typename _Tp, typename _Distance = ptrdiff_t,
	typename _Pointer = _Tp*, typename _Reference _Tp&>
struct iterator {
	typedef _Category iterator_category;
	typedef _Tp value_type;
	typedef _Distance difference_type;
	typedef _Pointer pointer;
	typedef _Reference reference;
};

template<typename _Iterator>
struct iterator_traits {
	typedef typename _Iterator::iterator_category iterator_category;
	typedef typename _Iterator::value_type value_type;
	typedef typename _Iterator::difference_type difference_type;
	typedef typename _Iterator::pointer pointer;
	typedef typename _Iterator::reference reference;
};

//partial specialization for pointer
template <typename _Tp>
struct iterator_traits<_Tp*> {
	typedef random_access_iterator_tag iterator_category;
	typedef _Tp	value_type;
	typedef ptrdiff_t difference_type;
	typedef _Tp* pointer;
	typedef _Tp& reference;
};

//partial specialization for const pointer
template <typename _Tp>
struct iterator_traits<const _Tp*> {
	typedef random_access_iterator_tag iterator_category;
	typedef _Tp	value_type;
	typedef ptrdiff_t difference_type;
	typedef _Tp* pointer;
	typedef _Tp& reference;
};

//获取迭代器的类型
//通过iterator来调用iterator_trarits,　然后通过iterator_traits来获取迭代器类型
template<typename _Iterator>
inline typename iterator_traits<_Iterator>::iterator_category 
	iterator_category(const _Iterator&){
		typedef typename iterator_traits<_Iterator>::iterator_category _category;
		return _category();
	}


//问题　：　为什么返回指针类型　？
template<typename _Iterator>
inline typename _Iterator::value_type* 
	value_type(const _Iterator&){
		return static_cast<typename iterator_trarits<_Iterator>::value_type*>(0);
	}


template<typename _Iterator>
inline typename _Iterator::difference_type*
	difference_type(const _Iterator&){
		return static_cast<typename iterator_trarits<_Iterator>::difference_type*>(0);
	}

//迭代器函数


//distance
template<typename _InputIterator, typename _Distance>
inline void _distance(_InputIterator _first, _InputIterator _last,
		_Distance &_n,input_iterator_tag _tag){
	while(_first != _last){
		n++;
		first++;
	}
}

template<typename _RandomAccessIterator, typename _Distance>
inline void _distance(_RandomAccessIterator _first, _RandomAccessIterator _last,
		_Distance &_n,input_iterator_tag _tag){
	n = _last - _first;
}

template<typename _InputIterator, typename _Distance>
inline void distance(_InputIterator _fist, _InputIterator _last, _Distance &_n){
	_distance(_first,_last,_n,iterator_traits(_first));
}


//advance
template<typename _InputIterator, typename _Distance>
inline void _advance(_InputIterator _curr, _Distance _n, input_iterator_tag) {
	while(_n--)
		_curr++;
}

template<typename _BidrectionalIterator, typename _Distance>
inline void _advance(_BidrectionalIterator _curr, _Distance _n, bidirectional_iterator_tag) {
	if(_n > = 0 ){
		while(_n -- )
			_curr++;
	}else{
		while(_n ++ )
			_curr--;
	}
}

template<typename _RandomAccessIterator,typename _Distance>
inline void _advance(_RandomAccessIterator _curr, _Distance _n,random_access_iterator_tag){
	_curr += _n;
}

template<typename _InputIterator,typename _Distance>
inline void advance(_InputIterator& _iter,_Distance _n){
	_advance(_iter, _n,iterator_category(_iter));
}

