#ifndef _ss_numeric_h_
#define _ss_numeric_h_

/*
 * accumulate
 *
 * base += binary_op(*iter1, *iter2)
 */
template<class InputIterator,class Tp>
Tp accumulate(InputIterator begin,InputIterator end,Tp base){
	while(begin != end){
		base += (*begin);
		++begin;
	}
	return base;
}

template<class InputIterator,class Tp,typename BinaryOperation>
Tp accumulate(InputIterator begin,InputIterator end,Tp base,BinaryOperation binary_op){
	while(begin != end){
		base = binary_op(base, *begin);
		++begin;
	}
	return base;
}

/*
 * inner_product
 *
 * base = binary_op1(base, binary_op2(*iter1,*iter2)
 */
template<typename InputIterator1, typename InputIterator2,typename Tp>
Tp inner_product(InputIterator1 begin1, InputIterator1 end1, InputIterator2 begin2,Tp base){
	while(begin1 != end1) {
		base += (*begin1) * (*begin2); 
		++begin1;
		++begin2;
	}
	return base;
}

template<typename InputIterator1,typename InputIterator2,typename Tp,typename BinaryOperation1,typename BinaryOperation2>
Tp inner_product(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,Tp base,BinaryOperation binary_op1,BinaryOperation2 binary_op2) {
	while(begin1 != end1) {
		base = binary_op1( base, binary_op2(*begin1, *begin2));
		++begin1;
		++begin2;
	}
	return base;
}

/*
 * partial_sum
 *
 * dest[i] = binary_op(source[i], source[i-1]);
 */
template<typename InputIterator,typename OutputIterator>
OutputIterator partial_sum(InputIterator begin,InputIterator end,OutputIterator dest) {
	if(begin == end) return dest;
	*dest = *begin;
	iterator_traits<InputIterator>::value_type value = *begin;
	while(++begin != end){
		value += *begin;
		*++dest = value;
	}
	return ++dest;
}
template<typename InputIterator,typename OutputIterator,typename BinaryOperation>
OutputIterator partial_sum(InputIterator begin,InputIterator end,OutputIterator dest,BinaryOperation binary_op) {
	if(begin == end) return dest;
	*dest = *begin;
	iterator_traits<InputIterator>::value_type value = *begin;
	while(++begin != end) {
		value = binary_op(value, *begin);
		*++dest = value;
	}
	return ++dest;
}

/*
 * adjacent_difference
 *
 * dest[i] = dest[i] - dest[i-1]
 */
template<typename InputIterator,typename OutputIterator>
OutputIterator adjacent_difference(InputIterator begin,InputIterator end,OutputIterator dest) {
	if(begin == end) return dest;
	*dest = *begin;
	iterator_traits<InputIterator>::value_type frontVal = *begin;
	while(++begin != end) {
		iterator_traits<InputIterator>::value_type backVal = *begin;
		*++dest = backVal - frontVal;
		frontVal = backVal;
	}
	return ++dest;
}

template<typename InputIterator,typename OutputIterator,typename BinaryOperation>
OutputIterator adjacent_difference(InputIterator begin,InputIterator end,OutputIterator dest,BinaryOperation binary_op) {
	if(begin == end) return dest;
	*dest = *begin;
	iterator_traits<InputIterator>::value_type frontVal = *begin;
	while(++begin != end) {
		iterator_traits<InputIterator>::value_type backVal = *begin;
		*++dest = binary_op(backVal,frontVal);
		frontVal = backVal;
	}
	return ++dest;
}

/*
 * iota
 *
 * begin -> end ascending
 */
template<typename ForwardIterator,typename Tp>
void iota(ForwardIterator beign,ForwardIterator end,Tp val) {
	while(beign != end) {
		*begin++ = val++;
	}
}
#endif
