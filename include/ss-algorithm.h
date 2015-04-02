#ifndef _ss_algorithm_base_
#define _ss_algorithm_base_

#include "ss-iterator-p.h"

/*
 * equal
 * 
 * check two range for equality
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
 * fill
 *
 * fill new val to given section
 */
template<typename InputIterator1,typename Tp>
inline void fill(InputIterator1 begin, InputIterator1 end, const Tp &val) {
	while(begin != end) {
		*begin = val;
		++begin;
	}
}

/*
 * fill_n
 *
 * fill new val to given range
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
 * iter_swap
 */
template<typename ForwardIterator1, typename ForwardIterator2>
inline void iter_swap(ForwardIterator1 iter1, ForwardIterator2 iter2) {
	typename iterator_traits<ForwardIterator1>::value_type tmp = *iter1;
	*iter1 = *iter2;
	*iter2 = tmp;
}

/*
 * lexicographical_compare
 *
 * using dict order. ( h > H), Is arr1 less than arr2 ?
 */
template<typename InputIterator1, typename InputIterator2>
inline bool lexicographical_compare(InputIterator1 begin1, InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2) {
	while(begin1 != end1 && begin2 != end2)　{
		if( *begin1 > *begin2 )
			return false;
		else if( *begin1 < *begin2 )
			return true;
		else {
			++begin1;
			++begin2;
		}
	}
	return begin1 == end1 && begin2 != end2;
}


template<typename InputIterator1, typename InputIterator2,typename Compare>
inline bool lexicographical_compare(InputIterator1 begin1, InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2,Compare comp) {
	while(begin1 != end1 && begin2 != end2)　{
		if( comp(*begin1 , *begin2) )
			return false;
		else if( !comp(*begin1 , *begin2 ) )
			return true;
		else {
			++begin1;
			++begin2;
		}
	}
	return begin1 == end1 && begin2 != end2;
}

/*
 * min max
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
 * mismatch
 *
 * find the first mis-match position
 */
template<typename InputIterator1,typename InputIterator2>
inline pair<InputIterator1, InputIterator2> mismatch(InputIterator1 begin1, InputIterator1 end1, InputIterator2 begin2) {
	while(begin1 != end1) {
		if( *begin1 != *begin2 ) {
			return pair<InputIterator1, InputIterator2>(beign1, begin2);
		}
		++begin1;
		++begin2;
	}
	return pair<InputIterator1, InputIterator2>(begin1, beign2);
}

template<typename InputIterator1,typename InputIterator2,typename BinaryPredicate>
inline pair<InputIterator1, InputIterator2> mismatch(InputIterator1 begin1, InputIterator1 end1, InputIterator2 begin2,BinaryPredicate bp) {
	while(begin1 != end1) {
		if( ! bp(*begin1 , *begin2 ) ) {
			return pair<InputIterator1, InputIterator2>(beign1, begin2);
		}
		++begin1;
		++begin2;
	}
	return pair<InputIterator1, InputIterator2>(begin1, beign2);
}

/*
 * swap
 */
template<typename Tp>
inline void swap(Tp& a,Tp& b) {
	Tp tmp = a;
	a = b;
	b = tmp;
}

/*
 * copy
 *
 * We consider the overlap case, there is four case (begin -> end ) copy to (destBegin -> destEnd)
 * 1) destEnd > begin (not overlapped)
 * 2) destBegin == begin (full overlapped)
 * 3) destBegin > begin && destBegin < end (partly overlapped, need back copy)
 * 4) destBegin >= end (not overlapped)
 */
template<typename InputIterator, typename OutputIterator>
inline OutputIterator copy(InputIterator begin,InputIterator end, OutputIterator dest) {
	return copy_dispatch(begin, end, dest,iterator_category(first) );
}

