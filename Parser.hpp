#pragma once
#include "Scanner.hpp"
#include "SyntaxNode.hpp"
#include "Symbol.hpp"

enum PriorityNames{P_Factor, P_Brace, P_Postfix, P_Term, P_PlusM, P_Shift, P_Compare,
			  P_TCompare, P_Andbin, P_Xor, P_Orbin, P_And, P_Or, P_Assign, P_Comma};

class Parser{
	Scanner* scan;
	SymTable Sym;
	ExprNode* res[500];
	public:
		Parser(Scanner* a): scan(a){}
		ExprNode* ParseFactor();
		ExprNode* ParseExpr(int);
		ExprNode* ParsePostfix();
		ExprNode* ParseBrace();
		ExprNode* ParseAssign();
		void ParseDeclaration();
		void print();
};