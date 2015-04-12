#ifndef _ss_advance_algorithm_
#define _ss_advance_algorithm_

#include "iterator-ext.h"
#include "simple-algorithm.h"
#include "ctime"
#include "cstdlib"
#include "tempbuffer.h"
#include <utility>

namespace Freeman {



	/*
	 * lower_bound(有序区间上)
	 *
	 * 返回第一个 >=value 的地址
	 * [begin,end)中返回pos,使得[begin,pos)都<value,[pos,end)中元素都>=value
	 */
	template<typename ForwardIterator, typename Tp, typename Distance>
	ForwardIterator _lower_bound(ForwardIterator begin,ForwardIterator end,const Tp& value,Distance*,forward_iterator_tag) {
		Distance len = 0, half = 0;
		distance(begin,end,len);

		ForwardIterator mid = begin;//使用了begin和len结合的方式,因为ForwardIterator的局限性
		while(len) {
			half = len / 2;
			mid = advance(begin,half);

			if(*mid < value) {
				begin = ++mid;//<value的值都可以跳过
				len = len - half - 1;
			}else {
				len = half;//因为是求lower_bound,当前这个>=value的*mid可能为最终结果,所以不能舍弃
			}
		}
		return begin;
	}
	template<typename RandomAccessIterator,typename Tp,typename Distance>
	RandomAccessIterator _lower_bound(RandomAccessIterator begin,RandomAccessIterator end,const Tp& value,Distance,random_access_iterator_tag) {
		Distance half = 0 , len = end - begin;

		RandomAccessIterator mid = begin;
		while(len) {
			half = len / 2;
			mid = begin + half;

			if(*mid < value) {
				len = len - half - 1;
				begin = ++mid;
			}else{
				len = half;
			}
		}
		return begin;
	}
	template<typename ForwardIterator,typename Tp>
	ForwardIterator lower_bound(ForwardIterator begin,ForwardIterator end,const Tp& value) {
		typename iterator_traits<ForwardIterator>::difference_type distance;
		typename iterator_traits<ForwardIterator>::iterator_category ic;
		return _lower_bound(begin,end,value,distance,ic);
	}



	/*
	 * upper_bound
	 *
	 * [begin,end)中第一个>value的位置
	 * 在不破坏顺序的情况下,可以插入valude的最后位置
	 *
	 * 与lower_bound的差别在于==value的对象的处理
	 */
	template<typename ForwardIterator,typename Tp,typename Distance>
	ForwardIterator _upper_bound(ForwardIterator begin,ForwardIterator end,const Tp& value,Distance*,forward_iterator_tag) {
		Distance len = 0;
		advance(begin,end,len);
		Distance half = 0;

		ForwardIterator mid = begin;
		while(len ) {
			half = len / 2;
			mid = advance(begin,half);

			if( *mid > value ) {//mid可能是结果,不能跳过
				len = half;
			}else {
				begin = ++mid;//<=的值都可以跳过
				len = len - half - 1;
			}

		}
		return begin;
	}
	template<typename RandomAccessIterator,typename Tp,typename Distance>
	RandomAccessIterator _upper_bound(RandomAccessIterator begin,RandomAccessIterator end,const Tp& value,Distance*,random_access_iterator_tag){
		Distance len = end - begin;
		Distance half = 0;

		RandomAccessIterator mid = begin;
		while(len) {
			half = len / 2;
			mid = begin + half;

			if(*mid <= value) {
				len = len - half - 1;
				begin = ++mid;
			}else {
				len = half;
			}
		}
		return begin;
	}
	template<typename ForwardIterator,typename Tp>
	ForwardIterator upper_bound(ForwardIterator begin,ForwardIterator end,const Tp& value) {
		typename iterator_traits<ForwardIterator>::difference_type distance;
		typename iterator_traits<ForwardIterator>::iterator_category ic;
		return _upper_bound(begin,end,value,distance,ic );
	}



