#pragma once
#include <string.h>
#include "Scanner.hpp"

using namespace std;

enum TreeState{T_None, T_Left, T_Right};

class SymVar;

class SyntaxNode{
	public:
};

class ExprNode : public SyntaxNode{
	public:
		virtual void print(int, TreeState) = 0;
};

class VarNode : public ExprNode{
	SymVar* var;
	public:
		VarNode(SymVar* _var): var(_var){}
		virtual void print(int, TreeState);
};

class iConstNode : public ExprNode{
	int value;
	public:
		iConstNode(int a): value(a){}
		virtual void print(int, TreeState);
};

class dConstNode : public ExprNode{
	double value;
	public:
		dConstNode(double a): value(a){}
		virtual void print(int, TreeState);
};

class BinOpNode : public ExprNode{
	Condition op;
	ExprNode *res, *next;
	public:
		BinOpNode(Condition _op, ExprNode* _res, ExprNode* _next): op(_op), res(_res), next(_next){}
		virtual void print(int, TreeState);
};

class UnOpNode : public ExprNode{
	Condition op;
	ExprNode *res;
	bool prefix;
	public:
		UnOpNode(Condition _op, ExprNode* _res): op(_op), res(_res), prefix(0){}
		UnOpNode(ExprNode* _res, Condition _op): op(_op), res(_res), prefix(1){}
		virtual void print(int, TreeState);
};

class ArrayNode : public ExprNode{
	ExprNode *name, *index;
	public:
		ArrayNode(ExprNode* _name, ExprNode* _index): name(_name), index(_index){}
		virtual void print(int, TreeState);
};

class TernOpNode: public ExprNode{
	ExprNode *condition, *expif, *exprelse;
	public:
		TernOpNode(ExprNode* _condition, ExprNode* _expif, ExprNode* _exprelse):
		condition(_condition), expif(_expif), exprelse(_exprelse){}
		virtual void print(int, TreeState);
};

class FuncVarNode: public ExprNode{
	Condition op;
	ExprNode *res, *next;
	public:
		FuncVarNode(Condition _op, ExprNode* _res, ExprNode* _next): op(_op), res(_res), next(_next){}
		virtual void print(int, TreeState);
};

class SyntaxError{
	public:
		string symb;
		string str;
		Scanner* scan;
		SyntaxError(string _str, Scanner* _scan): str(_str), scan(_scan), symb(" '" + _scan->get().getText() + "' token"){}
		SyntaxError(string _str, Scanner* _scan, bool _cap): str(_str), scan(_scan), symb(""){}
};