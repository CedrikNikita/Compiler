#include "Symbol.hpp"

void SymTable :: addVar(SymVar* var){
	if(list == NULL){
		list = new List(var, NULL);
		Table[var->getVar()] = var;
	}
	else{
		auto temp_list = list;
		while(temp_list->getNext() != NULL) temp_list = temp_list->getNext();
		temp_list->setNext(new List(var, NULL));
		temp_list->getNext()->setPrev(temp_list);
		Table[var->getVar()] = var;
	}

}

void SymTable :: print(){
	if(list == NULL) return;
	auto temp_list = list;
	while(temp_list != NULL){
		printf("%s\t%s\n", temp_list->var->getType().c_str(), temp_list->var->getVar().c_str());
		temp_list = temp_list->getNext();
	}
}