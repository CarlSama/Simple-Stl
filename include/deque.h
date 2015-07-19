#ifndef _freeman_deque_h_
#define _freeman_deque_h_

#include <cstddef>
#include "allocator.h"
#include "simple-algorithm.h"
#include "advance-algorithm.h"
#include <stdexcept>
#include "iterator-ext.h"
#include "memory.h"

namespace Freeman {

	/*
	 * Explination : 
	 *
	 *		There are two kind of range here. [first_of_buffer,end_of_buffer) in each buffer and [first_of_
	 *	
	 *		1) In deque_alloc, we use [first,last) to indicate the iterator range of valid rage in map.
	 *		So in deque, [begin,end) would be mapped to [first,last)
	 *
	 *		2) the start and finish iterator in deque_alloc have different way to explain memory
	 *		I mean **curr** move method and data store stategy
	 *
	 *
	 */

	/*
	 * used to decide the buffer_size 
	inline static size_t buffer_size(size_t size) { return (size < 512 ? 512 / size : size); }

	template<typename Tp,typename Ref,typename Ptr>
	struct deque_iterator {
		typedef Tp value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type; 

		typedef deque_iterator<Tp,Tp&,Tp*>				iterator;
		typedef deque_iterator<Tp,const Tp&,const Tp*>	const_iterator;

		typedef deque_iterator<Tp,Ref,Ptr> _self;

		Tp* curr, first, last;
		Tp** node;


		deque_iterator(Tp* x,Tp** y) : curr(x),first(*y),last(*y + buffer_size(sizeof(Tp))),node(y) {}
		deque_iterator(const iterator& x) : curr(x.curr),first(x.first),last(x.last),node(x.node) {}

		reference operator *() const { return *curr; }
		pointer operator -> () const { return curr; }

		void set_node(Tp** new_node) {
			node = new_node;
			first = *node;
			last = first + difference_type(buffer_size(sizeof(Tp)));
		}

		difference_type operator -(const _self& x) const {
			return return difference_type(buffer_size(sizeof(Tp)) * (node - x.node - 1)) + (curr - first) + (x.last - x.curr);
		}

		_self& operator ++ () { 
			++curr;
			if(curr == last) {
				set_node(node + 1);
				curr = first;
			}
			return *this;
		}
		_self operator ++ (int) {
			_self tmp = *this;
			++*this;
			return tmp;
		}
		_self& operator -- () {
			if(curr == first) {
				set_node(node - 1);
				curr = last;
			}
			--curr; //[first, last)
			return *this;
		}
		_self operator -- (int) {
			_self tmp = *this;
			-- *this;
			return tmp;
		}
		_self& operator += (difference_type n) {
			difference_type offset = curr - first + n;
			if(offset >= 0 && offset < difference_type(buffer_size(sizeof(Tp)))) {
				curr += n;//still in the same node after jumping
			}else{
				difference_type nodes_jumped = offset > 0 ? ( offset / buffer_size() ) :
					- difference_type(-1 * offset - 1 / buffer_size()) - 1;//-1 ~ -8 all should locate in -1 node

				set_node(node + nodes_jumped);
				curr = first + offset - nodes_jumped * buffer_size(sizeof(Tp));
				//if(offset > 0)	curr = first + offset - nodes_jumped * buffer_size();
				//else				curr = first + (-1*nodes_jumped) * buffer_size() - (-1) * offset
			}
			return *this;
		}
		_self operator+(difference_type n) const{
			_self tmp = *this;
			return tmp += n;
		}
		_self operator -=(difference_type n)		{ return *this += (-n);}
		_self& operator - (difference_type n) const { _self tmp = *this;	return tm -= n; }
		reference operator [](difference_type n) const {
			return *( *this += n );//using object.operator += n;
		}
		bool operator == (const _self& x) const { return curr == x.curr; }
		bool operator != (const _self& x) const { return curr != x.curr; }
		bool operator <  (const _self& x) const { return (node == x.node) ? (curr < x.curr) : (node < x.node); }//Must consider wether two iterator point to same place
	};

	template<typename Tp>
		class deque_alloc  {
			public:
				typedef Allocator<Tp> data_allocator; 
				typedef deque_iterator<Tp,Tp&,Tp*> iterator;
				typedef deque_iterator<Tp,const Tp&,const Tp*> const_iterator;
				
				deque_alloc() : map(nullptr),map_size(0) {}
				deque_alloc(size_t n){
					_initialize_map(n);
				}

				~deque_alloc();

			protected:
				static const initial_map_size = 8;
				Tp** map;
				size_type map_size;
				iterator start,finish;		//Attention : [start,finish]

				void _allocate_map(size_t); 
				void _deallocate_map(Tp**, size_t);
				void _allocate_node(size_t); 
				void _deallocate_node(Tp**, size_t);
				void _create_nodes(Tp** ,Tp** );//[ , )
				void _destory_nodes(Tp** ,Tp** );//[ , )
				void _initialize_map(size_t);
		};
	template<typename Tp> 
		Tp** deque_alloc<Tp>::_allocate_map(size_t map_size) {
			return data_allocator::allocate( map_size * sizeof(Tp*) );
		}
	template<typename Tp>
		void deque_alloc<Tp>::_deallocate_map(Tp** ptr,size_t n) {
			data_allocator::deallocate(ptr,n);
		}
	template<typename Tp> 
		Tp** deque_alloc<Tp>::_allocate_node() {
			return data_allocator::allocate( buffer_size(sizeof(Tp)); );
		}
	template<typename Tp>
		void deque_alloc<Tp>::_deallocate_node(Tp** ptr,size_t n) {
			data_allocator::deallocate(ptr,n);
		}
	template<typename Tp>
		deque_alloc<Tp>::~deque_alloc()  {
			if(map) {
				_destory_nodes(first.node, last.node);
				_deallocate_map(map,map_size);
			}
		}
	template<typename Tp>
		void deque_alloc<Tp>::_create_nodes(Tp** begin,Tp** end) {
			Tp** curr = begin;
			try {
				while(curr != end) {
					curr = data_allocator::allocate( buffer_size(sizeof(Tp) ));
					++curr;
				}
			}catch(...) {
				_destory_nodes(begin,curr);//destory alreay allocated notes
			}
		}
	template<typename Tp>
		void deque_alloc<Tp>::_destory_nodes(Tp** begin,Tp** end) {
			while(begin != end) {//we are destory the memory pointed by *begin
				data_allocator::deallocate(*begin,buffer_size(sizeof(Tp)));
				++begin;
			}
		}
	template<typename Tp>
		deque_alloc<Tp>::_initialize_map(size_t n) {
			size_t num_nodes = n / buffer_size(sizeof(Tp)) + 1;//if n is k times of buffer_size, we would allocate 1 more nodes
			map_size =  max(initial_map_size, num_nodes + 2);//we reserve 2 node in the head and tail for later expand
			_allocate_map(map_size);
			Tp** begin = map + (map_size - num_nodes)/2;//here, we make the first point to a symmetry begining. Also notes that we leave extra nodes
			Tp** end = begin + num_nodes;//[first,tail)
			try {
				_create_nodes(begin ,end );
			}catch(...){
				_deallocate_map(map,map_size);
				map = nullptr;		map_size = 0;
			}
			start.set_node(begin);
			start.curr = start.first;
			finish.set_node(end n- 1);
			finish.curr = finish.first + n % buffer_size(sizeof(Tp));
		}




	/*
	 * class deque
	 */
	template<typename Tp>
		class deque : public deque_alloc<Tp> {
			public:
				typedef Tp			value_type;
				typedef Tp*			pointer;
				typedef const Tp*	const_pointer;
				typedef Tp&			reference;
				typedef const Tp&	const_reference;
				typedef size_t		size_type;
				typedef ptrdiff_t	difference_type;


