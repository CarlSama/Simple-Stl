#ifndef _freeman_pair_h_
#define _freeman_pair_h_

namespace Freeman {

	template<typename T1,typename T2>
		struct pair {
			typename T1 first_type;
			typename T2 second_type;

			T1 first;
			T2 second;

			pair() : first(), second() {}
			pair(const T1& a,const T2& b) : first(a), second(b) {}
		};

	template<typename T1,typename T2>
		inline bool operator == (const pair<T1,T2>& x,const pair<T1,T2>& y) {
			return x.first == y.first && x.second == y.second;
		}

	template<typename T1,typename T2>
		inline bool operator < (const pair<T1,T2>& x,const pair<T1,T2>& y) {
			return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
		}

	template<typename T1,typename T2>
		inline pair<T1,T2> make_pair(const T1& x,const T2& y) {
			return pair<T1,T2>(x,y);
		}
}

#endif

