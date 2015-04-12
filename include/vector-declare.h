#pragma once

#include "memory.h"
#include "allocator.h"
#include "constructor.h"
#include "iterator.h"
#include "advance-algorithm.h"
#include <stdexcept>

namespace Freeman{

	template<typename Tp>
		class vector_alloc {
			protected:
				typedef Allocator<Tp> data_allocator;

				Tp* start;
				Tp* end_of_data;
				Tp* end_of_storage;

			protected:
				Tp* allocate(size_t n){ return data_allocator::allocate(n); }
				void deallocate(Tp* p,size_t n){data_allocator::deallocate(p,n);}

			public:
				vector_alloc() : start(nullptr),end_of_data(nullptr),end_of_storage(nullptr) {}
				vector_alloc(size_t n) ;

				~vector_alloc() { deallocate(start,end_of_storage - start); }
		};

	template<typename Tp>
		class vector : public vector_alloc<Tp> {
			public:
				typedef Tp									value_type;
				typedef Tp*									iterator; //vector's iterator is build-in pointer
				typedef const Tp*							const_iterator;
				typedef Tp*									pointer;
				typedef const Tp*							const_pointer;
				typedef Tp&									reference;
				typedef const Tp&							const_reference;
				typedef size_t								size_type;
				typedef ptrdiff_t							difference_type;
				typedef reverse_iterator<const_iterator>	const_reverse_iterator;
				typedef reverse_iterator<iterator>			reverse_iterator;

			private:
				void range_check(size_type n) const;

			public:
				iterator begin() {return this->start;}
				const_iterator begin()  const { return this->start; }

				iterator end() {return this->end_of_data;}
				const_iterator end() const {return this->end_of_data;}

				reverse_iterator rbegin() { return reverse_iterator(end());}
				const_reverse_iterator rbegin() const {return reverse_iterator(end());}

				reverse_iterator rend() { return reverse_iterator(begin());}
				const_reverse_iterator rend() const { return reverse_iterator(begin());}
				
				size_type size() const { return end() - begin(); }
				size_type capacity() const { return this->end_of_storage - begin(); }
				bool empty() const { return begin() == end(); }
				size_type max_size() const { return sizeof(-1) / sizeof(Tp); }

				reference operator [] (size_type n){ return *(begin() + n); }// directly use operator *
				const_reference operator [] (size_type n )const { return *(begin() + n); }

				reference at(size_type n);
				const_reference at(size_type n) const;

			public:
				explicit vector() : vector_alloc<Tp>() {}
				explicit vector(size_type n,const Tp& value) :vector_alloc<Tp>(n){ this->end_of_data = uninitialized_fill_n(begin(), n, value); }

				vector(const vector<Tp>& other):vector_alloc<Tp>(other.size()){
					this->end_of_data = uninitialized_copy(other.begin(),other.end(),begin());
				}
				
				vector(const Tp* first,const Tp* last) : vector_alloc<Tp> (end() - begin()) {
					this->end_of_data = uninitialized_copy(first,last,begin());
				}

				~vector() { destory(begin(),this->end_of_storage); }

			public:
				vector<Tp>& operator = (const vector<Tp>&);

				void reserve(size_type _n); //guarantee the minimal memory space

				void assign(size_type n, const Tp& val) { fill_assign(n,val); }
				void fill_assign(size_type n,const Tp& val);

			public:
				reference front() {	return *begin();}
				const_reference front() const { return *begin(); }
				reference back() { return *(end() - 1); }
				const_reference back() const { return *(end() - 1); }

				void push_back(const Tp&);
				void pop_back(); 

				void swap(vector<Tp>&);

				iterator insert(iterator,const Tp&);
				void insert(iterator pos,size_type n,const Tp& val) ;

				iterator erase(iterator pos);
				iterator erase(iterator begin,iterator end);

				void clear() ;

				void resize(size_type new_sz,const Tp& val);//多退少补
				void resize(size_type new_sz);
				
			protected:
				//return the beginning of allocated memory
				template< typename ForwardIterator>
					iterator allocate_and_copy(size_type n,ForwardIterator begin,ForwardIterator end) {
						iterator ite = this->allocate(n);
						try {
							uninitialized_copy(begin,end,ite);
							return ite;
						}catch(...){
							deallocate(begin,n);
						}
					}
		};
}
