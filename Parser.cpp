#include <map>
#include <set>
#include "Parser.hpp"

set<Condition> priorityTable[15] = {{}, {}, {}, {C_ASTERISK, C_DIV, C_MOD}, {C_MINUS, C_PLUS}, {C_SHIFTL, C_SHIFTR}, 
	{C_LESS, C_GREATER, C_LESSEQUAL, C_GREATEREQUAL}, {C_EQUAL}, {C_ANDBIN},
	{C_XOR}, {C_ORBIN}, {C_AND}, {C_OR}, {C_COMMA}};

static void IfNotBracer(Scanner :: Lexem tok){
	if(tok != C_BRACER)
		throw SyntaxError("error: expected ‘)’ ");
}

ExprNode* Parser :: parseExpr(int priority, bool brace, bool conv, Condition currentOp){
	if(priority == P_Postfix)
		return parsePostfix(brace, conv, currentOp);
	if(priority == P_Assign)
		return parseAssign(brace, conv, currentOp);
	auto temp  = priorityTable[priority];
	auto res = parseExpr(priority - 1, brace, conv, currentOp);
	if(temp.find(currentOp) != temp.end()) return res;
	while(temp.find(scan->get().getEnum()) != temp.end()){
		auto op = scan->get().getEnum();
		scan->next();
		res = new BinOpNode(op, res, parseExpr(priority, brace, conv, op));
	}
	return res;
}

ExprNode* Parser :: parseAssign(bool brace, bool conv, Condition currentOp){
	auto res = parseExpr(P_Or, brace, conv, currentOp);
	auto op = scan->get();
	if(op == C_ASSIGN || op == C_PLUSEQUAL || op == C_MINUSEQUAL 
	  || op == C_ASTERISKEQUAL || op == C_DIVEQUAL || op == C_MODEQUAL
	  || op == C_ANDBINEQUAL || op == C_ORBINEQUAL
	  || op == C_SHIFTLEQUAL || op == C_SHIFTREQUAL){
	  	if(!res->islvalue())
			throw SyntaxError("error: lvalue required as left operand of assignment");
		if(2 == res->islvalue())
			throw SyntaxError("error: assignment of read-only variable ");
		scan->next();
		res = new BinOpNode(op.getEnum(), res, parseAssign(brace, conv, op.getEnum()));
	}
	else if(op == C_TERNARYQ){
		scan->next();
		auto expif = parseExpr(P_Comma, brace, conv, currentOp);
		if(scan->get() != C_TERNARYC){
			throw SyntaxError("error: expected ‘:’ ");
			scan->next();
			res = new TernOpNode(res, expif, parseAssign(brace, conv, op.getEnum()));
		}		
	}
	return res;
}

ExprNode* Parser ::  parsePostfix(bool brace, bool conv, Condition currentOp){
	auto res = parseBrace(brace, conv, currentOp);
	auto op = scan->get();
	if(op == C_DECREMENT || op == C_INCREMENT){
		/*if(prefix)
			throw SyntaxError("error: lvalue required as increment operand", 0); */		//TODO 
		scan->next();
		res = new UnOpNode(op.getEnum(), res);
	}
	return res;
}

ExprNode* Parser :: parseBrace(bool brace, bool conv, Condition currentOp){
	auto res = parsePrefix(brace, conv, currentOp);
	auto op = scan->get();
	if(op == C_SQUAREBRACEL){
		ListStmt* list = NULL;
		while(op == C_SQUAREBRACEL){
			scan->next();
			list = addStmt(list, (StmtNode*)parseExpr(P_Comma, brace, conv, op.getEnum()));
			if(scan->get() != C_SQUAREBRACER)
				throw SyntaxError("error: expected ‘]’ ");	
			op = scan->next();
		}
		res = new ArrayNode(res, list);
	}
	else if(op == C_FIELD || op == C_FIELDPTR){
		scan->next();
		res = new BinOpNode(op.getEnum(), res, parseFactor(brace, conv, op.getEnum()));
	}
	else if(op == C_BRACEL){
		scan->next();
		bool comma = 1;
		ListStmt* list = NULL;
		while(comma){
			list = addStmt(list, (StmtNode*)parseExpr(P_Assign, brace, conv, op.getEnum()));
			if(scan->get() != C_COMMA) comma = 0;
			else scan->next();
		}
		IfNotBracer(scan->get());
		scan->next();
		res = new FuncVarNode(res, list);
	}
	return res;
}

ExprNode* Parser::parsePrefix(bool brace, bool conv, Condition currentOp){
	auto tok = scan->get();
	if ((tok == C_INCREMENT || tok == C_DECREMENT || tok == C_MINUS || tok == C_PLUS 
		|| tok == C_NOT || tok == C_NOTBIN || tok == C_ASTERISK || tok == C_ANDBIN)){
		scan->next();
		return new UnOpNode(parseFactor(brace, conv, tok.getEnum()), tok.getEnum());
	}
	return parseFactor(brace, conv, currentOp);
}

ExprNode* Parser ::  parseFactor(bool brace, bool conv, Condition currentOp){
	auto tok = scan->get();
	if(tok == C_INT){
		scan->next();
		return new ConstIntNode(tok);
	}
	if(tok == C_DOUBLE){
		scan->next();
		return new ConstDoubleNode(tok);
	}
	if(tok == C_CHAREND){
		scan->next();
		return new ConstCharNode(tok);
	}
	if(tok == C_IDENTIFIER){
		auto Sym = stack.peek();
		bool asterisk = false;
		ExprNode* res = NULL;
		scan->next();
		auto name = stack.requireIdentifier(tok.getstrValue(), true);
		if(name->isType() && brace){
			int temp = 0;
			int* brace_number = &temp;
			auto type = tok.getstrValue();
			if(scan->get() == C_ASTERISK) asterisk = true;
			auto symbol = parsePointer(stack.requireIdentifier(type, true), brace_number, true);
			IfNotBracer(scan->get());
			scan->next();
			if(Sym.Table.find(type) == Sym.Table.end() && asterisk)
				res = new ConvNode(parseExpr(P_Comma, brace, true, tok.getEnum()), (SymType*)symbol);	
			else if(Sym.Table.find(type) != Sym.Table.end())
				res = new ConvNode(parseExpr(P_Comma, brace, true, tok.getEnum()), (SymType*)symbol);
		}
		else
			res = new VarNode((SymVar*)name);
		return res;
	}
	if(tok == C_BRACEL){
		scan->next();
		auto result = parseExpr(P_Comma, true, conv, C_END);
		if(scan->get() != C_BRACER && !conv)
			throw SyntaxError("error: expected ‘)’ ", 0);
		if(scan->get() == C_BRACER) scan->next();
		return result;
	}
	if(tok == C_SIZEOF){
		scan->next();
		return new UnOpNode(parseExpr(P_Comma, brace, conv, currentOp), tok.getEnum());
	}
	if((currentOp == C_COMMA) && (scan->get() == C_END || scan->get() == C_BRACER))
		throw SyntaxError("error: expression list treated as compound expression in initializer");
	throw SyntaxError("error: expected unqualified-id before", true); 
}
