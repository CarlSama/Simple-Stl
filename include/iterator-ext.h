#ifndef __ss_iterator_h__
#define __ss_iterator_h__

#include "iterator.h"

namespace Freeman {

	/*
	 * back_insert_iterator
	 *
	 * 间接调用push_back
	 */
	template<typename Container>
	class back_insert_iterator{
		protected:
			Container *containter;
		public:
			typedef Container container_type;
			typedef output_iterator_tag iterator_type;
			typedef void value_type;
			typedef void difference_type;
			typedef void pointer;
			typedef void reference;
			
			explicit back_insert_iterator(Container& x) : containter(x) {}
			back_insert_iterator<Container>& operator= (const typename Container::value_type& _value) {//back_insert_iterator = value_type;
				containter->push_back(_value);
				return *this;
			}
			back_insert_iterator<Container>& operator* ()   { return *this; }
			back_insert_iterator<Container>& operator++()   { return *this; }
			back_insert_iterator<Container> operator++(int) { return *this; }
	};

	//获得Constainer的back_insert_iterator
	template<typename Container>
	inline back_insert_iterator<Container> back_inserter(Container& x) {
		return back_insert_iterator<Container>(x);
	}


	/*
	 * front_insert_iterator
	 *
	 * 间接调用push_front
	 */
	template<typename Container>
	class front_insert_iterator{
		protected:
			Container *containter;
		public:
			typedef Container container_type;
			typedef output_iterator_tag iterator_type;
			typedef void value_type;
			typedef void difference_type;
			typedef void pointer;
			typedef void reference;
			
			explicit front_insert_iterator(Container& x) : containter(x) {}
			front_insert_iterator<Container>& operator= (const typename Container::value_type& _value) {
				containter->push_front(_value);
				return *this;
			}
			front_insert_iterator<Container>& operator* ()   { return *this; }
			front_insert_iterator<Container>& operator++()   { return *this; }
			front_insert_iterator<Container> operator++(int) { return *this; }
	};
	//获取Container的front_insert_iterator
	template<typename Container>
	inline front_insert_iterator<Container> front_inserter(Container& x) {
		return front_insert_iterator<Container>(x);
	}

	/*
	 * insert_iterator
	 *
	 * 间接调用insert && ++iterator
	 */
	template<typename Container>
	class insert_iterator {
		protected:
			Container *containter;
			typename Container::iterator iter;
		public:
			typedef Container container_type;
			typedef output_iterator_tag iterator_type;
			typedef void value_type;
			typedef void difference_type;
			typedef void pointer;
			typedef void reference;

			insert_iterator(Container& x, typename Container::iterator it) : containter(&x), iter(it) {}
			insert_iterator<Container>& operator= (const typename Container::value_type& vt) {
				iter = containter->insert(iter,vt);
				++iter;
				return *this;
			}

			insert_iterator<Container>& operator* () {return *this;}
			insert_iterator<Container>& operator++ () {return *this;}
			insert_iterator<Container> operator++ (int) {return *this;}
	};
	//获取Container的inserter
	template<typename Container,typename Iterator>
	inline insert_iterator<Container> inserter(Container& x,Iterator it){
		typedef typename Container::iterator iterator;
		return insert_iterator<Container>(x,iterator(it));
	}

	/*
	 * reverse_bidirectional_iterator
	 *
	 *       begin            iter				     end
	 *        x                x               x 
	 * rend			   riter				 rbegin 
	 *
	 * 为了前开后闭区间, reverse_iterator并未直接指向所代表的对象
	 */
	template<typename BidirectionalIterator,typename Tp,typename Distance,typename Reference=Tp&>
	class reverse_bidirectional_iterator {
			typedef reverse_bidirectional_iterator<BidirectionalIterator,Tp,Reference,Distance> Self;
		protected:
			BidirectionalIterator current_iterator;
		public:
			typedef bidirectional_iterator_tag  iterator_category;
			typedef Tp							value_type;
			typedef Distance					difference_type;
			typedef Reference					reference;
			typedef Tp*							pointer;

			explicit reverse_bidirectional_iterator(BidirectionalIterator bi) : current_iterator(bi) {}

			BidirectionalIterator base() const { return current_iterator; }

			Reference operator* () const {
				BidirectionalIterator tmp = current_iterator;
				return *( -- tmp);
			}

			pointer operator->() const {  return &(operator*()); }

			Self& operator++() {
				-- current_iterator;
				return *this;
			}
			Self operator++(int) {
				Self tmp = *this;
				-- current_iterator;
				return tmp;
			}

