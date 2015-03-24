CXXFLAGS=-g -std=c++11 -pipe -Wall
SHARED=-shared
FPIC=-fPIC 

OBJ=main.o

LIB_OBJECT=main.so

BIN_OBJECT=stl

all : ${BIN_OBJECT}

${BIN_OBJECT} : main.cpp
	g++ ${CXXFLAGS} -o $@ $^

#${BIN_OBJECT} : ${LIB_OBJECT}
#	g++ ${CXXFLAGS} $^

#${LIB_OBJECT} : ${OBJ}
#	g++ ${SHARED} ${FPIC} ${OBJ} -o ${LIB_OBJECT}

#${OBJ} : main.cpp
#	g++ ${CXXFLAGS} $< -c -o $@

clean:
	rm -rf ${OBJ} ${LIB_OBJECT} ${BIN_OBJECT}
