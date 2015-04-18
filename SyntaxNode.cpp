#include <iostream>
#include "SyntaxNode.hpp"
#include "CodeGen.hpp"

std::ostream& operator << (std::ostream& out, const Condition& t){
	if((t >= C_PLUS) && (t <= C_NOTBIN))
    	return (out << punctuator[t - 11]);
    return (out << punctuator[t - 22]);
}

static string set_space(int a){
	string str;
	for(int i = 0; i < a; i++)
		str += ' ';
	return str;
}
void ListStmt :: print(int space, TreeState state){ 
	val->print(space, state);
}
void ArrayNode :: print(int i, TreeState state){
	name->print(i + 3, T_Right); 
	if(state == T_Left) 
		cout << set_space(i - 1) + "\\" << endl;
	cout << set_space(i) + "[]" << endl; 
	if(state == T_Right)
		cout << set_space(i - 1) + "/"  << endl;
	if(list == NULL) return;
	auto temp_list = list;
	while(temp_list != NULL){
		temp_list->getVal()->print(i + 3, T_Left);
		temp_list = temp_list->getNext();
	}
}

void UnOpNode :: print(int i, TreeState state){
	if(prefix){
		cout << set_space(i) << op << endl;
		res->print(i, state);
	}
	else{
		res->print(i + 3, state);
		cout << set_space(i) << op << endl;
	}
}

void BinOpNode :: print(int i, TreeState state){
	left->print(i + 3, T_Right);
	if(state == T_Left)
		cout << set_space(i - 1) + "\\" << endl;
	cout << set_space(i) << op << endl;
	if(state == T_Right) 
		cout << set_space(i - 1) + "/"  << endl;
	right->print(i + 3, T_Left);
}

void ConstIntNode :: print(int i, TreeState state){					//TODO : Одинаковый код в параметрах разные типы
	if(state == T_Left)
		cout << set_space(i - 1) + "\\" << endl; 
	cout << set_space(i) << value.getintValue() << endl;
	if(state == T_Right)
		cout << set_space(i - 1) + "/"  << endl;
}

void ConstCharNode :: print(int i, TreeState state){
	if(state == T_Left)
		cout << set_space(i - 1) + "\\" << endl; 
	cout << set_space(i) << value.getcharValue() << endl;
	if(state == T_Right)
		cout << set_space(i - 1) + "/"  << endl;
}

void ConstDoubleNode :: print(int i, TreeState state){
	if(state == T_Left)
		cout << set_space(i - 1) + "\\" << endl; 
	cout << set_space(i) << value.getdoubValue() << endl;
	if(state == T_Right)
		cout << set_space(i - 1) + "/"  << endl;
}

void VarNode :: print(int i, TreeState state){
	if(state == T_Left) cout << set_space(i - 1) + "\\" << endl;
	cout << set_space(i) + var->getName() << endl;
	if(state == T_Right) cout << set_space(i - 1) + "/" << endl;
}

void TernOpNode :: print(int i, TreeState state){
	condition->print(i + 3, state);
	cout << endl; cout << set_space(i) + "?" << endl;
	cout << set_space(i) + "true:" << endl;
	expif->print(i + 3, state);
	cout << endl << set_space(i) + "false:" << endl;
	exprelse->print(i + 3, state); cout << endl;
}

void FuncVarNode :: print(int i, TreeState state){
	name->print(i + 3, T_Right); 
	if(state == T_Left) 
		cout << set_space(i - 1) + "\\" << endl;
	cout << set_space(i) + "()" << endl; 
	if(state == T_Right)
		cout << set_space(i - 1) + "/"  << endl;
	if(list == NULL) return;
	auto temp_list = list;
	while(temp_list != NULL){
		temp_list->getVal()->print(i + 3, T_Left);
		temp_list = temp_list->getNext();
	}
}

void ConvNode :: print(int i, TreeState state){
	if(state == T_Left) cout << set_space(i - 1) + "\\" << endl;
	else if(state == T_Right) right->print(i + 3, state);
	cout << set_space(i) + "(" + type->getType()  + ")" << endl;
	if(state == T_Right) cout << set_space(i - 1) + "/" << endl;
	else if(state == T_Left) right->print(i + 3, state);
}


