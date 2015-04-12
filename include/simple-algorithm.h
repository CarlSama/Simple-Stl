#ifndef _ss_algorithm_base_
#define _ss_algorithm_base_

#include "iterator.h"
#include <utility>

namespace Freeman {



	/*
	 * Function : Check two range for equality
	 *
	 * Return type : bool
	 */
	template<typename InputIterator1,typename InputIterator2>
	inline bool equal(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2){
		while(begin1 != end1){ 
			if( *begin1 != *begin2)
				return false;
		}
		return true;
	}
	template<typename InputIterator1,typename InputIterator2,typename BinaryPredicate>
	inline bool equal(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,BinaryPredicate bp){
		while(begin1 != end1){ 
			if( !bp(*begin1, *begin2) )
				return false;
		}
		return true;
	}



	/*
	 * Function : fill new val on given interval
	 *
	 * Return type : void
	 */
	template<typename InputIterator1,typename Tp>
	inline void fill(InputIterator1 begin, InputIterator1 end, const Tp &val) {
		while(begin != end) {
			*begin = val;
			++begin;
		}
	}



	/*
	 * Function : fill new val to given range
	 *
	 * Return type : void 
	 */
	template<typename InputIterator1,typename Tp,typename Size>
	inline void fill_n(InputIterator1 begin,Size n, const Tp& val) {
		while( n > 0) {
			*begin = val;
			--n;
			++begin;
		}
	}



	/*
	 * Function : swapt iterator
	 *
	 * Return type : void
	 */
	template<typename ForwardIterator1, typename ForwardIterator2>
	inline void iter_swap(ForwardIterator1 iter1, ForwardIterator2 iter2) {
		typename iterator_traits<ForwardIterator1>::value_type tmp = *iter1;
		*iter1 = *iter2;
		*iter2 = tmp;
	}

	
	
	/*
	 * Function : compare two arr with dict order. ( Attention : 'h' > 'H')
	 *
	 * Return : bool (Is arr1 less than arr2 ?)
	 */
	template<typename InputIterator1, typename InputIterator2>
	inline bool lexicographical_compare(InputIterator1 begin1, InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2) {
		while(begin1 != end1 && begin2 != end2){
			if( *begin1 > *begin2 )
				return false;
			else if( *begin1 < *begin2 )
				return true;
			else {
				++begin1;	++begin2;
			}
		}
		return begin1 == end1 && begin2 != end2;
	}
	template<typename InputIterator1, typename InputIterator2,typename Compare>
	inline bool lexicographical_compare(InputIterator1 begin1, InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2,Compare comp) {
		while(begin1 != end1 && begin2 != end2){
			if( comp(*begin1 , *begin2) )
				return false;
			else if( !comp(*begin1 , *begin2 ) )
				return true;
			else {
				++begin1;	++begin2;
			}
		}
		return begin1 == end1 && begin2 != end2;
	}



	/*
	 * Function : min max
	 *
	 * Return type : min max
	 */
	template<typename Tp>
	inline Tp& min(const Tp& a, const Tp& b ){
		return a < b ? a : b;
	}
	template<typename Tp>
	inline Tp& max(const Tp& a,const Tp& b) {
		return b > a ? b : a;
	}



