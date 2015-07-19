#ifndef _freeman_hash_table_h_
#define _freeman_hash_table_h_

#include "allocator.h"
#include "iterator.h"
#include <cstddef>
#include "simple-algorithm.h"

namespace Freeman {

	template<typename Tp>
		struct Hashtable_Node {
			Hashtable_Node* next;
			Tp val;
		};



	/*
	 * iterator
	 */
	template<typename Val,typename Key,typename HashFunc,typename ExtractKey,typename EqualKey>
		class hashtable;

	template<typename Val,typename Key,typename HashFunc,typename ExtractKey,typename EqualKey>
		struct Hashtable_Iterator {
			typedef hashtable<Val,Key,HashFunc,ExtractKey,EqualKey> Hashtbl;
			typedef Hashtable_Iterator<Val,Key,HashFunc,ExtractKey,EqualKey> Iterator;
			typedef Hashtable_Node<Val> Node;

			typedef forward_iterator_tag iterator_category;
			typedef Val value_type;
			typedef ptrdiff_t difference_type;
			typedef size_t size_type;
			typedef Val& reference;
			typedef Val* pointer;

			Node* curr;
			Hashtbl* ht; 

			Hashtable_Iterator(Node* n,Hashtbl* htbl) : curr(n), ht(htbl) {}

			reference operator*() const { return curr -> val; }
			pointer operator->() const { return &(operator*()); }

			Iterator& operator++();
			Iterator operator(int);
			bool operator==(const Iterator& it) const {
				return curr == it.curr;
			}
			bool operator != (const Iterator& it) const {
				return curr != it.curr;
			}
		};




	/*
	 * hashtable
	 */
	enum { hashtable_primes_num = 28 };

	static const unsigned long prime_list[__stl_num_primes] =
	{
	  53ul,         97ul,         193ul,       389ul,       769ul,
	  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
	  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
	  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
	  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
	  1610612741ul, 3221225473ul, 4294967291ul
	};

	inline unsigned long next_prime(unsigned long n) {
		const unsigned long *first = prime_list;
		const unsigned long *last = prime_list + (int)hashtable_primes_num;
		const unsigned long *pos = lower_bound(first,last,n);
		return pos == last ? (last - 1) : *pos;
	}

	template<typename Val,typename Key,typename HashFunc,typename ExtractKey,typename EqualKey>
		class hashtable {
			public:
				typedef Key key_type;
				typedef Val value_type;
				typedef HashFunc hasher;
				typedef EqualKey key_equal;

				typedef size_t size_type;
				typedef ptrdiff_t difference;
				typedef value_type* pointer;
				typedef value_type& reference;

				hasher hash_funct() const { return hash; }
				key_type key_eq() const { return equals; }
			private:
				typedef Hashtable_Node<Val> Node;

				Node* get_node() { return allocator.allocate(sizeof(Node)); }
				void put_node(Node* p) { allocator.deallocate(p,sizeof(Node)); }



#endif