				using deque_alloc<Tp>::iterator;
				using deque_alloc<Tp>::const_iterator;
				typedef reverse_iterator<const_iterator>	const_reverse_iterator;
				typedef reverse_iterator<iterator>			reverse_iterator;

			public:
				deque() : deque_alloc<Tp>() {}
				deque(deque<Tp>& x) : deque_alloc<Tp>(x.size()) { uninitialized_copy(x.begin(),x.end(),begin()); }
				explicit deque(size_type n) : deque_alloc<Tp>(n) {}

				~deque() {}

				deque& operator = (const deque& );

				void swap(deque&);

			public:
				iterator				begin() { return start; }
				const_iterator			begin() const { return start;}
				iterator				end() { return finish + 1; }
				const_iterator			end() const { return finish + 1;}
				reverse_iterator		rbegin() { return reverse_iterator(end()); }
				const_reverse_iterator	rbegin() const { return reverse_iterator(end()); }
				reverse_iterator		rend() { return reverse_iterator(begin()); }
				const_reverse_iterator	rend() const { return reverse_iterator(begin()); }

				reference		operator[](size_type n) {return start[n]; }
				const_reference operator [](size_type n) const { return start[n]; }
				reference		at(size_type n){
					_range_check(n);	return (*this)[n];
				}
				const_reference at(size_type n)const {
					_range_check(n);	return (*this)[n];
				}
				reference		front() { return *start; }
				const_reference front()const { return *start; }
				reference		back() { return *finish; }	
				const_reference back() const { return *finish; }	
				size_type		size() const { return finish - start;//using operator - in class deque_iterator }
				size_type		max_size() const {	return size_type(-1); }
				bool			empty() const {
					return ;//to do later
				}
				
			public:
				void push_back(const value_type& x);
				void pop_back();
				void push_front(const value_type& x);
				void pop_front();

				iterator insert(iterators, const value_type&);

				void resize(size_type new_size,const Tp& x);

				void erase(iterator pos);

			protected:
				using deque_alloc<Tp>::map;
				using deque_alloc<Tp>::map_size;
				using deque_alloc<Tp>::start;
				using deque_alloc<Tp>::finish;
				using deque_alloc<Tp>::initial_map_size;
				using deque_alloc<Tp>::_initialize_map;

				void _range_check(size_type n) const {
					if(n >= size()){
						throw std::range_error("deque");
					}
				}

				void _reserve_map_at_front(size_type _node_to_add = 1);
				void _reserve_map_at_back(size_type _node_to_add = 1);
				void _reallocate_map(size_type _node_to_add,bool add_to_front);

		};
	template<typename Tp>
		void deque<Tp>::_reserve_map_at_front(size_type _node_to_add) {
			if(_node_to_add > start.node - map){// more space are needed
				_reallocate_map(_node_to_add,true);
			}
		}
	template<typename Tp>
		void deque<Tp>::_reserve_map_at_back(size_type _node_to_add) {
			if(_node_to_add > map_size - (finish - map))
				_reallocate_map(_node_to_add,false);
		}
	template<typename Tp>
		void deque<Tp>::_reallocate_map(size_type node_to_add,bool add_to_front) {
			size_type old_nodes = finish.node - start.node + 1;
			size_type new_nodes = old_nodes + node_to_add;

			value_type** new_start = nullptr;
			if(map_size > 2 * new_node) {//已有空间是需求空间的2倍以上,仍然可以使用已有空间
				new_start = map + (map_size - new_nodes) / 2 + (add_to_front ? node_to_add : 0);

				if(new_start < start.node) {
					copy(start.node,finish.node + 1,new_start);
				}else{
					copy_backward(start.node,finish.node + 1,new_start + old_nodes);
				}
			}else{//we just need to find a new map[] :)
				size_type new_map_size = map_size + max(map_size, node_to_add) + 2;
				value_type** new_map = _allocate_map(new_map_size);
				new_start = new_map + (new_map_size - new_nodes)/2 + add_to_front ? node_to_add : 0;
				copy(start.node,finish.node+1,new_start);
				_deallocate_map(map,map_size);

				map = new_map;
				map_size = new_map_size;
			}

			start.set_node(new_start);
			finish.set_node(new_start + old_nodes - 1);
		}

