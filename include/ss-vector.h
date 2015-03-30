#pragma once

#include "ss-uninitialized.h"

namespace Freeman{

	template<typename Tp,typename Allocator>
		class vector_private {
			protected:
				typedef allocator<Tp,Allocator> data_allocator;

				Tp* start;
				Tp* end_of_data;
				Tp* end_of_storage;

			private:
				Tp* allocate(size_t n){
					return data_allocator::allocate(n);
				}

				void deallocate(Tp* p,size_t n){
					data_allocator::deallocate(p,n);
				}

			public:
				typedef Allocator allocator_type;
				allocator_type get_allocator() const {
					return allocator_type();
				}

				vector_private(const Allocator&) : start(0),end_of_data(0),end_of_storage(0) {}
				vector_private(size_t n,const Allocator&) : start(0),end_of_data(0),end_of_storage(0) {
					start = end_of_data = data_allocator(n);
					end_of_storage = start + n;
				}

				~vector_private() {
					deallocate(start,end_of_storage - start);
				}
		};


	template<typename Tp,typename Alloc = allocator>
		class vector : public vector_private {
			public:
				typedef Tp value_type;
				typedef Tp* iterator;
				typedef const Tp* const_iterator;
				typedef Tp* pointer;
				typedef const Tp* const_pointer;
				typedef Tp& reference;
				typedef const Tp& const_reference;
				typedef size_t size_type;
				typedef ptrdiff_t difference_type;
				typedef vector_private<Tp,Alloc>::allocator_type allocator_type;

				allocator_type get_allocator() const {	return vector_private<Tp,Alloc>::get_allocator(); }

			protected:
				void insert_aux(iterator, const Tp&);
				void insert_aux(iterator);

				void range_check(size_type _n) const {A
					if(_n >= size())
						throw range_error("range error in vector");
				}

			public:
				iterator begin() {return start;}
				const_iterator begin()  const { return start; }

				iterator end() {return end_of_data;}
				const_iterator end() const {return end_of_data;}

				reverse_iterator rbegin() {
					return reverse_iterator(end_of_data);
				}
				const_reverse_iterator rbegin() cosnt {
					return reverse_iterator(end_of_data);
				}

				reverse_iterator rend() {
					return reverse_iterator(start);
				}

				const_reverse_iterator rend() const {
					return reverse_iterator(start);
				}
				
				size_type size() const {
					return end_of_data - start;
				}
				size_type capacity() const {
					return end_of_storage - start;
				}
				bool empty() const {
					return end_of_data - start;
				}
				size_type max_size() const {
					//0xffff ffff / size_of_element
					return sizeof(-1) / sizeof(Tp);
				}

				reference operator [] (size_type _n){
					return *(begin + _n);
				}

				const_reference operator [] (size_type _n ){
					return *(begin + _n);
				}

				reference at(size_type _n){
					range_check(_n);
					return (*this)[_n];
				}
				const_reference at(size_type _n){
					range_check(_n);
					return (*this)[_n];
				}

			public:

				explicit vector(const  allocator_type& _a = allocator_type() ) :  vector_private( _a) {}

				vector(size_type _n,const Tp& _value, const allocator_type& _a = allocator_type()) :vector_private(_n,_a){
					end_of_data = uninitialized_fill_n(start, _n, _value);
				}

				explicit vector(size_type _n) :vector_private(_n, allocator_type()) {
					end_of_data = uninitialized_fill_n(start,_n,Tp());
				}

				vector(const vector<Tp,Alloc> & _other) :vector_private(_other.size(), _other.get_allocator()) {
					end_of_data = uninitialized_copy(_other.begin(), _other.end() , start);
				}
				
				vector(const Tp* _first,const Tp* _last , const allocator_type& _at = allocator_type) : vector_private(end_of_data - start, _at) {
					end_of_data = uninitialized_copy(_first,_last,start);
				}

				~vector() {
					//check end here !
					destory(start,end_of_storage);
				}

				vector<Tp,Alloc>& operator = (const vector<Tp,Alloc>&);

				//保证空间最小值
				void reserve(size_type _n) {
					if(capacity() < _n){
						const old_size = size();
						iterator iter = allocate_and_copy(_n, start, end_of_data);
						destory(start,end_of_data);

						start = iter;
						end_of_data = start + old_size;
						end_of_storage=  start + _n;
					}
				}

				void fill_assign(size_type _n,const Tp& _val);
				void assign(size_type _n, const Tp& _val) {
					fill_assign(_n,_val);
				}


			public:
				reference front() {	return *start;}
				const_reference front() const { return *start; }
				reference back() { return *(end_of_data - 1); }
				const_reference back() const { return *(end_of_data - 1); }

				void push_back() (const Tp& _val){
					if(end_of_data != end_of_storage) {
						construct(end_of_data,_val);
						++end_of_data;
					}else｛
						insert_aux(end_of_data);
				}

