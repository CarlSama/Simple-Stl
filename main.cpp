#include<iostream>
//#include"sim-allocator.h"
#include<vector>
#include<algorithm>
#include<new>

using namespace std;
union test{
	union test* ptr;
	char data[1];
};

int main(){
	test t;
	cout<<sizeof(t)<<endl;
	cout<<sizeof(t.data[0])<<endl;
}

