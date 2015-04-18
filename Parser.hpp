#pragma once
#include "Scanner.hpp"
#include "SyntaxNode.hpp"
#include "Symbol.hpp"
#include <set>

enum PriorityNames{P_Factor, P_Brace, P_Postfix, P_Term, P_PlusM, P_Shift, P_Compare,
			  P_TCompare, P_Andbin, P_Xor, P_Orbin, P_And, P_Or, P_Assign, P_Comma};

struct Stack{
	SymTable Sym;
	Stack *next, *prev;
	Stack(){}
};

class TypeContainer{
	public:
	Symbol* type;
	bool b_const;
	bool b_typedef;
	TypeContainer(Symbol* _type, bool _b_typedef, bool _b_const): type(_type), b_const(_b_const), b_typedef(_b_typedef){}
};

List* addListElem(List*, Symbol*);
ListStmt* addStmt(ListStmt*, StmtNode*);

class SymStack{
	Stack* stack;
	public:
		SymStack(): stack(new Stack()){
			stack->Sym.addSymbol(new SymVoid());
			stack->Sym.addSymbol(new SymInt());
			stack->Sym.addSymbol(new SymChar());
			stack->Sym.addSymbol(new SymDouble());
			stack->prev = NULL;
		}
		~SymStack(){for (int i = 0; stack != NULL; 1) { auto temp = stack->next; delete stack; stack = temp; } }
		void pop(){stack = stack->prev; delete stack->next; stack->next = NULL;}		//TODO : Underflow
		void push(SymTable);
		SymTable& peek(){return stack->Sym;}
		Symbol* requireIdentifier(string, bool);
		void notrequireIdentifier(string);
};

class CodeGen;

class Parser{
	Scanner* scan;
	SymStack stack;
	set<string> functions;
	public:
		Parser(Scanner* a): scan(a){}
		ExprNode* parseFactor(bool, bool, Condition);
		ExprNode* parseExpr(int, bool, bool, Condition);
		ExprNode* parsePostfix(bool, bool, Condition);
		ExprNode* parseBrace(bool, bool, Condition);
		ExprNode* parseAssign(bool, bool, Condition);
		ExprNode* parsePrefix(bool, bool, Condition);

		StmtNode* parseExprStmt();
		StmtNode* parseStmt();
		StmtNode* parseBlock();
		StmtNode* parseDoWhile();
		StmtNode* parseWhile();
		StmtNode* parseIf();
		StmtNode* parseFor();

		StmtNode* parsePrintf();


		void parseDeclaration(TypeContainer);
		Symbol* parseSimpleDeclaration(TypeContainer);
		Symbol* parsePointer(Symbol*, int*, bool);		//TODO
		Symbol* parseArray(Symbol*, string, bool);
		Symbol* parseFunction(Symbol*, string, bool);
		void parseProgramm();
		TypeContainer parseType(bool, bool, bool);
		TypeContainer parseStruct(bool, bool);

		void print();
		void generate(CodeGen&);
};
