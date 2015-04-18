all:
	for number in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15; do \
	    ccc -s test$$number.c -out output$$number.out; \
	done
	g++ -std=c++11 validator.cpp
	./a.out
	g++ -std=c++11 onefile.cpp
	./a.out
