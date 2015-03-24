#include<iostream>
//#include"sim-allocator.h"
#include<vector>
#include<algorithm>
#include<new>

int main(){
	try{
		for(int i=0;i<1024*1024*100;i++){
			int* nnew = new int [1024*1024*1024*1024];
			nnew[0] = 1;
		}
	}catch(std::bad_alloc &ba){
		std::cerr<<ba.what()<<std::endl;
	}
}