	/*
	 * Function : Find the first mis-match position (Attention : user must guarantee the arr2 is no shorter than arr1)
	 *
	 * Return type : the first mismatch positoin
	 */
	template<typename InputIterator1,typename InputIterator2>
	inline std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 begin1, InputIterator1 end1, InputIterator2 begin2) {
		while(begin1 != end1) {
			if( *begin1 != *begin2 ) {
				return std::pair<InputIterator1, InputIterator2>(begin1, begin2);
			}
			++begin1; ++begin2;
		}
		return std::pair<InputIterator1, InputIterator2>(begin1, begin2);
	}
	template<typename InputIterator1,typename InputIterator2,typename BinaryPredicate>
	inline std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 begin1, InputIterator1 end1, InputIterator2 begin2,BinaryPredicate bp) {
		while(begin1 != end1) {
			if( ! bp(*begin1 , *begin2 ) ) {
				return std::pair<InputIterator1, InputIterator2>(begin1, begin2);
			}
			++begin1; ++begin2;
		}
		return std::pair<InputIterator1, InputIterator2>(begin1, begin2);
	}



	/*
	 * Function :swap
	 *
	 * Return type : void
	 */
	template<typename Tp>
	inline void swap(Tp& a,Tp& b) {
		Tp tmp = a;  a = b;  b = tmp;
	}



	/*
	 *  Function : copy [begin,end) to [dest, xx)  
	 *
	 *  Warning : it's the programmer job to avoid using this method in overlapped situation
	 *
	 *	Return type : the end of dest(xx)
	 */

	template<typename InputIterator,typename OutputIterator>
	inline OutputIterator copy_dispatch(InputIterator begin,InputIterator end,OutputIterator dest, input_iterator_tag) {
		if(begin == end || begin == dest) return dest;
		while(begin != end) {//In InputIterator, we have to ( move & compare ) the iterator
			*dest = *begin;
			++begin; ++dest;
		}
		return dest;
	}
	template<typename InputIterator,typename OutputIterator>
	inline OutputIterator copy_dispatch(InputIterator begin, InputIterator end, OutputIterator dest, random_access_iterator_tag) {
		if(begin == end || begin == dest) return dest;

		typename iterator_traits<InputIterator>::difference_type n = end - begin + 1;
		while(--n) { // Using number is faster for loop check, that's the benefit cause by random_access_iterator
			*dest = *begin;
			++dest; ++begin;
		}
		return dest;
	}
	template<typename InputIterator, typename OutputIterator>
	inline OutputIterator copy(InputIterator begin,InputIterator end, OutputIterator dest) {
		typename iterator_traits<InputIterator>::iterator_category ic;
		return copy_dispatch(begin, end, dest,ic );
	}
	/*		We consider the overlap case, there is four case (begin -> end ) copy to (destBegin -> destEnd)
	 *		1) destEnd > begin (not overlapped)
	 *		2) destBegin == begin (full overlapped)
	 *		3) destBegin > begin && destBegin < end (partly overlapped, need back copy)
	 *		4) destBegin >= end (not overlapped)
	 */
	template<>
	inline char* copy(char *begin,char *end, char *dest){
		if(begin == end || begin == dest) return dest;

		if(dest > begin && dest < end) {//need back copy
			char *destEnd = dest + (end - begin);
			for( --end,--destEnd; end >= begin; --end,--destEnd) {
				*destEnd = *end;
			}
		}else{
			for(; begin != end; ++begin, ++end) {
				*dest = *begin;
			}
		}
		return dest + (end - begin);
	}



	/*
	 * Function : get the union of two set
	 *
	 * Return type : the begining of union
	 */
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_union(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2,OutputIterator dest) {
		while(begin1 != end1 && begin2 != end2) {
			if(*begin1 < *begin2){
				*dest = *begin1;
				++begin1;
			}else if(*begin1 > *begin2) {
				*dest = *begin2;
				++begin2;
			}else {
				*dest = *begin1;
				++begin1; ++begin2;
			}
			++dest;
		}
		return copy(begin1,end1,copy(begin2,end2,dest));
	}



	/*
	 * Function : get the intersection of two set
	 *
	 * Return type : the begining of intersection
	 */
	template<typename InputIterator1,typename InputIterator2,typename OutputIterator>
	inline OutputIterator set_intersection(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2,OutputIterator dest){
		while(begin1 != end1 && begin2 != end2){
			if(*begin1 < *begin2){
				++begin1;
			}else if(*begin1 > *begin2){
				++begin2;
			}else{
				*dest = *begin1;
				++dest;	  ++begin1;	  ++begin2;
			}
		}
		return dest;
	}



	/*
	 * Function : get the unique part of first set
	 *
	 * Return type : the begining of difference
	 */
	template<typename InputIterator1,typename InputIterator2,typename OutputIterator>
	inline OutputIterator set_difference(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2,OutputIterator dest){
		while(begin1 != end1 && begin2 != end2) {
			if(*begin1 < *begin2){
				*dest = *begin1;
				++dest;  ++begin1;
			}else if(*begin1 > *begin2){
				++begin2;
			}else {
				++begin1; ++begin2;
			}
		}
		return copy(begin1,end1,dest);
	}



	/*
	 * Function : get the unique part of each set
	 *
	 * Return type : the begining of difference
	 */
	template<typename InputIterator1,typename InputIterator2,typename OutputIterator>
	OutputIterator set_symmetric_difference(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2,OutputIterator dest){
		while(begin1 != end1 && begin2 != end2) {
			if(*begin1 < *begin2){
				*dest = *begin1;
				++begin1;	++dest;
			}else if(*begin1 > *begin2){
				*dest = *begin2;
				++begin2;	++dest;
			}else{
				++begin1;	++begin2;
			}
		}

		return copy(begin1,begin2,copy(begin2,end2,dest));
	}



	/*
	 * Function : find the adjacent elem witch meets the BinaryPredicate
	 *
	 * Return type : the first elem
	 */
	template<typename ForwardIterator,typename BinaryPredicate>
	ForwardIterator adjacent_find(ForwardIterator begin,ForwardIterator end,BinaryPredicate bp){
		ForwardIterator prev = begin;
		while(++begin != prev) {
			if(bp(*begin, *prev))
				return begin;
			prev = begin;
		}
		return end;
	}




	/*
	 * Function : count the val appears in [begin ,end)
	 *
	 * Return type : number
	 */
	template<typename InputIterator,typename Tp>
	typename iterator_traits<InputIterator>::difference_type count(InputIterator begin, InputIterator end,const Tp& val) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		while(begin != end) {
			if(*begin == val) { ++n; }
		}
		return n;
	}
	template<typename InputIterator, typename Predicate>
	typename iterator_traits<InputIterator>::difference_type count(InputIterator begin, InputIterator end,Predicate pred) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		while(begin != end) {
			if( pred( *begin ) ) {
				++n;
			}
		}
		return n;
	}



	/*
	 * Function : find the elem appear in [begin,end)
	 *
	 * Return type : position
	 */
	template<typename InputIterator,typename Tp>
	InputIterator find(InputIterator begin,InputIterator end,const Tp& val) {
		while(begin != end && *begin != val){
			++begin;
		}
		return begin;
	}
	template<typename InputIterator,typename Predicaete>
	InputIterator find_if(InputIterator begin,InputIterator end,Predicaete pred) {
		while(begin != end && !pred(*begin)) {
			++begin;
		}
		return begin;
	}
	template<typename InputIterator1,typename InputIterator2>
	InputIterator1 find_first_of(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2){
		while(begin1 != end1) {
			InputIterator1 tmp1 = begin1;
			InputIterator2 tmp2 = begin2;

			while(tmp2 != end2 && *tmp2 == *tmp1){
				++tmp1;	++tmp2;
			}

			if(tmp2 == end2)
				return begin1;
			else
				++begin1;
		}
		return end1;
	}

	template<typename InputIter1,typename InputIter2>
	InputIter1 _find_last_of(InputIter1 begin1,InputIter1 end1,InputIter2 begin2,InputIter2 end2,input_iterator_tag,input_iterator_tag) {
		if(begin1 == end1) return end1;

		InputIter1 result = end1;
		InputIter1 curr = begin1;
		while(curr != end1) {//we have to search from begining in InputIterator
			curr = search(begin1,end1,begin2,end2);

			if(curr != end1){
				result = curr;
				begin1 = ++ curr;
			}
		}
		return result;
	}

	template<typename BidirectionalIterator1,typename BidirectionalIterator2>
	BidirectionalIterator1 _find_last_of(BidirectionalIterator1 begin1,BidirectionalIterator1 end1,BidirectionalIterator2 begin2,BidirectionalIterator2 end2,bidirectional_iterator_tag,bidirectional_iterator_tag) {//it's awesome that we can search from back in BidirectionalIterator
		typedef reverse_iterator<BidirectionalIterator1> reverse_bidirect_1;
		typedef reverse_iterator<BidirectionalIterator2> reverse_bidirect_2;

		reverse_bidirect_1 rbegin1(end1), rend1(begin1);
		reverse_bidirect_2 rbegin2(end2), rend2(begin2);

		reverse_bidirect_1 revese_result = search(rbegin1,rend1,rbegin2,rend2);
		if(revese_result == rend1)
			return end1;
		else {
			BidirectionalIterator1 result = revese_result.base();
			advance(result, -1 * distance(begin2,end2));
			return result;
		}
	}
	template<typename InputIterator1,typename InputIterator2>
	InputIterator1 find_last_of(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2) {
		typename iterator_traits<InputIterator1>::iterator_category ic1;
		typename iterator_traits<InputIterator2>::iterator_category ic2;
		return _find_last_of(begin1,end1,begin2,end2,ic1,ic2);
	}



	/*
	 * Function : traverase the [begin,end) with Function
	 *
	 * Return type : function
	 */
	template<typename InputIterator,typename Function>
	inline Function for_each(InputIterator begin,InputIterator end,Function func) {
		while(begin != end) {
			func(*begin);
		}
		return func;
	}



	/*
	 * Function : asssign the interval [begin, end) with value genenrate from Generator
	 *
	 * Return type : void
	 */
	template<typename ForwardIterator,typename Generator>
	inline void generate(ForwardIterator begin,ForwardIterator end,Generator gen) {
		while(begin != end) {
			*begin = gen();
		}
	}



	/*
	 * Function : check wether second set was contains in first set 
	 *
	 * Attention : Only makes sense when used on sorted elements
	 *
	 * Return : bool
	 */
	template<typename InputIterator1,typename InputIterator2>
	bool includes(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2) {
		while(begin1 !=  end1){
			if(*begin1 < *begin2){
				++begin1;
			}else if(*begin1 > *begin2){
				return false;
			}else{
				++begin1; ++begin2;
			}
		}
		return begin2 == end2;
	}



	/*
	 * Function : get the max or min elem 
	 *
	 * Return type : position
	 */
	template<typename InputIterator>
	InputIterator max_element(InputIterator begin,InputIterator end){
		InputIterator res = begin;
		while(++begin != end) {
			if(*begin > *res)
				res = begin;
		}
		return res;
	}
	template<typename InputIterator>
	InputIterator min_element(InputIterator begin,InputIterator end) {
		InputIterator res = begin;
		while(++begin!=end) {
			if(*begin < *res)
				res = begin;
		}
		return res;
	}



	/*
	 * Function : partition the array ,which would make the elem satisfy pred() all locate before elem don't
	 *
	 * Return type : the first elem that not satisfy pred()
	 */
	template<typename BidirectionalIterator,typename Predicate>
	BidirectionalIterator partition(BidirectionalIterator begin, BidirectionalIterator end,Predicate pred) {
		BidirectionalIterator minPtr = --begin ;
		while(begin != end) {
			if( pred(*begin) ){
				swap(*begin, *(++minPtr));
			}
			++begin;
		}
		return ++minPtr;//return the first element that not satisfy particular case
	}



	/*
	 * Function : remove the elem with value 
	 *
	 * Return type : the end of removed array
	 */
	template<typename ForwardIterator, typename Tp>
	ForwardIterator remove(ForwardIterator begin, ForwardIterator end, const Tp& value){
		begin = find(begin,end,value);
		ForwardIterator next = begin;

		return begin == end ? begin : remove_copy(++next,end,begin,value);
	}
	template<typename InputIterator, typename OutputIterator, typename Tp>
	InputIterator remove_copy(InputIterator begin,InputIterator end,OutputIterator result,const Tp& val) {
		while(begin != end) {
			if(*begin != val){
				*result = *begin;
				++result;
			}
			++begin;
		}
		return result;
	}

	template<typename ForwardIterator, typename Predicate>
	ForwardIterator remove_if(ForwardIterator begin,ForwardIterator end, Predicate pred) {
		begin = find_if(begin,end,pred);
		ForwardIterator next = begin;

		return begin == end ? begin : remove_copy_if(++next,end,begin,pred);
	}
	template<typename InputIterator,typename OutputIterator,typename Predicate>
	InputIterator remove_copy_if(InputIterator begin,InputIterator end, OutputIterator result,Predicate pred) {
		while(begin != end) {
			if( pred(*begin) ){
				*result = *begin;
				++result;
			}
			++begin;
		}
		return begin;
	}



	/*
	 * Function : replace old_value with new_value
	 *
	 * Return type : the end of replaced array
	 */
	template<typename ForwardIterator,typename Tp>
	void replace(ForwardIterator begin,ForwardIterator end,const Tp& old_value, const Tp& new_value) {
		while(begin != end) {
			if(*begin == old_value)
				*begin = new_value;
			++begin;
		}
	}
	template<typename InputIterator,typename OutputIterator,typename Tp>
	OutputIterator replace_copy(InputIterator begin,InputIterator end,OutputIterator result,const Tp& old_value,const Tp& new_value) {
		while(begin != end) {
			*result = (*begin == old_value) ? new_value : *begin;
			++begin; ++result;
		}
		return result;
	}

	template<typename ForwardIterator,typename Predicate ,typename Tp>
	void replace_if(ForwardIterator begin,ForwardIterator end,Predicate pred,const Tp& value) {
		while(begin != end) {
			if(pred(*begin))
				*begin = value;
			++begin;
		}
	}
	template<typename InputIterator,typename OutputIterator, typename Predicate,typename Tp>
	OutputIterator replace_copy_if(InputIterator begin,InputIterator end,OutputIterator dest, const Tp& value) {
		while(begin != end) {
			*dest = pred(*begin) ? value : *begin;
			++begin;  ++dest;
		}
		return dest;
	}



	/*
	 * Function : reverse the given [begin,end)
	 * 
	 * Return type : void 
	 */
	template<typename BidirectionalIterator>
	inline void __reverse(BidirectionalIterator begin,BidirectionalIterator end,bidirectional_iterator_tag) {
		while(begin != end && begin != --end){
			iter_swap(begin, end);
			++begin;
		}
	}
	template<typename RandomAccessIterator>
	inline void __reverse(RandomAccessIterator begin,RandomAccessIterator end,random_access_iterator_tag) {
		while(begin < end) {
			iter_swap(begin, end);
		}
	}
	template<typename BidirectionalIterator>
	inline void reverse(BidirectionalIterator begin,BidirectionalIterator end){
		typename iterator_traits<BidirectionalIterator>::iterator_category ic;
		__reverse(begin,end, ic);
	}


	
	/*
	 * Function : reverse copy
	 *
	 * Return type : dest
	 */
	template<typename BidirectionalIterator,typename OutputIterator>
	OutputIterator reverse_copy(BidirectionalIterator begin,BidirectionalIterator end,OutputIterator dest) {
		while( end != begin ) {
			*dest = * (-- end);
			++dest;
		}
		return dest;
	}



	/*
	 * Function : rotate [begin ---- mid ---- end) to [mid ---- end-1 ---- begin new_end)
	 *
	 * Return type : the begining of rotated array
	 */
	template<typename ForwardIterator,typename Distance>
	void __rotate(ForwardIterator begin,ForwardIterator mid,ForwardIterator end,Distance*,forward_iterator_tag) {
		//	Recursively swap two range, which may occurs the front longer or shorter than the back
		if(begin == mid) return end;
		if(mid == end)   return begin;

		ForwardIterator begin_for_back = mid;
		while(begin_for_back != end && begin != mid) {
			while(begin != mid && begin_for_back != end){
				iter_swap(begin, begin_for_back);
				++begin; ++begin_for_back;
			}
			if(begin == mid) {//the front segment has finished
				mid = begin_for_back;
			}else {// the back segment has finished
				begin_for_back = mid;
			}
		}
	}

	template<typename BidirectionalIterator,typename Distance>
	void __rotate(BidirectionalIterator begin,BidirectionalIterator mid,BidirectionalIterator end,Distance*,bidirectional_iterator_tag) {
		// Reverse each interval and reverse together
		// When manage the string with length of x, this needs x times swap, which means 3x times assignment
		reverse(begin,mid);
		reverse(mid,end);
		reverse(begin,end);
	}

	//求最大公因子 -->  辗转相除法
	template<typename EuclideanRingElement>
	EuclideanRingElement gcd(EuclideanRingElement m ,EuclideanRingElement n) {
		// gcd(m,n) == gcd( n, m % n );
		EuclideanRingElement t;
		while( n != 0) {
			t = m % n;//the first step of this would makes m bigger than n
			m = n;
			n = t;
		}
		return m;
	}
	template<typename RandomAccessIterator,typename Distance>
	void __rotate(RandomAccessIterator begin,RandomAccessIterator mid,RandomAccessIterator end,Distance*,random_access_iterator_tag) {
		//Only RandomAccessIterator have the ability to add/sub between iterators
		Distance n = gcd(end - begin, mid - begin);
		while( n -- ){
			typename iterator_traits<RandomAccessIterator>::value_type value = *(begin + n);
			RandomAccessIterator initial = begin + n;
			Distance shift = mid - begin;
			RandomAccessIterator before = initial;
			RandomAccessIterator after = initial + shift;
			while(after != initial) {
				*before = *after;
				before = after;
				if(after + shift < end){
					after = after + shift;
				}else{
					after = begin + ( shift - (end - after));
				}
			}
				*before = value;
		}
	}
	template<typename ForwardIterator>
	void rotate(ForwardIterator begin,ForwardIterator mid,ForwardIterator end) {
		if(begin == mid || mid == end) return ;

		typename iterator_traits<ForwardIterator>::difference_type dt;
		typename iterator_traits<ForwardIterator>::iterator_category ic;
		__rotate(begin,mid,end,dt,ic);
	}



	/*
	 * Function : copy [begin --- mid --- end) to [mid --- end-1 --- begin new_end)
	 *
	 * Return type : the end of new place 
	 */
	template<typename ForwardIterator, typename OutputIterator>
	OutputIterator rotate_copy(ForwardIterator begin,ForwardIterator mid,ForwardIterator end,OutputIterator result) {
		return copy(begin,mid,copy(mid, end,result));
	}



	/*
	 * search
	 */
	template<typename  ForwardIterator1,typename ForwardIterator2,typename Distance1,typename Distance2>
	inline ForwardIterator1 __search(ForwardIterator1 begin1,ForwardIterator1 end1,ForwardIterator2 begin2,ForwardIterator2 end2,Distance1*,Distance2*) {
		Distance1 d1 = 0;
		distance(begin1,end1,d1);
		Distance2 d2 = 0;
		distance(begin2,end2,d2);

		if(d1 < d2) return end1;

		ForwardIterator1 curr1 = begin1;
		ForwardIterator2 curr2 = begin2;
		while(curr1 != end1) {
			 if( *curr1 == *curr2) {
				 ++curr1;  ++curr2;
			 }else{
				 if(d1 == d2) //d1会根据查找的过程而缩减
					 return false;
				 else {
					 curr1 = ++begin1;//增进begin1并赋值给curr1
					 curr2 = begin2;//curr2重定位到source的开头
					 --d1;
				 }
			 }
		}
		return end1;
	}
	template<typename ForwardIterator1,typename ForwardIterator2>
	inline ForwardIterator1 search(ForwardIterator1 begin1,ForwardIterator1 end1,ForwardIterator2 begin2,ForwardIterator2 end2) {
		if(begin1 == end1 || begin2 == end2) return begin1;//empty

		ForwardIterator2 tmp = begin2;//only one 
		++tmp;
		if(tmp == end2) {
			return find(begin1,end1,*begin2);
		}

		while(begin1 != end1){
			begin1 = find(begin1,end1,*begin2);
			if(begin1 == end1)
				return begin1;

			ForwardIterator1 curr1 = begin1 + 1;
			ForwardIterator2 curr2 = begin2 + 1;
			if(curr2 == end2)//we already handle the only one elem in arr2
				return end1;//So, this must be wrong
			while(curr1 != end1 && curr2 != end2){
				if(++curr2 == end2){
					return begin1;
				}
				if(++curr1 == end1){
					return end1;
				}
			}

			++begin1;
		}
		return end1;
	}


	/*
	 * Function : find the first adjacent n elem with particular value
	 *
	 * Return type : the begining of adjacent or end (not found)
	 */
	template<typename ForwardIterator,typename Integer,typename Tp>
	ForwardIterator search_n(ForwardIterator begin,ForwardIterator end,Integer count,const Tp& value) {
		if(count < 0) return end;
		begin = find(begin ,end, value);
		while(begin != end) {
			Integer n = count - 1;
			ForwardIterator currFind = begin;
			++currFind;
			
			while(currFind != end && n && *currFind == value) {
				++currFind;
				--n;
			}

			if(0 == n) {
				return begin;
			}else {
				begin = find(currFind,end,value);//currFind现在指向不等于value的位置，从这里开始下一次查找
			}
		}
		return end;
	}



	/*
	 * Function : swap [begin1, end1) with [begin2, end2)
	 *
	 * Return type : end1
	 */
	template<typename ForwardIterator1,typename ForwardIterator2>
	ForwardIterator2 swap_ranges(ForwardIterator1 begin1,ForwardIterator1 end1,ForwardIterator2 begin2) {
		while(begin1 != end1 ) {
			iter_swap(begin1,begin2);
			++begin1;
			++begin2;
		}
		return begin1;
	}



	/*
	 * Function : using UnaryOperation to transfrom each elem in [begin,end) and stored to [dest, dest + (end-begin) )
	 *
	 * Return type : dest + (end - begin)
	 */
	template<typename InputIterator,typename OutputIterator,typename UnaryOperation>
	OutputIterator transform(InputIterator begin,InputIterator end,OutputIterator dest,UnaryOperation uo) {
		while(begin != end) {
			*dest = uo(*begin);
			++begin;
			++dest;
		}
		return begin;
	}
	template<typename InputIterator1,typename InputIterator2,typename OutputIterator,typename BinaryPredicate>
	OutputIterator transform(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2,OutputIterator result,BinaryPredicate bp) {
		while(begin1 != end1 && begin2 != end2) {
			*result = bp(*begin1,*begin2);
			++begin1;  ++begin2;  ++result;
		}
		return result;
	}



	/*
	 * Function : delete the duplicate elem in [begin ,end)
	 *
	 * Return type : end of managed array
	 */
	template<typename InputIterator,typename ForwardIterator>
	ForwardIterator __unique_copy(InputIterator begin,InputIterator end,ForwardIterator result,forward_iterator_tag) {
		*result = *begin;
		while(++begin != end) {
			if(*begin != *result) {
				*++result = *begin;
			}
		}
		return ++result;
	}
	template<typename InputIterator,typename OutputIterator>
	OutputIterator __unique_copy(InputIterator begin,InputIterator end,OutputIterator result,output_iterator_tag) {
		typename iterator_traits<OutputIterator>::value_type value = *begin; 
		*result = value;

		while( ++begin != end) {
			if(value != *begin) {// 在OutputIterator中,无法使用*result == *begin的判读,因为只能写入不能读取
				*++result = *begin;// 因而只能和value对比
				value = *begin;
			}
		}
		return ++result;
	}
	template<typename ForwardIterator>
	ForwardIterator unique(ForwardIterator begin,ForwardIterator end) {
		begin = adjacent_find(begin,end);

		if(begin == end) return end;
		typename iterator_traits<ForwardIterator>::iterator_category ic;
		return __unique_copy(begin,end,begin,ic);
	}

}
#endif
