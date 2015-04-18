#pragma once
#include <string.h>
#include "Scanner.hpp"
#include "Symbol.hpp"

using namespace std;

enum TreeState{T_None, T_Left, T_Right};

enum Type{Char, Int, Double, None};

class SymVar;
class SymType;
class SymTable;
class AsmCode;
class StmtNode;
class List;

class ListStmt{
	ListStmt* prev, *next;
	StmtNode* val;
public:
	ListStmt(StmtNode* _val, ListStmt* _next) : val(_val), next(_next){}
//	~ListStmt();
	void setNext(ListStmt *_next){ if (_next != NULL) next = _next; }
	void setPrev(ListStmt *_prev){ if (_prev != NULL) prev = _prev; }
	void print(int space, TreeState state);
	StmtNode* getVal(){ return val; }
	ListStmt* getNext(){ return next; }
	ListStmt* getPrev(){ return prev; }
};

class SyntaxNode{
	public:
		virtual void print(int, TreeState) = 0;

		virtual void generate(AsmCode&) = 0;
		virtual void generateLvalue(AsmCode&) = 0;
//		virtual ~SyntaxNode(){}
};

class ExprNode: public SyntaxNode{
	public:
		virtual void print(int, TreeState) = 0;
		virtual int islvalue() = 0;
		virtual SymType* getType() = 0;
		virtual int getInt(bool) = 0;

		virtual void generate(AsmCode&) = 0;
		virtual void generateLvalue(AsmCode&) = 0;
////		virtual ~ExprNode(){}
};

class VarNode : public ExprNode{
	SymVar* var;
	public:
		VarNode(SymVar* _var): var(_var){}
		virtual void print(int, TreeState);
		virtual int islvalue();
		virtual SymType* getType();
		virtual int getInt(bool);

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);

};

class ConstIntNode : public ExprNode{
	SymType* type;
	Scanner :: Lexem value;
	public:
		ConstIntNode(Scanner :: Lexem);
////		~ConstIntNode(){}
		virtual void print(int, TreeState);
		virtual int islvalue(){return 0;}
		virtual SymType* getType(){return type;}
		virtual Scanner :: Lexem getValue(){return value;}
		virtual int getInt(bool a){return value.getintValue();}
		
		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);
};

class ConstDoubleNode : public ExprNode{
	SymType* type;
	Scanner :: Lexem value;
	public:
		ConstDoubleNode(Scanner :: Lexem);
//		~ConstDoubleNode(){}
		virtual void print(int, TreeState);
		virtual int islvalue(){return 0;}
		virtual SymType* getType(){return type;}
		virtual Scanner :: Lexem getValue(){return value;}
		virtual int getInt(bool);

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);
};

class ConstCharNode : public ExprNode{
	SymType* type;
	Scanner :: Lexem value;
	public:
		ConstCharNode(Scanner :: Lexem);
//		~ConstCharNode(){}
		virtual void print(int, TreeState);
		virtual int islvalue(){return 0;}
		virtual SymType* getType(){return type;}
		virtual Scanner :: Lexem getValue(){return value;}
		virtual int getInt(bool a){return (int)value.getcharValue();}

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);
};

class BinOpNode : public ExprNode{
	SymType* type;
	Condition op;
	ExprNode *left, *right;
	public:
		BinOpNode(Condition, ExprNode*, ExprNode*);
//		~BinOpNode(){delete left; delete right;}
		virtual void print(int, TreeState);
		virtual int islvalue(){return 0;}
		virtual SymType* getType(){return type;}
		virtual int getInt(bool);

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);
};

class UnOpNode : public ExprNode{
	SymType* type;
	Condition op;
	ExprNode *res;
	bool prefix;
	public:
		UnOpNode(Condition _op, ExprNode* _res): op(_op), res(_res), prefix(0){}
		UnOpNode(ExprNode* _res, Condition _op): op(_op), res(_res), prefix(1){}
//		~UnOpNode(){delete res;}
		virtual void print(int, TreeState);
		virtual int islvalue(){return 0;}
		virtual SymType* getType(){return type;}
		virtual int getInt(bool a){if(op == C_MINUS) return -res->getInt(a);}				//TODO

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);
};


class ArrayNode : public ExprNode{
	SymType* type;
	ExprNode *name;
	ListStmt* list;
	public:
		ArrayNode(ExprNode* _name, ListStmt* _list): name(_name), list(_list){}
//		~ArrayNode(){delete name; for(int i = 0; list != NULL; 1){auto temp_list = list->getNext(); delete list; list = temp_list;}}
		virtual void print(int, TreeState);
		virtual int islvalue(){return 1;}
		virtual SymType* getType(){return type;}
		virtual int getInt(bool){ return 0; }

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);
};

class TernOpNode: public ExprNode{
	SymType* type;
	ExprNode *condition, *expif, *exprelse;
	public:
		TernOpNode(ExprNode* _condition, ExprNode* _expif, ExprNode* _exprelse):
		condition(_condition), expif(_expif), exprelse(_exprelse){}
//		~TernOpNode(){delete condition; delete expif; delete exprelse;}
		virtual void print(int, TreeState);
		virtual int islvalue(){return 0;}
		virtual SymType* getType(){return type;}
		virtual int getInt(bool){ return false; }

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);
};

