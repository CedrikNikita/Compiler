#pragma once
#include <map>
#include "SyntaxNode.hpp"

static void setSpace(int space){
	for(int i = 0; i < space; i++) printf(" ");
}

//first to second
enum TypeConvertion{TC_None, TC_IntDouble, TC_DoubleInt, TC_IntChar, TC_CharInt, TC_IntPtr, TC_PtrPtr,
					TC_CharChar, TC_ArrayInt, TC_CharDouble, TC_DoubleChar};

class ExprNode;
class StmtNode;
class AsmCode;

class Symbol{
	string name;
	public:
		virtual string getName() = 0;
		virtual void print(int) = 0;
		virtual bool isType() = 0;
		virtual string getType() = 0;
		virtual bool isAlias() = 0;
		virtual bool isConst() = 0;
		virtual int getSize() = 0;

		virtual void generate(AsmCode&) = 0;
		virtual ~Symbol(){}
};

class SymArray;
class SymPointer;
class SymTypeScalar;
class SymAlias;
class SymChar;
class SymInt;
class SymDouble;
class SymStruct;
class SymVoid;

class SymType: public Symbol{
	string name;
	public:
		virtual TypeConvertion meet(SymType*, Condition) = 0;
		virtual TypeConvertion meet(SymAlias*, Condition) = 0;
		virtual TypeConvertion meet(SymPointer*, Condition) = 0;
		virtual TypeConvertion meet(SymArray*, Condition) = 0;
		virtual TypeConvertion meet(SymChar*, Condition) = 0;
		virtual TypeConvertion meet(SymInt*, Condition) = 0;
		virtual TypeConvertion meet(SymDouble*, Condition) = 0;
		virtual TypeConvertion meet(SymStruct*, Condition) = 0;
		virtual TypeConvertion meet(SymVoid*, Condition) = 0;

		virtual bool isConst(){return false;}												//TODO : Беда
		virtual void print(int space) = 0;
		virtual string getName() = 0;
		virtual string getType() = 0;
		virtual bool isType() = 0;
		virtual bool isAlias() = 0;
		virtual int getSize() = 0;

		virtual void generate(AsmCode&) = 0;
//		virtual ~SymType(){}
};


class SymArray: public SymType{
	Symbol* type;
	int size;
	void* arr;
	bool is_const;
	public:
		virtual TypeConvertion meet(SymType*, Condition);
		virtual TypeConvertion meet(SymAlias*, Condition);
		virtual TypeConvertion meet(SymPointer*, Condition);
		virtual TypeConvertion meet(SymArray*, Condition);
		virtual TypeConvertion meet(SymChar*, Condition);
		virtual TypeConvertion meet(SymInt*, Condition);
		virtual TypeConvertion meet(SymDouble*, Condition);
		virtual TypeConvertion meet(SymStruct*, Condition);
		virtual TypeConvertion meet(SymVoid*, Condition);

		SymArray(Symbol* _type, int _size, bool _const): type(_type), size(_size), is_const(_const){	//TODO : массив массивов
			/*if(arr != NULL)
				delete arr;*/
			arr = malloc(size);
		}
//		~SymArray(){}
		virtual void print(int space){}
		virtual string getName(){return "array of " + type->getName();}
		virtual string getType();
		virtual int getSize(){return size;}
		virtual bool isType(){ return true; }
		virtual bool isAlias(){return false;}

		virtual void generate(AsmCode&);
};

class SymPointer: public SymType{
	string name;
	bool is_const;
	Symbol* type;
	public:
		virtual TypeConvertion meet(SymType*, Condition);
		virtual TypeConvertion meet(SymAlias*, Condition);
		virtual TypeConvertion meet(SymPointer*, Condition);
		virtual TypeConvertion meet(SymArray*, Condition);
		virtual TypeConvertion meet(SymChar*, Condition);
		virtual TypeConvertion meet(SymInt*, Condition);
		virtual TypeConvertion meet(SymDouble*, Condition);
		virtual TypeConvertion meet(SymStruct*, Condition);
		virtual TypeConvertion meet(SymVoid*, Condition);

		SymPointer(string _name,  Symbol* _type, bool _is_const): name(_name), type(_type), is_const(_is_const){};
		~SymPointer(){}
		virtual void print(int space){setSpace(space); printf("type %s\n", name.c_str());}
		virtual string getName(){return name;}
		virtual string getType(){return name;}
		virtual bool isType(){return true;}
		virtual bool isAlias(){return false;}
		virtual int getSize(){return sizeof(type);}

		virtual void generate(AsmCode&);
};

