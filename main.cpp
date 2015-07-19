#include<iostream>
#include"include/vector.h"
#include "include/list.h"

int main(){
	int arr[4] = {1,2,3,4};
    Freeman::vector<int> v(arr,arr+4);
	for(int i=0;i<4;i++)
		std::cout<<v[i]<<std::endl;
}

