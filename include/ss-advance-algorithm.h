#ifndef _ss_advance_algorithm_
#define _ss_advance_algorithm_

#include "ss-iterator-p.h"
#include "ss-simple-algorithm.h"
#include "ctime"
#include "cstdlib"

/*
 * lower_bound(有序区间上)
 *
 * 返回第一个 >=value 的地址
 * [begin,end)中返回pos,使得[begin,pos)都<value,[pos,end)中元素都>=value
 */
template<typename ForwardIterator,typename Tp>
ForwardIterator lower_bound(ForwardIterator begin,ForwardIterator end,const Tp& value) {
	return __lower_bound(begin,end,value,distance_type(begin),iterator_category(begin));
}

template<typename ForwardIterator, typename Tp, typename Distance>
ForwardIterator __lower_bound(ForwardIterator begin,ForwardIterator end,const Tp& value,Distance*,forward_iterator_category) {
	Distance len = 0;
	distance(begin,end,len);
	Distance half = 0;

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
RandomAccessIterator __lower_bound(RandomAccessIterator begin,RandomAccessIterator end,const Tp& value,Distance,random_access_iterator_category) {
	Distance len = end - begin;
	Distance half = 0;

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

/*
 * upper_bound
 *
 * [begin,end)中第一个>value的位置
 * 在不破坏顺序的情况下,可以插入valude的最后位置
 * 与lower_bound的差别在于==value的对象的处理
 */
template<typename ForwardIterator,typename Tp>
ForwardIterator upper_bound(ForwardIterator begin,ForwardIterator end,const Tp& value) {
	return __upper_bound(begin,end,value,distance_type(begin),iterator_category(begin)):
}
template<typename ForwardIterator,typename Tp,typename Distance>
ForwardIterator __upper_bound(ForwardIterator begin,ForwardIterator end,const Tp& value,Distance*,forward_iterator_category) {
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
RandomAccessIterator __upper_bound(RandomAccessIterator begin,RandomAccessIterator end,const Tp& value,Distance*,random_access_iterator_category) {
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

/*
 * binary_search
 */
template<typename ForwardIterator,typename Tp>
bool binary_search(ForwardIterator begin,ForwardIterator end,const Tp& value) {
	ForwardIterator res = lower_bound(begin,end,value);
	return res != end && value == *res;
}

/*
 * next_permutation
 */
template<typename BidirectionIteration>
bool next_permutation(BidirectionIteration begin,BidirectionIteration end) {
	if(begin == end) return false;//no elem
	BidirectionIteration i = end;
	--i;
	if(begin == i) return false;//only one elem

	BidirectionIteration ii;
	while( i != begin) {
		ii = i;
		--i;

		if(*i < *ii) {
			BidirectionIteration j = end;
			while( * --j  < *i) {}//we need the elem first bigger than i

			iter_swap(i,j);
			reverse(ii,end);
			return true;
		}
	}

	reverse(begin,end);//it's biggest ! let's make it smalllest
	return false;
}

/*
 * prev_permutation
 */
template<typename BidirectionIteration>
bool prev_permutation(BidirectionIteration begin,BidirectionIteration end) {
	if(begin == end) return false;//no elem
	BidirectionIteration i = end;
	--i;
	if(begin == i) return false;//only one elem

	BidirectionIteration ii;
	while(i != begin) {
		ii = i;
		--i;

		if( *i > *ii) {
			BidirectionIteration j = end;
			while( *--j > *i) {}//the first elem smaller the *i
			iter_swap(i,j);
			reverse(ii,end);
			return true;
		}
	}

	reverse(begin,end);//already the smallest
	return false;
}

/*
 * random_shuffle
 *
 * 随机重排
 */
template<typename RandomAccessIterator>
inline random_shuffle(RandomAccessIterator begin,RandomAccessIterator end) {
	if(begin == end) return ;
	srand(time(NULL));

	__random_shuffle(begin,end,distance_type(begin));
}
template<typename RandomAccessIterator,typename Distance>
inline void __random_shuffle(RandomAccessIterator begin,RandomAccessIterator end,Distance*) {
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
 */

template<typename RandomAccessIterator>
void __insertion_sort(RandomAccessIterator begin,RandomAccessIterator end) {
	if(begin == end) return ;
	for(RandomAccessIterator curr = begin + 1; curr != end; ++curr) {
		__linear_sort(begin, curr, value_type(begin));//put each elem to appropriate place
	}
}
template<typename RandomAccessIterator,typename Tp>
void __linear_sort(RandomAccessIterator begin, RandomAccessIterator tail,Tp*) {
	if(*tail < *begin) {
		copy_backward(begin,tail,tail+1);
		*begin = *tail;
	}else{
		__ungarded_linear_insert(tail,(Tp)(*tail));
	}
}
template<typename RandomAccessIterator,typename Tp>
void __ungarded_linear_insert(RandomAccessIterator tail,Tp value) {
	RandomAccessIterator curr = tail - 1;
	RandomAccessIterator prev;
	while( *curr > value ) {
		prev = curr;
		--curr;
		*(curr + 1) = *curr;
	}
	*prev = value;
}
template<typename Tp>
const Tp& __median(const Tp& a,const Tp& b,const Tp& c) {
	if(a < b) {
		if(b < c) return b;
		if(a < c) 
			return c;
		else 
			return a;
	}else{
		if(a < c) 
			return a;
		if(b < c)
			return c;
		else 
			return b;
	}
}

template<typename RandomAccessIterator,typename Tp>
RandomAccessIterator __ungarded_partition(RandomAccessIterator begin,RandomAccessIterator end,const Tp& pivot) {
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

template<typename Size>
Size __lg(Size n) {//lg2(n)
	Size k;
	for(k = 0;n > 1;n = n>>1)
		++k;
	return k;
}

const int threshold = 16;

template<typename RandomAccessIterator,typename Tp,typename Size>
void __introsrot_loop(RandomAccessIterator begin,RandomAccessIterator end,Tp*,Size depth_limit) {
	while(end - begin > threshold) {
		if(depth_limit == 0) {
			partial_sort(begin,end,end);//use heap sort
			return ;
		}
		
		-- depth_limit;
		RandomAccessIterator cut = __ungarded_partition(begin,end,(Tp)(__median(*begin,*(begin + (end - begin)/2),*(end-1))));

		__introsrot_loop(cut,end,value_type(cut,depth_limit));//right side
		end = cut;//let's manage left side
	}
}

template<typename RandomAccessIterator,typename Tp>
void __ungarded_insertion_sort_aux(RandomAccessIterator begin,RandomAccessIterator end,Tp*) {
	for(RandomAccessIterator curr = begin; curr!=end;++curr) {
		__ungarded_linear_insert(curr,(Tp)(*curr));
	}
}

template<typename RandomAccessIterator>
void __ungarded_insertion_sort(RandomAccessIterator begin,RandomAccessIterator end) {
	__ungarded_insertion_sort_aux(begin,end,value_type(begin));
}

template<typename RandomAccessIterator>
void __final_insertion_sort(RandomAccessIterator begin,RandomAccessIterator end) {
	if(end - begin > threshold) {
		__insertion_sort(begin,begin+threshold);
		__ungarded_insertion_sort(begin + threshold,end);
	}else {
		__insertion_sort(begin,end);
	}
}
template<typename RandomAccessIterator>
RandomAccessIterator sort(RandomAccessIterator begin,RandomAccessIterator end) {
	if(begin != end) {
		__introsrot_loop(begin,end,value_type(begin),__lg(end-begin) * 2);
		__final_insertion_sort(begin,end);
	}
}

#endif
