#include <map>
#include <set>
#include "Parser.hpp"

bool function = 0;
bool prefix = 0;
Scanner :: Lexem querentOp;

map<int, set<Condition>> priorityTable = {
	{3, {C_ASTERISK, C_DIV, C_MOD}}, {4, {C_MINUS, C_PLUS}}, {5, {C_SHIFTL, C_SHIFTR}}, 
	{6, {C_LESS, C_GREATER, C_LESSEQUAL, C_GREATEREQUAL}}, {7,  {C_EQUAL, C_NOT}}, {8, {C_ANDBIN}},
	{9, {C_XOR}}, {10, {C_ORBIN}}, {11, {C_AND}}, {12, {C_OR}}, {14, {C_COMMA}}};

ExprNode* Parser :: ParseExpr(int priority){
	if(priority == P_Postfix)
		return ParsePostfix();
	if(priority == P_Assign)
		return ParseAssign();
	auto temp  = priorityTable.find(priority)->second;
	auto res = ParseExpr(priority - 1);
	if(temp.find(querentOp.getEnum()) != temp.end()) return res;
	while(temp.find(scan->get().getEnum()) != temp.end()){
		auto op = querentOp = scan->get();
		scan->next();
		if((function) && (priority == P_Comma))
			res = new FuncVarNode(op.getEnum(), res, ParseExpr(priority));
		else
			res = new BinOpNode(op.getEnum(), res, ParseExpr(priority));
	}
	return res;
}

ExprNode* Parser :: ParseAssign(){
	auto res = ParseExpr(P_Or);
	auto op = scan->get();
	if(op == C_ASSIGN || op == C_PLUSEQUAL || op == C_MINUSEQUAL 
	  || op == C_ASTERISKEQUAL || op == C_DIVEQUAL || op == C_MODEQUAL
	  || op == C_ANDBINEQUAL || op == C_ORBINEQUAL
	  || op == C_SHIFTLEQUAL || op == C_SHIFTREQUAL){
		scan->next();
		res = new BinOpNode(op.getEnum(), res, ParseAssign());
	}
	else if(op == C_TERNARYQ){
		scan->next();
		auto expif = ParseExpr(P_Comma);
		if(scan->get() != C_TERNARYC){
			throw SyntaxError("error: expected ‘:’ ", scan, 0);
		}
		else{
			scan->next();
			res = new TernOpNode(res, expif, ParseAssign());    			//ParseExpr()
		}
	}
	return res;
}

ExprNode* Parser ::  ParsePostfix(){
	auto res = ParseBrace();
	auto op = scan->get();
	if(op == C_DECREMENT || op == C_INCREMENT){
		if(prefix)
			throw SyntaxError("error: lvalue required as increment operand", scan);
		scan->next();
		res = new UnOpNode(op.getEnum(), res);
	}
	prefix = 0;
	return res;
}

ExprNode* Parser :: ParseBrace(){
	auto res = ParseFactor();
	auto op = scan->get();
	if(op == C_SQUAREBRACEL){
		prefix = 0;
		scan->next();
		auto index = ParseBrace();
		if(scan->get() != C_SQUAREBRACER)
			throw SyntaxError("error: expected ‘]’ ", scan, 0);	
		scan->next();
		res = new ArrayNode(res, index);
	}
	else if(op == C_FIELD || op == C_FIELDPTR){
		scan->next();
		res = new BinOpNode(op.getEnum(), res, ParseFactor());
	}
	else if(op == C_BRACEL){
		prefix = 0;
		function = 1;
		scan->next();
		auto result = ParseExpr(P_Comma);
		function = 0;
		if(scan->get() != C_BRACER)
			throw SyntaxError("error: expected ‘)’ ", scan, 0);
		scan->next();
		res = new BinOpNode(C_BRACEL, res, result);
	}
	return res;
}

