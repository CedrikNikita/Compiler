#pragma once
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

enum Condition{C_END, C_ERROR, C_BEGIN, C_INT, C_INT_EXP, C_INT_8, C_INT_16, 
		   C_DOUBLE, C_DOUBLE_CHECK_EXP, C_DOUBLE_EXP, C_IDENTIFIER, 
		   C_PLUS, C_INCREMENT, C_PLUSEQUAL, C_MINUS, C_DECREMENT,
		   C_MINUSEQUAL, C_FIELDPTR, C_ASTERISK, C_ASTERISKEQUAL,
		   C_DIV, C_DIVEQUAL, C_ANDBIN, C_AND, C_ANDBINEQUAL, C_ORBIN, C_OR,
		   C_ORBINEQUAL, C_GREATER, C_SHIFTR, C_GREATEREQUAL, C_SHIFTREQUAL,
		   C_LESS, C_SHIFTL, C_LESSEQUAL, C_SHIFTLEQUAL, C_ASSIGN, C_EQUAL,
		   C_NOT, C_NOTEQUAL, C_MOD, C_MODEQUAL, C_XOR, C_XOREQUAL, C_NOTBIN,
		   C_COMMENT, C_MLCOMMENT, C_MLCOMMENT_CHECK, C_MLCOMMENTEND, C_STRING,
		   C_STRINGSPECIAL, C_STRINGEND, C_CHAR, C_CHARSPECIAL, C_CHARCHECK,
		   C_CHAREND, C_FIELD, C_COMMA, C_SEMICOLON, C_TERNARYQ, C_TERNARYC,
		   C_BRACEL, C_BRACER, C_SQUAREBRACEL, C_SQUAREBRACER, C_CURLYBRACEL,
		   C_CURLYBRACER, C_SIZEOF};

static string punctuator[] = {"+", "++", "+=", "-", "--", "-=", "->", "*", "*=", "/", "/=", "&", 
			  "&&", "&=", "|", "||", "|=", ">", ">>", ">=", ">>=", "<", "<<", "<=", 
			  "<<=", "=", "==", "!", "!=", "%", "%=", "^", "^=", "~", ".", ",",
			  ";", "?", ":", "(", ")", "[", "]", "{", "}", "sizeof"};

static string declaration[] = {"const", "int", "char", "double", "void", "typedef", "struct"};

enum Declaration{D_const, D_int, D_char, D_double, D_void, D_typedef, D_struct};					//TODO

static string blocks[] = {"if", "while", "for", "break", "continue", "return", "else", "do", "printf"};		//TODO

enum Blocks{B_if, B_while, B_for, B_break, B_continue, B_return, B_else, B_do, B_printf};

enum State{Eof, Lexem};

class Scanner{
	public:
		char* filename;
		ifstream file;
		Scanner(char*);
		class Lexem{
			int x, y;
			int state;
			string text, strValue;
			int intValue;
			double doubValue;
			char charValue;
			public:
				Lexem(): x(0), y(0), text(), intValue(0), doubValue(0), charValue(), strValue(){};

				Lexem(int _x, int _y, string _text, int _intValue, int _state): 
				x(_x), y(_y), text(_text), intValue(_intValue), state(_state){};

				Lexem(int _x, int _y, string _text, string _strValue, int _state): 
				x(_x), y(_y), text(_text), strValue(_strValue), state(_state){};

				Lexem(int _x, int _y, string _text, double _doubValue, int _state): 
				x(_x), y(_y), text(_text), doubValue(_doubValue), state(_state){};

				Lexem(int _x, int _y, string _text, char _charValue, int _state):	
				x(_x), y(_y), text(_text), charValue(_charValue), state(_state){};
				void print();
				string getText(){return text;}
				int getintValue(){return intValue;}
				string getstrValue(){return strValue;}
				double getdoubValue(){return doubValue;}
				char getcharValue(){return charValue;}
				Condition getEnum(){return static_cast<Condition>(state);}

				int getX(){return x;}
				int getY(){return y;}

				bool operator == (Condition a){return a == state;}
				bool operator != (Condition a){return a != state;}
				bool operator != (State a){return a != state;}
				bool operator == (Blocks a){return blocks[a] == text;}
				bool operator != (Blocks a){return blocks[a] != text;}
				bool operator == (Declaration a){return declaration[a] == text;}
				bool operator != (Declaration a){return declaration[a] != text;}
		};
		Lexem next();
		Lexem peek();
		Lexem get(){return Lex;}
		Lexem Lex;
		~Scanner(){file.close();}
};

class Error{
	public:
		Scanner :: Lexem Lex;
		string str;
		int end;
		Error(Scanner :: Lexem, bool);
		Error(){};
};
