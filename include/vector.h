#pragma once

#include "vector-declare.h"

namespace Freeman{

	template<typename Tp>
		vector_alloc<Tp>::vector_alloc(size_t n) {
			start = end_of_data = allocate(n);
			this->end_of_storage = start + n;
		}

	template<typename Tp>
		void vector<Tp>::range_check(size_type n) const {
			if ( n > size() )
				throw std::range_error("Range error in vector");
		}

	template<typename Tp>
        typename vector<Tp>::reference vector<Tp>::at(size_type n){
			range_check(n);
			return (*this)[n];//directly use operator []
		}

	template<typename Tp>
        typename vector<Tp>::const_reference vector<Tp>::at(size_type n)const{
			range_check(n);
			return (*this)[n];
		}

	/*
	 * guarantee minimal memory
	 */
	template<typename Tp>
		void vector<Tp>::reserve(size_type n){ 
			if(capacity() < n) {
				const size_type old_size = size();//need to increase from old_size to n
				iterator tmp = allocate_and_copy(n,begin(),end());

				destory(begin(),end());//deconstruct objects
                deallocate(begin(), this->end_of_storage - begin());//release memory

                this->start = tmp;//adjust
                this->end_of_data = begin()+ old_size;
                this->end_of_storage = begin() + n;
			}
		}

	/*
	 * fill with particular value
	 */
	template<typename Tp>
        void vector<Tp>::fill_assign(size_type n, const Tp& val) {
			if(n > capacity()) {
				vector<Tp> tmp(n, val);//建立一个满足条件的临时对象,保证安全性
				tmp.swap(*this);
			}else if( n > size() ) {
				fill(begin() , end(), val);
				this->end_of_data = uninitialized_fill_n(end() , n - size(), val);
			}else {
				erase(fill_n(begin(), n, val) , end());
			}
		}

	template<typename Tp>
		void vector<Tp>::push_back(const Tp& ref) {
			insert(end(),ref);
		}

	template<typename Tp>
		void vector<Tp>::pop_back() {
			if(begin() != end()) {
                --this->end_of_data;
                destory(this->end_of_data);
			}
		}

	template<typename Tp>
		inline void __swap(Tp *&a,Tp *&b) {
			Tp *tmp = a;
			a = b;
			b = tmp;
		}

	template<typename Tp>
		void vector<Tp>::swap(vector<Tp>& ref) {
            __swap(ref.start, this->start);
            __swap(ref.end_of_data,this->end_of_data);
            __swap(ref.end_of_storage,this->end_of_storage);
		}


	template<typename Tp>
        typename vector<Tp>::iterator vector<Tp>::insert(iterator pos,const Tp& ref) {
            if(this->end_of_storage != this->end_of_data && pos == this->end_of_data) {//insert last
                construct(this->end_of_data, ref);
                ++ this->end_of_data;
			}else {
                if(this->end_of_data != this->end_of_storage) {//have space
                    construct(this->end_of_data, *(this->end_of_data - 1));
                    ++ this->end_of_data;
                    copy_backward(pos, this->end_of_data - 2, this->end_of_data - 1);//copy_backward need to be used on construct memory
                    *pos = ref;
				}else{//needs to apply
					const size_type len = 2 * (size() == 0 ? 1 : size());
					iterator new_start = this->allocate(len);
					iterator new_end_of_data = new_start;

					try {
						new_end_of_data = uninitialized_copy(begin(),pos,new_start);
						construct(new_end_of_data, ref);
						++new_end_of_data;
						new_end_of_data = uninitialized_copy(pos, end(), new_end_of_data);
					}catch(...) {
						destory(new_start, new_end_of_data);
						deallocate(new_start, len);
					}

					destory(begin(),end());//deconstruct objects and realse memory
					deallocate(begin(), this->end_of_storage - begin());

                    this->start = new_start;
                    this->end_of_data = this->new_end_of_data;
                    this->end_of_storage = this->new_start + len;
				}
			}
			return pos;
		}