class FuncVarNode: public ExprNode{
	SymType* type;
	ExprNode *name;
	ListStmt* list;
	public:
		FuncVarNode(ExprNode* _name, ListStmt* _list): name(_name), list(_list){}
//		~FuncVarNode(){delete name; for(int i = 0; list != NULL; 1){auto temp_list = list->getNext(); delete list; list = temp_list;}}
		virtual void print(int, TreeState);
		virtual int islvalue(){return 0;}
		virtual SymType* getType(){return type;}
		virtual int getInt(bool){ return 0; }

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);
};

class SyntaxError{
	public:
		string str;
		bool a;
		SyntaxError(string _str, bool _a): str(_str), a(true){}
		SyntaxError(string _str): str(_str), a(false){}
};

class ConvNode: public ExprNode{
	ExprNode* right;
	SymType* type;
	public:
		ConvNode(ExprNode* _right, SymType* _type): right(_right), type(_type){}
//		~ConvNode(){delete right;}
		virtual void print(int, TreeState);
		virtual int islvalue(){return 1;}
		virtual SymType* getType(){return type;}
		virtual int getInt(bool a){return right->getInt(a);}

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);
};


class StmtNode: public SyntaxNode{
	public:
		virtual void print(int, TreeState) = 0;
		virtual void generate(AsmCode&) = 0;
		virtual void generateLvalue(AsmCode&) = 0;

//		virtual ~StmtNode(){}
};

class PrintfStmt: public StmtNode{
	ListStmt* list;
	string format, fmt;
	public:
		PrintfStmt(ListStmt* _list,  string _format, string _fmt): list(_list), format(_format), fmt(_fmt){}
		virtual void print(int, TreeState){}
		virtual void generate(AsmCode& a);
		virtual void generateLvalue(AsmCode&){}
};

class ExprStmt: public StmtNode{
	public:
		ExprNode* stmt;									//TODO : лежит внутри
		ExprStmt(ExprNode* _stmt): stmt(_stmt){}
		virtual void print(int, TreeState);

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);

//		virtual ~ExprStmt(){}
};

class EmptyStmt: public StmtNode{
	public:
		EmptyStmt(){}
		virtual void print(int i, TreeState state){}

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);
};

class BlockStmt: public StmtNode{
	SymTable* sym;
	ListStmt* body; 
	public:
		BlockStmt(ListStmt* _body, SymTable* _sym);
//		~BlockStmt();
		virtual void print(int, TreeState);

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);
};

class WhileStmt: public StmtNode{
	ExprNode* cond;
	StmtNode* body;
	public:
		WhileStmt(ExprNode* _cond, StmtNode* _body): cond(_cond), body(_body){}
//		~WhileStmt(){delete cond; delete body;}
		virtual void print(int space, TreeState state);

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);
};

class DoWhileStmt: public StmtNode{
	ExprNode* cond;
	StmtNode* body;
	public:
		DoWhileStmt(ExprNode* _cond, StmtNode* _body): cond(_cond), body(_body){}
//		~DoWhileStmt(){delete cond; delete body;}
		virtual void print(int, TreeState);

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);
};

class IfStmt: public StmtNode{
	ExprNode* cond;
	StmtNode* ifbody, *elsebody;
	public:
		IfStmt(ExprNode* _cond, StmtNode* _ifbody, StmtNode* _elsebody): cond(_cond), ifbody(_ifbody), elsebody(_elsebody){}
		IfStmt(ExprNode* _cond, StmtNode* _ifbody): cond(_cond), ifbody(_ifbody), elsebody(new EmptyStmt()){}
//		~IfStmt(){delete cond; delete ifbody; if(elsebody != NULL) delete elsebody;}
		virtual void print(int, TreeState);

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);
};

class ForStmt: public StmtNode{
	ExprNode* from, *cond, *operation;
	StmtNode* body;
	public:
		ForStmt(ExprNode* _from, ExprNode* _cond, ExprNode* _operation, StmtNode* _body): from(_from), cond(_cond), operation(_operation), body(_body){}
//		~ForStmt(){delete from; delete cond; if(operation != NULL) delete operation; delete body;}
		virtual void print(int, TreeState);

		virtual void generate(AsmCode&);
		virtual void generateLvalue(AsmCode&);
};

class BreakStmt: public StmtNode{
	public:
	virtual void print(int, TreeState){}

	virtual void generate(AsmCode&);
	virtual void generateLvalue(AsmCode&);
};

class ContinueStmt: public StmtNode{
	public:
	virtual void print(int, TreeState){}

	virtual void generate(AsmCode&);
	virtual void generateLvalue(AsmCode&);
};

class ReturnStmt: public StmtNode{
	public:
	virtual void print(int, TreeState){}

	virtual void generate(AsmCode&);
	virtual void generateLvalue(AsmCode&);
};