void ExprStmt :: print(int i, TreeState state){
	stmt->print(i + 2, state);
}

void BlockStmt :: print(int i, TreeState state){
	if(sym != NULL)
		sym->print(i + 2);
	if(body == NULL) return;
	auto temp_list = body;
	while(temp_list != NULL){
		temp_list->print(i + 2, state);
		temp_list = temp_list->getNext();
	}
}

void WhileStmt::print(int space, TreeState state){
	cout << set_space(space) << "while" << endl; 
	cond->print(space + 2, state);
	cout << set_space(space + 2) << "{"<< endl;
	body->print(space, state);
	cout << set_space(space + 2) << "}"<< endl;
}

void DoWhileStmt::print(int space, TreeState state){
	cout << set_space(space) << "do" << endl; 
	cout << set_space(space + 2) << "{" << endl;
	body->print(space, state);
	cout << set_space(space + 2) << "}" << endl;
	cout << set_space(space) << "while" << endl;
	cond->print(space + 2, state);
}

void IfStmt::print(int space, TreeState state){
	cout << set_space(space) << "if" << endl;
	cond->print(space + 2, state);
	cout << set_space(space + 2) << "{" << endl;
	ifbody->print(space, state);
	cout << set_space(space + 2) << "}" << endl;
	if(elsebody != NULL){
		cout << set_space(space + 2) << "{" << endl;
		elsebody->print(space, state);
		cout << set_space(space + 2) << "}" << endl;
	}
}

void ForStmt::print(int space, TreeState state){
	cout << set_space(space) << "for" << endl;
	from->print(space + 2, state);
	cout << set_space(space) << "_" << endl;
	cond->print(space + 2, state);
	cout << set_space(space) << "_" << endl;
	if(operation != NULL)
		operation->print(space + 2, state);
	cout << set_space(space) << "{" << endl;
	body->print(space + 2, state);
	cout << set_space(space) << "}" << endl;
}

BlockStmt::BlockStmt(ListStmt* _body, SymTable* _sym): body(_body){
	sym = (SymTable*)malloc(sizeof(SymTable));
	memcpy(sym, _sym, sizeof(SymTable));
}

BlockStmt :: ~BlockStmt(){
	if(sym != NULL) delete sym; 
	while(body != NULL){
		auto temp = body->getNext();
		delete body;
		body = temp;
	}
}

ListStmt :: ~ListStmt(){
	delete val;
}

int VarNode :: islvalue(){				//TODO : беда
	if(var->isConst()) return 2;
	return 1;
}

SymType* VarNode :: getType(){
	return var->getSymType();
}

BinOpNode :: BinOpNode(Condition _op, ExprNode* _left, ExprNode* _right): op(_op), left(_left), right(_right){
	/*auto left_type = left->getType();
	auto right_type = right->getType();
	right_type->getType();
	auto convert = left_type->meet(right_type, op);
	if(convert == TC_IntDouble || convert == TC_CharDouble)
		type = new SymDouble();
	if(convert == TC_DoubleInt || convert == TC_CharInt)
		type = new SymInt();
	if(convert == TC_IntChar || convert == TC_DoubleChar)
		type = new SymChar();
	if(convert != TC_None){
		left = new ConvNode(left, type);
		right = new ConvNode(right, type);
	}
	if(convert == TC_None) type = right->getType();			*///ну беда
}


ConstCharNode :: ConstCharNode(Scanner :: Lexem a): value(a){
	type = new SymChar();
}

ConstDoubleNode :: ConstDoubleNode(Scanner :: Lexem a): value(a){
	type = new SymDouble();
}

ConstIntNode :: ConstIntNode(Scanner :: Lexem a): value(a){
	type = new SymInt();
}

int ConstDoubleNode :: getInt(bool rough){
	if(rough)
		throw SyntaxError("error: size of array has non-integer type");
	return (int)value.getdoubValue();

}

int VarNode :: getInt(bool rough){
	throw SyntaxError("error: variably modified array at file scope");
}

static int calculate(int left, int right, Condition op){
	int res = 0;
	switch(op){
		case C_PLUS: res = left + right; break;
		case C_MINUS: res = left - right; break;
		case C_ASTERISK: res = left * right; break;
		case C_DIV: res = left / right; break;
	}
	return res;
}

