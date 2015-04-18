#include "Symbol.hpp"
#include "CodeGen.hpp"

List* addListElem(List* list, Symbol* sym){
	if(list == NULL)
		list = new List(sym, NULL);
	else{
		auto temp_list = list;
		while(temp_list->getNext() != NULL) temp_list = temp_list->getNext();
		temp_list->setNext(new List(sym, NULL));
		temp_list->getNext()->setPrev(temp_list);
	}
	return list;
}

void SymTable :: addSymbol(Symbol* var){
	if (Table.find(var->getName()) != Table.end()) return;
	list = addListElem(list, var);
	Table[var->getName()] = var;
}

void SymTable :: print(int space){
	if(list == NULL) return;
	auto temp_list = list;
	while(temp_list != NULL){
		temp_list->var->print(space);
		temp_list = temp_list->getNext();
	}
}

Symbol* SymAlias :: getAlias(){
	auto temp_type = type;
	while(type->isAlias())
		temp_type = ((SymAlias*)temp_type)->getAlias();
	return temp_type;
}

string SymArray :: getType(){
	/*printf("array");
	for(int i = 0; arr[i] != 0; i++)
		printf("[%d]", arr[i]);
	printf(" of ");
	if(is_const)printf("сonst ");*/
	return type->getType();
}


void SymFunction :: print(int space){
	printf("%s", (type->getName()).c_str()); 
	if(b_const) printf(" const"); 
	printf(" %s", name.c_str());
	if(arg != NULL) printf("<- ");
	auto temp = arg;
	while(temp != NULL){
		printf("%s", temp->var->getType().c_str());
		temp = temp->getNext();
		if(temp != NULL) printf(", ");
	}
	printf("\n");
	if (body != NULL)
		body->print(space, T_None);
}

int SymAlias :: getSize(){
	return getAlias()->getSize();
}

TypeConvertion SymArray :: meet(SymType* _type, Condition op){
	return _type->meet(this, op);
}

TypeConvertion SymArray::meet(SymAlias* _type, Condition op){ return TC_None; }			//TODO

TypeConvertion SymArray:: meet(SymPointer* _type, Condition op){				//TODO
	return TC_None;
}

TypeConvertion SymArray :: meet(SymArray* _type, Condition op){
	if(op == C_MINUS)
		return TC_ArrayInt;
	if(op == C_EQUAL)
		return TC_None;
	throw SyntaxError("error: invalid operands of types ‘" + _type->getType() + " []’ and ‘" + type->getName() + " []’ to binary ‘operator");
} 

//TODO TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO

TypeConvertion SymArray :: meet(SymChar* _type, Condition op){
	return TC_None;
}

TypeConvertion SymArray :: meet(SymInt* _type, Condition op){
	return TC_None;
}

TypeConvertion SymArray :: meet(SymDouble* _type, Condition op){
	return TC_None;
}

TypeConvertion SymArray :: meet(SymStruct* _type, Condition op){
	return TC_None;
}
//TODO TODOTODOTODOTODOTODOTODOTODOTODOTODOTODO



 TypeConvertion SymInt :: meet(SymType* _type, Condition op){
 	return _type->meet(this, op);
 }

TypeConvertion SymInt :: meet(SymInt* _type, Condition op){
	return TC_None;
}

TypeConvertion SymInt :: meet(SymDouble* _type, Condition op){ 		//Для равно 
	if(op == C_PLUSEQUAL || C_MINUSEQUAL || op == C_ASTERISKEQUAL ||
	   op == C_DIVEQUAL || op ==C_ANDBINEQUAL || op == C_ORBINEQUAL || 
	   op == C_GREATEREQUAL || op == C_SHIFTREQUAL || op == C_LESSEQUAL 
	   || op == C_SHIFTLEQUAL || op == C_EQUAL || op == C_NOTEQUAL 
	   || op == C_MODEQUAL || op ==  C_XOREQUAL)
		return TC_IntDouble;
	if(op == C_OR)
		throw SyntaxError("error: invalid operands to binary | (have ‘double’ and ‘int’)");
	return TC_DoubleInt;
}

TypeConvertion SymInt :: meet(SymChar* _type, Condition op){
	if(op == C_ASSIGN) 
		return TC_IntChar;
	return TC_CharInt;
}

TypeConvertion SymInt :: meet(SymPointer* _type, Condition op){
	throw SyntaxError("error: invalid operands of types ‘" + _type->getType() + "' and ‘" + name + "’ to binary ‘operator");
}

TypeConvertion SymInt :: meet(SymAlias* _type, Condition op){						//Менять
	/*Никогда не произойдёт*/ return TC_None;
}


TypeConvertion SymInt :: meet(SymArray* _type, Condition op){
	return TC_None;
}

TypeConvertion SymInt :: meet(SymStruct* _type, Condition op){
	throw SyntaxError("error: invalid operands of types ‘struct " + _type->getType() + "' and ‘" + name + "’ to binary ‘operator");
}



TypeConvertion SymChar :: meet(SymType* _type, Condition op){
 	return _type->meet(this, op);
}

