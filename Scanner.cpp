#include "Scanner.hpp"
#include "table.hpp"

using namespace std;

//#define debug

int x = 1;
int temp_y = 0;
char prev_symbol = 0;

Scanner :: Scanner(char* a){
	filename = a;
	freopen(a, "r", stdin);
}

Scanner :: Lexem Scanner :: next(){
	char c;
	int prev_state, state = C_BEGIN;
	int lex_size = 0;
	int value_size = 0;
	string text, value;
	while(state != C_END){
		if(prev_symbol == 0){
			c = getchar();
			temp_y++;
		}
		else{
			c = prev_symbol;
			prev_symbol = 0;

		}
		if(c == -1){
			Lexem a(x, temp_y - lex_size + 1, "", "", 0);
			Lex = a;
			return Lex;
		}
		if((c == '\n') && (state == C_BEGIN)){
			x++;
			temp_y = 0;
		}

		prev_state = state;
		state = table[state][c < -1 ? 128 : c];
#ifdef debug
		cout << "prev_state " << prev_state << " state " << state << " c -- " << c << endl;
#endif
		if((prev_state == C_INT || state == C_INT) && (lex_size == 9))   
			throw Error(Lexem(x, temp_y - lex_size + 1, text, text, prev_state), 
						prev_state, filename, 1);
		if(state == C_ERROR)
			throw Error(Lexem(x, temp_y - lex_size, text + c, text + c, prev_state),
						prev_state, filename, 0);
		if(prev_state == C_STRINGSPECIAL || prev_state == C_CHARSPECIAL){
			text += c;
			value_size++;
			if(special.find(c) != special.end())
				value += special.find(c)->second;
			lex_size++;
			continue;
		}
		text += c;
		lex_size++;
		if(prev_state == C_STRINGSPECIAL || state == C_STRINGSPECIAL || prev_state == C_CHARSPECIAL || state == C_CHARSPECIAL) continue;
		value += c;
		value_size++;
		if(state == C_BEGIN){
			if(prev_state == C_COMMENT || prev_state == C_MLCOMMENT) prev_symbol = c;
			value = "";
			text = "";
			lex_size = 0;
			value_size = 0;
		}
	}
	prev_symbol = c;
	text = text.substr(0, lex_size - 1);
	value = value.substr(0, lex_size - 1);
	int base = 0;
	switch(prev_state){
		case C_INT: base = 10; break;
		case C_INT_16: base = 16; break;
		case C_INT_8: base = 8; break;
	}
	Lexem a;
	if(prev_state == C_INT_EXP){
		int value =  static_cast<int>(stod(text));
		Lexem a(x, temp_y - lex_size + 1, text, value, prev_state);
		Lex = a;
	}
	if(base){
		char *endp;
		int value = strtol(text.c_str(), &endp, base);
		Lexem a(x, temp_y - lex_size + 1, text, value, prev_state);
		Lex = a;
	}
	else if(prev_state == C_DOUBLE || prev_state == C_DOUBLE_EXP || prev_state == C_DOUBLE_CHECK_EXP){
		double value = atof(text.c_str());
		Lexem a(x, temp_y - lex_size + 1, text, value, prev_state);
		Lex = a;
	}
	else if(prev_state == C_CHAREND){
		char cvalue = value.substr(1, lex_size - 3)[0];
		Lexem a(x, temp_y - lex_size + 1, text, cvalue, prev_state);
		Lex = a;
	}
	else if(prev_state == C_STRINGEND){
		value = value.substr(1, value_size - 3);
		Lexem a(x, temp_y - lex_size + 1, text, value, prev_state);
		Lex = a;
	}
	else{
		if(text == "sizeof") prev_state = C_SIZEOF;
		Lexem a(x, temp_y - lex_size + 1, text, text, prev_state);
		Lex = a;
	}
	lex_size = 0;
	return Lex;
}	

void Scanner :: Lexem :: print(){
	if(state == C_INT || state == C_INT_EXP || state == C_INT_16 || state == C_INT_8) cout << x << '\t' << y << '\t' << "int"  <<  '\t' << text + '\t' << intValue << endl;
	else if(state == C_DOUBLE || state == C_DOUBLE_EXP || state == C_DOUBLE_CHECK_EXP) cout << x << '\t' << y << '\t' << "double"  <<  '\t' << text + '\t' << doubValue << endl;
	else if(state == C_CHAREND) cout << x << '\t' << y << '\t' << "char"  <<  '\t' << text + '\t' << charValue << endl;
	else if(state == C_STRINGEND) cout << x << '\t' << y << '\t' << "char*"  <<  '\t' << text + '\t' << strValue << endl;
	else{
		if((state >= C_PLUS) && (state <= C_NOTBIN) || state == C_SIZEOF) cout << x << '\t' << y << '\t' << "operation"  <<  '\t' << text + '\t' << strValue << endl;
		else if(state >= C_FIELD) cout << x << '\t' << y << '\t' << "separator"  <<  '\t' << text + '\t' << strValue << endl;
		else if(keyword.find(text) != keyword.end()) cout << x << '\t' << y << '\t' << "keyword"  <<  '\t' << text + '\t' << strValue << endl;
		else cout << x << '\t' << y << '\t' << "identifier" << '\t' << text + '\t' << strValue << endl;
	}
}

Error :: Error(Scanner :: Lexem _Lex, int prev_state, char* _filename, bool a): Lex(_Lex), filename(_filename), end(1){
	if(prev_state == C_DOUBLE || prev_state == C_DOUBLE_EXP || prev_state == C_DOUBLE_CHECK_EXP
	|| prev_state == C_INT || prev_state == C_INT_8 || prev_state == C_INT_16){
		if(a){
			str = "integer constant is too large for its type";
			end = 0;
		}
		else str = "unable to find numeric literal operator ‘operator\"\"";
	}
	else if(prev_state == C_CHAREND || prev_state == C_CHARCHECK)
		str = "missing terminating ' character";
	else if(prev_state == C_STRING)
		str = "missing terminating \" character";		
	else if(prev_state == C_STRINGSPECIAL || prev_state == C_CHARSPECIAL){
		str = "unknown escape sequence ";
		end = 2;
	}
}