template<>
inline char* copy(char *beign,char *end, char *dest){
	if(begin == end || begin == dest) return dest;

	if(dest > begin && dest < end) {//need back copy
		char *destEnd = dest + (end - begin);
		for( --end,--destEnd; end >= begin; --end,--destEnd) {
			*destEnd = *end;
		}
	}else{
		for(; beign != end; ++beign, ++end) {
			*dest = *begin;
		}
	}
	return dest + (end - begin);
}


//hard to check wether dest are locate in [begin, end)
template<typename InputIterator,typename OutputIterator>
inline copy_dispatch(InputIterator begin,InputIterator end,OutputIterator dest, input_iterator_tag) {
	if(begin == end || begin == dest) return dest;
	while(begin != end) {
		*dest = *begin;
		++begin;
		++dest;
	}
	return dest;
}

template<typename InputIterator,typename OutputIterator>
inline copy_dispatch(InputIterator begin, InputIterator end, OutputIterator dest, random_access_iterator_tag) {
	if(begin == end || begin == dest) return dest;

	iterator_traits<InputIterator>::difference_type n = end - begin + 1;
	while(--n) { // using n is faster for loop check, that's the benefit cause by random_access_iterator
		*dest = *begin;
		++dest;
		++begin;
	}
	return dest;
}

/*
 * set_union
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
			++beign1;
			++begin2;
		}
		++dest;
	}
	return copy(begin1,end1,copy(begin2,end2,dest));
}

/*
 * set_intersection
 */
template<typename InputIterator1,typename InputIterator2,typename OutputIterator>
inline OutputIterator set_intersection(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2,OutputIterator dest){
	while(begin1 != end1 && begin2 != end2){
		if(*begin1 < *begin2){
			++begin1;
		}else if(*begin1 > *begin2){
			++begin2;
		}else{
			*dest = *begin;
			++dest;
			++begin1;
			++begin2;
		}
	}
	return dest;
}

/*
 * set_difference
 */
tempalte<typename InputIterator1,typename InputIterator2,typename OutputIterator>
inline OutputIterator set_difference(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2,OutputIterator dest){
	while(begin1 != end1 && begin2 != end2) {
		if(*begin1 < *begin2){
			*dest = *beign1;
			++dest;
			++begin1;
		}else if(*begin1 > *begin2){
			++begin2;
		}else {
			++begin1;
			++begin2;
		}
	}
	return copy(begin1,end1,dest);
}

/*
 * set_symmetric_difference
 */
tempalte<typename InputIterator1,typename InputIterator2,typename OutputIterator>
inline OutputIterator set_symmetric_difference(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2,OutputIterator dest){
	while(begin1 != end1 && begin2 != end2) {
		if(*begin1 < *begin2){
			*dest = *begin1;
			++begin1;
			++dest;
		}else if(*begin1 > *begin2){
			*dest = *begin2;
			++begin2;
			++dest;
		}else{
			++begin1;
			++begin2;
		}
	}

	return copy(begin1,begin2,copy(begin2,end2,dest));
}

/*
 * adjacent_find
 */
template<typename ForwardIterator,typename BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator begin,ForwardIterator end,BinaryPredicate bp){
	ForwardIterator prev = begin;
	while(++beign != prev) {
		if(bp(*begin, *prev))
			return begin;
		prev = begin;
	}
	return end;
}


/*
 * count
 */
template<typename InputIterator,typedef Tp>
typename iterator_traits<InputIterator>::difference_type count(InputIterator begin, InputIterator end,const Tp& val) {
	iterator_traits<InputIterator>::difference_type n = 0;
	while(begin != end) {
		if(*begin == val) {
			++n;
		}
	}
	return n;
}

template<typename InputIterator, typename Predicate>
typename iterator_traits<InputIterator>::difference_type count(InputIterator begin, InputIterator end,Predicate pred) {
	iterator_traits<InputIterator>::difference_type n = 0;
	while(begin != end) {
		if( pred( *begin ) ) {
			++n;
		}
	}
	return n;
}

/*
 * find
 */
