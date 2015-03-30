#include<iostream>

using namespace std;

/*
 * static_template_member
 * With or without partial specialization, static member need to be assigned with Tp
 */
template <typename Tp>
class test {
	public:
		static int si;
};

template<>
class test<char> {
	public:
		static int si;
};

template<typename Tp>
int test<Tp>::si = -1;

int test<char>::si = 1;

/*
 * class partial specialization
 *		less abstract type in template argument, 
 *		more concrete represent in class name 
 */
template<typename Tp1,typename Tp2>
class ps {
	public:
		Tp1 tp1;
		Tp2 tp2;
};

template<typename Tp1>
class ps<Tp1, int>{
	public:
		Tp1 tp1;
};

template<typename Tp1>
class ps<Tp1, Tp1*>{
	public:
		Tp1 tp1;
};

template<>
class ps<int*, char*>{
	public:
};

/*
 * partial specialization of function templates
 */

/*
 * function template partial order 偏序
 */

/*
 * member template
 */
template<typename Tp>
class mt {
	public:
		template<typename Tpp>
			void show(){
				typedef mt<Tpp> other;
			}
};

/*
 *	bound friend template friend
 */
template<typename Tp>
class stack {
	public:
		friend bool operator == (const stack<Tp>& , const stack<Tp>&){
			cout<<"== 1"<<endl;
		}
		/*
		 * error: defining explicit specialization ‘operator==<>’ in friend declaration
		 */
		//friend bool operator == <> (const stack& , const stack&){
		//	cout<<"== 2"<<endl;
		//}
};

int main(){
	cout<<(test<int>::si)<<endl;
	cout<<(test<char>::si)<<endl;

	test<int> ti;
	ti.si = 100;
	cout<<(test<int>::si)<<endl;
	cout<<(test<char>::si)<<endl;

	stack<int> st1,st2;
	st1==st2;

}
