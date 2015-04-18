#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string>

//#define debug

using namespace std;
int main(){
	int i = 1;
	freopen("alltest.txt", "w", stdout);
	while(1){
		int r = 1;
		FILE *test, *prog;
		string file_test = "test";
		string file_prog = "output";
		ostringstream convert;
		convert << i;
		file_test += convert.str();
		file_test += ".c";
		file_prog += convert.str();
		file_prog += ".out";
#ifdef debug
		printf("%s\n", file_test.c_str());
		printf("%s\n", file_prog.c_str());
#endif
		test = fopen(file_test.c_str(), "r");
		if(test == NULL) break;
		prog = fopen(file_prog.c_str(), "r");
		if(prog == NULL) break;

		char c_p = getc(prog);
		char c_t = getc(test);
		printf("////////////////////////////////\n");
		printf(" ////////////TEST%d///////////\n\n", i);
		while(c_t != EOF){
			printf("%c", c_t);
			c_t = getc(test);
		}
		printf("\n\n");
		while(c_p != EOF){
			printf("%c", c_p);
			c_p = getc(prog);
		}
		fclose(prog);
		fclose(test);
		i++;
	}
	return 0;
}