class SymTypeScalar: public SymType{
	string name;
	public:
		virtual TypeConvertion meet(SymType*, Condition) = 0;
		virtual TypeConvertion meet(SymAlias*, Condition) = 0;
		virtual TypeConvertion meet(SymPointer*, Condition) = 0;
		virtual TypeConvertion meet(SymArray*, Condition) = 0;
		virtual TypeConvertion meet(SymChar*, Condition) = 0;
		virtual TypeConvertion meet(SymInt*, Condition) = 0;
		virtual TypeConvertion meet(SymDouble*, Condition) = 0;
		virtual TypeConvertion meet(SymStruct*, Condition) = 0;
		virtual TypeConvertion meet(SymVoid*, Condition) = 0;

		virtual string getName() = 0;
		virtual string getType() = 0;
		virtual bool isType() = 0;
		virtual bool isAlias() = 0;
		virtual int getSize() = 0;

		virtual void generate(AsmCode&) = 0;
//		virtual ~SymTypeScalar(){}
};

class SymAlias: public SymType{
	string name;
	Symbol* type;
	public:
		virtual TypeConvertion meet(SymType*, Condition);
		virtual TypeConvertion meet(SymAlias*, Condition);
		virtual TypeConvertion meet(SymPointer*, Condition);
		virtual TypeConvertion meet(SymArray*, Condition);
		virtual TypeConvertion meet(SymChar*, Condition);
		virtual TypeConvertion meet(SymInt*, Condition);
		virtual TypeConvertion meet(SymDouble*, Condition);
		virtual TypeConvertion meet(SymStruct*, Condition);
		virtual TypeConvertion meet(SymVoid*, Condition);

		SymAlias(string _name, Symbol* _type): name(_name), type(_type){}
//		~SymAlias(){delete type;}
		Symbol* getAlias();
		
		virtual void print(int space){setSpace(space); printf("type %s\n", name.c_str());}
		virtual string getName(){return name;}
		virtual string getType(){return getAlias()->getName();}
		virtual bool isType(){return true;}
		virtual bool isAlias(){ return true; }
		virtual int getSize();

		virtual void generate(AsmCode&);
};

class SymChar: public SymTypeScalar{
	string name;
	public:
		virtual TypeConvertion meet(SymType*, Condition);
		virtual TypeConvertion meet(SymAlias*, Condition);
		virtual TypeConvertion meet(SymPointer*, Condition);
		virtual TypeConvertion meet(SymArray*, Condition);
		virtual TypeConvertion meet(SymChar*, Condition);
		virtual TypeConvertion meet(SymInt*, Condition);
		virtual TypeConvertion meet(SymDouble*, Condition);
		virtual TypeConvertion meet(SymStruct*, Condition);
		virtual TypeConvertion meet(SymVoid*, Condition);

		SymChar(): name("char"){}
		virtual void print(int space){setSpace(space); printf("type %s\n", name.c_str());}
		virtual string getName(){return name;}
		virtual string getType(){return name;}
		virtual bool isType(){return true;}
		virtual bool isAlias(){return false;}
		virtual int getSize() {return sizeof(char);}

		virtual void generate(AsmCode&);
};

class SymVoid: public SymTypeScalar{
	string name;
	public:
		virtual TypeConvertion meet(SymType*, Condition);
		virtual TypeConvertion meet(SymAlias*, Condition);
		virtual TypeConvertion meet(SymPointer*, Condition);
		virtual TypeConvertion meet(SymArray*, Condition);
		virtual TypeConvertion meet(SymChar*, Condition);
		virtual TypeConvertion meet(SymInt*, Condition);
		virtual TypeConvertion meet(SymDouble*, Condition);
		virtual TypeConvertion meet(SymStruct*, Condition);
		virtual TypeConvertion meet(SymVoid*, Condition);

		SymVoid(): name("void"){}
		virtual void print(int space){setSpace(space); printf("type %s\n", name.c_str());}
		virtual string getName(){return name;}
		virtual string getType(){return name;}
		virtual bool isType(){return true;}
		virtual bool isAlias(){return false;}
		virtual int getSize() {return sizeof(char);}

		virtual void generate(AsmCode&);
};

class SymInt: public SymTypeScalar{
	string name;
	public:
		virtual TypeConvertion meet(SymType*, Condition);
		virtual TypeConvertion meet(SymAlias*, Condition);
		virtual TypeConvertion meet(SymPointer*, Condition);
		virtual TypeConvertion meet(SymArray*, Condition);
		virtual TypeConvertion meet(SymChar*, Condition);
		virtual TypeConvertion meet(SymInt*, Condition);
		virtual TypeConvertion meet(SymDouble*, Condition);
		virtual TypeConvertion meet(SymStruct*, Condition);
		virtual TypeConvertion meet(SymVoid*, Condition);

		SymInt(): name("int"){}
		virtual void print(int space){setSpace(space); printf("type %s\n", name.c_str());}
		virtual string getName(){return name;}
		virtual string getType(){return name;}
		virtual bool isType(){return true;}
		virtual bool isAlias(){return false;}
		virtual int getSize(){return sizeof(int);}