int BinOpNode :: getInt(bool rough){
	return calculate(left->getInt(rough), right->getInt(rough), op);
}

void PrintfStmt::generate(AsmCode& a){
	if (a.fmt.find(format) == a.fmt.end())
		a.fmt[format] = fmt;
	auto temp_list = list;
	int i = 1;
	while (temp_list != NULL){
		temp_list->getVal()->generate(a);
		temp_list = temp_list->getNext();
		i++;
	}
	a.addCmdVar(c_push_offset, fmt);
	a.addCmd0(c_call_printf);				//TODO
	switch (i){
	case 1:
		a.addCmd2(c_add, esp, o_4);
		break;
	case 2:
		a.addCmd2(c_add, esp, o_8);
		break;
	case 3:
		a.addCmd2(c_add, esp, o_12);
		break;
	default:
		for (int j = 0; j < i; j++)
			a.addCmd2(c_add, esp, o_4);
	}
}
void BlockStmt::generate(AsmCode& a){
	//sym->generate();
	auto temp = body;
	while (temp != NULL){
		temp->getVal()->generate(a);
		temp = temp->getNext();
	}
}

void EmptyStmt::generate(AsmCode& a){

}

void ExprStmt::generate(AsmCode& a){
	stmt->generate(a);
	a.addCmd1(c_pop, eax);
}

void WhileStmt::generate(AsmCode& a){

}

void DoWhileStmt::generate(AsmCode& a){

}

void IfStmt::generate(AsmCode& a){

}

void ForStmt::generate(AsmCode& a){

}

void VarNode::generate(AsmCode& a){
	if(var->getType() == "int")
		a.addCmdVar(c_push, "_" + var->getName());
}

void BinOpNode::generate(AsmCode& a){
	right->generate(a);
	if(op == C_ASSIGN || op == C_PLUSEQUAL || op == C_MINUSEQUAL || op == C_ASTERISKEQUAL || op == C_DIVEQUAL)
		left->generateLvalue(a);
	else
		left->generate(a);
	a.addCmd1(c_pop, eax);
	a.addCmd1(c_pop, ebx);
	switch(op){
		case C_PLUS: 
			a.addCmd2(c_add, eax, ebx); 
			a.addCmd1(c_push, eax);
			break;
		case C_ASTERISK: 
			a.addCmd1(c_imul, ebx);
			a.addCmd1(c_push, eax);
			break;
		case C_MINUS: 
			a.addCmd2(c_sub, eax, ebx);
			a.addCmd1(c_push, eax);
			break;
		case C_GREATER:
			a.addCmd2(c_xor, ecx, ecx);
			a.addCmd2(c_cmp, eax, ebx);
			a.addCmd1(c_setg, cl);
			a.addCmd1(c_push, ecx);
			break;
		case C_GREATEREQUAL:
			a.addCmd2(c_xor, ecx, ecx);
			a.addCmd2(c_cmp, eax, ebx);
			a.addCmd1(c_setge, cl);
			a.addCmd1(c_push, ecx);
			break;
		case C_LESS:
			a.addCmd2(c_xor, ecx, ecx);
			a.addCmd2(c_cmp, eax, ebx);
			a.addCmd1(c_setl, cl);
			a.addCmd1(c_push, ecx);
			break;
		case C_LESSEQUAL:
			a.addCmd2(c_xor, ecx, ecx);
			a.addCmd2(c_cmp, eax, ebx);
			a.addCmd1(c_setle, cl);
			a.addCmd1(c_push, ecx);
			break;
		case C_EQUAL:			
			a.addCmd2(c_xor, ecx, ecx);
			a.addCmd2(c_cmp, eax, ebx);
			a.addCmd1(c_sete, cl);
			a.addCmd1(c_push, ecx);
			break;
		case C_NOTEQUAL:			
			a.addCmd2(c_xor, ecx, ecx);
			a.addCmd2(c_cmp, eax, ebx);
			a.addCmd1(c_setne, cl);
			a.addCmd1(c_push, ecx);
			break;
		case C_AND:
			a.addCmd2(c_and, eax, ebx);
			a.addCmd1(c_push, eax);
			break;
		case C_OR:
			a.addCmd2(c_or, eax, ebx);
			a.addCmd1(c_push, eax);
		case C_ASSIGN:
			a.addCmd2(c_mov, eax_sq, ebx);
			a.addCmd1(c_push, eax_sq);
			break;
		case C_PLUSEQUAL:
			a.addCmd2(c_add, eax_sq, ebx);
			a.addCmd1(c_push, eax_sq);
			break;
		case C_MINUSEQUAL:
			a.addCmd2(c_sub, eax_sq, ebx);
			a.addCmd1(c_push, eax_sq);
			break;
		case C_ASTERISKEQUAL:
			a.addCmd2(c_imul, eax_sq, ebx);
			a.addCmd1(c_push, eax_sq);
			break;
		default:
			throw SyntaxError("error: can not generate, not implemented yet");
	}
}

