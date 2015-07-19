a.out:main.cpp
	g++ -g -pipe -fpermissive -std=c++11 $^
clean :
	rm -rf a.out
