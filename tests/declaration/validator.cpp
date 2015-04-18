#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string>

//#define debug

using namespace std;
int main(){
	int pass = 0;
	int failed = 0;
	string fail = "";
	int i = 1;
	while(1){
				ostringstream convert;
		convert << i;
		int r = 1;
		FILE *test, *prog;
		string file_test = "test";
		string file_prog = "output";
		file_test += convert.str();
		file_test += ".et";
		file_prog += convert.str();
		file_prog += ".out";
#ifdef debug
		printf("%s\n", file_test.c_str());
		printf("%s\n", file_prog.c_str());
#endif
		test = fopen(file_test.c_str(), "r");
		if(test == NULL) break;
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
			fail += i;
			fail += ", ";
		}
		fclose(prog);
		fclose(test);
		i++;
	}
	printf("PASS %d, FAILED %d\n", pass, failed);
	if(failed) printf("Good tests %s\n", fail.substr(0, fail.size() - 2).c_str());
	return 0;
}
