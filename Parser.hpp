#include "Scanner.hpp"

static string Set_cap(int pos){
	string str;
	for(int i = 0; i < pos; i++)
		str += ' ';
	str += '^';
	return str;
}

static string Set_space(int a){
	string str;
	for(int i = 0; i < a; i++)
		str += ' ';
	return str;
}

class SyntaxNode{
	public:
};

class ExprNode : public SyntaxNode{
	public:
		int temp;
		virtual void print(int) = 0;
};

class Parser{
	public:
		ExprNode* res;
		Scanner* scan;
		Parser(Scanner* a){scan = a;}
		ExprNode* ParseFactor();
		ExprNode* ParseTerm();
		ExprNode* ParseExpr();
		ExprNode* ParsePostfix();
		ExprNode* ParseSquarebrace();
		ExprNode* ParseSimpleop();
		ExprNode* ParseShift();
		ExprNode* ParseCompare();
		ExprNode* ParseEqual();
		ExprNode* ParseAnd();
		ExprNode* ParseOr();
		ExprNode* ParseOrbin();
		ExprNode* ParseAndbin();
		ExprNode* ParseXor();
		void print(){res = ParseExpr(); res->print(0);}
};

class VarNode : public ExprNode{
	public:
		string value;
		VarNode(string a){value = a;}
		virtual void print(int i){cout << Set_space(i) + value;}
};

class ConstNode : public ExprNode{
	public:
		bool Int;
		int ivalue;
		double dvalue;
		ConstNode(int a){ivalue = a; Int = 1;}
		ConstNode(double a){dvalue = a; Int = 0;}
		virtual void print(int i){if(Int) cout << Set_space(i) << ivalue; else cout << Set_space(i) << dvalue;}
};

class BinOpNode : public ExprNode{
	public:
		string op;
		ExprNode *res, *next;
		BinOpNode(string _op, ExprNode* _res, ExprNode* _next){op = _op; res = _res; next = _next;}
		virtual void print(int i){res->print(i + 3); cout << endl; cout << Set_space(i) + op << endl; next->print(i + 3); cout << endl;}
};

class UnOpNode : public ExprNode{
	public:
		string op;
		ExprNode *res;
		bool prefix;
		UnOpNode(string _op, ExprNode* _res){op = _op; res = _res; prefix = 0;}
		UnOpNode(ExprNode* _res, string _op){op = _op; res = _res; prefix = 1;}
		virtual void print(int i){if(prefix){cout << Set_space(i + 3) + op; res->print(1); cout << endl;}
							 else {res->print(i + 3); cout << Set_space(i + 3) + op << endl;}}
};

class ArrayNode : public ExprNode{
	public:
		ExprNode *name, *index;
		ArrayNode(ExprNode* _name, ExprNode* _index){name = _name; index = _index;}
		virtual void print(int i){name->print(i + 3); cout << endl; cout << Set_space(i) + "[]" << endl; index->print(i + 3); cout << endl;}
};

class TernOpNode: public ExprNode{
	public:
		ExprNode *condition, *expif, *exprelse;
		TernOpNode(ExprNode* _condition, ExprNode* _expif, ExprNode* _exprelse){condition = _condition; expif = _expif; exprelse = _exprelse;}
		virtual void print(int i){condition->print(i + 3); cout << endl; cout << Set_space(i) + "?" << endl; cout << Set_space(i) + "true:" << endl;
		 expif->print(i + 3); cout << endl << Set_space(i) + "false:" << endl; exprelse->print(i + 3); cout << endl;}
};

class SyntaxError{
	public:
	SyntaxError(string str, Scanner* scan){cout << scan->filename << ":" << scan->get().getX() << ":" << scan->get().getY() + scan->get().getText().size() << ' ' + str << endl 
				<< '\t' + scan->get().getText() << endl
				<< '\t' + Set_cap(scan->get().getText().size()) << endl;}
};