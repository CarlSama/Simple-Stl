#ifndef _SS_LIST_
#define _SS_LIST_

template<class Tp>
struct List_Node {
	List_Node *next;
	List_Node *prev;
	Tp data;
};

template<class Tp,class Ref,class Ptr>
class List_Iterator {
	public:
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef bidirectional_iterator_tag iterator_category;

		typedef List_Iterator<Tp,Tp&,Tp*> iterator;
		typedef List_Iterator<Tp,const Tp&,const Tp*> const_iterator;
		typedef List_Iterator<Tp,Ref,Ptr> self;

		typedef Tp value_type;
		typedef Ref reference;
		typedef Ptr pointer;
		typedef List_Node<Tp> Node;

		//constructor
		List_Iterator(Node *nptr) {
			head = nptr;
		}
		List_Iterator(const iterator& it){
			head = it -> head;
		}

		//dereference
		reference operator* () const {
			return head->data;
		}

		//member access
		pointer operator -> () const {
			//为什么这么做？
			return &(operator*());
		}

		//increment
		List_Iterator& operator ++() {
			head = head->next;
			return *this;
		}
		List_Iterator operator ++(int){
			List_Iterator<Tp,Ref,pointer> tmp = *this;
			head = head->next;
			return tmp;
		}

		//decrement
		List_Iterator& operator--(){
			head = head->prev;
			return *this;
		}
		List_Iterator operator -- (int){
			List_Iterator<Tp,Ref,pointer> tmp = *this;
			head = head->prev;
			return tmp;
		}

		bool operator == (const List_Iterator<Tp,Ref,Pointer>& x) const {
		}

		bool operator == (const List_Iterator<Tp,Ref,Pointer>& x) const {
		}

	private:
		Node *head;//not a good name for single elem
};

//iterator_category
//value_type
//distance_type

	
template<typename Tp,typename Alloc>
class List_Alloc {
	public:
		typedef typename alloc_traits<Tp,Alloc>::allocator_type allocator_type;
		allocator_type get_allocator() const {
			return allocator;
		}
	protected:
		List_Node<Tp>* get_node() {
			return allocator.allocate(1);
		}
		void release_node(List_Node<Tp>* p) {
			allocator.deallocate(p,1);
		}

	protected:
		typename alloc_traits<List_Node<Tp>, Alloc>::allocator_type allocator;
		List_Node<Tp>* node;
};

template<typename Tp,typename Alloc>
class List_Base : public List_Alloc<Tp,Alloc>{
	public:
		typedef List_Alloc<Tp,Alloc>::allocator_type allocator_type;

		List_Base(const allocator_type&){
			head = get_node();
			head->next = head;
			head->prev = head;
		}

		~List_Base() {
			clear();
			release_node(head);
		}

	protected:
		List_Node<Tp> *head;
};

template<typename Tp,typename Alloc>
void List_Base<Tp,Alloc>::clear(){
	List_Node<Tp>* curr = head->next;
	while(curr != head){
		List_Node<Tp>* tmp = curr;
		curr = curr->next;
		destory(& tmp->data);
		release_node(_tmp);
	}
	head->next = head->prev = head;
}

template<typename Tp,typename Alloc =  Allocator>
class list : protected List_Base<Tp,Alloc> {
	public:
		typedef Tp value_type;
		typedef Tp* pointer;
		typedef const Tp* const_pointer;
		typedef Tp& reference;
		typedef const Tp& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef List_Node<Tp> Node;
		typedef List_Base<Tp,Alloc>::allocator_type allocator_type;
		allocator_type get_allocator() const { return List_Base<Tp,Alloc>::get_allocator(); }

	public:
		typedef List_Iterator<Tp,Tp&,Tp*> iterator;
		typedef List_Iterator<Tp,const Tp&,const Tp*> const_iterator;
		typedef reverse_iterator<iterator> reverse_iterator;
		typedef reverse_iterator<const_iterator> const_reverse_iterator;

	protected:
		Node *create_node(const Tp& x){
			Node * tmp = get_node();
			try {
				construct(&tmp->data,x);
			}catch (...) {
				cerr<<"wrong"<<endl;
			}
			return tmp;
		}

	public:
		explicit List (const allocator_type& a = allocator_type() ) : List_Base<Tp,Alloc>(a) {}
		iterator begin() { return head->next;}
		const_iterator begin() { return head->next; }

		iterator end() { return head; }
		const_iterator end() { return head; }

		reverse_iterator rbegin(){ return reverse_iterator(end()); }
		const_reverse_iterator rbegin()const{ return const_reverse_iterator(end()); }

		reverse_iterator rend(){return reverse_iterator(begin()); }
		const_reverse_iterator rend() const {return reverse_iterator(begin()); }

		bool empty()const{return head->next = head;}
		size_type size()const{
			size_type _res = 0;
			distance(begin(),end(),_res);
			return _res;
		}
		size_type max_size()const{ return size_type(-1); }

		reference front(){return *begin();}
		const_reference front(){return *begin();}
		reference back(){return *(--end());}
		const_reference back(){return *(--end());}

		void swap(list<Tp,Alloc>& x){
			List_Base<Tp,Alloc> * tmp = head;
			head = x.head;
			x.head = tmp;
		}

		iterator insert(iterator pos, const Tp& x){
			Node *tmp = create_node(x);
			tmp->next = pos.head;
			tmp->prev = pos.head->prev;
			pos.head->prev->next = tmp;
			pos->head->prev = tmp;
			return tmp;
		}

		void push_front(const Tp& x) {insert(begin(),x);}
		void push_back(const Tp& x) {insert(end(),x);}











#endif
