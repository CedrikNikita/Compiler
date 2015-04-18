#pragma once
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "iostream"

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
		   C_CURLYBRACER, C_SIZEOFS, C_SIZEOFI, C_SIZEOFZ, C_SIZEOFE, C_SIZEOFO,
		   C_SIZEOFF}; 

enum State{Eof, Lexem};
using namespace std;

class Scanner{
	public:
		char* filename;
		Scanner(char*);
		class Lexem{
			int x, y;
			int state;
			string type, text, strValue;
			int intValue;
			double doubValue;
			char charValue;
			public:
				Lexem(): x(0), y(0), type(), text(), intValue(0), doubValue(0), charValue(), strValue(){};

				Lexem(int _x, int _y, string _type, string _text, int _intValue, int _state): 
				x(_x), y(_y), type(_type), text(_text), intValue(_intValue), state(_state){};

				Lexem(int _x, int _y, string _type, string _text, string _strValue, int _state): 
				x(_x), y(_y), type(_type), text(_text), strValue(_strValue), state(_state){};

				Lexem(int _x, int _y, string _type, string _text, double _doubValue, int _state): 
				x(_x), y(_y), type(_type), text(_text), doubValue(_doubValue), state(_state){};

				Lexem(int _x, int _y, string _type, string _text, char _charValue, int _state):	
				x(_x), y(_y), type(_type), text(_text), charValue(_charValue), state(_state){};
				Lexem operator != (char);
				void print(){
					if(state == C_INT || state == C_INT_EXP || state == C_INT_16 || state == C_INT_8) cout << x << '\t' << y << '\t' << type + '\t' << text + '\t' << intValue << endl;
					else if(state == C_DOUBLE || state == C_DOUBLE_EXP || state == C_DOUBLE_CHECK_EXP) cout << x << '\t' << y << '\t' << type + '\t' << text + '\t' << doubValue << endl;
					else if(state == C_CHAREND) cout << x << '\t' << y << '\t' << type + '\t' << text + '\t' << charValue << endl;
					else cout << x << '\t' << y << '\t' << type + '\t' << text + '\t' << strValue << endl;
				}
				string getText(){return text;}
				string getType(){return type;}
				int getintValue(){return intValue;}
				string getstrValue(){return strValue;}
				double getdoubValue(){return doubValue;}
				char getcharValue(){return charValue;}
				int getX(){return x;}
				int getY(){return y;}
				bool operator == (Condition a){return a == state;}
				bool operator != (Condition a){return a != state;}
				bool operator != (State a){return a != state;}
		};
		Lexem next();
		Lexem get(){return Lex;}
		Lexem Lex;
		~Scanner(){fclose(stdin);}
};

class Error{
	Scanner :: Lexem Lex;
	string str;
	public:
		Error(Scanner :: Lexem, int, char*, bool);
};
