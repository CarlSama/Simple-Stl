#ifndef _freeman_list_h_
#define _freeman_list_h_

#include <cstddef>
#include "iterator.h"
#include "iterator-ext.h"
#include "allocator.h"
#include "climits"
#include "constructor.h"
#include "simple-algorithm.h"

namespace Freeman {

	/*
	 * The node struct used in double-end list
	 */
	template<typename Tp>
		struct list_node {
			Tp data;
			struct list_node *prev;
			struct list_node *next;
		};


	/*
	 * We may encounter two type of accessing list member :
	 *		 using iterator or directly using pointer
	 *
	 * Encouraging using iterator to finish as many as jobs.
	 *
	 * When forced to using pointer, remember that, using **iter->base** to get the pointer from iterator.
	 *		Besides, list_node was defined as struct, so feel free to access it's memeber   :)
	 */
	template<typename Tp,typename Ref=Tp& ,typename Ptr=Tp*>
		struct list_iterator {
			typedef size_t									size_type;
			typedef ptrdiff_t								difference_type;
			typedef Tp										value_type;
			typedef Ref										reference;
			typedef Ptr										pointer;
			typedef bidirectional_iterator_tag				iterator_category;//double-end list
			typedef list_iterator<Tp,Tp&,Tp*>				iterator;
			typedef list_iterator<Tp,const Tp&,const Tp*>	const_iterator;
			typedef list_iterator<Tp,Ref,Ptr> _self;

			list_iterator(list_node<Tp> *lnptr) : base(lnptr) {}
			list_iterator(const list_iterator& lib) : base(lib.base) {}
			list_iterator() :base(nullptr) {}

			bool operator == (const list_iterator<Tp,Ref,Ptr>& ref) const {
				return *ref == *this;
			}
			bool operator != (const list_iterator<Tp,Ref,Ptr>& ref) const {
				return !(operator == (ref));
			}

			reference operator *() const	{ return base -> data; }
			pointer   operator ->() const	{ return &(operator*()); }

			_self& operator++() {
				_incr();	return *this;
			}
			_self operator++(int) {
				_self tmp = *this;	_incr() ; return tmp;
			}
			_self& operator--() {
				_decr();	return *this;
			}
			_self operator--(int){
				_self tmp = *this; _decr(); return tmp;
			}


			list_node<Tp> *base;

			void _incr() { base = base -> next; }
			void _decr() { base = base -> prev; }
		};

	template<typename Tp>
		class list_alloc {
			public:
				typedef Allocator<Tp> data_allocator;

			public:
				list_alloc() {
					sentinal_head = _allocate_node();
					sentinal_head -> next = sentinal_head;		sentinal_head -> prev = sentinal_head;
				}
				
				~list_alloc() {
					clear(); _deallocate_node(sentinal_head);
				}

				void clear();

				void swap(list_alloc<Tp> &other);

			private:
				list_node<Tp> *_allocate_node()				{ return data_allocator::allocate(sizeof(list_node<Tp>) * 1); }
				void _deallocate_node(list_node<Tp> *ptr)	{ data_allocator::deallocate(ptr,1); }

			protected:
				list_node<Tp> *sentinal_head;
		};

	template<typename Tp>
		class list : public list_alloc<Tp> {
			public:
				typedef size_t						size_type;
				typedef ptrdiff_t					difference_type;
				typedef Tp							value_type;
				typedef Tp*							pointer;
				typedef const Tp*					const_pointer;
				typedef Tp&							reference;
				typedef const Tp&					const_reference;

				typedef list_iterator<Tp,Tp&,Tp*>				iterator;
				typedef list_iterator<Tp,const Tp&,const Tp*>	const_iterator;
				typedef reverse_iterator<const_iterator>		const_reverse_iterator;
				typedef reverse_iterator<iterator>				reverse_iterator;
			protected:
				using list_alloc<Tp>::sentinal_head;
				using list_alloc<Tp>::_allocate_node;
				using list_alloc<Tp>::_deallocate_node;

			public:
				explicit list() : list_alloc<Tp>() {}

				iterator begin()			{ return sentinal_head->next; }
				const_iterator begin()const	{ return sentinal_head->next; }

				iterator end()				{ return sentinal_head; }
				const_iterator end() const	{ return sentinal_head; }

				reverse_iterator rbegin()				{ return reverse_iterator(end()); }
				const_reverse_iterator rbeign() const	{ return reverse_iterator(end());}

				reverse_iterator rend()					{ return reverse_iterator(begin()); }
				const_reverse_iterator rend() const		{ return reverse_iterator(begin()); }

				bool empty ()const						{ return begin() == end();}
				size_type size() const ;
				size_type max_size() const				{ return INT_MAX;}

