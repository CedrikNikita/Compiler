all:
	for number in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 ; do \
	    ccc -s test$$number.c -out output$$number.out; \
	done
	g++ -std=c++11 validator.cpp
	./a.out
	g++ -std=c++11 onefile.cpp
	./a.out
