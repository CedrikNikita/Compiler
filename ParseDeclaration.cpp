#include "Parser.hpp"
#include "CodeGen.hpp"

void SymStack :: push(SymTable a){
	stack->next = new Stack();
	stack->next->next = NULL;
	stack->next->prev = stack;
	stack = stack->next;
	stack->Sym = a;
}

Symbol* SymStack :: requireIdentifier(string a, bool error){
	auto tmp_stack = stack;
	while(tmp_stack != NULL){
		if(tmp_stack->Sym.Table.find(a) != tmp_stack->Sym.Table.end())
			return tmp_stack->Sym.Table[a];
		tmp_stack = tmp_stack->prev;
	}
	if(error)
		throw SyntaxError("error: \"" + a + "\" was not declarated in this scope");
	return NULL;
}

void SymStack :: notrequireIdentifier(string a){
	if(stack->Sym.Table.find(a) != stack->Sym.Table.end())
		throw SyntaxError("error: \"" + a + "\" was declarated in this scope before");
}

TypeContainer Parser :: parseStruct(bool b_typedef, bool b_const){
	auto tok = scan->next();
	SymTable newSym;
	if(tok != C_IDENTIFIER)
		throw SyntaxError("error: expected identifier");
	string name = "struct " + tok.getstrValue();
	scan->next();
	if(scan->get() == C_IDENTIFIER)
		return TypeContainer(stack.requireIdentifier(name, true), b_typedef, b_const);

	SymTable& Sym = stack.peek();
	auto StructSym = new SymStruct(name);
	stack.notrequireIdentifier(name);
	Sym.addSymbol(StructSym);
	tok = scan->get();
	if(tok == C_SEMICOLON)
		return TypeContainer(NULL, b_typedef, b_const);
	stack.push(newSym);
	scan->next();
	while(scan->get() != C_CURLYBRACER)
		parseDeclaration(parseType(false, false, true));
	scan->next();
	((SymStruct*)Sym.Table[name])->Sym = stack.peek();
	stack.pop();
	if(scan->get() == C_SEMICOLON){
		scan->next();
		return TypeContainer(NULL, b_typedef, b_const);
	}
	return TypeContainer(StructSym, b_typedef, b_const);
}

TypeContainer Parser :: parseType(bool b_typedef, bool b_const, bool b_struct){
	auto Sym = stack.peek();
	auto tok = scan->get();
	if(b_typedef && b_struct)											//TODO
		throw SyntaxError("error: 'typedef' in 'struct'");
	if(b_typedef && tok == D_typedef)
		throw SyntaxError("error: duplicate 'typedef'");
	if(tok == D_typedef && !b_typedef){;
		scan->next();
		return parseType(true, b_const, b_struct);
	}
	if(tok == D_const){
		scan->next();
		return parseType(b_typedef, true, b_struct);
	}
	if(tok == D_struct)
		return parseStruct(b_typedef, b_const);
	auto symbol = stack.requireIdentifier(tok.getstrValue(), true);
	if(symbol != NULL && symbol->isType()){
		scan->next();
		return TypeContainer(symbol, b_typedef, b_const);
	}
	if(tok == C_IDENTIFIER && b_typedef)
		throw SyntaxError("error: '" + tok.getstrValue() + "' does not name a type");
	if(tok == C_IDENTIFIER)
		throw SyntaxError("error: unknown type name ‘" + tok.getstrValue() + "'");
	throw SyntaxError("error: expected unqualified-id‘" + tok.getstrValue() + "’");
}

Symbol* Parser :: parseFunction(Symbol* type_symbol, string name, bool is_const){		//TODO : addSymbol
	auto tok = scan->next();
	bool bel = functions.find(name) != functions.end();
	List* prev_args = NULL;
	if(bel) prev_args = ((SymFunction*)stack.requireIdentifier(name, true))->getArgs();
	else stack.notrequireIdentifier(name);
	SymTable& Sym = stack.peek();
	List* arg = NULL;
	SymTable tempsym;
	stack.push(tempsym);
	while(tok == C_IDENTIFIER){
		TypeContainer cont = parseType(false, false, false);
		if (bel){
			if (prev_args == NULL || prev_args->var->getType() != cont.type->getType())
				throw SyntaxError("error: conflicting types for \"" + name + "\"");
			prev_args = prev_args->getNext();
		}
		arg = addListElem(arg, (scan->get() == C_IDENTIFIER) ? parseSimpleDeclaration(cont) : cont.type);
		if (scan->get() == C_COMMA && scan->next() != C_IDENTIFIER)
			throw SyntaxError("error: expected declaration specifiers");
		tok = scan->get();
	}
	if (prev_args != NULL)
		throw SyntaxError("error: conflicting types for \"" + name + "\"");
	stack.pop();
	if(tok != C_BRACER)
		throw SyntaxError("error: expected ‘,’ or ‘)’");
	tok = scan->next(); 
	SymFunction* func = NULL;
	if (!bel){
		func = new SymFunction(name, type_symbol, arg, NULL, is_const);
		functions.insert(name);
	}
	SymTable newsym;
	stack.push(newsym);
	if (tok == C_CURLYBRACEL){
		if (bel)func = (SymFunction*)stack.requireIdentifier(name, true);
		else Sym.addSymbol(func);
		auto temp = arg;
		while (temp != NULL){
			if (temp->var->isType())
				throw SyntaxError("error: parameter name omitted");
			temp = temp->getNext();
		}
		func->setBody(parseBlock());
		func = NULL;
	}
	else if(bel)func = (SymFunction*)stack.requireIdentifier(name, true);
	stack.pop();
	return func;
}

