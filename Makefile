all:
	g++ -std=c++11 -o ccc ccc.cpp Scanner.cpp SyntaxNode.cpp Parser.cpp ParseDeclaration.cpp Symbol.cpp ParseBlock.cpp CodeGen.cpp