template<typename InputIterator,typename Tp>
InputIterator find(InputIterator begin,InputIterator end,const Tp& val) {
	while(begin != end && *begin != val)
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
		ForwardIterator tmp1 = begin1;
		ForwardIterator tmp2 = begin2;

		while(tmp2 != end2 && *tmp2 == *tmp1){
			++tmp1;
			++tmp2;
		}

		if(tmp2 == end2)
			return begin1;
		else{
			++begin1;
		}
	}
	return end1;
}

template<typename InputIterator1,typename InputIterator2>
InputIterator1 find_last_of(InputIterator1 beign1,InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2) {
	typedef typename iterator_traits<InputIterator1>::iterator_category ic1;
	typedef typename iterator_traits<InputIterator2>::iterator_category ic2;
	return _find_last_of(begin1,end1,begin2,end2,ic1(),ic2());
}

template<typename InputIterator1,typename InputIterator2>
InputIterator1 _find_last_of(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2,input_iterator_tag,input_iterator_tag) {
	if(begin1 == end1) return end1;

	while(1) {
		InputIterator1 result;
		InputIterator1 pos = search(begin1,end1,begin2,end2);

		if(pos == end1)
			return end1;
		else {
			result = pos;
			begin1 = ++pos;
		}
	}
}

template<typename BidirectionalIterator1,typename BidirectionalIterator2>
BidirectionalIterator1 _find_last_of(BidirectionalIterator1 begin1,BidirectionalIterator1 end,BidirectionalIterator2 begin2,BidirectionalIterator2 end2,bidirectional_iterator_tag,bidirectional_iterator_tag) {// 反向查找，更加直接！
	typedef reverse_iterator<BidirectionalIterator1> riter1;
	typedef reverse_iterator<BidirectionalIterator2> riter2;

	riter1 rend1(begin1);

	riter1 rresult = search(riter1(end1),rend1,riter2(end2),riter2(end2));
	if(rresult == rend1)
		return end1;
	else {
		BidirectionalIterator1 result = rresult.base();
		advance(result, -distance(begin2,end2));
		return result;
	}
}

/*
 * for_each
 */
template<typename InputIterator,typename Function>
inline Function for_each(InputIterator begin,InputIterator end,Function func) {
	while(begin != end) {
		func(*begin);
	}
	return func;
}

/*
 * generate
 */
template<typename ForwardIterator,typename Generator>
void generate(ForwardIterator begin,ForwardIterator end,Generator gen) {
	while(begin != end) {
		*begin = gen();
	}
}

/*
 * inlcudes ( used on sorted elements)
 */
template<typename InputIterator1,typename InputIterator2>
bool includes(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2) {
	while(begin1 !=  end1){
		if(*begin1 < *beign2){
			++begin1;
		}else if(*begin1 > *begin2){
			retur false;
		}else{
			++begin1;
			++begin2;
		}
	}
	return begin2 == end2;
}

/*
 * max_element
 */
template<typename InputIterator>
InputIterator max_element(InputIterator begin,InputIterator end){
	InputIterator res = begin;
	while(begin != end) {
		if(*begin > *res)
			res = begin;
		++begin;
	}
	return res;
}

/*
 * min_element
 */
template<typename InputIterator>
InputIterator min_element(InputIterator begin,InputIterator end) {
	InputIterator res = begin;
	while(begin!=end) {
		if(*begin < *res){
			res = begin;
		}
	}
	return res;
}

/*
 * merge
 */
template<typename InputIterator1,typename InputIterator2,typename OutputIterator>
OutputIterator merge(InputIterator1 begin1,InputIterator1 end1,InputIterator2 beign2,InputIterator2 end2,OutputIterator result) {
	while(begin1 != end1 && begin2 != end2) {
		if(*begin1 < *begin2) {
			*result = *begin1;
			++begin1;
		}else {
			*result = *begin2;
			++begin2;
		}
		++result;
	}

	return copy(begin1,end1,copy(begin2,end2,result));	
}