TypeConvertion SymChar :: meet(SymAlias* _type, Condition op){
	return TC_None;
}

TypeConvertion SymChar :: meet(SymPointer* _type, Condition op){
	if(op == C_PLUSEQUAL || op == C_PLUS)
		return TC_CharInt;
	throw SyntaxError("error: invalid operands of types ‘and ‘" + name + "’ to binary ‘operator");
}

TypeConvertion SymChar :: meet(SymArray* _type, Condition op){
	return TC_None;
}

TypeConvertion SymChar :: meet(SymChar* _type, Condition op){
	return TC_CharChar;
}

TypeConvertion SymChar :: meet(SymInt* _type, Condition op){
	return TC_CharInt;
}

TypeConvertion SymChar :: meet(SymDouble* _type, Condition op){
	return TC_CharDouble;
}

TypeConvertion SymChar :: meet(SymStruct* _type, Condition op){
	throw SyntaxError("error: invalid operands of type struct" + _type->getType() + "' and type '" + name + "to binary operation");
}



TypeConvertion SymDouble :: meet(SymType* _type, Condition op){
 	return _type->meet(this, op);
}

TypeConvertion SymDouble :: meet(SymAlias* _type, Condition op){
	return TC_None;
}

TypeConvertion SymDouble :: meet(SymPointer* _type, Condition op){
	throw SyntaxError("error: invalid operands of types ‘struct’ and ‘" + name + "’ to binary ‘operator");
}

TypeConvertion SymDouble :: meet(SymArray* _type, Condition op){
	return TC_None;
}

TypeConvertion SymDouble :: meet(SymChar* _type, Condition op){
	if(op == C_PLUSEQUAL || op == C_ASSIGN)
		return TC_DoubleChar;											
	return TC_CharDouble;
}

TypeConvertion SymDouble :: meet(SymInt* _type, Condition op){
	if(op == C_PLUSEQUAL || op == C_ASSIGN)
		return TC_DoubleInt;
	return TC_IntDouble;
}

TypeConvertion SymDouble :: meet(SymDouble*_type, Condition op){
	return TC_None;
}

TypeConvertion SymDouble :: meet(SymStruct*_type, Condition op){
	throw SyntaxError("error: invalid operands of types ‘struct’ and ‘" + name + "’ to binary ‘operator");
}





TypeConvertion SymPointer :: meet(SymType* _type, Condition op){
 	return _type->meet(this, op);
}

TypeConvertion SymPointer :: meet(SymAlias*_type, Condition op){
	return TC_None;
}

TypeConvertion SymPointer :: meet(SymPointer*_type, Condition op){
	if(op == C_PLUS || op == C_ASTERISK || op == C_DIV ||
	   op == C_PLUSEQUAL || op == C_ASTERISKEQUAL || op == C_DIVEQUAL)
		throw SyntaxError("error: invalid operands of types ‘" + _type->getType() + "’ and ‘" + name+ "’ to binary ‘operator’"); 		//Подумать
	return TC_PtrPtr;
}

TypeConvertion SymPointer :: meet(SymArray*_type, Condition op){
	return TC_None;
}

TypeConvertion SymPointer :: meet(SymChar*_type, Condition op){
	throw SyntaxError("error: invalid conversion from ‘" + name + "’ to ‘char’");
}

TypeConvertion SymPointer :: meet(SymInt* _type, Condition op){
	throw SyntaxError("error: invalid conversion from ‘" + name + "’ to ‘int’");									//Добавить тип ошибки
}

TypeConvertion SymPointer :: meet(SymDouble*_type, Condition op){
	throw SyntaxError("error: invalid operands of types ‘double’ and ‘" + name + "’ to binary ‘operator");
}

TypeConvertion SymPointer :: meet(SymStruct*_type, Condition op){
	throw SyntaxError("error: invalid operands of types ‘struct’ and ‘" + name + "’ to binary ‘operator");
}


TypeConvertion SymVar :: meet(SymType* _type, Condition op){
	return _type->meet((SymType*)type, op);
}



TypeConvertion SymAlias :: meet(SymType* _type, Condition op){
 	return _type->meet((SymType*)getAlias(), op);								
}

TypeConvertion SymAlias :: meet(SymAlias* _type, Condition op){
	return ((SymType*)(_type->getAlias()))->meet((SymType*)getAlias(), op);
}

TypeConvertion SymAlias :: meet(SymPointer* _type, Condition op){
	return _type->meet((SymType*)getAlias(), op);
}

TypeConvertion SymAlias :: meet(SymArray* _type, Condition op){
	return _type->meet((SymType*)getAlias(), op);
}

TypeConvertion SymAlias :: meet(SymChar* _type, Condition op){
	return _type->meet((SymType*)getAlias(), op);
}

TypeConvertion SymAlias :: meet(SymInt* _type, Condition op){
	return _type->meet((SymType*)getAlias(), op);
}

TypeConvertion SymAlias :: meet(SymDouble* _type, Condition op){
	return _type->meet((SymType*)getAlias(), op);
}

