#include "Scanner.hpp"
#include "table.hpp"

//#define debug
using namespace std;

int x = 1;
int temp_y = 0;
char prev_symbol = 0;

Scanner::Scanner(char* a) : filename(a), file(a){
	if (!file.is_open())
		throw Error();
}

Scanner::Lexem Scanner::peek(){
	if (prev_symbol == -1) return Lexem(x, temp_y + 1, "", "", 0);
	char c;
	int prev_state, state = C_BEGIN;
	int lex_size = 0;
	int value_size = 0;
	string text, value;
	while (state != C_END){
		if (prev_symbol == 0){
			c = file.get();
			temp_y++;
		}
		else{
			c = prev_symbol;
			prev_symbol = 0;
		}
		if ((c == '\n') && (state == C_BEGIN)){
			x++;
			temp_y = 0;
		}

		prev_state = state;
		state = table[state][c < -1 ? 128 : c];
#ifdef debug
		cout << "prev_state " << prev_state << " state " << state << " c -- " << c << endl;
#endif
		if ((prev_state == C_INT || state == C_INT) && (lex_size == 9))
			throw Error(Lexem(x, temp_y - lex_size + 1, text, text, prev_state), 1);
		if (state == C_ERROR){
			if (c == -1){ lex_size++; break; }
			throw Error(Lexem(x, temp_y - lex_size, text + c, text + c, prev_state), 0);
		}
		if (prev_state == C_STRINGSPECIAL || prev_state == C_CHARSPECIAL){
			text += c;
			value_size++;
			if (special.find(c) != special.end())
				value += special.find(c)->second;
			lex_size++;
			continue;
		}
		text += c;
		lex_size++;
		if (prev_state == C_STRINGSPECIAL || state == C_STRINGSPECIAL || prev_state == C_CHARSPECIAL || state == C_CHARSPECIAL) continue;
		value += c;
		value_size++;
		if (state == C_BEGIN){
			if (prev_state == C_COMMENT || prev_state == C_MLCOMMENT) prev_symbol = c;
			value = "";
			text = "";
			lex_size = 0;
			value_size = 0;
		}
	}
	if (lex_size - 1 == 0) return Lexem(x, temp_y + 1, "", "", 0);
	prev_symbol = c;
	text = text.substr(0, lex_size - 1);
	value = value.substr(0, lex_size - 1);
	int base = 0;
	switch (prev_state){
	case C_INT: base = 10; break;
	case C_INT_16: base = 16; break;
	case C_INT_8: base = 8; break;
	}
	Lexem a;
	if (prev_state == C_INT_EXP)
		return Lexem(x, temp_y - lex_size + 1, text, static_cast<int>(stod(text)), C_INT);
	if (base){
		char *endp;
		return Lexem(x, temp_y - lex_size + 1, text, (int)strtol(text.c_str(), &endp, base), C_INT);
	}
	if (prev_state == C_DOUBLE || prev_state == C_DOUBLE_EXP || prev_state == C_DOUBLE_CHECK_EXP)
		return Lexem(x, temp_y - lex_size + 1, text, atof(text.c_str()), C_DOUBLE);
	if (prev_state == C_CHAREND)
		return Lexem(x, temp_y - lex_size + 1, text, (char)value.substr(1, lex_size - 3)[0], prev_state);
	if (prev_state == C_STRINGEND)
		return Lexem(x, temp_y - lex_size + 1, text, value.substr(1, value_size - 3), prev_state);
	if (text == "sizeof") prev_state = C_SIZEOF;
	return Lexem(x, temp_y - lex_size + 1, text, text, prev_state);
}

void Scanner :: Lexem :: print(){
	if(state == C_INT) cout << x << '\t' << y << '\t' << "int"  <<  '\t' << text + '\t' << intValue << endl;
	else if(state == C_DOUBLE) cout << x << '\t' << y << '\t' << "double"  <<  '\t' << text + '\t' << doubValue << endl;
	else if(state == C_CHAREND) cout << x << '\t' << y << '\t' << "char"  <<  '\t' << text + '\t' << charValue << endl;
	else if(state == C_STRINGEND) cout << x << '\t' << y << '\t' << "char*"  <<  '\t' << text + '\t' << strValue << endl;
	else if((state >= C_PLUS) && (state <= C_NOTBIN) || state == C_SIZEOF) cout << x << '\t' << y << '\t' << "operation"  <<  '\t' << text + '\t' << strValue << endl;
	else if(state >= C_FIELD) cout << x << '\t' << y << '\t' << "separator"  <<  '\t' << text + '\t' << strValue << endl;
	else if(keyword.find(text) != keyword.end()) cout << x << '\t' << y << '\t' << "keyword"  <<  '\t' << text + '\t' << strValue << endl;
	else cout << x << '\t' << y << '\t' << "identifier" << '\t' << text + '\t' << strValue << endl;
}
Scanner :: Lexem Scanner :: next(){
	Lex = peek();
	return Lex;
}

Error :: Error(Scanner :: Lexem _Lex, bool a): Lex(_Lex), end(1){
	auto state = Lex.getEnum();
	if(state == C_DOUBLE || state == C_DOUBLE_EXP || state == C_DOUBLE_CHECK_EXP
	|| state == C_INT || state == C_INT_8 || state == C_INT_16){
		if(a){
			str = "integer constant is too large for its type";
			end = 0;
		}
		else str = "unable to find numeric literal operator ‘operator\"\"";
	}
	if(state == C_CHAREND || state == C_CHARCHECK)
		str = "missing terminating ' character";
	if(state == C_STRING)
		str = "missing terminating \" character";		
	if(state == C_STRINGSPECIAL || state == C_CHARSPECIAL){
		str = "unknown escape sequence ";
		end = 2;
	}
}