	/*
	 * equal_range
	 *
	 * [lower_bound, upper_bound)
	 */
	template<typename ForwardIterator,typename Tp,typename Distance>
	std::pair<ForwardIterator,ForwardIterator> _equal_range(ForwardIterator begin,ForwardIterator end,const Tp& value,Distance*,forward_iterator_tag) {
		Distance len = 0,half = 0;
		distance(begin,end,len);

		ForwardIterator mid;
		while(len > 0) {
			half = len >> 1;
			mid = advance(begin,half);

			if(*mid < value) {
				begin = ++mid;
				len = len - half - 1;
			}else if(*mid > value) {
				len = len - half;
			}else{
				begin = lower_bound(begin,mid,value);
				advance(begin,len);//that's currr end
				end = upper_bound(++mid,begin,value);
				return std::pair<ForwardIterator,ForwardIterator>(begin,end);
			}
		}
		return std::pair<ForwardIterator,ForwardIterator>(begin,begin);
	}
	template<typename RandomAccessIterator,typename Tp,typename Distance>
	std::pair<RandomAccessIterator,RandomAccessIterator> _equal_range(RandomAccessIterator begin,RandomAccessIterator end,const Tp& value,Distance*,random_access_iterator_tag) {
		Distance len = 0,half =0;
		distance(begin,end,len);
	
		RandomAccessIterator mid;
		while(len > 0) {
			half = len >> 1;
			mid = begin + half;

			if(*mid < value){
				begin = begin + half + 1;
				len = len - half - 1;
			}else if(*mid > value) {
				len = half;
			}else {
				begin = lower_bound(begin, mid, value);
				end = upper_bound(++mid, begin+len, value);
				return std::pair<RandomAccessIterator,RandomAccessIterator>(begin,end);
			}
		}
		return std::pair<RandomAccessIterator,RandomAccessIterator>(begin,begin);
	}
	template<typename ForwardIterator,typename Tp>
	std::pair<ForwardIterator,ForwardIterator> equal_range(ForwardIterator begin,ForwardIterator end,const Tp& value) {
		typename iterator_traits<ForwardIterator>::difference_type distance;
		typename iterator_traits<ForwardIterator>::iterator_category ic;
		return _equal_range(begin,end,value,distance,ic);
	}



	/*
	 * binary_search
	 */
	template<typename ForwardIterator,typename Tp>
	bool binary_search(ForwardIterator begin,ForwardIterator end,const Tp& value) {
		ForwardIterator res = lower_bound(begin,end,value);
		return res != end && value == *res;
	}

	

	/*
	 * next_permutation & prev_permutation
	 */
	template<typename BidirectionIteration>
	bool next_permutation(BidirectionIteration begin,BidirectionIteration end) {
		if(begin == end) return false;//no elem
		BidirectionIteration prev_of_adjacent = end;
		--prev_of_adjacent;
		if(begin == prev_of_adjacent) return false;//only one elem

		BidirectionIteration back_of_adjacent;
		while(prev_of_adjacent != begin) {
			back_of_adjacent = prev_of_adjacent;
			--prev_of_adjacent;

			if(*prev_of_adjacent < *back_of_adjacent) {
				BidirectionIteration idx_for_bigger = end;
				while( * --idx_for_bigger  < *prev_of_adjacent) {}//we need the elem first bigger than i

				iter_swap(idx_for_bigger, prev_of_adjacent);
				reverse(back_of_adjacent,end);
				return true;
			}
		}
		reverse(begin,end);//it's biggest ! let's make it smalllest
		return false;
	}
	template<typename BidirectionIteration>
	bool prev_permutation(BidirectionIteration begin,BidirectionIteration end) {
		if(begin == end) return false;//no elem
		BidirectionIteration prev_of_adjacent = end;
		-- prev_of_adjacent;
		if(begin == prev_of_adjacent) return false;//only one elem

		BidirectionIteration back_of_adjacent;
		while(prev_of_adjacent != begin) {
			back_of_adjacent = prev_of_adjacent;
			-- prev_of_adjacent;

			if( *prev_of_adjacent > *back_of_adjacent) {
				BidirectionIteration idx_for_smaller = end;
				while( *-- idx_for_smaller > *prev_of_adjacent) {}//the first elem smaller the *i
				iter_swap(idx_for_smaller, prev_of_adjacent);
				reverse(back_of_adjacent, end);
				return true;
			}
		}

		reverse(begin,end);//already the smallest
		return false;
	}



	/*
	 * 将[begin, end) 随机重排
	 */
	template<typename RandomAccessIterator,typename Distance>
	inline void _random_shuffle(RandomAccessIterator begin,RandomAccessIterator end,Distance*) {
	}
	template<typename RandomAccessIterator>
	inline void random_shuffle(RandomAccessIterator begin,RandomAccessIterator end) {
		if(begin == end) return ;

		srand(time(NULL));
		typedef typename iterator_traits<RandomAccessIterator>::difference_type Distance; 

		for(RandomAccessIterator curr = begin + 1; curr != end; ++curr) {
			iter_swap(curr, begin + Distance( rand() % (curr - begin + 1) ) );//we need Distance here
		}
	}



	/*
	 * partial_sort
	 *
	 * need heap to help this !
	 * Finish later
	 */

	