				reference front()						{ return *begin(); }//using operaot * in  list_iterator
				const_reference front() const			{ return *begin(); }
				reference back()						{ return *( --end() ); }
				const_reference back() const			{ return *( --end() ); }

			public:
				void swap(list<Tp>& ref);

				iterator insert(iterator pos,const Tp& ref);
				void insert(iterator pos,iterator begin,iterator end);
				void insert(iterator pos,size_type n,const Tp& val);
				iterator erase(iterator pos);
				void erase(iterator begin,iterator end);

				void push_front(const Tp& ref)			{ insert(begin(),ref); }
				void push_back(const Tp& ref)			{ insert(end(),ref) ; }
				void pop_front()						{ erase(begin()); }
				void pop_back()							{ iterator tmp = end();	erase(--tmp); }

				void clear() { list_alloc<Tp>::clear(); }

				void resize(size_type _new_size,const Tp& x);

			public:
				list(size_type n,const Tp& value) : list_alloc<Tp>() {
					while(--n >= 0) {
						insert(begin(),value);
					}
				}
				
				~list() {}

				list<Tp>& operator = (const list<Tp>& x);

				void splice(iterator pos,list<Tp>& x)					{  transfer(pos,x.begin(),x.end()); }
				void splice(iterator pos,iterator first,iterator last)	{  if(first != last)	transfer(pos,first,last); }
				void splice(iterator pos,iterator i){
					iterator j = i;		--j;
					if(pos == i || pos == j) return ;
					transfer(pos,i,j);
				}

				void remove(const Tp& value);
				void unique();
				void merge(list<Tp>& x);
				void reverse();
				void sort();

			protected:
				list_node<Tp>* _create_node(const Tp& ref);
				void transfer(iterator pos,iterator first,iterator last);//move [first,last) to the front of pos
		};
		
	template<typename Tp>
		void Freeman::list_alloc<Tp>::clear() {
			list_node<Tp> *curr = sentinal_head -> next;
			while(curr != sentinal_head){
				list_node<Tp> *tmp = curr;
				curr = curr -> next;
				destory(& tmp->data);
				_deallocate_node(tmp);
			}
			sentinal_head -> next = sentinal_head;
			sentinal_head -> prev = sentinal_head;
		}



	template<typename Tp>
		void list<Tp>::swap(list<Tp> &other) {
			swap(sentinal_head, other.sentinal_head);//Oh my.. don't know this before. Becase two same class, it's ok to access protected member
		}//Using same class data / function member is valid.


	template<typename Tp>
		typename list<Tp>::size_type list<Tp>::size() const {
			size_type len = 0;
			distance(begin(),end(),len);
			return len;
		}

	template<typename Tp>
		list_node<Tp>* list<Tp>::_create_node(const Tp& ref) {
			list_node<Tp> *tmp = _allocate_node();
			try {
				construct(&(tmp->data), ref);
			}catch(...){
				_deallocate_node(tmp);
				tmp = nullptr;
			}
			return tmp;
		}



	/*
	 * insert method for list
	 */
	template<typename Tp>
		typename list<Tp>::iterator list<Tp>::insert(iterator pos,const Tp& ref) {
			list_node<Tp> *tmp = _create_node(ref);//create a new node base on ref, and insert it into the list
			tmp->next = pos->base;
			tmp->prev = pos->base->prev;
			pos->base->prev->next = tmp;
			pos->base->prev = tmp;
			return tmp;
		}
	template<typename Tp>
		void list<Tp>::insert(iterator pos,iterator begin,iterator end) {
			if(begin == end) return ;

			iterator tail = --end;//when manage iterator, using defined oprator, not directly manage pointer

			begin->base->prev->next = end->base;
			end->base->prev = begin->base->prev;//cutting [begin,tail] from it's original sequence

			pos->base->prev->next = begin->base;
			begin->base->prev = pos->base->prev;

			tail->base->next = pos->base;
			pos->base->prev = tail->base;
		}
	template<typename Tp>
		void list<Tp>::insert(iterator pos,size_type n,const Tp& val) {
			while(--n >= 0) {//just call n times
				insert(pos,val);
			}
		}



	/*
	 * erase method for list
	 */
	template<typename Tp>//return then next iterator after **iter**
		typename list<Tp>::iterator list<Tp>::erase(iterator iter) {
			iterator res = iter->next;

			iter->base->prev->next = iter->base->next;//link it's neighbourhoods
			iter->base->next->prev = iter->base->prev;

			destory(iter);
			_deallocate_node(iter);
			return res;
		}
	template<typename Tp>
		void list<Tp>::erase(iterator begin,iterator end) {
			while(begin != end) {
				erase(begin);
				++begin;
			}
		}