	template<typename Tp>
		void vector<Tp>::insert(iterator pos,size_type n,const Tp& val) {
			if(n == 0)
				return ;

			if(this->end_of_storage - end() >= n){//剩余空间足够容纳
				const size_type elements_after = end() - pos;

				if(elements_after > n){//当pos后元素多于n时,需要采用反向复制(overwrite)
					//pos ---- pos + n --- end
					uninitialized_copy(this->end_of_data - n, this->end_of_data, this->end_of_data);

					iterator old_end_of_data = this->end_of_data;
					this->end_of_data += n;

					copy_backward(pos, old_end_of_data - n,old_end_of_data); 

					fill(pos, pos + n, val);
				}else{//当pos后元素少于n时,
					uninitialized_fill_n(end(), n - elements_after,val);

					iterator old_end_of_data = this->end_of_data;
					this->end_of_data += (n - elements_after);

					uninitialized_copy(pos, old_end_of_data, this->end_of_data);

					this->end_of_data += elements_after;

					fill(pos, old_end_of_data, val);
				}
			}else{
				//新的长度为max(旧长度,新长度),也可以旧长度 + max(新长度,旧长度)
				const size_type new_size = 2 * (size() > n ? size() : n);
				iterator new_start = this->allocate(new_size);
				iterator new_end_of_data = new_start;

				try {// begin() ----  pos ***** pos + n ----- end()
					new_end_of_data = uninitialized_copy(begin(),pos,new_start);
					new_end_of_data = uninitialized_fill_n(new_end_of_data,n,val);
					new_end_of_data = uninitialized_copy(pos,end(),new_end_of_data);
				}catch(...) {
					destory(new_start,new_end_of_data);
					deallocate(new_start, new_size);
				}

				destory(begin(),end());
				this->start = new_start;
				this->end_of_data = new_end_of_data;
				this->end_of_storage = this->new_start + new_size;
			}
		}


	
	template<typename Tp>
		typename vector<Tp>::iterator vector<Tp>::erase(iterator pos) {
			if(pos + 1 != end() ) {
				copy(pos+1 ,end(), pos);//overwrite
			}
			pop_back();
			return pos;
		}


	template<typename Tp>
		typename vector<Tp>::iterator vector<Tp>::erase(iterator begin,iterator end) {
			iterator tail = copy(end, end(), begin);
			destory(tail, end());
			end() = end() - (end - begin);
			return begin;
		}



	template<typename Tp>
		void vector<Tp>::clear() {
			erase(begin(),end());
		}


	template<typename Tp>
        void vector<Tp>::resize(size_type new_sz,const Tp& val){//多退少补
			if(new_sz < size()){
				erase(begin() + new_sz, end());
			}else if(new_sz > size()){
				insert(end(), new_sz - size(), val);
			}else{
				;
			}
		}




	/*
	 * operator
	 */
	template<typename Tp>
		inline bool operator == (const vector<Tp>& x, const vector<Tp>& y) {
			return x.size() == y.size() && equal(x.begin(),x.end(),y.begin());
		}
	template<typename Tp>
        inline bool operator != (const vector<Tp>&x , const vector<Tp>& y) { return !(x = y); }

	template<typename Tp>
		inline bool operator < (const vector<Tp>& x, const vector<Tp>& y) {
			return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end() );
		}
	template<typename Tp>
		inline bool operator > (const vector<Tp>& x, const vector<Tp>& y) { return y < x; }
	template<typename Tp>
		inline bool operator <= (const vector<Tp>& x, const vector<Tp>& y) { return ! (y > x); }
	template<typename Tp>
		inline bool operator >= (const vector<Tp>& x, const vector<Tp>& y) { return !( y < x); }

	template<typename Tp>
		vector<Tp>& vector<Tp>::operator = (const vector<Tp>& ref) {
			if( &ref != this) {
				const size_type len = ref.size();
				if(len > capacity() ){
					iterator new_pos = allocate_and_copy(len, ref.begin(),ref.end());

					destory(begin(), end());//deconstruct and realese memory
                    deallocate(begin(), this->end_of_storage - begin());

                    this->start = new_pos;
                    this->end_of_storage = this->start + len;
				}else if(size() >= len){
					iterator iter = copy(ref.begin(), ref.end(), begin());
					destory(iter,end());
				}else {
					copy(ref.begin(), ref.begin() + size() , begin());
					uninitialized_copy(ref.begin() + size(), ref.end(), end());
				}

                this->end_of_data = this->start + this->len;
			}
			return *this;
		}
}