	/*
	 * sort
	 *
	 * heap_sort + insertion_sort + 
	 */
	const int threshold = 16;//quick sort会产生许多递归调用, 当处理小数量数据时,性能可能不如insertion_sort
	template<typename Size>//used to set the max recursion level
	Size _lg(Size n) {//lg2(n)
		Size k;
		for(k = 0;n > 1;n = n>>1)//lg1 == 0 ;  lg2 == 1
			++k;
		return k;
	}
	template<typename Tp>
	const Tp& _median_of_three(const Tp& a,const Tp& b,const Tp& c) {
		if(a < b) {
			if(b < c) return b; // a < b < c
			if(c < a) return a; // c < a < b
			return c;			// a < b = c    c = a < b	a < c < b
		}else if(b < a){
			if(a < c) return a; // b < a < c
			if(c < b) return b; // c < b < a
			return c;			// b < a = c	c = b < a	b < c < a	
		}else {//b == a
			return a;
		}
	}
	template<typename RandomAccessIterator>
	void _insertion_sort(RandomAccessIterator begin,RandomAccessIterator end) {
		if(begin == end) return ;
		for(RandomAccessIterator curr = begin + 1; curr != end; ++curr) {
			if(*curr < *begin) {//the front is smaller than every elem
				copy_backward(begin, end, end + 1);
				*begin = *curr;
			}else{
				RandomAccessIterator idx_for_compare = curr - 1;
				while(*idx_for_compare > *curr) {//因为已经排除了完全小于队列中所有元素的情况,所以可以不用担心边界条件
					-- idx_for_compare;
					*(idx_for_compare + 1) = *(idx_for_compare);
				}
				*(idx_for_compare + 1) = *curr;
			}
		}
	}
	template<typename RandomAccessIterator,typename Tp>
	RandomAccessIterator _ungarded_partition(RandomAccessIterator begin,RandomAccessIterator end,const Tp& pivot) {//return the first pos that no less than pivot
		if(begin == end) return begin;
		while(true) {
			while(*begin < pivot)
				++begin;
			--end;
			while(*end > pivot)
				--end;

			if(!(begin<end)) return begin;

			iter_swap(begin,end);
			++begin;
		}
	}
	template<typename RandomAccessIterator, typename Size>
	void _introsort_loop(RandomAccessIterator begin,RandomAccessIterator end,Size depth_limit) {//混合式排序算法:introspective sorting.
		while(end - begin > threshold) {//when the elem are less than threshold, just stop sorting.
			if(depth_limit == 0) {//when the partition level is already too deep, we use heap sort instead.
				partial_sort(begin,end,end);//heap sort
				return ;
			}
			-- depth_limit;
			typename iterator_traits<RandomAccessIterator>::value_type pivot = __median(*begin, *(end-1), *((begin + end)/2));
			RandomAccessIterator cut = __ungarded_partition(begin, end, pivot);

			_introsrot_loop(cut,end,value_type(cut,depth_limit));//manage right side
			end = cut;//let's manage left side
		}
	}
	template<typename RandomAccessIterator>
	void __final_insertion_sort(RandomAccessIterator begin,RandomAccessIterator end) {
		if(end - begin > threshold) {
			_insertion_sort(begin,begin+threshold);
			//here is a trick : 
			//Because we have manage the sub-sequence than longer than "threshold", so [begin, begin+threshold) must all less than *(begin + threshold), 
			//It's safe to omit judge *begin < value.
			for(RandomAccessIterator tail = begin + threshold ; tail != end ; ++tail) {
				typename iterator_traits<RandomAccessIterator>::value_type value = *tail;
				RandomAccessIterator curr = tail - 1;
				RandomAccessIterator prev;
				while( *curr > *tail) {
					prev = curr;
					--curr;
					*(curr + 1) = *curr;
				}
				*prev = value;
			}
		}else {
			_insertion_sort(begin,end);
		}
	}
	template<typename RandomAccessIterator>
	RandomAccessIterator sort(RandomAccessIterator begin,RandomAccessIterator end) {
		if(begin != end) {
			_introsort_loop(begin,end,__lg(end-begin) * 2);
			//when _introsrot_loop finish, there would be many sub-sequence,which all shorter than threshold
			//that already partly sorted by introsort
			_final_insertion_sort(begin,end);
		}
	}



	/*
	 * nth_element
	 */
	template<typename RandomAccessIterator>
	inline void nth_elem(RandomAccessIterator begin,RandomAccessIterator nth,RandomAccessIterator end) {
		typedef typename iterator_traits<RandomAccessIterator>::value_type Tp;

		while(end - begin > 3) {
			RandomAccessIterator cut = __ungarded_partition(begin,end,(Tp)(__median(*begin,*(begin + (end - begin)>>1),*(end-1))));

			if(cut <= nth){//这里需要被检验,在cut==nth时能否够直接返回
				begin = cut;
			}else{
				end = cut;
			}
		}
		_insertion_sort(begin,end);
	
	}


