#include "Scanner.hpp"
#include "Parser.hpp"
#include <stdlib.h>
#include <stdio.h>

void mes_error_input(){
	printf("%s\n", "ccc:\tfatal error: no input files");
}

void mes_option(char* op){
	printf("%s%s'\n", "ccc: error: unrecognize command line option '", op);
}

void mes_help(){
	printf("%s\n", "C Compiler\nUsage: ccc [options] file ...\nOptions:\n\t-h\tDisplay this information\n\t-l\tScan only\n");
}

void Scan(char* filename){
	Scanner Sc(filename);
	try{
		while(Sc.next() != Eof)
			Sc.get().print();
	}
	catch(Error& e){}
}

void Parse(char* filename){
	Scanner Sc(filename);
	if(Sc.next() != Eof){
		try{
			Parser Pa(&Sc);
			Pa.print();
		}
		catch(SyntaxError& e){}
	}
}

int main(int argc, char *argv[]){
	switch(argc){
	case 1:
		mes_help();
		break;
	case 2:
		if(strcmp(argv[1], "-h") == 0) mes_help();
		else mes_error_input();
		break;
	case 3:
		if((strcmp(argv[1], "-l") != 0) && (strcmp(argv[1], "-s") != 0) && (strcmp(argv[1], "-h") != 0)) mes_option(argv[1]);
		else if(strcmp(argv[1], "-h") == 0) mes_help();
		else if(strcmp(argv[1], "-l") == 0) Scan(argv[2]);
		else if(strcmp(argv[1], "-s") == 0)Parse(argv[2]);
		break;
	case 5:
		if((strcmp(argv[1], "-l") != 0) && (strcmp(argv[1], "-s") != 0)) mes_option(argv[1]);
		else if(strcmp(argv[3], "-out") != 0) mes_option(argv[3]);
		else if(strcmp(argv[3], "-out") == 0){
			freopen(argv[4], "w", stdout);
			if(strcmp(argv[1], "-l") == 0)
				Scan(argv[2]);
			else if(strcmp(argv[1], "-s") == 0)
				Parse(argv[2]);
			fclose(stdout);
		}
		break;
	}
	return 0;
}