void UnOpNode::generate(AsmCode& a){
	if(op == C_INCREMENT || op == C_DECREMENT)
		res->generateLvalue(a);
	else
		res->generate(a);
	a.addCmd1(c_pop, eax);
	switch(op){
		case C_NOTBIN:
			a.addCmd1(c_notbin, eax);
			a.addCmd1(c_push, eax);
			break;
		case C_MINUS:
			a.addCmd1(c_neg, eax);
			a.addCmd1(c_push, eax);
			break;
		case C_PLUS:
			a.addCmd1(c_push, eax);
			break;
		case C_INCREMENT:
			if(prefix) a.addCmd1(c_push, eax_sq);
			a.addCmd1(c_inc, eax_sq);
			if(!prefix) a.addCmd1(c_push, eax_sq);
		case C_DECREMENT:
			if(prefix) a.addCmd1(c_push, eax_sq);
			a.addCmd1(c_dec, eax_sq);
			if(!prefix) a.addCmd1(c_push, eax_sq);
		default:
			SyntaxError("error: can not generate, not implemented yet");
	}
}

void ConstCharNode::generate(AsmCode& a){
	//a.addCmd1(c_push,)
}
void ConstDoubleNode::generate(AsmCode& a){
}

void ConstIntNode::generate(AsmCode& a){
	a.addCmdConst(c_push_dword_ptr, value);
}

void ArrayNode::generate(AsmCode& a){
}
void TernOpNode::generate(AsmCode& a){
}
void FuncVarNode::generate(AsmCode& a){
}
void ConvNode::generate(AsmCode& a){
}



void BlockStmt::generateLvalue(AsmCode& a){
}
void EmptyStmt::generateLvalue(AsmCode& a){
}
void ExprStmt::generateLvalue(AsmCode& a){
	//stmt->generateLvalue(a);
}
void WhileStmt::generateLvalue(AsmCode& a){
}
void DoWhileStmt::generateLvalue(AsmCode& a){
}
void IfStmt::generateLvalue(AsmCode& a){
}
void ForStmt::generateLvalue(AsmCode& a){
}
void VarNode::generateLvalue(AsmCode& a){
	a.addCmdVar(c_push_offset, "_" + var->getName());
}
void BinOpNode::generateLvalue(AsmCode& a){
	/*left->generateLvalue(a);
	ght->generateLvalue(a);
	a.addCmd1(c_pop, eax);
	a.addCmd1(c_pop, ebx);
	switch(op){
		case C_PLUS: a.addCmd2(c_add, eax, ebx); break;
		case C_ASTERISK: a.addCmd1(c_imul, ebx); break;
		case C_MINUS: a.addCmd2(c_sub, eax, ebx); break;
	}
	a.addCmd1(c_push, eax);*/
}
void ConstCharNode::generateLvalue(AsmCode& a){
	//a.addCmd1(c_push,)
}
void ConstDoubleNode::generateLvalue(AsmCode& a){
}
void ConstIntNode::generateLvalue(AsmCode& a){
	//a.addCmdConst(c_push, value);
}
void ArrayNode::generateLvalue(AsmCode& a){
}
void UnOpNode::generateLvalue(AsmCode& a){
}
void TernOpNode::generateLvalue(AsmCode& a){
}
void FuncVarNode::generateLvalue(AsmCode& a){
}
void ConvNode::generateLvalue(AsmCode& a){
}