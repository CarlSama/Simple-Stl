#ifndef _freeman_list_h_
#define _freeman_list_h_

#include <cstddef>
#include "iterator.h"
#include "iterator-ext.h"
#include "allocator.h"
#include "climits"

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


	template<typename Tp,typename Ref,typename Ptr>
	class list_iterator {
		public:
			typedef size_t									size_type;
			typedef ptrdiff_t								difference_type;
			typedef Tp										value_type;
			typedef Ref										reference;
			typedef Ptr										pointer;
			typedef bidirectional_iterator_tag				iterator_category;//double-end list
			typedef list_iterator<Tp,Tp&,Tp*>				iterator;
			typedef list_iterator<Tp,const Tp&,const Tp*>	const_iterator;

			typedef list_iterator<Tp,Ref,Ptr>				_self;

		public:
			list_node<Tp> *node;

			list_iterator(list_node<Tp> *lnptr) : node(lnptr) {}
			list_iterator(const list_iterator& lib) : node(lib.base()) {}
			list_iterator() {}

			bool operator == (const _self& ref) const {
				return ref.base() == base();
			}
			bool operator != (const _self& ref) const {
				return ref.base() != base();
			}
		public:
			reference operator *() const	{ return node -> data; }
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
				_self tmp = *this, _decr(); return tmp;
			}

		private:
			void _incr() { node = node -> next; }
			void _decr() { node = node -> prev; }
		};

	template<typename Tp>
		class list_alloc {
			public:
				typedef Allocator<Tp> data_allocator;

			public:
				list_alloc() {
					sentinal_head = _allocate_node();
					sentinal_head -> next = sentinal_head;
					sentinal_head -> prev = sentinal_head;
				}
				
				~list_alloc() {
					clear();
					deallocate(sentinal_head);
				}

				void clear();

				list_node<Tp>*& base() const { return sentinal_head; }

			private:
				list_node<Tp> *_allocate_node() { return data_allocator::allocate(sizeof(list_node<Tp>) * 1); }
				void _deallocate_node(list_node<Tp> *ptr) { data_allocator::deallocate(ptr,1); }

			private:
				list_node<Tp> *sentinal_head;
		};

	template<typename Tp>
		class list : public list_alloc<Tp> {
			private:
			public:
				typedef size_t size_type;
				typedef ptrdiff_t difference_type;
				typedef Tp value_type;
				typedef Tp* pointer;
				typedef const Tp* const_pointer;
				typedef Tp& reference;
				typedef const Tp& const_reference;

				typedef list_iterator<Tp,Tp&,Tp*> iterator;
				typedef list_iterator<Tp,const Tp&,const Tp*> const_iterator;
				typedef reverse_iterator<const_iterator> const_reverse_iterator;
				typedef reverse_iterator<iterator> reverse_iterator;

			public:
				explicit list() : list_alloc() {}

				iterator begin()			{ return this->sentinal_head->next; }
				const_iterator begin()const	{ return this->sentinal_head->next; }

				iterator end()				{ return this->sentinal_head; }
				const_iterator end() const	{ return this->sentinal_head; }

				reverse_iterator rbegin()	{ return reverse_iterator(end()); }
				const_reverse_iterator rbeign() const { return reverse_iterator(end());}

				reverse_iterator rend()		{ return reverse_iterator(begin()); }
				const_reverse_iterator rend() const { return reverse_iterator(begin()); }

				bool empty ()const { return this->sentinal_head->next=this->sentinal_head;}
				size_type size() const ;
				size_type max_size() const { return INT_MAX;}

				reference front() { return *begin(); }
				const_reference front() const { return *begin(); }
				reference back()  { return *( --end() ); }
				const_reference back() const { return *( --end() ); }

			public:
				void swap(list<Tp>& ref);
				iterator insert(iterator pos,const Tp& ref);
				void push_front(const Tp& ref) { insert(begin(),ref); }
				void push_back(const Tp& ref)  { insert(end(),ref) ; }

				iterator erase(iterator pos);
				void clear() { this->clear(); }

				void resize() (size_type _new_size,const Tp& x);

				void pop_front() { erase(begin()); }
				void pop_back() { iterator tmp = end();	erase(--tmp): }

			public:
				list(size_type n,const Tp& value) : list_alloc() {
					while(--n >= 0) {
						insert(begin(),value):
					}
				}
				
				~list() {}

				list<Tp>& operator = (const list<Tp>& x);

				void assign(size_type n,const Tp& val);

				void splice(iterator pos,list<Tp>& x) {  transfer(pos,x.begin(),x.end()): }
				void splice(iterator pos,iterator first,iterator last) {
					if(first != last)	transfer(pos,first,last);
				}
				void splice(iterator pos,iterator i){
					iterator j = i;
					--j;
					if(pos == i || pos == j) return ;
					transfer(pos,i,j);
				}

				void reomve(const Tp& value);
				void unique();
				void merge(list<Tp>& x);
				void reverse();
				void sort();

			protected:
				list_node<Tp>* _create_node(const Tp& ref);
				//move [first,last) to the front of pos
				void transfer(iterator pos,iterator first,iterator last);
		};

		
				

}



#endif
