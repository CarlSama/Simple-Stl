#ifndef _freeman_heap_h_
#define _freeman_heap_h_

#include "iterator.h"

namespace Freeman {
	/*
	 * the heap is not like other container class, it's not possessing any data, it's just some kind of method to adjust  range of data , stored in some kind of sequential container, to make them permute as a heap.
	 */

	//Compare is used for comparision between two value, this can help us build max-heap or min-heap
	//Compare(first, second) return bool, which indicate wether first elem is considerd to be less than second. And we always make the lesser elem to the lower level of heap.
	//In a word, compare(first, second) indicate whether first should be put down than second ?
	


	/*
	 * Function : rise *(begin + holeIdx - 1) to proper position, note that the rising ends at topIdx
	 *	Use parent value or value to pade the hole
	 */
	template <typename RandomAccessIterator,typename Distance,typename Tp,typename Compare>
		inline void __rise_from_holeIdx(RandomAccessIterator begin, Distance holeIdx,Distance topIdx,Tp value,Compare comp) {
			Distance parent = holeIdx / 2;
			while(parent > topIdx && comp( *(begin + parent - 1) , value) ) {
				*(begin + holeIdx - 1) = *(begin + parent - 1);//parent should be put 
				holeIdx = parent;
				parent = holeIdx / 2;
			}
			*(parent + holeIdx - 1) = value;
		}
	template <typename RandomAccessIterator,typename Compare>
		inline void push_heap(RandomAccessIterator begin,RandomAccessIterator end,Compare comp) { // the elem to be pushed is located in (end - 1)
			typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;

			if(end <= begin) return ;
			__rise_from_holeIdx(begin, difference_type(end - begin), difference_type(1), *(end-1), comp);
		}



	/*
	 * Function : start from *(begin + holeIdx) to check its left & right son, and adjust position. 
	 *		The value of *(begin + holeIdx) is already stored in value, we need to rise it to proper position.
	 */
	template<typename RandomAccessIterator,typename Distance,typename Tp,typename Comp>
		void __fill_hole_with_child(RandomAccessIterator begin,Distance holeIdx,Distance len,Tp value,Comp comp) {
			Distance topIdx = holeIdx;
			Distance rightSon = 2 * holeIdx + 1;//start from 1
			while(rightSon < len) {
				if( comp(*(begin + rightSon), *(begin + rightSon - 1) ) ) 
					-- rightSon;//the right one should be place down, so we up left one
				*(begin + holeIdx) = *(begin + rightSon);
				holeIdx = rightSon;
				rightSon = 2 * holeIdx + 1;
			}

			if(rightSon == len) {//最底层没有右儿子,导致while循环少处理了一层
				*(begin + holeIdx) = *(begin + rightSon - 1);
				holeIdx = rightSon - 1;
			}

			__rise_from_holeIdx(begin,holeIdx,topIdx,value,comp);
		}



	template<typename RandomAccessIterator>
		inline void pop_heap(RandomAccessIterator begin,RandomAccessIterator end) {
			if(begin >= end) return ;

			typedef iterator_traits<RandomAccessIterator>::value_type value = *(end - 1);
			*(end - 1) = *begin; // put the top elem in the back

			//fill top with chid and rist value to proper position
			__fill_hole_with_child(begin, 0, end - begin - 1, value,comp);
		}

	template<typename RandomAccessIterator,typename Compare>
		inline void make_heap(RandomAccessIterator begin,RandomAccessIterator end,Compare comp) {
			typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
			typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;

			if(end <= begin + 1) return ;

			difference_type len = end - begin;
			difference_type holeIdx = len / 2;//start from 1

			while(parent >= 1) {
				__fill_hole_with_child(begin, holeIdx, len, *(begin + holeIdx - 1));
				-- parent;
			}
		}

	template <typename RandomAccessIterator,typename Compare>
		void sort_heap(RandomAccessIterator begin,RandomAccessIterator end,Compare comp) {
			while(end - beign > 1) {
				pop_heap(begin,end,comp);
				--end;
			}
		}


}
#endif