ExprNode* Parser ::  ParseFactor(){
	auto tok = scan->get();
	if((tok == C_INCREMENT || tok == C_DECREMENT || tok == C_MINUS || 															
		tok == C_PLUS || tok == C_NOT || tok == C_NOTBIN || tok == C_ASTERISK || tok == C_ANDBIN) && (prefix == 0)){
		prefix = 1;
		if(tok == C_MINUS || tok == C_PLUS || tok == C_NOT || tok == C_NOTBIN || tok == C_ASTERISK || tok == C_ANDBIN) prefix = 0;
		scan->next();
		return new UnOpNode(ParseFactor(), tok.getEnum());
	}

	if((tok == C_INT || tok == C_INT_EXP || tok == C_INT_8 || tok == C_INT_16) && (prefix == 0)){
		scan->next();
		return new iConstNode(tok.getintValue());
	}
	else if((tok == C_DOUBLE || tok == C_DOUBLE_CHECK_EXP || tok == C_DOUBLE_EXP) && (prefix == 0)){
		scan->next();
		return new dConstNode(tok.getdoubValue());
	}
	else if(tok == C_IDENTIFIER){
		scan->next();
		querentOp = tok;
		if(Sym.Table.find(tok.getstrValue()) == Sym.Table.end())
			throw SyntaxError("error: \"" + tok.getstrValue() + "\" was not declarated in this scope", scan, 0);
		return new VarNode((SymVar*)Sym.Table.find(tok.getstrValue())->second);
	}
	else if(tok == C_BRACEL){
		prefix = 0;
		Scanner :: Lexem a(1, 1, "", "", C_END);
		function = 0;
		scan->next();
		querentOp = a;
		auto result = ParseExpr(P_Comma);
		querentOp = a;
		if(scan->get() != C_BRACER)
			throw SyntaxError("error: expected ‘)’ ", scan);
		scan->next();
		return result;
	}
	else if(tok == C_SIZEOF){
		scan->next();
		return new UnOpNode(ParseExpr(P_Comma), tok.getEnum());
	}
	else
		if((querentOp == C_COMMA) && (scan->get() == C_END || scan->get() == C_BRACER))
			throw SyntaxError("error: expression list treated as compound expression in initializer", scan, 0);			
		else 
			throw SyntaxError("error: expected unqualified-id before", scan);
}

static bool MainCheck(Scanner* scan){
	if(scan->get().getstrValue() == "main"){
		scan->next();
		if(scan->get() == C_BRACEL){
			scan->next();
			if(scan->get() == C_BRACER){
				scan->next();
				if(scan->get() == C_CURLYBRACEL) return 1;
			}
			else
				throw SyntaxError("error: expected \"(\" after \"main\"", scan);
		}
		else 
			throw SyntaxError("error: expected \")\" after (", scan);
	}
	return 0;
}
/*
static void AddType(Scanner* scan){
	scan->next();
	auto tok = sacn->get();
	if(Sym.Table.find(tok.getstrValue()) != Sym.Table.end() && Sym.Table[tok.getstrValue()]->Istype()){
		scan->next();
		tok = scan->get();
		if(Sym.Table.find(tok.getstrValue()) != Sym.Table.end() && !Sym.Table[tok.getstrValue()]->Istype())
			throw SyntaxError(error: ‘typedef int a’ redeclared as different kind of symbol)
	}


}*/

void Parser :: ParseDeclaration(){
	auto tok = scan->get();
	if(tok == D_const) scan->next();						//Поменять потом
	string type = tok.getstrValue();
	scan->next();
	if(tok == D_int && MainCheck(scan)){
		scan->next();
		return;
	}
	tok = scan->get();
	bool comma = 1;
	while(comma){
		int count_asterisk = 0;
		while(tok == C_ASTERISK){
			scan->next();
			tok = scan->get();
			count_asterisk++;
		}
		if(tok == C_IDENTIFIER && Sym.Table.find(tok.getstrValue()) == Sym.Table.end()){
			string temp_type = type;
			if(count_asterisk){
				for(int i = 0; i < count_asterisk; i++) temp_type += '*';
				Sym.Table[temp_type] = new SymTypePointer(temp_type);	
			}
			Sym.addVar(new SymVar(Sym.Table[temp_type], tok.getstrValue()));
			scan->next();
			if(scan->get() != C_COMMA){
				comma = 0;
				if(scan->get() != C_SEMICOLON)
					throw SyntaxError("error: expected initializer", scan);
			}
			scan->next();
			tok = scan->get();
		}
		else if(tok != C_IDENTIFIER)
			throw SyntaxError("error: expected unqualified-id before numeric constant", scan);
		else if(Sym.Table.find(tok.getstrValue()) != Sym.Table.end())
			throw SyntaxError("error: rediclaration of \"" + type + " " + tok.getstrValue() + "\"", scan, 0);
	}
}

void Parser :: print(){
	Sym.Table["int"] = new SymInt();
	Sym.Table["char"] = new SymChar();
	Sym.Table["double"] = new SymDouble();
	auto tok = scan->get();
	while((Sym.Table.find(tok.getstrValue()) != Sym.Table.end() 
		  && Sym.Table[tok.getstrValue()]->Istype()) || tok == D_const || tok == D_typedef){
		//if(tok == D_typedef) AddType(scan);
		ParseDeclaration();
		tok = scan->get();
	}
	int i = 0;
	while(scan->get() != C_CURLYBRACER){
		res[i] = ParseExpr(P_Comma);
		i++;
		scan->next();
	}
	for(int j = 0; j < i; j++){
		res[j]->print(0, T_None);
		printf("\n");
	}
	Sym.print();
}