Symbol* Parser :: parseArray(Symbol* type_symbol, string name, bool is_const){			//TODO 
	SymTable& Sym = stack.peek();
	Symbol* type = type_symbol;
	while(scan->get() == C_SQUAREBRACEL){
		scan->next();
		auto size = parseExpr(P_Assign, false, false, C_END);
		int temp_size = size->getInt(true);
		if(temp_size < 0)
			throw SyntaxError("error: size of array ‘" + name + "’ is negative");
		if(scan->get() != C_SQUAREBRACER)
			throw SyntaxError("error: expected ']' before", 0);
		scan->next();
		type = new SymArray(type_symbol, temp_size, is_const);
	}
	return new SymArray(type_symbol, 5, is_const);
}

Symbol* Parser :: parsePointer(Symbol* type_symbol, int* brace_number, bool conv){
	SymTable& Sym = stack.peek();
	auto tok = scan->get();
	int temp = 0;
	string type = type_symbol->getName();
	while(tok == C_ASTERISK){
		bool as_const = false;
		tok = scan->next();
		while(tok == D_const || tok == C_BRACER || tok == C_BRACEL){
			if(tok == D_const) as_const = true;
			if(tok == C_BRACEL){
				if(conv)
					throw SyntaxError("error: cast specifies function type");
				temp++;
			}
			if(tok == C_BRACER){
				if(conv) break;
				temp--;
			}
			if(temp < 0)
				throw SyntaxError("error: expected identifier or ‘(’ before", true);
			tok = scan->next();
		}
		type += '*';
		if(Sym.Table.find(type) == Sym.Table.end()){
			type_symbol = new SymPointer(type, type_symbol, as_const);
			Sym.addSymbol(type_symbol);
		}
	}
	*brace_number = temp;
	return type_symbol;
}

Symbol* Parser :: parseSimpleDeclaration(TypeContainer cont){
	SymTable& Sym = stack.peek();		
	if(scan->get() == C_SEMICOLON)
		throw SyntaxError("error: declaration does not declare anything");

	int temp = 0;
	int* brace_number = &temp;
	auto type_symbol = parsePointer(cont.type, brace_number, false);		

	auto tok = scan->get();
	if(tok != C_IDENTIFIER)
		throw SyntaxError("error: expected identifier"); 
	if(Sym.Table.find(tok.getstrValue()) != Sym.Table.end() && Sym.Table[tok.getstrValue()]->isType())
		throw SyntaxError("error: expected identifier or ‘(’ before ‘" + tok.getstrValue() + "’");

	auto name = tok.getstrValue();
	Symbol* sym = NULL;
	tok = scan->next();
	if(tok == C_BRACEL)
		sym = parseFunction(type_symbol, name, cont.b_const);
	else{
		auto type_name = type_symbol->getType();
		if (type_name == "void")
			throw SyntaxError("error: storage size of \"" + type_name + "\" isn't known");
		stack.notrequireIdentifier(name);
		if(tok == C_SQUAREBRACEL)
			sym = parseArray(type_symbol, name, cont.b_const);
		else if(cont.b_typedef)
			sym = new SymAlias(name, type_symbol);
		else
			sym = new SymVar(type_symbol, name, cont.b_const);
	}

	if(scan->get() == C_ASSIGN){
		scan->next();
		((SymVar*)sym)->setInit(parseExpr(P_Assign, false, false, C_END));    //TODO : Не работает присваивание
	}
	for(int i = 0; i < *brace_number; i++){
		if(scan->get() != C_BRACER)
			throw SyntaxError("error: expected ‘)’ before", true);
		scan->next();
	}
	return sym;
}

void Parser :: parseDeclaration(TypeContainer cont){
	bool comma = true;
	SymTable& Sym = stack.peek();		
	while(comma){
		auto symbol = parseSimpleDeclaration(cont);
		if(symbol == NULL) break;
		Sym.addSymbol(symbol);
		auto tok = scan->get();
		if(tok == C_SEMICOLON)
			comma = false;
		else if(tok != C_SEMICOLON && tok != C_COMMA)
			throw SyntaxError("error: expected ‘,’ or ‘;’ before '" + tok.getstrValue() + "'");
		scan->next();
	}
}

void Parser :: parseProgramm(){
	while (scan->get() != Eof)
		parseDeclaration(parseType(false, false, false));
	if (functions.find("main") == functions.end())
		throw SyntaxError("error: no \"main\" function in the file");
	if ((stack.requireIdentifier("main", true))->getType() != "int")
		throw SyntaxError("error: function \"main\" must return int");
}

void Parser :: print(){
	parseProgramm();
	stack.peek().print(0);
}

void Parser :: generate(CodeGen& a){
	parseProgramm();
	stack.peek().generate(a.getAsm());
}