		virtual void generate(AsmCode&);
};

class SymDouble: public SymTypeScalar{
	string name;
	public:
		virtual TypeConvertion meet(SymType*, Condition);
		virtual TypeConvertion meet(SymAlias*, Condition);
		virtual TypeConvertion meet(SymPointer*, Condition);
		virtual TypeConvertion meet(SymArray*, Condition);
		virtual TypeConvertion meet(SymChar*, Condition);
		virtual TypeConvertion meet(SymInt*, Condition);
		virtual TypeConvertion meet(SymDouble*, Condition);
		virtual TypeConvertion meet(SymStruct*, Condition);
		virtual TypeConvertion meet(SymVoid*, Condition);

		SymDouble(): name("double"){}
		virtual void print(int space){setSpace(space); printf("type %s\n", name.c_str());}
		virtual string getName(){return name;}
		virtual string getType(){return name;}
		virtual bool isType(){return true;}
		virtual bool isAlias(){return false;}
		virtual int getSize(){return sizeof(double);}

		virtual void generate(AsmCode&);
};

class SymVar: public Symbol{
	ExprNode* init;
	string name;
	Symbol* type;
	bool is_const;
	public:
		virtual TypeConvertion meet(SymType*, Condition);
		SymVar(Symbol* _type, string _name): type(_type), name(_name), init(NULL), is_const(0){}
		SymVar(Symbol* _type, string _name, bool _is_const): type(_type), name(_name), init(NULL), is_const(_is_const){}
		~SymVar();

		string getType(){return type->getType();}
		SymType* getSymType(){return (SymType*)type;}
		void setInit(ExprNode* _init){init = _init;}
		bool isConst(){return is_const;}
		ExprNode* getInit(){return init;}

		virtual void print(int space){setSpace(space); printf("%s", (type->getName()).c_str()); if(is_const) printf(" const");  printf(" %s\n", name.c_str());}
		virtual string getName(){return name;}
		virtual bool isType(){return false;}
		virtual bool isAlias(){return 0;}
		virtual int getSize(){return 0;}

		virtual void generate(AsmCode&);
};

class List{
	List *prev, *next;
	public:
		Symbol *var;
		List(Symbol *_var,  List *_next): var(_var), next(_next){}
		~List(){delete var;}
		void setNext(List *_next){if(_next != NULL) next = _next;}
		void setPrev(List *_prev){if(_prev != NULL) prev = _prev;}
		List* getNext(){return next;}
		List* getPrev(){return prev;}
};

class SymTable{
	List* list;
	public:	
		map<string, Symbol*> Table;
		void addSymbol(Symbol*);
		SymTable(): list(){}
		~SymTable(){/*for(int i = 0; list != NULL; 1){auto temp = list->getNext(); delete list; list = temp;}*/}
		void print(int);

		virtual void generate(AsmCode&);
};

class SymFunction: public Symbol{						//TODO : наследование
	StmtNode *body;
	Symbol* type;
	bool b_const;
	string name;
	List* arg;
	public:
		SymFunction(string _name, Symbol* _type, List* _arg, StmtNode* _body, bool _b_const): type(_type), name(_name), arg(_arg), body(_body), b_const(_b_const){}
		~SymFunction();
		void setBody(StmtNode* _body){body = _body;}
		List* getArgs(){ return arg; }

		virtual void print(int space);
		virtual bool isType(){return false;}
		virtual string getName(){return name;}
		virtual string getType(){return type->getType();}
		virtual bool isAlias(){return false;}
		virtual bool isConst(){ return b_const; }
		virtual int getSize(){return 0;}

		virtual void generate(AsmCode&);
};

class SymStruct: public SymType{
	string name;
	public:
		virtual TypeConvertion meet(SymType*, Condition);
		virtual TypeConvertion meet(SymAlias*, Condition);
		virtual TypeConvertion meet(SymPointer*, Condition);
		virtual TypeConvertion meet(SymArray*, Condition);
		virtual TypeConvertion meet(SymChar*, Condition);
		virtual TypeConvertion meet(SymInt*, Condition);
		virtual TypeConvertion meet(SymDouble*, Condition);
		virtual TypeConvertion meet(SymStruct*, Condition);
		virtual TypeConvertion meet(SymVoid*, Condition);

		SymTable Sym;
		SymStruct(string _name): name(_name){}

		virtual void print(int space){setSpace(space); printf("type %s\n", name.c_str());}
		virtual string getName(){return name;}
		virtual string getType(){return name;}
		virtual bool isType(){return true;}
		virtual bool isAlias(){return 0;}
		virtual int getSize(){return 0;}

		virtual void generate(AsmCode&);
};
