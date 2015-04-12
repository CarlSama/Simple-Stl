#ifndef _memory_
#define _memory_

#include "constructor.h"
#include "iterator.h"
#include "type-traits.h"
#include "cstring"
#include <utility>
#include "memory.h"
#include "simple-algorithm.h"

namespace Freeman {
	//Becare of overwrite
	
	/*
	 * The uninitialized_xx method was used on uninitialized memory
	 *
	 * There are four type of method names :
	 *		1) uninitialized_copy
	 *		2) uninitialized_copy_n
	 *		3) uninitialized_fill
	 *		4) uninitialized_fill_n
	 *
	 *	These method corresponds to copy , fill and fill_n in "simple-algorithm.h"
	 */

	/*
	 * uninitialized_copy
	 *
	 * Function : copy [first, last) to [res ...)
	 *
	 * Return Type : res + (last - first)
	 */
	//is POD
	template<typename InputIter,typename ForwardIter>
    inline ForwardIter _uninitialized_copy(InputIter _first, InputIter _last, ForwardIter _res, _true_type) {
		return copy(_first,_last,_res);
	}
	//not POD
	template<typename InputIter,typename ForwardIter>
    inline ForwardIter _uninitialized_copy(InputIter _first,InputIter _last,ForwardIter _res,_false_type) {
		ForwardIter _curr = _res; 
		try{
			while(_first != _last) {
				construct(_curr, *_first);
				++ _first;
				++ _last;
			}
			return _curr;
		}catch(...){
			destory(_res,_curr);
		}
	}
	//common interface
	//value_type -> Tp -> type_traits<Tp>::is_POD_type
	template<typename InputIter,typename ForwardIter>
	inline ForwardIter uninitialized_copy(InputIter _first,InputIter _last,ForwardIter _res) {
		typedef typename iterator_traits<ForwardIter>::value_type TYPE;
		typedef typename type_traits<TYPE>::is_POD_type isPOD;
        return _uninitialized_copy(_first,_last,_res,isPOD());
	}
	//specialization
	inline char* uninitialized_copy(const char* _first,const char* _last,char* _res) {
		memmove(_res,_first,_last - _first);
		return _res + (_last - _first);
	}



	/*
	 * uninitialized_copy_n
	 *
	 * Function : copy [first , first + n ) to [ res , res + n)
	 *
	 * Return type : std::pair<first + n , res + n>
	 */
	template<typename InputIter,typename Size,typename ForwardIter>
	std::pair<InputIter,ForwardIter> _uninitialized_copy_n(InputIter _first,Size _count,ForwardIter _res, input_iterator_tag) {
		ForwardIter _curr = _res;
		try {
			for (; _count > 0 ;--_count, ++_first, ++_curr){
				construct(_curr, *_first);
			}
			return std::pair<InputIter, ForwardIter>(_first,_curr);
		}catch (...){
			destory(_res,_curr);
		}
	}
	template<typename RandomAccessIter,typename Size,typename ForwardIter>
	std::pair<RandomAccessIter,ForwardIter> _uninitialized_copy_n(RandomAccessIter _first,Size _count,ForwardIter _res, random_access_iterator_tag) {
		RandomAccessIter _last = _first + _count;
		return std::pair<RandomAccessIter,ForwardIter>(_last,uninitialized_copy(_first,_last,_res));
	}
	template<typename IputIter,typename Size,typename ForwardIter>
	inline std::pair<IputIter,ForwardIter> uninitialized_copy_n(IputIter _first,Size _count,ForwardIter _res) {
		return _uninitialized_copy_n(_first,_count,_res, iterator_category(_first));
	}

	

	/*
	 * uninitialized_fill
	 *
	 * Function : use val to fill [first , last ) 
	 *
	 * Return type : void
	 */
	template<typename ForwardIter,typename Tp>
	inline void _uninitialized_fill(ForwardIter _first,ForwardIter _last,const Tp& _val, _true_type) {
		fill(_first,_last,_val);
	}
	template<typename ForwardIter,typename Tp>
	void _uninitialized_fill(ForwardIter _first,ForwardIter _last,const Tp& _val, _false_type) {
		ForwardIter _cur = _first;
		try {
			for( ; _cur != _last ; ++_cur) {
				construct(_cur, _val);
			}
		}catch (...) {
			destory(_first,_cur);
		}
	}
	template<typename ForwardIter, typename Tp>
	inline void uninitialized_fill(ForwardIter _first,ForwardIter _last,const Tp& _val) {
		typedef typename iterator_traits<ForwardIter>::value_type TYPE;
		typedef typename type_traits<TYPE>::is_POD_type isPOD;
		_uninitialized_fill(_first,_last,_val,isPOD());
	}
		


	/*
	 * uninitialized_fill_n
	 *
	 * Function : use val to fill [first, first + n)
	 *
	 * Return type : first + n
	 */
	template <typename ForwardIter,class Size,class Tp>
	inline ForwardIter _uninitialized_fill_n_aux(ForwardIter _first, Size _n,const Tp& _val, _true_type) {
		return fill_n(_first , _n, _val);
	}
	template <typename ForwardIter,class Size,class Tp>
	inline ForwardIter _uninitialized_fill_n_aux(ForwardIter _first, Size _n,const Tp& _val, _false_type){
		ForwardIter _cur = _first;
		try { 
			for(;_n >= 0 ; --_n, ++ _cur) {
				construct(_cur, _val);
			}
			return _cur;
		}catch (...){
			destory(_first,_cur);
		}
	}
	template <typename ForwardIter,typename Size,typename Tp>
		inline ForwardIter uninitialized_fill_n(ForwardIter _first,Size _n,const Tp & _val){
			typedef typename iterator_traits<ForwardIter>::value_type TYPE;
			typedef typename type_traits<TYPE>::is_POD_type isPOD;
			return _uninitialized_fill_n(_first,_n,_val,isPOD());
		}
}
#endif