	/*
	 * inplace_merge
	 */
	/*
	 * Function : merge two sorted array
	 *
	 * Return type : the begining of merged array
	 */
	template<typename InputIterator1,typename InputIterator2,typename OutputIterator>
	OutputIterator merge(InputIterator1 begin1,InputIterator1 end1,InputIterator2 begin2,InputIterator2 end2,OutputIterator result) {
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
	template<typename BidirectionalIterator1,typename BidirectionalIterator2,typename BidirectionalIterator3>
	BidirectionalIterator3 merge_backward(BidirectionalIterator1 begin1,BidirectionalIterator1 end1,BidirectionalIterator2 begin2,BidirectionalIterator2 end2,BidirectionalIterator3 dest) {
		if(begin1 == end1) return copy_backward(begin2,end2,dest);
		if(begin2 == end2) return copy_backward(begin1,end1,dest);

		while(end1 != begin1 && end2 != begin2) {
			-- dest;
			if(* --end1 < * --end2){
				*dest = *end2;
				--end2;
			}else{
				*dest = *end1;
				--end1;
			}
		}
		if(end1 == begin1)
			return copy_backward(begin2,++end2,dest);
		else
			return copy_backward(begin1,++end1,dest);
	}


	template<typename BidirectionalIterator,typename Iterator,typename Distance>
	BidirectionalIterator _rotate_adaptive(BidirectionalIterator begin,BidirectionalIterator mid,BidirectionalIterator end,Distance len1,Distance len2,Iterator buffer,Distance buffer_size) {
		Iterator buffer_end;
		if(len1 < len2 && len1 <= buffer_size) {//put front into buffer
			buffer_end = copy(begin,mid,buffer);
			copy(mid,end,begin);
			return copy_backward(buffer,buffer_end,end);
		}else if(len2 <= buffer_size) {//put back into buffer
			buffer_end = copy(mid,end,buffer);
			copy_backward(begin,mid,end);
			return copy(buffer,buffer_end,begin);
		}else{//not enough buffer
			return rotate(begin,mid,end);
		}
	}

	template<typename BidirectionIteration,typename Iterator,typename Distance>
	void _merge_adaptive(BidirectionIteration begin,BidirectionIteration mid,
			BidirectionIteration end,Distance len1,Distance len2,
			Iterator buffer,Distance buffer_size) {
		if(len1 < len2 && len1 <= buffer_size) {
			Iterator end_of_buffer = copy(begin, mid, buffer);//now the [begin,mid)are free to usr);
			merge(buffer, end_of_buffer, mid, end,buffer);
		}else if(len2 < len1 && len2 <= buffer_size) {
			Iterator end_of_buffer = copy(mid,end,buffer);//the [mid,end) are free to use, so we need backward algorithm
			merge_backward(begin,mid,buffer,end_of_buffer,end);
		}else{
			 BidirectionIteration front_begin = begin;
			 BidirectionIteration back_begin = mid;
			 Distance front_len = 0,back_len = 0;

			 if(len1 > len2) {
				 front_len = len1 / 2;
				 advance(front_begin, front_len);
				 back_begin = lower_bound(mid, end, *front_begin);
				 distance(back_begin, end, back_len);
			 }else {
				 back_len = len2 / 2;
				 advance(back_begin,back_len);
				 front_len = lower_bound(begin,mid,*back_begin);
				 distance(begin,front_begin,front_len);
			 }
			 
			 mid = _rotate_adaptive(front_begin,mid,back_begin,len1-front_len,back_len,buffer,buffer_size);
			 _merge_adaptive(begin,front_begin,mid,front_len,back_len,buffer,buffer_size);
			 _merge_adaptive(mid,back_begin,end,len1-front_len,len2-back_len,buffer,buffer_size);
		}
	}
	template<typename BidirectionIteration>
	void inplace_merge(BidirectionIteration begin,BidirectionIteration mid,BidirectionIteration end) {
		if(begin == mid || mid == end) return ;

		typedef typename iterator_traits<BidirectionIteration>::value_type Tp;
		Temporary_buffer<BidirectionIteration,Tp> buf(begin,end);

		typedef typename iterator_traits<BidirectionIteration>::difference_type Distance;
		Distance len1, len2;
		distance(begin,mid,len1);
		distance(mid,end,len2);

		if(buf.len() == 0)
			_merge_without_buffer(begin,mid,end,len1,len2);
		else
			_merge_adaptive(begin,mid,end,len1,len2,buf.begin(),Distance(buf.size()));
	}

	/*
	 * merge_sort
	 */
	template<typename BidirectionIteration>
	void merge_sort(BidirectionIteration begin,BidirectionIteration end) {
		typename iterator_traits<BidirectionIteration>::difference_type len = distance(begin,end);

		if(len == 0 || len == 1) return;

		BidirectionIteration mid = begin + len / 2;
		merge_sort(begin,mid);
		merge_sort(mid,end);
		inplace_merge(begin,mid,end);
	}

}
#endif
