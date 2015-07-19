#ifndef _freeman_function_h_
#define _freeman_function_h_

namespace Freeman {
	/*
	 * res function(arg);
	 * res function(arg1, arg2);
	 */
	template<typename Arg,typename Res>
		struct unary_function {
			typedef Arg argument_type;
			typedef Res result_type;
		};

	template<typename Arg1, typename Arg2,typename Res>
		struct binary_function {
			typedef Arg1 fisrt_argument_type;
			typedef Arg2 second_argument_type;
			typedef Res result_type;
		};


	/*
	 * + - * / > < <= >= && || !
	 */
	template<typename Tp>
		struct plus : public binary_function<Tp,Tp,Tp> {
			Tp operator () (const Tp& x,const Tp& y) const{ return x + y; }
		}
	template<typename Tp>
		struct minus: public binary_function<Tp,Tp,Tp> {
			Tp operator () (const Tp& x,const Tp& y) const{ return x - y; }
		}
	template<typename Tp>
		struct multiply:public binary_function<Tp,Tp,Tp> {
			Tp operator () (const Tp& x,const Tp& y) const{ return x * y; }
		}
	template<typename Tp>
		struct divide: public binary_function<Tp,Tp,Tp> {
			Tp operator () (const Tp& x,const Tp& y) const{ return x / y; }
		}
	template<typename Tp>
		struct modulus: public binary_function<Tp,Tp,Tp> {
			Tp operator () (const Tp& x,const Tp& y) const{ return x % y; }
		}
	template<typename Tp>
		struct negate: public unary_function<Tp,Tp> {
			Tp operator () (const Tp& x) const{ return -x; }
		}
	template<typename Tp>
		struct equal_to: public binary_function<Tp,Tp,Tp> {
			Tp operator () (const Tp& x,const Tp& y) const{ return x == y; }
		}
	template<typename Tp>
		struct no_equal_to: public binary_function<Tp,Tp,Tp> {
			Tp operator () (const Tp& x,const Tp& y) const{ return  x != y; }
		}
	template<typename Tp>
		struct greater: public binary_function<Tp,Tp,Tp> {
			Tp operator () (const Tp& x,const Tp& y) const{ return  x > y; }
		}
	template<typename Tp>
		struct less : public binary_function<Tp,Tp,Tp> {
			Tp operator () (const Tp& x,const Tp& y) const{ return  x < y; }
		}
	template<typename Tp>
		struct greater_equal: public binary_function<Tp,Tp,Tp> {
			Tp operator () (const Tp& x,const Tp& y) const{ return  x >= y; }
		}
	template<typename Tp>
		struct less_equal: public binary_function<Tp,Tp,Tp> {
			Tp operator () (const Tp& x,const Tp& y) const{ return  x <= y; }
		}
	template<typename Tp>
		struct logical_and: public binary_function<Tp,Tp,Tp> {
			Tp operator () (const Tp& x,const Tp& y) const{ return  x && y; }
		}
	template<typename Tp>
		struct logical_or: public binary_function<Tp,Tp,Tp> {
			Tp operator () (const Tp& x,const Tp& y) const{ return  x || y; }
		}
	template<typename Tp>
		struct logical_not: public unary_function<Tp,Tp> {
			Tp operator () (const Tp& x,const Tp& y) const{ return  !x; }
		}




	/*
	 * unary_negate
	 * binary_negate
	 */
	template<typename Predicate>
		class unary_negate:public unary_function<typename Predicate::argument_type,bool> {
			protected:
				Predicate pred;
			public:
				explicit unary_negate(const Predicate& x) : pred(x) {}
				bool operator()(const typename Predicate::argument_type& x) const {
					return !pred(x);
				}
		}
	template<typename Predicate>
		inline unary_negate<Predicate> not1(const Predicate& pred) {
			return unary_negate<Predicate>(pred);
		}
	template<typename Predicate>
		class binary_negate :
			public binary_function<typename Predicate::first_argument_type,typename Predicate::second_argument_type,bool> {
				protected:
					Predicate pred;
				public:
					explicit binary_negate(const Predicate& x) : pred(x) {}
					bool operator() (const typename Predicate::first_argument_type& x,const typename Predicate::second_argument_type& y) {
						return !pred(x,y);
					}
			}
	template<typename Predicate>
		inline binary_negate<Predicate> not2(const Predicate& pred) {
			return binary_negate<Predicate>(pred);
		}


	/*
	 * binder1st
	 * binder2nd
	 *
	 * bind1st(greater<int>(), 5); //即5作为第一个固定参数。返回5大于的数字的个数
	 * bind2nd(greater<int>(), 5); //即5作为第二个固定参。返回大于5的数字的个数
	 */
	template<typename Operation>
		class binder1st : public unary_function<typename Operation::second_argument_type,typename Operation::result_type> {
			protected:
				Operation op;
				typename Operation::first_argument_type value;
			public:
				binder1st(const Operation& x,const typename Operation::first_argument_type& y) : op(x),value(y) {}
				typename Operation::result_type operator() (const typename Operation::second_argument_type& x) const {
					return op(value,x);
				}
		};
	template<typename Operation,typename Tp>
		inline binder1st<Operation> binder1st(const Operation& fn,const Tp& x) {
			return binder1st<Operation>(fn,(typename Operation::first_argument_type)x);
		}

	template<typename Operation>
		class binder2nd :
			public unary_function<typename Operation::first_argument_type,typename Operation::result_type> {
				protected:
					Operation op;
					typename Operation::second_argument_type value;
				public:
					binder2nd(const Operation& x,const typename Operation::second_argument_type& y) : op(x),value(y) {}
					typename Operation::result_type operator() (const typename Operation::first_argument_type& x) const {
						return op(x,value);
					}
			};
	template<typename Operation,typename Tp>
		inline binder2nd<Operation> bind2nd(const Operation& fn,const Tp& x) {
			return bind2nd<Operation>(fn,typename Operation::second_argument_type(x));
		}


#endif
