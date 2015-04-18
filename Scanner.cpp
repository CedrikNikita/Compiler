#include "Scanner.hpp"
#include "table.hpp"

using namespace std;

//#define debug

static string Set_cap(int pos){
	string str;
	for(int i = 0; i < pos; i++)
		str += ' ';
	str += '^';
	return str;
}

static void Error_message(Scanner :: Lexem Lex, string mes, char* filename, int a){ 
	int last = Lex.getText().size();
	cout << filename << ":" << Lex.getX() << ":" << Lex.getY() + last << ' ' + mes << (Lex.getText()).substr(last - a, a) << endl 
	<< '\t' + Lex.getText() << endl
	<< '\t' + Set_cap(last - a) << endl;
}

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
	string text, type, value;
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
			Lexem a(x, temp_y - lex_size + 1, "", "", "", 0);
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
		cout << "prev_state " << prev_state << " state " << state << " c " << c << endl;
#endif
		if((prev_state == C_INT || state == C_INT) && (lex_size == 9))   
			throw Error(Lexem(x, temp_y - lex_size + 1, types.find(prev_state)->second, text, text, prev_state), 
						prev_state, filename, 1);
		if(state == C_ERROR)
			throw Error(Lexem(x, temp_y - lex_size, types.find(prev_state)->second, text + c, text + c, prev_state),
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
	type = types.find(prev_state)->second;
	if((prev_state == C_IDENTIFIER) && (keyword.find(text) != keyword.end())) type = "keyword";
	int base = 0;
	switch(prev_state){
		case C_INT: base = 10; break;
		case C_INT_16: base = 16; break;
		case C_INT_8: base = 8; break;
	}
	Lexem a;
	if(prev_state == C_INT_EXP){
		int value =  static_cast< int >(stod(text));
		Lexem a(x, temp_y - lex_size + 1, type, text, value, prev_state);
		Lex = a;
	}
	if(base){
		char *endp;
		int value = strtol(text.c_str(), &endp, base);
		Lexem a(x, temp_y - lex_size + 1, type, text, value, prev_state);
		Lex = a;
	}
	else if(prev_state == C_DOUBLE || prev_state == C_DOUBLE_EXP || prev_state == C_DOUBLE_CHECK_EXP){
		double value = atof(text.c_str());
		Lexem a(x, temp_y - lex_size + 1, type, text, value, prev_state);
		Lex = a;
	}
	else if(prev_state == C_CHAREND){
		char cvalue = value.substr(1, lex_size - 3)[0];
		Lexem a(x, temp_y - lex_size + 1, type, text, cvalue, prev_state);
		Lex = a;
	}
	else if(prev_state == C_STRINGEND){
		value = value.substr(1, value_size - 3);
		Lexem a(x, temp_y - lex_size + 1, type, text, value, prev_state);
		Lex = a;
	}
	else{
		Lexem a(x, temp_y - lex_size + 1, type, text, text, prev_state);
		Lex = a;
	}
	lex_size = 0;
	return Lex;
}

Error :: Error(Scanner :: Lexem Lex, int prev_state, char* filename, bool a){ 		
	if(prev_state == C_DOUBLE || prev_state == C_DOUBLE_EXP || prev_state == C_DOUBLE_CHECK_EXP
	|| prev_state == C_INT || prev_state == C_INT_8 || prev_state == C_INT_16){
		if(a)
			Error_message(Lex, "integer constant is too large for its type", filename, 0);
		else
			Error_message(Lex, "unable to find numeric literal operator ‘operator\"\"", filename, 1);
	}
	else if(prev_state == C_CHAREND || prev_state == C_CHARCHECK)
		Error_message(Lex, "missing terminating ' character", filename, 1);
	else if(prev_state == C_STRING)
		Error_message(Lex, "missing terminating \" character", filename, 1);		
	else if(prev_state == C_STRINGSPECIAL)
		Error_message(Lex, "unknown escape sequence ", filename, 2);
}
