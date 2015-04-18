#include "Scanner.hpp"
#include "Parser.hpp"
#include "CodeGen.hpp"
#include <stdlib.h>
#include <stdio.h>

static string Set_cap(int pos){
	string str;
	for(int i = 0; i < pos; i++)
		str += ' ';
	str += '^';
	return str;
}

void mes_error_input(){
	printf("%s\n", "ccc:\tfatal error: no input files");
}

void mes_option(char* op){
	printf("%s%s'\n", "ccc: error: unrecognize command line option '", op);
}

void mes_help(){
	printf("%s\n", "C Compiler\nUsage: ccc [options] file ...\n\
		   Options:\n\
		   \t-h\tDisplay this information\n\
		   \t-l\tScan only\n\
		   \t-s\tParse syntax tree\n\
		   \t-S\tCompile only; do not assemble or link\n\
		   \t-c\tCompile, assemble and link\n");
}

void mes_error_lex(Error& e, char* filename){
	int last = e.Lex.getText().size();
	cout << filename << ":" << e.Lex.getX() << ":" << e.Lex.getY() + last << ' ' + e.str << (e.Lex.getText()).substr(last - e.end, e.end) << endl 
	<< '\t' + e.Lex.getText() << endl
	<< '\t' + Set_cap(last - e.end) << endl;	
}

void mes_error_nosuchfile(Error& e, char* filename){
	cout << "ccc: error: " << filename << ": No such file in directory\n" 
	<<"ccc: fatal error: no input files\ncompilation terminated." << endl;
}

void mes_error_synt(SyntaxError &e, char* filename, Scanner :: Lexem Lex){
	auto tok = Lex;
	cout << filename << ":" << tok.getX() << ":" << tok.getY() + tok.getText().size() << ' ';
	e.a?(cout << e.str + " ‘" + tok.getText() + "’"):(cout << e.str);
	cout << endl;
}

void scan(char* filename){
	try{
		Scanner Sc(filename);
		try{
			Sc.next();
			while(Sc.get() != Eof){
				Sc.get().print();
				Sc.next();
			}
		}
		catch(Error& e){
			mes_error_lex(e, filename);
		}
	}
	catch(Error& e){
		mes_error_nosuchfile(e, filename);
	}

}

void parse(char* filename){
	try{	
		Scanner Sc(filename);
		try{ 
			Sc.next();
		}
		catch(Error& e){
			mes_error_lex(e, filename);
		}
		if(Sc.get() != Eof){
			try{
				Parser Pa(&Sc);
				Pa.print();
			}
			catch(SyntaxError& e){
				mes_error_synt(e, filename, Sc.get());
			}
		}
	}
	catch(Error& e){
		mes_error_nosuchfile(e, filename);
	}
}

void genAssemble(char* filename, char* outfile){
	try{	
		Scanner Sc(filename);
		try{ 
			Sc.next();
		}
		catch(Error& e){
			mes_error_lex(e, filename);
		}
		if(Sc.get() != Eof){
			try{
				Parser Pa(&Sc);
				CodeGen CG(outfile);
				Pa.generate(CG);
				CG.print();
			}
			catch(SyntaxError& e){
				mes_error_synt(e, filename, Sc.get());
			}
		}
	}
	catch(Error& e){
		mes_error_nosuchfile(e, filename);
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
		if(strcmp(argv[1], "-l") != 0 && strcmp(argv[1], "-s") != 0 && strcmp(argv[1], "-h") != 0) mes_option(argv[1]);
		else if(strcmp(argv[1], "-h") == 0) mes_help();
		else if(strcmp(argv[1], "-l") == 0) scan(argv[2]);
		else if(strcmp(argv[1], "-s") == 0) parse(argv[2]);
		break;
	case 5:
		if(strcmp(argv[1], "-l") != 0 && strcmp(argv[1], "-s") != 0 && strcmp(argv[1], "-S") != 0 ) mes_option(argv[1]);
		else if(strcmp(argv[3], "-out") != 0) mes_option(argv[3]);
		else if(strcmp(argv[3], "-out") == 0){
			if(strcmp(argv[1], "-S") == 0) genAssemble(argv[2], argv[4]);
			else{
				freopen(argv[4], "w", stdout);
				if(strcmp(argv[1], "-l") == 0) scan(argv[2]);
				else if(strcmp(argv[1], "-s") == 0) parse(argv[2]);
				fclose(stdout);
			}
		}
		break;
	}
	return 0;
}
