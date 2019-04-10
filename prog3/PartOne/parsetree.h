/*
 * parsetree.h
 */

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <map>
using std::vector;
using std::map;
map<string, int> uniqueIden;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE };

class ParseTree {
	int			linenum;
	ParseTree	*left;
	ParseTree	*right;

public:
	ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~ParseTree() {
		delete left;
		delete right;
	}

	int LeafCount() {
		if(this == 0){
			return 0;
		}
		if(left == 0 && right == 0){
			return 1;
		}else{
			return left->LeafCount() + right->LeafCount();
		}
	}

	int IdenCount() {
		if(this == 0){
			return 0;
		}
		if((left == 0 && right == 0) && (this->GetType() == STRTYPE)){
			uniqueIden[this->GetVal()] += 1;
			return 1;
		}else{
			return left->IdenCount() + right->IdenCount();
		}
	}

	int GetLineNumber() const { return linenum; }
	virtual string GetVal() const {return 0;}
	virtual NodeType GetType() const { return ERRTYPE; }
};

class StmtList : public ParseTree {
	public:
		StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
};

class VarDecl : public ParseTree {
	string val;

	public:
		VarDecl(int line, ParseTree *l, string iden) : ParseTree(line, l) {
			val = iden;
		}
};

class Assignment : public ParseTree {
	string val;

	public:
		Assignment(int line, ParseTree *l, string iden) : ParseTree(line, l) {
			val = iden;
		}
};

class Print : public ParseTree {
	string val = "print";

	public:
		Print(int line, ParseTree *l) : ParseTree(line, l) {}
};

class Repeat : public ParseTree {
	string val = "repeat";

	public:
		Repeat(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class PlusExpr : public ParseTree {
	string val = "+";

	public:
		PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class MinusExpr : public ParseTree {
	string val = "-";

	public:
		MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class TimesExpr : public ParseTree {
	string val = "*";

	public:
		TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class SliceExpr : public ParseTree {};

class SliceOperand : public ParseTree {
	string val = "test";

	public:
		SliceOperand(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class IConst : public ParseTree {
	int val;

	public:
		IConst(Token& t) : ParseTree(t.GetLinenum()) {
			val = stoi(t.GetLexeme());
		}
		NodeType GetType() const { return INTTYPE; }
};

class SConst : public ParseTree {
	string val;

	public:
		SConst(Token& t) : ParseTree(t.GetLinenum()) {
			val = t.GetLexeme();
		}
		string GetVal() const {return val;}
};
class Ident : public ParseTree {
	string val;

	public:
		Ident(Token& t) : ParseTree(t.GetLinenum()) {
			val = t.GetLexeme();
		}
		NodeType GetType() const { return STRTYPE; }
		string GetVal() const { return val; }
};


#endif /* PARSETREE_H_ */
