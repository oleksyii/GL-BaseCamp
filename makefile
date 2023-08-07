
output: main.o lib_findingFile.a
	g++ -o output main.o -L. lib_findingFile.a

main.o: main.cpp header.h
	g++ -c -std=c++17 main.cpp -o main.o

lib_findingFile.a: lib_functions.o
	ar rcs lib_findingFile.a lib_functions.o

lib_functions.o: library.cpp
	g++ -c -std=c++17 library.cpp -o lib_functions.o

clear:
	rm *.o