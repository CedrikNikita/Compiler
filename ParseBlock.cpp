#include "Parser.hpp"

static void throwIfNotBracel(Scanner* scan){
	if(scan->get() != C_BRACEL)
		throw SyntaxError("error: expected ‘(’ before ", true);
	scan->next();
}

static void throwIfNotBracer(Scanner* scan){
	if(scan->get() != C_BRACER)
		throw SyntaxError("error: expected ‘)’ before ", true);
	scan->next();
}

StmtNode* Parser :: parseWhile(){
	scan->next();
	throwIfNotBracel(scan);
	auto cond = parseExpr(P_Comma, false, false, C_END);
	throwIfNotBracer(scan);
	return new WhileStmt(cond, parseStmt());
}

StmtNode* Parser :: parseDoWhile(){
	scan->next();
	StmtNode* body = parseStmt();
	if(scan->get() != B_while)
		throw SyntaxError("error: expected ‘while’ before ", true);
	scan->next();
	throwIfNotBracel(scan);
	auto cond = parseExpr(P_Comma, false, false, C_END);
	throwIfNotBracer(scan);
	return new DoWhileStmt(cond, body);
}

StmtNode* Parser :: parseFor(){
	scan->next();
	throwIfNotBracel(scan);
	auto from = (ExprStmt*)parseExprStmt();
	auto cond = (ExprStmt*)parseExprStmt();
	ExprNode* operation = NULL;
	if(scan->get() != C_BRACER)
		operation = parseExpr(P_Comma, false, false, C_END);
	throwIfNotBracer(scan);
	if(scan->get() == C_CURLYBRACER)
		throw SyntaxError("error: expected expression before ‘}’ token");
	auto body = parseStmt();
	return new ForStmt(from->stmt, cond->stmt, operation, body);
}

StmtNode* Parser :: parseIf(){
	auto tok = scan->next();
	throwIfNotBracel(scan);
	auto cond = parseExpr(P_Comma, false, false, C_END);
	throwIfNotBracer(scan);
	StmtNode* body = parseStmt();
	if(scan->get() == B_else){
		scan->next();
		return new IfStmt(cond, body, parseStmt());
	}
	return new IfStmt(cond, body);
}

StmtNode* Parser :: parseExprStmt(){
	if(scan->get() == C_SEMICOLON){
		scan->next();
		return new EmptyStmt();
	}
	auto res = parseExpr(P_Comma, false, false, C_END);
	if(scan->get() != C_SEMICOLON)
		throw SyntaxError("error: expected ‘;’ before ", true);
	scan->next();
	return new ExprStmt(res);
}

ListStmt* addStmt_rev(ListStmt* list, StmtNode* node){
	if(list == NULL){
		list = new ListStmt(node, NULL);
		list->setPrev(NULL);
	}
	else{
		auto temp_list = list;
		while(temp_list->getPrev() != NULL) temp_list = temp_list->getPrev();
		temp_list->setPrev(new ListStmt(node, NULL));
		temp_list->getPrev()->setPrev(temp_list);
		list = temp_list;
	}
	return list;
}

StmtNode* Parser :: parsePrintf(){
	auto tok = scan->next();
	throwIfNotBracel(scan);
	auto str = scan->get().getstrValue();
	auto format = str;
	string fmt = "fmt";
	int size = 0;
	string :: size_type n = str.find("%d");
	while(n != string :: npos){
		str = str.substr(n + 2);
		n = str.find("%d");
		fmt += "d";
		size++;
	}
	scan->next();
	ListStmt* list = NULL;
	for(int i = 0; i < size; i++){
		if(scan->get() != C_COMMA)
			throw SyntaxError("error: expected ',' before", true);
		scan->next();
		list = addStmt_rev(list, (StmtNode*)parseExpr(P_Comma, false, false, C_END));
	}
	throwIfNotBracer(scan);
	return new PrintfStmt(list, format, fmt);
}

StmtNode* Parser :: parseStmt(){
	auto tok = scan->get();
	if(tok == C_SEMICOLON){
		scan->next();
		return new EmptyStmt();
	}
	StmtNode* res = NULL;
	SymTable newsym;
	stack.push(newsym);
	if(tok == C_CURLYBRACEL) res = parseBlock();		
	else if(tok == B_if) res = parseIf();				//TODO : сравнение со строкой
	else if(tok == B_while) res = parseWhile();	
	else if(tok == B_for) res = parseFor();
	else if(tok == B_do) res = parseDoWhile();
	else if(tok == B_printf) res = parsePrintf();
	stack.pop();
	/*if(tok == B_return) return parseReturn(); //Не уверен
	if(tok == B_break) return parseBreak();
	if(tok == B_continue) return parseContinue();*/
	if(res != NULL) return res;
	return parseExprStmt();
}

ListStmt* addStmt(ListStmt* list, StmtNode* node){
	if(list == NULL)
		list = new ListStmt(node, NULL);
	else{
		auto temp_list = list;
		while(temp_list->getNext() != NULL) temp_list = temp_list->getNext();
		temp_list->setNext(new ListStmt(node, NULL));
		temp_list->getNext()->setPrev(temp_list);
	}
	return list;
}

StmtNode* Parser :: parseBlock(){
	bool check = false;
	ListStmt* body = NULL;
	auto tok = scan->next();
	while(tok != C_CURLYBRACER){
		Symbol* symbol = tok == C_IDENTIFIER ? stack.requireIdentifier(tok.getstrValue(), false) : NULL;
		if (tok == D_const || tok == D_typedef || tok == D_struct || (symbol != NULL && symbol->isType())){
			parseDeclaration(parseType(false, false, false));
		}
		else{
			check = true;
			body = addStmt(body, parseStmt());
		}
		tok = scan->get();
	}
	scan->next();
	if(!check) body = NULL;
	SymTable* sym = &stack.peek();
	return new BlockStmt(body, sym);
}