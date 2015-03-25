#pragma once

/*
 * back_intsert_iterator
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
		
		explicit back_insert_iterator(Container& _x) : containter(x) {}
		back_insert_iterator<Container> & operator = (const typename Container::value_type& _value) {
			containter->push_back(_value);
			return *this;
		}
		
		back_insert_iterator<Container>& operator* () {
			return *this;
		}

		back_insert_iterator<Container>& operator++(){
			return *this;
		}

		back_insert_iterator<Container> operator++(int){
			return *this;
		}
};

template<typename Container>
inline back_insert_iterator<Container> back_inserter(Container& _x) {
	return back_insert_iterator<Container>(_x);
}


/*
 * front_insert_iterator
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
		
		explicit front_insert_iterator(Container& _x) : containter(x) {}
		front_insert_iterator<Container> & operator = (const typename Container::value_type& _value) {
			containter->push_front(_value);
			return *this;
		}
		
		front_insert_iterator<Container>& operator* () {
			return *this;
		}

		front_insert_iterator<Container>& operator++(){
			return *this;
		}

		front_insert_iterator<Container> operator++(int){
			return *this;
		}
};

template<typename Container>
inline front_insert_iterator<Container> back_inserter(Container& _x) {
	return front_insert_iterator<Container>(_x);
}

/*
 * insert_iterator
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

		insert_iterator(Container& _x, typename Container::iterator _iter) : containter(&x), iter(_iter) {}
		insert_iterator<Container>& operator = (const typename Container::value_type& _value) {
			iter = containter->insert(iter,_value);
			++iter;
			return *this;
		}

		insert_iterator<Container>& operator* () {return *this;}
		insert_iterator<Container>& operator++ () {return *this;}
		insert_iterator<Container> operator++ (int) {return *this;}
};

template<typename Container,typename Iterator>
inline insert_iterator<Container> insert(Container& _x,Iterator _i){
	typedef typename Container::iterator _iter;
	return insert_iterator<Container>(_x,_iter(_i));
}

//未完待续