			Self& operator--() {
				++ current_iterator;
				return *this;
			}
			Self operator--(int) {
				Self tmp = *this;
				++ current_iterator;
				return tmp;
			}
	};

	template<typename BidirectionalIterator,typename Tp,typename Distance,typename Reference=Tp&>
	inline bool operator==(const reverse_bidirectional_iterator<BidirectionalIterator,Tp,Distance,Reference>& x,
			const reverse_bidirectional_iterator<BidirectionalIterator,Tp,Distance,Reference>& y) {
		return x.base() == y.base();//使用BidirectionalIterator重载的==
	}
	template<typename BidirectionalIterator,typename Tp,typename Distance,typename Reference=Tp&>
	inline bool operator!=(const reverse_bidirectional_iterator<BidirectionalIterator,Tp,Distance,Reference>& x,
			const reverse_bidirectional_iterator<BidirectionalIterator,Tp,Distance,Reference>& y) {
		return !(x == y);//使用BidirectionalIterator重载的==
	}

	/*
	 * reverse_iterator
	 */
	template<typename Iterator>
	class reverse_iterator {
		protected:
			Iterator current_iterator;
		public:
			typedef typename iterator_traits<Iterator>::value_type			value_type;
			typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;
			typedef typename iterator_traits<Iterator>::difference_type		difference_type;
			typedef typename iterator_traits<Iterator>::reference			reference;
			typedef typename iterator_traits<Iterator>::pointer				pointer;
			typedef Iterator												iterator_type;
			typedef reverse_iterator<Iterator>								Self;
		public:
			explicit reverse_iterator(Iterator x) : current_iterator(x) {}
			reverse_iterator(const Self& x) : current_iterator(x.current_iterator) {}

			Iterator base() const { return current_iterator; }

			reference operator*() {
				Iterator tmp = current_iterator;
				return * (-- tmp);
			}
			pointer operator->() {	return &(operator*()); }

			Self& operator++() {
				-- current_iterator;
				return *this;
			}
			Self operator++(int) {
				Self tmp = *this;
				-- current_iterator;
				return tmp;
			}

			Self& operator--() {
				++ current_iterator;
				return *this;
			}
			Self operator--(int) {
				Self tmp = *this;
				++ current_iterator;
				return tmp;
			}

			Self operator+ (difference_type len) const { return Self(current_iterator - len); }//not affect himself
			Self operator +=(difference_type len) {//affect himself
				current_iterator -= len;
				return *this;
			}

			Self operator - (difference_type len) { return Self(current_iterator + len); }//not affect himself 
			Self operator -= (difference_type len) {//affect himself
				current_iterator += len;
				return *this;
			}

			//[len]代表当前迭代器"前进"len步,并返回所指项的对象 -> *(reverse_iterator + len) -> *(*this + len)
			//this代表指向当前reverse_iterator对象的指针,*this代表reverse_iterator迭代器
			//[]其实是在间接的调用operator +
			reference operator[](difference_type len) const { return *(*this + len); }
	};

	//迭代器间的判断
	template<typename Iter>
	inline bool operator==(const reverse_iterator<Iter> &x,const reverse_iterator<Iter> &y) {
		return x.base() == y.base();
	}
	template<typename Iter>
	inline bool operator<(const reverse_iterator<Iter> &x,const reverse_iterator<Iter> &y) {
		return x.base() < y.base();
	}
	template<typename Iter>
	inline bool operator!=(const reverse_iterator<Iter> &x,const reverse_iterator<Iter> &y) {
		return !(x == y);
	}
	template<typename Iter>
	inline bool operator>(const reverse_iterator<Iter> &x,const reverse_iterator<Iter> &y) {
		return y < x;
	}
	template<typename Iter>
	inline bool operator>=(const reverse_iterator<Iter> &x,const reverse_iterator<Iter> &y) {
		return !(x < y);
	}
	template<typename Iter>
	inline bool operator<=(const reverse_iterator<Iter> &x,const reverse_iterator<Iter> &y) {
		return !(x > y);
	}

	//迭代器之间的减法操作
	template<typename Iter>
	inline typename reverse_iterator<Iter>::difference_type operator - (const reverse_iterator<Iter>& x, const reverse_iterator<Iter>& y) {
		return y.base() - x.base();
	}
	//迭代器递增操作
	template<typename Iter>
	inline reverse_iterator<Iter> operator + (const reverse_iterator<Iter>& x, const typename reverse_iterator<Iter>::difference_type n) {
		return reverse_iterator<Iter>(x.base() - n);//返回临时变量,借助于构造函数
	}

}

#endif
