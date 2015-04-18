#include <iostream>
#include "Symbol.hpp"
#include "SyntaxNode.hpp"

std::ostream& operator << (std::ostream& out, const Condition& t){
	if((t >= C_PLUS) && (t <= C_NOTBIN))
    	return (out << punctuator[t - 11]);
    else return (out << punctuator[t - 22]);
}

static string Set_space(int a){
	string str;
	for(int i = 0; i < a; i++)
		str += ' ';
	return str;
}

void ArrayNode :: print(int i, TreeState state){
	name->print(i + 3, T_Right); 
	if(state == T_Left) 
		cout << Set_space(i - 1) + "\\" << endl;
	cout << Set_space(i) + "[]" << endl; 
	if(state == T_Right)
		cout << Set_space(i - 1) + "/"  << endl;
	index->print(i + 3, T_Left);
}

void UnOpNode :: print(int i, TreeState state){
	if(prefix){
		cout << Set_space(i) << op << endl;
		res->print(i, state);
	}
	else{
		res->print(i + 3, state);
		cout << Set_space(i) << op << endl;
	}
}

void BinOpNode :: print(int i, TreeState state){
	res->print(i + 3, T_Right);
	if(state == T_Left)
		cout << Set_space(i - 1) + "\\" << endl;
	cout << Set_space(i) << op << endl;
	if(state == T_Right) 
		cout << Set_space(i - 1) + "/"  << endl;
	next->print(i + 3, T_Left);
}

void iConstNode :: print(int i, TreeState state){
	if(state == T_Left)
		cout << Set_space(i - 1) + "\\" << endl; 
	cout << Set_space(i) << value << endl;
	if(state == T_Right)
		cout << Set_space(i - 1) + "/"  << endl;
}

void dConstNode :: print(int i, TreeState state){
	if(state == T_Left)
		cout << Set_space(i - 1) + "\\" << endl; 
	cout << Set_space(i) << value << endl;
	if(state == T_Right)
		cout << Set_space(i - 1) + "/"  << endl;
}

void VarNode :: print(int i, TreeState state){
	if(state == T_Left) cout << Set_space(i - 1) + "\\" << endl;
	cout << Set_space(i) + var->getVar() << endl;
	if(state == T_Right) cout << Set_space(i - 1) + "/" << endl;
}

void TernOpNode :: print(int i, TreeState state){
	condition->print(i + 3, state);
	cout << endl; cout << Set_space(i) + "?" << endl;
	cout << Set_space(i) + "true:" << endl;
	expif->print(i + 3, state);
	cout << endl << Set_space(i) + "false:" << endl;
	exprelse->print(i + 3, state); cout << endl;
}

void FuncVarNode :: print(int i, TreeState state){
	res->print(i, T_None);
	cout << Set_space(i) + "," << endl;
	next->print(i, T_None);
}