	template<typename Tp>
		deque<Tp>& deque<Tp>::operator = (const deque& x) {
			if(&x != this) {
				if(x.size() < size()) {
					erase(copy(x.begin(),x.end(),begin()),end());
				}else{
					const_iterator tail_of_capacity = x.begin() + size();
					copy(x.begin(),tail_of_capacity,begin());
					insert(end(),tail_of_capacity,end());
				}
			}
			return *this;
		}
	template<typename Tp>
		void deque<Tp>::swap(deque& x) {
			swap(x.map,		map);
			swap(x.first,	first);
			swap(x.finish,	finish);
			swap(x.node,	node);
		}

	template<typename Tp>
		void deque<Tp>::push_back(const value_type& x){
			if(finish.curr != finish.last - 1){//still have at least one place in the back
				construct(curr,x);
				++curr;
			}else{//No memory available, NEED to allocate new memory in advance
				_reserve_map_at_back();
				*(finish.node + 1) = _allocate_node();
				try {
					construct(finish.curr, x);
					finish.set_node(finish.node + 1);
					finish.curr = finish.first;
				}catch(...){
					_deallocate_node(finish.node + 1);
				}
			}
		}
	template<typename Tp>
		void deque<Tp>::push_front(const value_type& x){
			if(start.curr != start.first){//still have at least one place in the back
				construct(start.curr - 1,x);
				--curr;
			}else{
				_reserve_map_at_front();
				*(start.node - 1) = _allocate_node();
				try{
					start.set_node(start.node - 1);
					start.curr = start.last;//pointer to the last
					construct(-- start.curr);
				}catch(...){
					++start;
					_deallocate_node(*(start.node - 1));
				}
			}
		}
	template<typename Tp>
		void deque<Tp>::pop_back(){
			if(finish.curr != finish.first) {//the node have at least one elem, destory the backer
				-- finish.curr;
				destory(finish.curr);
			}else{//now the back node have 0 elem, we have to deallocate this node and destory the backer of previous node
				_deallocate_node(finish.first);
				finish.set_node(finish.node - 1);
				finish.curr = finish.last - 1;
				destory(finish.curr);
			}
		}
	template<typename Tp>
		void deque<Tp>::pop_front(){
			if(start.curr != start.last - 1){//the front memory have more than 1 elem
				destory(start.curr);
				++start.curr;
			}else{//the front memory have only 1 elem, delete elem and deallocate node
				destory(start.curr);
				_deallocate_node(start.node);
				start.set_node(start.node + 1);
				start.curr = start.first;//here ! we have to take the full node as normal
			}
		}
	template<typename Tp>
		typename deque<Tp>::iterator deque<Tp>::insert(iterator pos,const value_type& x) {
			if(pos.curr == start.curr){
				push_front(x);
				return start;
			}else if(pos.curr == finish.curr) {
				push_back(x);

				iterator tmp = finish;
				--tmp;
				return tmp;
			}else{
				if(pos - start < size() / 2) {//pos is in the front half interval
					push_front(front());
					iterator front1 = start;
					++front1;

					iterator front2 = front1;
					++front2;

					iterator after_pos = pos;
					++after_pos;

					copy(front2, after_pos, front1);
				}else{
					push_back(back());
					iterator back1 = finish;
					--back1;

					iterator back2 = back1;
					--back2;

					copy_backward(pos,back2,back1);
				}
			}
			*pos = x;
			return pos;
		}

	template<typename Tp>
		void deque<Tp>::resize(size_type new_size,const Tp& x) {
			if(new_size < size()){
				erase(start + new_size, end());
			}else{
				size_type n - new_size - size();
				while(--n >= 0)
					insert(end(),x);
			}
		}
	template<typename Tp>
		void deque<Tp>::erase(iterator pos) {
			iterator next = pos;
			++next;

			difference_type len = pos - begin();
			if(pos - start < size()/2) {
				copy_backward(begin(),pos,next);
				pop_front();
			}else{
				copy(next,end(),pos);
				pop_back();
			}
			return begin() + len;
		}









}

#endif