/*
 * partition
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
 * remove
 */
template<typename ForwardIterator, typename Tp>
InputIterator remove(ForwardIterator begin, ForwardIterator end, const Tp& value){
	begin = find(begin,end,value);
	ForwardIterator next = begin;

	return begin == end ? begin : remove_copy(++next,end,begin,value);
}

template<typename InputIterator, typename OutputIterator, typename Tp>
InputIterator remove_copy(InputIterator begin,InputIterator end,OutputIterator result,const Tp& value) {
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
 * replace
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
	}
	return result;
}

template<typename ForwardIterator,typename Predicate ,typename Tp>
void replace_if(ForwardIterator begin,ForwardIterator end,Predicate pred,const Tp& value) {
	while(begin != end) {
		if(　pred(*begin) ) {
			*begin = value;
		}
		++begin;
	}
}

template<typename InputIterator,typename OutputIterator, typename Predicate,typename Tp>
OutputIterator replace_copy_if(InputIterator begin,InputIterator end,OutputIterator dest, const Tp& value) {
	while(begin != end) {
		*dest = pred(*begin) ? value : *begin;
		++begin;
	}
	return dest;
}

/*
 * reverse
 */
template<typename BidirectionalIterator>
inline void reverse(BidirectionalIterator begin,BidirectionalIterator end){
	__reverse(begin,end,iterator_category(begin));
}

template<typename BidirectionalIterator>
inline void __reverse(BidirectionalIterator begin,BidirectionalIterator end,bidirectional_iterator_tag) {
	while(begin != end && begin != end + 1){
		iter_swap(begin, end);
		++begin;
		--end;
	}
}

template<typename RandomAccessIterator>
inline void __reverse(RandomAccessIterator begin,RandomAccessIterator end,random_access_iterator_tag) {
	while(begin < end) {
		iter_swap(begin, end);
	}
}

/*
 * reverse_copy
 */
template<typename BidirectionalIterator,typedef OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator begin,BidirectionalIterator end,OutputIterator dest) {
	while( end != begin ) {
		--end;
		*dest = *end;
		++dest;
	}
	return result;
}

/*
 * rotate
 */
template<typename ForwardIterator>
void rotate(ForwardIterator begin,ForwardIterator mid,ForwardIterator end) {
	if(begin == mid || mid == end) return ;
	__rotate(begin,mid,end,distance_type(begin),iterator_category(begin));
}

template<typename ForwardIterator,typename Distance>
void __rotate(ForwardIterator begin,ForwardIterator mid,ForwardIterator end,Distance*,forward_iterator_tag) {
	ForwardIterator begin_for_back = mid;
	while(begin_for_back != end && begin != mid) {
		while(begin != mid && begin_for_back != end){
			iter_swap(begin, begin_for_back);
			++begin;
			++begin_for_back;
		}
		if(begin == mid) {//the front segment has finished
			mid = i;
		}else {// the back segment has finished
			begin_for_back = mid;
		}
	}
}

template<typename BidirectionalIterator,typename Distance>
void __rotate(BidirectionalIterator begin,BidirectionalIterator mid,BidirectionalIterator end,Distance*,bidirectional_iterator_tag) {
	reverse(begin,mid);
	reverse(mid,end);
	reverse(begin,end);
}

//求最大公因子
//辗转相除法
template<typename EuclideanRingElement>
EuclideanRingElement gcd(EuclideanRingElement m ,EuclideanRingElement n) {
	EuclideanRingElement t;
	while( n != 0) {
		t = m % n;
		m = n;
		n = t;
	}
	return m;
}

template<typename RandomAccessIterator,typename Distance>
void __rotate(RandomAccessIterator begin,RandomAccessIterator mid,RandomAccessIterator end,Distance*,random_access__iterator) {
	Distance n = gcd(end - first, mid - first);
	while( n -- ){
		__rotate_cycle(begin, end, begin + n, mid - begin, value_type(begin));
	}
}








































#endif