TypeConvertion SymAlias :: meet(SymStruct* _type, Condition op){
	return _type->meet((SymType*)getAlias(), op);
}




TypeConvertion SymStruct :: meet(SymType* _type, Condition op){
 	return _type->meet(this, op);
}

TypeConvertion SymStruct :: meet(SymAlias* _type, Condition op){
	throw SyntaxError("error : not implemented yet");
}

TypeConvertion SymStruct :: meet(SymPointer* _type, Condition op){
	throw SyntaxError("error: invalid operands of types ‘struct’ and ‘" + name + "’ to binary ‘operator");
}

TypeConvertion SymStruct :: meet(SymArray* _type, Condition op){
	throw SyntaxError("error : not implemented yet");
}

TypeConvertion SymStruct :: meet(SymChar* _type, Condition op){
	throw SyntaxError("error : not implemented yet");
}

TypeConvertion SymStruct :: meet(SymInt* _type, Condition op){
	throw SyntaxError("error : not implemented yet");
}
TypeConvertion SymStruct :: meet(SymDouble* _type, Condition op){
	throw SyntaxError("error : not implemented yet");
}
TypeConvertion SymStruct :: meet(SymStruct* _type, Condition op){
	throw SyntaxError("error : not implemented yet");
}


TypeConvertion SymStruct :: meet(SymVoid* _type, Condition op){
	throw SyntaxError("error: void value not ignoted as it ought to be");
}

TypeConvertion SymVoid :: meet(SymVoid* _type, Condition op){
	throw SyntaxError("error: void value not ignoted as it ought to be");
}

TypeConvertion SymChar :: meet(SymVoid* _type, Condition op){
	throw SyntaxError("error: void value not ignoted as it ought to be");
}

TypeConvertion SymInt :: meet(SymVoid* _type, Condition op){
	throw SyntaxError("error: void value not ignoted as it ought to be");
}

TypeConvertion SymDouble :: meet(SymVoid* _type, Condition op){
	throw SyntaxError("error: void value not ignoted as it ought to be");
}

TypeConvertion SymAlias :: meet(SymVoid* _type, Condition op){
	throw SyntaxError("error: void value not ignoted as it ought to be");
}

TypeConvertion SymPointer :: meet(SymVoid* _type, Condition op){
	throw SyntaxError("error: void value not ignoted as it ought to be");
}

TypeConvertion SymArray :: meet(SymVoid* _type, Condition op){
	throw SyntaxError("error: void value not ignoted as it ought to be");
}

TypeConvertion SymVoid :: meet(SymType* _type, Condition op){
 	throw SyntaxError("error: void value not ignoted as it ought to be");
}

TypeConvertion SymVoid :: meet(SymAlias* _type, Condition op){
	throw SyntaxError("error: void value not ignoted as it ought to be");
}

TypeConvertion SymVoid :: meet(SymPointer* _type, Condition op){
	throw SyntaxError("error: void value not ignoted as it ought to be");
}

TypeConvertion SymVoid :: meet(SymArray* _type, Condition op){
	throw SyntaxError("error: void value not ignoted as it ought to be");
}

TypeConvertion SymVoid :: meet(SymChar* _type, Condition op){
	throw SyntaxError("error: void value not ignoted as it ought to be");
}

TypeConvertion SymVoid :: meet(SymInt* _type, Condition op){
	throw SyntaxError("error: void value not ignoted as it ought to be");
}

TypeConvertion SymVoid :: meet(SymDouble* _type, Condition op){
	throw SyntaxError("error: void value not ignoted as it ought to be");
}

TypeConvertion SymVoid :: meet(SymStruct* _type, Condition op){
	throw SyntaxError("error: void value not ignoted as it ought to be");
}

/*
void  SymFunction :: setBody(StmtNode* _body){
	body = (StmtNode*)malloc(sizeof(BlockStmt)); 
	memcpy(body, _body, sizeof(BlockStmt));
}
*/
SymFunction :: ~SymFunction(){
	if(body != NULL) delete body;
	if(arg != NULL) delete arg;
}

SymVar :: ~SymVar(){
	if(init != NULL) delete init; 
}

void SymVar :: generate(AsmCode& a){
	if(type->getType() == "int")
		a.addCmdInit(c_dword, "_" + name, 0);
}

void SymChar::generate(AsmCode& a){

}

void SymInt::generate(AsmCode& a){

}

void SymDouble::generate(AsmCode& a){

}

void SymFunction::generate(AsmCode& a){
	a.addCmd0(c_code);
	a.addCmd0(c_main_proc);
	body->generate(a);
}

void SymStruct::generate(AsmCode& a){
}

void SymAlias::generate(AsmCode& a){
}

void SymPointer::generate(AsmCode& a){
}

void SymVoid::generate(AsmCode& a){
}

void SymArray::generate(AsmCode& a){
}

void SymTable::generate(AsmCode& a){
	auto temp_list = list;
	while(temp_list != NULL){
		temp_list->var->generate(a);
		temp_list = temp_list->getNext();
	}
}
