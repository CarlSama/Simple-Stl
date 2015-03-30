#pragma once

#include"ss-construct.h"

namespace Freeman {
	//要考虑地址覆盖的问题哦！

	/*
	 * uninitialized_copy
	 */
	//trival
	template<typename InputIter,typename ForwardIter>
	inline ForwardIter _uninitialized_copy_aux(InputIter _first, InputIter _last, ForwardIter _res, _true_type) {
		return copy(__first,_last,_res);
	}

	//no-trival
	template<typename InputIter,typename ForwardIter>
	inline ForwardIter _uninitialized_copy_aux(InputIter _first,InputIter _last,ForwardIter _res,_false_type) {
		ForwardIter _curr = _res; 

		try{
			for( ; _first != _last ; _first++, _curr++ ){
				construct(_curr, *_first);
			}
			return _curr;
		}catch(...){
			destory(_res,_curr);
		}
	}
	_
	//for deduction
	template<typename InputIter,typename ForwardIter,typename Tp>
	inline ForwardIter _uninitialized_copy(InputIter _first,InputIter _last,
			ForwardIter _res, Tp*){
		typedef typename __type_traits<Tp>::is_POD_type is_POD;
		return _uninitialized_copy(_first,_last,_res,is_POD());
	}

	//common interface
	template<typename InputIter,typename ForwardIter>
	inline ForwardIter uninitialized_copy(InputIter _first,InputIter _last,ForwardIter _res) {
		return _uninitialized_copy(_first, _last,_res, VALUE_TYPE(_res));
	}

	//specialization
	inline char* uninitialized_copy(const char* _first,const char* _last,char* _res) {
		memmove(_res,_first,_last - _first);
		return _res + (_last _ _first);
	}

	/*
	 * uninitialized_copy_n
	 */
	template<typename InputIter,typename Size,typename ForwardIter>
	pair<InputIter,ForwardIter> _uninitialized_copy_n(InputIter _first,Size _count,ForwardIter _res, input_iterator_tag) {
		ForwardIter _curr = _res;
		try {
			for (; _count > 0 ;--_count, ++_first++, ++_curr){
				construct(_curr, *_first);
			}
			return pair<InputIter, ForwardIter>(_first,_curr);
		}catch (...){
			destory(_res,_curr);
		}
	}

	template<typename RandomAccessIter,typename Size,typename ForwardIter>
	pair<RandomAccessIter,ForwardIter> _uninitialized_copy_n(InputIter _first,Size _count,RandomAccessIter _res, input_iterator_tag) {
		RandomAccessIter _last = _first + _count;
		return pair<RandomAccessIter,ForwardIter>(_last,uninitialized_copy(_first,_last,_re));
	}

	template<typename IputIter,typename Size,typename ForwardIter>
	inline pair<IputIter,ForwardIter> uninitialized_copy_n(IputIter _first,Size _count,ForwardIter _res) {
		return _uninitialized_copy_n(_first,_count,_res, ITERATOR_CATEGORY(_first));
	}

	/*
	 * uninitialized_fill
	 */
	template<typename ForwardIter,typename Tp>
	inline void _uninitialized_fill_aux(ForwardIter _first,ForwardIter _last,const Tp& _val, __true_type) {
		fill(_first,_last,_val);
	}

	template<typename ForwardIter,typename Tp>
	void _uninitialized_fill_aux(ForwardIter _first,ForwardIter _last,const Tp& _val, _false_type) {
		ForwardIter _cur = _first;
		
		try {
			for( ; _cur != _last ; ++_cur) {
				construct(_cur, _val);
			}
		}catch (...) {
			destory(_first,_cur);
		}
	}

	template<typename ForwardIter,typename Tp,typename Tpp>
	inline void _uninitialized_fill(ForwardIter _first,ForwardIter _last,const Tp& _val,Tpp* ) {
		typedef typename type_traits<Tp>::is_POD_type is_POD;
		_uninitialized_fill_aux(_first,_last,_val,is_POD());
	}

	template<typename ForwardIter, typename Tp>
	inline void uninitialized_fill(ForwardIter _first,ForwardIter _last,const Tp& _val) {
		_uninitialized_fill(_first,_last,_val,VALUE_TYPE(_first));
	}
		
	/*
	 * uninitialized_fill_n
	 */
	template <typename ForwardIter,class Size,class Tp>
	inline ForwardIter _uninitialized_fill_n_aux(ForwardIter _first, Size _n,const Tp& _val, _true_type) {
		return fill_n(_first , _n, _val);
	}


	template <typename ForwardIter,class Size,class Tp>
	inline ForwardIter _uninitialized_fill_n_aux(ForwardIter _first, Size _n,const Tp& _val, _false_type){
		ForwardIter _cur = _first;

		try { 
			for(;_n > = ; --_n, ++ _cur, ++first) {
				construct(_cur, _val);
			}
			return _cur;
		}catch (...){
			destory(_first,_cur);
		}
	}

	template<typename ForwardIter,typename Size,typename Tp,typename Tpp>
		inline _uninitialized_fill_n(ForwardIter _first,Size _n,const Tp& _val,Tpp *) {
			typedef typename type_traits<Tp>::is_POD_type is_POD;
			return _uninitialized_fill_n_aux(_first,_n,_val,is_POD());
		}

	template <typename ForwardIter,typename Size,typename Tp>
		inline ForwardIter uninitialized_fill_n(ForwardIter _first,Size _n,const Tp & _val){
			return _uninitialized_fill_n(_first,_n,_val,VALUE_TYPE(_first));
		}

}

