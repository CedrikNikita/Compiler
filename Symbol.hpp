#pragma once
#include <map>
#include "SyntaxNode.hpp"

class ExprNode;

class Symbol{
	string name;
	public:
		virtual bool Istype() = 0;
		virtual string getType() = 0;
};

class SymType: public Symbol{
	string name;
	public:
		virtual string getType() = 0;
		virtual bool Istype() = 0;
};

class SymTypePointer: public SymType{
	string name;
	public:
		SymTypePointer(string _name): name(_name){};
		virtual string getType(){return name;}
		virtual bool Istype(){return 1;}	
};

class SymTypeScalar: public SymType{
	string name;
	public:
		virtual string getType() = 0;
		virtual bool Istype() = 0;
};

class SymChar: public SymTypeScalar{
	string name;
	public:
		SymChar(): name("char"){}
		virtual string getType(){return name;}
		virtual bool Istype(){return 1;}
};

class SymInt: public SymTypeScalar{
	string name;
	public:
		SymInt(): name("int"){}
		virtual string getType(){return name;}
		virtual bool Istype(){return 1;}
};

class SymDouble: public SymTypeScalar{
	string name;
	public:
		SymDouble(): name("double"){}
		virtual string getType(){return name;}
		virtual bool Istype(){return 1;}
};

class SymVar: public Symbol{
	ExprNode* init;
	string name;
	Symbol* type;
	public:
		SymVar(Symbol* _type, string _name): type(_type), name(_name){}
		string getVar(){return name;}
		string getType(){return type->getType();}
		virtual bool Istype(){return 0;}
};

class SymVarParam: public SymVar{

};

class SymVarConst: public SymVar{

};

class SymVarLocal: public SymVar{

};

class SymVarGlobal: public SymVar{

};

class SymProc: public Symbol{

};

class List{
	List *prev, *next;
	public:
		SymVar *var;
		List(SymVar *_var,  List *_next): var(_var), next(_next){};
		void setNext(List *_next){if(_next != NULL) next = _next;}
		void setPrev(List *_prev){if(_prev != NULL) prev = _prev;}
		List* getNext(){return next;}
		List* getPrev(){return prev;}
};

class SymTable{
	List* list;
	public:
		map<string, Symbol*> Table;
		void addVar(SymVar*);
		SymTable(): list(){};
		void print();
};
