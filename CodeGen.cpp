#include "CodeGen.hpp"

std::ostream& operator << (std::ostream& out, const Cmd& t){
    return (out << commands[t]);
}

std::ostream& operator << (std::ostream& out, const Operands& t){
    return (out << operands[t]);
}

ListCmd* addListCmd(ListCmd* list, AsmCmd* node){
	if (list == NULL)
		list = new ListCmd(node, NULL);
	else{
		auto temp_list = list;
		while (temp_list->getNext() != NULL) temp_list = temp_list->getNext();
		temp_list->setNext(new ListCmd(node, NULL));
		temp_list->getNext()->setPrev(temp_list);
	}
	return list;
}

void AsmCode::addCmd0(Cmd cm){
	list = addListCmd(list, new AsmCmd0(cm));
}

void AsmCode::addCmd1(Cmd cm, Operands oper){
	list = addListCmd(list, new AsmCmd1(cm, new AsmReg(oper)));
}

void AsmCode::addCmdConst(Cmd cm, Scanner::Lexem Lex){
	list = addListCmd(list, new AsmCmd1(cm, new AsmImmediate(Lex)));
}

void AsmCode::addCmdVar(Cmd cm, string a){
	list = addListCmd(list, new AsmCmd1(cm , new AsmMem(a)));
}

void AsmCode::addCmdInit(Cmd cm, string a, int init){
	list = addListCmd(list, new AsmInit(cm, a, init));
}

void AsmCode::addCmd2(Cmd cm, Operands first_oper, Operands sec_oper){
	list = addListCmd(list, new AsmCmd2(cm , new AsmReg(first_oper), new AsmReg(sec_oper)));
}

void AsmCode :: print(){
	printf(".686p\n.model flat\n");
	if(!fmt.empty()){
		printf("includelib MSVCRT.lib\n");
		printf(".data\n");
		printf("extrn _printf: proc\n");
		map<string, string>::iterator it;
		for(it = fmt.begin(); it != fmt.end(); it++)
			printf("%s db '%s', 10, 0\n", (it->second).c_str(), (it->first).c_str());
	}
	else
		printf(".data\n");
	auto temp_list = list;
	while(temp_list != NULL){
		temp_list->print();
		temp_list = temp_list->getNext();
	}
	printf("ret\nmain endp\nend\n");
}

void ListCmd :: print(){
	val->print();
}

void AsmCmd0 :: print(){
	cout << command << endl;
}
void AsmInit :: print(){
	cout << var << " " << command << " " << val << endl;
}

void AsmCmd1 :: print(){
	cout << command << " ";
	oper->print();
	cout << endl;
}

void AsmCmd2 :: print(){
	cout << command << " ";
	first_oper->print(); 
	cout << ", ";
	sec_oper->print();
	cout << endl;
}

void AsmReg :: print(){
	cout << operand;
}