				void swap(vector<Tp,Alloc>& _val){
					std::swap(start,           _val.start);
					std::swap(end_of_data,     _val.end_of_data);
					std::swap(end_of_storage,  _val.end_of_storage);
				}

				iterator insert(iterator _pos,const Tp& _val){
					size_type _n = _pos - start;
					if(end_of_data != end_of_storage && _pos == end_of_data){
						construct(end_of_data,_val);
						++end_of_data;
					}else{
						insert_aux(_pos,_val);
					}

					return start + _n;
				}

				void pop_back() {
					-- end_of_data;
					destory(end_of_data);
				}

				iterator erase(iterator _pos){
					if(_pos + 1 != end_of_data ) {
						copy(_pos + 1, end_of_data, _pos);
					}
					--end_of_data;
					destory(end_of_data);
				}

				iterator erase(iterator _first,iterator _last){
					iterator _tmp = copy(_last,end_of_data,_first);
					destory(_tmp,end_of_data);
					end_of_data = end_of_data - (_last - _first );
				}

				void resize(size_type _new_sz,const Tp& _val){
					//多退少补
					if(_new_sz > size() ){
						insert(end_of_data, _new_sz - size() , _val);
					}else{
						erase(start + _new_sz, end_of_data);
					}
				}

				void resize(size_type _new_sz){
					resize(_new_sz,Tp() );
				}

				void clear() {
					erase(begin(), end());
				}
				
			protected:
				template< typename ForwardIter>
					iterator allocate_and_copy(size_type _n,ForwardIter _first,ForwardIter _last) {
						iterator _res = allocate(_n);
						try {
							uninitialized_copy(_first,_last,_res);
							return _res;
						}catch (...) {
							deallocate(_res, _n);
						}
					}
				
				template<typename InputIter>
					void range_initialize(InputIter _first,InputIter _last,input_iterator_tag) {
						for ( ; _first != _last; ++_first){
							push_back(*_first);
						}
					}

				template<typename InputIter>
					void range_insert(iterator _pos,InputIter _first,InputIter _last,input_iterator_tag);

				template<typename InputIter>
					void range_insert(iterator _pos,InputIter _first,InputIter _last,forward_iterator_tag);
		};

		template<typename Tp, typename Alloc>
			inline bool operator == (const vector<Tp,Alloc>& _x, const vector<Tp,Alloc>& _y) {
				//To do : algorithm equal
				return _x.size() == _y.size() && equal(_x.begin(),_x.end(),_y.begin());
			}

		template<typename Tp, typename Alloc>
			inline bool operator != (const vector<Tp,Alloc>& _x, const vector<Tp,Alloc>& _y) {
				return !(_x == _y);
			}

		template<typename Tp,typename Alloc>
			inline bool operator < (const vector<Tp,Alloc>& _x, const vector<Tp,Alloc>& _y) {
				//To do : algorithm lexicographical_compare
				return lexicographical_compare(_x.begin(), _x.end(), _y.begin(), _y.end() );
			}

		template<typename Tp,typename Alloc>
			inline bool operator < (const vector<Tp,Alloc>& _x, const vector<Tp,Alloc>& _y) {
				//To do : algorithm lexicographical_compare
				return _y < _x;
			}

		template<typename Tp,typename Alloc>
			inline bool operator <= (const vector<Tp,Alloc>& _x, const vector<Tp,Alloc>& _y) {
				//To do : algorithm lexicographical_compare
				return ! (_y > _x);
			}

		template<typename Tp,typename Alloc>
			inline bool operator >= (const vector<Tp,Alloc>& _x, const vector<Tp,Alloc>& _y) {
				return !( _y < _x);
			}

		template<typename Tp,typename Alloc>
			vector<Tp,Alloc>& operator = (const vector<Tp,Alloc>& _val) {
				if( &val != this) {
					const size_type _xlen = _val.size();
					if(_xlen > capacity() ){
						iterator _tmp = allocate_and_copy(_xlen, _val.begin(),_val.end());
						destory(start,end_of_data);
						deallocate(start,,end_of_storage - start);
						start = _tmp;
						end_of_storage = start + _xlen;
					}else if(size() >= _xlen){
						iterator _iter = copy(_val.begin(), _val.end(), begin());
						destory(_iter,end_of_data);
					}else {
						copy(_val.begin(), _val.begin() + size() , start);
						uninitialized_copy(_val.begin() + size(), _val.end(),end_of_data);
					}

					end_of_data = start + _xlen;
				}
				return *this;
			}

	template<typename Tp,typename Alloc>
		void vector<Tp,Alloc>::fill_assign(size_type _n, const value_type& _val) {
			if(_n > capacity()) {
				vector<Tp,Alloc> _tmp(_n, _val,get_allocator() );
				_tmp.swap(*this);
			}else if( _n > size() ) {
				//To do: fill
				fill(begin() , end(), _val);
				end_of_data = uninitialized_fill_n(end_of_data, _n - size(), _val);
			}else {
				erase(fill_n(begin(),_n,_val) , end());
			}
		}
