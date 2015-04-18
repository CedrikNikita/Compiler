#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

//#define debug

using namespace std;
int main(){
	int pass = 0;
	int failed = 0;
	string fail = "";
	for(int i = 1; i < 69; i++){
		int r = 1;
		FILE *test, *prog;
		string file_test = "output";
		string file_prog = file_test;
		file_test += "_test";
		file_test += to_string(i);
		file_test += ".txt";
		file_prog += to_string(i);
		file_prog += ".txt";
#ifdef debug
		printf("%s\n", file_test.c_str());
		printf("%s\n", file_prog.c_str());
#endif
		test = fopen(file_test.c_str(), "r");
		prog = fopen(file_prog.c_str(), "r");
#ifdef debug
		if (test == NULL) printf("%s\n", "Error opening file");
		if (prog == NULL) printf("%s\n", "Error opening file");
#endif
		char c_p = getc(prog);
		char c_t = getc(test);
		while((c_p != EOF) || (c_t != EOF)){
			r = c_p == c_t;
			c_p = getc(prog);
			c_t = getc(test);
		}
		if(r) pass++;
		else{
			failed++;
			fail += to_string(i);
			fail += ", ";
		}
		fclose(prog);
		fclose(test);
	}
	printf("PASS %d, FAILED %d\n", pass, failed);
	if(failed) printf("Good tests %s\n", fail.substr(0, fail.size() - 2).c_str());
	return 0;
}
