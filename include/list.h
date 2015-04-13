#ifndef _freeman_list_h_
#define _freeman_list_h_

#include "list-declare.h"
#include "constructor.h"
#include "simple-algorithm.h"

namespace Freeman {

	template<typename Tp>
		void list_alloc<Tp>::clear() {
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
		typename list_node<Tp>* list<Tp>::_create_node(const Tp& ref) {
			list_node<Tp> *tmp = _allocate_node();
			try {
				construct(&(tmp->data), ref);
			}catch(...){
				_deallocate_node(tmp);
				tmp = nullptr;
			}
			return tmp;
		}

	template<typename Tp>
		typename list<Tp>::size_type list<Tp>::size() const { 
			size_type len = 0;
			distance(begin(),end(),len);
			return len;
		}

	template<typename Tp>
		void list<Tp>::swap(list<Tp>& ref) {
			list_node<Tp> *tmp = this->sentinal_head;
			this->sentinal_head = ref.base();	
			ref.base() = tmp; 
		}

	template<typename Tp>
		typename list<Tp>::iterator list<Tp>::insert(iterator pos,const Tp& ref) {
			list_node<Tp> *tmp = _create_node(ref);
			tmp->next = pos->node;
			tmp->prev = pos->node->prev;
			pos->node->prev = tmp;
			pos->node->prev->next = tmp;
			return tmp;
		}

	template<typename Tp>
		typename list<Tp>::iterator list<Tp>::erase(iterator iter) {
			iter->node->prev->next = iter->node->next;
			iter->node->next->prev = iter->node->prev;
			iterator res = iter->base->next;
			destory(iter);
			this->_deallocate_node(iter);
			return res;
		}



	template<typename Tp>
		list<Tp>& list<Tp>::operator = (const list<Tp>& x) {
		}



	template<typename Tp>
		void list<Tp>::assign(size_type n,const Tp& val){
		}



	template<typename Tp>
		void list<Tp>::transfer(iterator pos,iterator first,iterator last) {
			if(pos != last) {
				iterator tail = last->node->prev;

				first->node->prev->next = last->node;
				last->node->prev = first->node->prev;//move [first, last) out

				tail->node->next = pos->node;
				pos->node->prev->next = first->node;
				first->node->prev = pos->node->prev;
				pos->node->prev = tail->node;//set [first,last) into  pos->prev and pos
			}
		}

	template<typename Tp>
		inline bool operator == (const list<Tp>& x,const list<Tp>& y) {
			typedef typename list<Tp>::const_iterator const_iterator;
			const_iterator begin1 = x.begin();
			const_iterator end1 = x.end();
			const_iterator begin2 = y.begin();
			const_iterator end2 = y.end();

			while(begin1 != end1 && begin2 != end2 && *begin1 == *begin) {
				++begin1;	++begin2;
			}
			return begin1 == end1 && begin2 == end2;
		}
	template<typename Tp>
		inline bool operator < (const list<Tp>& x, const list<Tp>& y) {
			return lexicographical_compare(x.begin(),x.end(),y.begin(),y.end()):
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
			return !(x < y):
		}
	template<typename Tp>
		inline void swap(list<Tp>& x,list<Tp>& y) {
			x.swap(y);
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

	template<typename Tp>
		void list<Tp>::resize(size_type _new_size,const Tp& x) {
			iterator curr = begin();
			size_type len = 0;
			while(len < _new_size && curr != end()) {
				++len;
				++curr;
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

				while(*next == *curr) {
					next = erase(next);
				}

				curr->node->next = next->node;
			}
		}
	template<typename Tp>
		void list<Tp>::merge(list<Tp>& x) {
			iterator begin1 = begin();
			iterator end1 = end();
			iterator begin2 = x.begin();
			iterator end2 = x.end();

			while(begin1 != end1 && begin2 != end2) {
				if(*begin1 < *begin2) {
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
			iterator curr = begin();
			while(curr != end()) {
				swap(curr->base->next,curr->base->prev);
				curr = curr->base->prev;
			}
		}

	template<typename Tp>
		void list<Tp>::sort() {
			if(size() == 0 || size() ==1 ) return ;

			list<Tp> carry;
			list<Tp> counter[64];
			int fill = 0;
			while( !empty() ) {
				carry.splice(carry.begin(),*this,begin());
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
			swap(coutner[fill-1]);
		}
}
						

#endif
