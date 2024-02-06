all: hello testtool.so

hello: hello.c testtool.so
	clang -Wall -Wextra -g -O0 hello.c -fopenmp -o hello

testtool.so: testtool.c
	clang -shared -o libtesttool.so -fPIC testtool.c

run: hello testtool.so
	OMP_TOOL_LIBRARIES=./libtesttool.so ./hello

clean:
	rm -f hello libtesttool.so
