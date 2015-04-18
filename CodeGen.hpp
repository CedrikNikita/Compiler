#pragma once
#include "Parser.hpp"
#include <map>

enum Cmd{ c_push, c_push_offset, c_push_dword_ptr, c_pop, c_add, c_sub, c_inc, c_dec, c_imul, c_setg, c_setge, c_setl, 
		  c_setle, c_sete, c_setne, c_xor, c_dword, c_main_proc, c_code, c_and, c_or, c_mov, c_cmp, c_notbin,
		  c_neg, c_call_printf};
enum Operands{ eax, ebx, ecx, esp, eax_sq, ebx_sq, cl, o_1, o_4, o_8, o_12};

static string commands[] = { "push", "push offset", "push dword ptr", "pop", "add", "sub", "inc", "dec",
							 "imul", "setg", "setge", "setl", "setle", "sete", "setne", "xor",
							 "dword" , "main proc", ".code", "and", "or", "mov", "cmp", "not", "neg", "call _printf"};
static string operands[] = { "eax", "ebx", "ecx" , "esp", "[eax]", "[ebx]", "cl", "1", "4", "8", "12"};

class AsmOperand;
class AsmCmd;

class ListCmd{
	ListCmd* prev, *next;
	AsmCmd* val;
public:
	ListCmd(AsmCmd* _val, ListCmd* _next) : val(_val), next(_next){}
	void setNext(ListCmd *_next){ if (_next != NULL) next = _next; }
	void setPrev(ListCmd *_prev){ if (_prev != NULL) prev = _prev; }
	void print();
	AsmCmd* getVal(){ return val; }
	ListCmd* getNext(){ return next; }
	ListCmd* getPrev(){ return prev; }
};


class AsmCode{
	ListCmd* list;
public:
	map<string, string> fmt;
	AsmCode(): list(NULL){}
	void addCmd0(Cmd);
	void addCmd1(Cmd, Operands);
	void addCmd2(Cmd, Operands, Operands);
	void addCmdConst(Cmd, Scanner::Lexem);
	void addCmdVar(Cmd, string);
	void addCmdInit(Cmd, string, int);

	void print();
	void optimize();
};

class CodeGen{
	char * filename;
	AsmCode as;
public:
	CodeGen(char* _filename): filename(_filename){
		freopen(filename, "w", stdout);
	}
	AsmCode& getAsm(){return as;}
	void print(){as.print();}
	void optimize(){as.optimize();}
	~CodeGen(){ fclose(stdout);}
};


class AsmCmd{
public:
	virtual void print() = 0;
	virtual bool operator == (Cmd) = 0;
	virtual Operands getOper() = 0;
};

class AsmCmd0 : public AsmCmd{
	Cmd command;
public:
	AsmCmd0(Cmd _command): command(_command){}
	virtual bool operator == (Cmd a){return command == a;}
	virtual Operands getOper(){}
	virtual void print();
};

class AsmInit: public AsmCmd{										//TODO : int
	string var;
	Cmd command;
	int val;
public:
	AsmInit(Cmd _command, string _var, int _val): command(_command), var(_var), val(_val){}
	virtual bool operator == (Cmd a){return command == a;}
	virtual Operands getOper(){}
	virtual void print();
};

class AsmCmd1 : public AsmCmd{
	AsmOperand* oper;
	Cmd command;
	bool var;
public:
	AsmCmd1(Cmd _command, AsmOperand* _oper): command(_command), oper(_oper){}
	virtual bool operator == (Cmd a){return command == a;}
	virtual void print();
	virtual Operands getOper();
};

class AsmCmd2 : public AsmCmd{
	AsmOperand* first_oper;
	AsmOperand* sec_oper;
	Cmd command;
public:
	AsmCmd2(Cmd _command, AsmOperand* _first_oper, AsmOperand* _sec_oper) : command(_command), first_oper(_first_oper), sec_oper(_sec_oper){}
	virtual bool operator == (Cmd a){return command == a;}
	virtual Operands getOper(){}
	virtual void print();
};

class AsmOperand{
public:
		virtual void print() = 0;
		virtual bool operator == (string a)= 0;
		virtual bool operator == (Operands a) = 0;
		virtual bool operator == (Scanner :: Lexem a) = 0;
		virtual Operands getOper() = 0;
};

class AsmImmediate : public AsmOperand{
	Scanner :: Lexem Lex;
public:
	AsmImmediate(Scanner :: Lexem _Lex): Lex(_Lex){}
	virtual void print(){ if (Lex.getEnum() == C_INT) cout << Lex.getintValue(); }
	virtual bool operator == (string a){return false;}
	virtual bool operator == (Operands a){return false;}
	virtual bool operator == (Scanner :: Lexem a){return a.getintValue() == Lex.getintValue();}
	virtual Operands getOper(){}
};

class AsmReg : public AsmOperand{
	Operands operand;
public:
	AsmReg(Operands _operand): operand(_operand){}
	virtual void print();
	virtual bool operator == (string a){return false;}
	virtual bool operator == (Operands a){return operand == a;}
	virtual bool operator == (Scanner :: Lexem a){return false;}
	virtual Operands getOper(){return operand;}
};

class AsmMem : public AsmOperand{
	string temp;
public:
	AsmMem(string _temp): temp(_temp){}
	virtual void print(){cout << temp;}
	virtual Operands getOper(){}
	virtual bool operator == (string a){return temp == a;}
	virtual bool operator == (Operands a){return false;}
	virtual bool operator == (Scanner :: Lexem a){return false;}
};
