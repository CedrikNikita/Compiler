#include "Parser.hpp"

bool prefix = 0;
bool sub = 0;

ExprNode* Parser :: ParseExpr(){
	auto res = ParseOr();
	auto op = scan->get();
	if(op == C_ASSIGN || op == C_PLUSEQUAL || op == C_MINUSEQUAL 
	  || op == C_ASTERISKEQUAL || op == C_DIVEQUAL || op == C_MODEQUAL
	  || op == C_ANDBINEQUAL || op == C_ORBINEQUAL
	  || op == C_SHIFTLEQUAL || op == C_SHIFTREQUAL){
		scan->next();
		res = new BinOpNode(op.getText(), res, ParseExpr());
	}
	else if(op == C_TERNARYQ){
		scan->next();
		auto expif = ParseExpr();
		if(scan->get() != C_TERNARYC){
			throw SyntaxError("error: expected ‘:’ ", scan);
		}
		else{
			scan->next();
			res = new TernOpNode(res, expif, ParseExpr());
		}
	}
	return res;
}

ExprNode* Parser :: ParseOr(){
	auto res = ParseAnd();
	auto op = scan->get();
	if(op == C_OR){
		scan->next();
		res = new BinOpNode(op.getText(), res, ParseExpr());
	}
	return res;
}

ExprNode* Parser :: ParseAnd(){
	auto res = ParseOrbin();
	auto op = scan->get();
	if(op == C_AND){
		scan->next();
		res = new BinOpNode(op.getText(), res, ParseExpr());
	}
	return res;
}

ExprNode* Parser :: ParseOrbin(){
	auto res = ParseXor();
	auto op = scan->get();
	if(op == C_ORBIN){
		scan->next();
		res = new BinOpNode(op.getText(), res, ParseExpr());
	}
	return res;
}

ExprNode* Parser :: ParseXor(){
	auto res = ParseAndbin();
	auto op = scan->get();
	if(op == C_XOR){
		scan->next();
		res = new BinOpNode(op.getText(), res, ParseExpr());
	}
	return res;
}

ExprNode* Parser :: ParseAndbin(){
	auto res = ParseEqual();
	auto op = scan->get();
	if(op == C_ANDBIN){
		scan->next();
		res = new BinOpNode(op.getText(), res, ParseExpr());
	}
	return res;
}

ExprNode* Parser :: ParseEqual(){
	auto res = ParseCompare();
	auto op = scan->get();
	if(op == C_EQUAL | op == C_NOT){
		scan->next();
		res = new BinOpNode(op.getText(), res, ParseExpr());
	}
	return res;
}

ExprNode* Parser :: ParseCompare(){
	auto res = ParseShift();
	auto op = scan->get();
	if(op == C_LESS || op == C_GREATER || op == C_LESSEQUAL || op == C_GREATEREQUAL){
		scan->next();
		res = new BinOpNode(op.getText(), res, ParseExpr());
	}
	return res;
}

ExprNode* Parser :: ParseShift(){
	auto res = ParseSimpleop();
	auto op = scan->get();
	if(op == C_SHIFTL || op == C_SHIFTR){
		scan->next();
		res = new BinOpNode(op.getText(), res, ParseExpr());
	}
	return res;
}

ExprNode* Parser ::  ParseSimpleop(){
	auto res = ParseTerm();
	if(sub) return res;
	sub = 1;
	while(sub){
		auto op = scan->get();
		sub = 0;
		if(op == C_PLUS || op == C_MINUS){
			scan->next();
			if(op == C_MINUS) sub = 1;
			res = new BinOpNode(op.getText(), res, ParseSimpleop());
		}
	}
	return res;
}

ExprNode* Parser ::  ParseTerm(){
	auto res = ParsePostfix();
	auto op = scan->get();
	if(op == C_INCREMENT || op == C_DECREMENT)
		throw SyntaxError("error: lvalue required as increment operand", scan);
	if(op == C_ASTERISK || op == C_DIV || op == C_MOD){
		scan->next();
		res = new BinOpNode(op.getText(), res, ParseTerm());
	}
	return res;
}

ExprNode* Parser ::  ParsePostfix(){
	auto res = ParseSquarebrace();
	auto op = scan->get();
	if(op == C_DECREMENT || op == C_INCREMENT){
		if(prefix)
			throw SyntaxError("error: lvalue required as increment operand", scan);
		scan->next();
		res = new UnOpNode(op.getText(), res);
	}
	prefix = 0;
	return res;
}

ExprNode* Parser :: ParseSquarebrace(){
	auto res = ParseFactor();
	auto op = scan->get();
	if(op == C_SQUAREBRACEL){
		prefix = 0;
		scan->next();
		auto index = ParseExpr();
		if(scan->get() != C_SQUAREBRACER){
			throw SyntaxError("error: expected ‘]’ ", scan);
		}
		scan->next();
		res = new ArrayNode(res, index);	
	}
	if((op == C_FIELD || op == C_FIELDPTR)){
		scan->next();
		res = new BinOpNode(op.getText(), res, ParseFactor());
	}
	return res;
}

ExprNode* Parser ::  ParseFactor(){
	auto tok = scan->get();
	if((tok == C_INCREMENT || tok == C_DECREMENT || tok == C_MINUS || 															
		tok == C_PLUS || tok == C_NOT || tok == C_NOTBIN || tok == C_ASTERISK || tok == C_ANDBIN) && (prefix == 0)){
		prefix = 1;
		if(tok == C_MINUS || tok == C_PLUS || tok == C_NOT || tok == C_NOTBIN || tok == C_ASTERISK || tok == C_ANDBIN) prefix = 0;
		scan->next();
		return new UnOpNode(ParseFactor(), tok.getText());
	}
	else if((tok == C_INCREMENT || tok == C_DECREMENT) && (prefix == 1))
		throw SyntaxError("error: lvalue required as increment operand", scan);

	if((tok == C_INT || tok == C_INT_EXP || tok == C_INT_8 || tok == C_INT_16) && (prefix == 0)){
		scan->next();
		return new ConstNode(tok.getintValue());
	}
	else if((tok == C_DOUBLE || tok == C_DOUBLE_CHECK_EXP || tok == C_DOUBLE_EXP) && (prefix == 0)){
		scan->next();
		return new ConstNode(tok.getdoubValue());
	}
	else if(tok == C_IDENTIFIER){
		scan->next();
		return new VarNode(tok.getstrValue());
	}
	else if(tok == C_BRACEL){
		prefix = 0;
		scan->next();
		auto result = ParseExpr();
		if(scan->get() != C_BRACER)
			throw SyntaxError("error: expected ‘)’ ", scan);
		scan->next();
		return result;
	}
	else if(tok == C_SIZEOFF){
		scan->next();
		res = new UnOpNode(ParseExpr(), tok.getText());
	}
	else
		throw SyntaxError("error: wrong symbol", scan);
	return res;
}