	/*
	 * move [first,last) to the front of pos
	 */
	template<typename Tp>
		void list<Tp>::transfer(iterator pos,iterator first,iterator last) {
			if(pos != last) {
				list_node<Tp>* tail = last->node->prev;

				first->node->prev->next = last->node;
				last->node->prev = first->node->prev;//move [first, last) out

				tail->node->next = pos->node;
				pos->node->prev->next = first->node;
				first->node->prev = pos->node->prev;
				pos->node->prev = tail->node;//set [first,last) into  pos->prev and pos
			}
		}


	template<typename Tp>
		list<Tp>& list<Tp>::operator = (const list<Tp>& x) {
			if(this != &x) {
				iterator begin1 = begin();
				iterator end1 = end();
				const_iterator begin2 = x.begin();
				const_iterator end2 = x.end();
				while(begin1 != end1 && begin2 != end2) {
					*begin1 = *begin2;
					++begin1;		++begin2;
				}
				if(begin1 == end1)
					insert(end1,begin2,end2);
				else
					erase(begin1,end1);
			}
			return *this;
		}


	/*
	 * kinds of operator
	 */
	template<typename Tp>
		inline bool operator == (const list<Tp>& x,const list<Tp>& y) {
			typedef typename list<Tp>::const_iterator const_iterator;
			const_iterator begin1 = x.begin(), end1 = x.end();
			const_iterator begin2 = y.begin(), end2 = y.end();

			while(begin1 != end1 && begin2 != end2 && *begin1 == *begin2) {
				++begin1;	++begin2;
			}
			return begin1 == end1 && begin2 == end2;
		}
	template<typename Tp>
		inline bool operator < (const list<Tp>& x, const list<Tp>& y) {
			return lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
		}
	template<typename Tp>
		inline bool operator != (const list<Tp>& x,const list<Tp>& y) {
			return !(x == y);
		}
	template<typename Tp>
		inline bool operator > (const list<Tp>& x,const list<Tp>& y) {
			return y < x;
		}
	template<typename Tp>
		inline bool operator <= (const list<Tp>& x,const list<Tp>& y) {
			return !(x > y);
		}
	template<typename Tp>
		inline bool operator >= (const list<Tp>& x,const list<Tp>& y) {
			return !(x < y);
		}
	template<typename Tp>
		inline void swap(list<Tp>& x,list<Tp>& y) {
			x.swap(y);
		}


	template<typename Tp>
		void list<Tp>::resize(size_type _new_size,const Tp& x) {
			iterator curr = begin();
			size_type len = 0;
			while(len < _new_size && curr != end()) {
				++len;		++curr;
			}
			if(len == _new_size)
				erase(curr,end());
			else
				insert(end(),_new_size - len,x);
		}



	template<typename Tp>
		void list<Tp>::remove(const Tp& value ) {
			iterator curr = begin();
			while(curr != end) {
				if(*curr == value) {
					iterator tmp = curr;
					++curr;
					erase(tmp);
				}else
					++curr;
			}
		}


	template<typename Tp>
		void list<Tp>::unique() {
			iterator curr = begin();
			while(curr != end() ){
				iterator next = curr;
				++next;

				while(*next == *curr)
					next = erase(next);

				curr->node->next = next->node;
				++curr;
			}
		}



	template<typename Tp>
		void list<Tp>::merge(list<Tp>& x) {
			iterator begin1 = begin(),		end1 = end();
			iterator begin2 = x.begin(),	end2 = x.end();

			while(begin1 != end1 && begin2 != end2) {
				if(*begin1 > *begin2) {//we need to insert *beign2 in front of *begin1
					//let's transfer :L
					iterator next = begin2;
					transfer(begin1,begin2,++next);
					begin2 = next;
				}else
					++begin1;
			}

			if(begin2 != end2)
				transfer(end1,begin2,end2);
		}



	template<typename Tp>
		void list<Tp>::reverse() {
			if(size()==0 || size() == 1)	return ;
			list_node<Tp> *curr = sentinal_head->next;
			do{
				swap(curr->next,curr->prev);
				curr = curr->prev;//the current prev is previous next
			}while(curr != sentinal_head);
		}



	template<typename Tp>
		void list<Tp>::sort() {
			if(size() == 0 || size() ==1 ) return ;

			list<Tp> carry;
			list<Tp> counter[64];
			int fill = 0;
			while( !empty() ) {
				carry.splice(carry.begin(),begin());//one by one , we cut begin() from list and insert into carry
				int i = 0;
				while(i < fill && !counter[i].empty()){
					counter[i].merge(carry);
					carry.swap(counter[i++]);
				}
				carry.swap(counter[i]);
				if(i == fill)
					++fill;
			}

			for(int i = 1;i<fill;i++) {
				counter[i].merge(counter[i-1]);
			}
			swap(counter[fill-1]);
		}

}



#endif
