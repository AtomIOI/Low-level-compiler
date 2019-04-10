/*
 * parsetree.h
 */

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <map>
using namespace std;

static map<string,int> intTable;
static map<string,string> strTable;
static int runtimeError = 0;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE };

class Value {
  int ival;
	string sval;
	NodeType type;
public:
	Value() : ival(0),type(ERRTYPE) {}
	Value(int i) : ival(i),type(INTTYPE) {}
	Value(string s) : ival(0),sval(s),type(STRTYPE) {}

	NodeType GetType() const { return type; }

	int GetIntValue() const {
		if( type != INTTYPE ){
			throw std::runtime_error("using GetIntValue on a Value that is not an INT");
		}
    return ival;
	}

	string GetStrValue() const {
 		if( type != STRTYPE ){
			throw std::runtime_error("using GetStrValue on a Value that is not a STRING");
		}
    return sval;
	}
};

class ParseTree {
public:
  int			linenum;
	ParseTree	*left;
	ParseTree	*right;

	ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~ParseTree() {
		delete left;
		delete right;
	}

	int GetLineNumber() const { return linenum; }

	int Traverse(int input, int (ParseTree::*func)(void)) {
		if( left ) input += left->Traverse(input, func);
		if( right ) input += right->Traverse(input, func);
		return Visit(input, func);
	}

	virtual NodeType GetType() const { return ERRTYPE; }

	virtual int Visit(int input, int (ParseTree::*func)(void)) {
		return input;
	}

	int LeafCount() const {
		int lc = 0;
		if( left ) lc += left->LeafCount();
		if( right ) lc += right->LeafCount();
		if( left == 0 && right == 0 )
			lc++;
		return lc;
	}

	virtual bool IsIdent() const { return false; }
	virtual bool IsVar() const { return false; }
	virtual string GetId() const { return ""; }

	int IdentCount() const {
		int cnt = 0;
		if( left ) cnt += left->IdentCount();
		if( right ) cnt += right->IdentCount();
		if( IsIdent() )
			cnt++;
		return cnt;
	}

	void GetVars(map<string,bool>& var) {
		if( left ) left->GetVars(var);
		if( right ) right->GetVars(var);
		if( IsVar() )
			var[ this->GetId() ] = true;
	}

	virtual Value Eval() = 0;
};

class StmtList : public ParseTree {

public:
	StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}

//	int Visit(int input, int (ParseTree::*func)(void)) {
//		for( auto s : statements ) {
//			input += s->Visit(input, func);
//		}
//		return input;
//	}
		NodeType GetType(){
			if((left && left->GetType() == ERRTYPE) || (right && right->GetType() == ERRTYPE)){
				//cout << "StmtList ERRTYPE" << endl;
				return ERRTYPE;
			}
			return INTTYPE;
		}

		Value Eval(){
			if(left && runtimeError == 0){
				//cout << "executes left StmtList eval" << endl;
				left->Eval();
			}
			if(right && runtimeError == 0){
				//cout << "executes right StmtList eval" << endl;
				right->Eval();
			}
			return Value();
		}
};

class VarDecl : public ParseTree {
	string id;

public:
	VarDecl(Token& t, ParseTree *ex) : ParseTree(t.GetLinenum(), ex), id(t.GetLexeme()) {}

	bool IsVar() const { return true; }
	string GetId() const { return id; }

	NodeType GetType(){
		if(left) return left->GetType();
	}

	Value Eval(){
		NodeType t= left->GetType();
    string Iden = this->GetId();
    if(t == INTTYPE){
      if(intTable.count(Iden) > 0){
        cout << this->GetLineNumber() << ": Double declaration of " << Iden << endl;
        return Value();
      }
      intTable[Iden] = left->Eval().GetIntValue();
    }else if(t == STRTYPE){
      if(strTable.count(Iden) > 0){
        cout << this->GetLineNumber() << ": Double declaration of " << Iden << endl;
        return Value();
      }
      strTable[Iden] = left->Eval().GetStrValue();
    }
    return Value();
	}
};

class Assignment : public ParseTree {
	string id;

public:
	Assignment(Token& t, ParseTree *e) : ParseTree(t.GetLinenum(), e), id(t.GetLexeme()) {}

  string GetId() const { return id; }

  NodeType GetType(){
		if(left) return left->GetType();
	}

  Value Eval(){
    NodeType t = left->GetType();
    string Iden = this->GetId();
    if(intTable.count(Iden) != 0){
      if(t != INTTYPE){
        cout << this->GetLineNumber() << ": Invalid type for assignment" << endl;
      }
      intTable[Iden] = left->Eval().GetIntValue();
    }else if(strTable.count(Iden) == 0){
      cout << this->GetLineNumber() << ": Variable not declared before use " << Iden << endl;
      return Value();
    }else{
      if(t != STRTYPE){
        cout << this->GetLineNumber() << ": Invalid type for assignment" << endl;
      }
      strTable[Iden] = left->Eval().GetStrValue();
    }
    return Value();
	}
};

class Print : public ParseTree {
public:
	Print(int line, ParseTree *e) : ParseTree(line, e) {}

	NodeType GetType(){
		if(left) return left->GetType();
		//cout << "Print ERRTYPE" << endl;
		return ERRTYPE;
	}

	Value Eval(){
		//cout << "executes print eval" << endl;
		NodeType type = left->GetType();
    NodeType Vtype = left->Eval().GetType();
    if(Vtype != ERRTYPE) {
  		if(type == STRTYPE){
        cout << left->Eval().GetStrValue() << endl;
  		}else if(type == INTTYPE){
  			cout << left->Eval().GetIntValue() << endl;
  		}
    }
    return Value();
	}
};

class Repeat : public ParseTree {

public:
	Repeat(int line, ParseTree *e, ParseTree *s) : ParseTree(line, e, s) {}

	Value Eval(){
		if(right && left){
      int repeatNum = left->Eval().GetIntValue();
      if(repeatNum < 0){
        cout << "RUNTIME ERROR: Slice expression is negative" << endl;
        runtimeError++;
      }else{
        for(int i = 0; i < repeatNum; i++){
  				right->Eval();
  			}
      }
		}
		return Value();
	}
};


class PlusExpr : public ParseTree {
public:
	PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	NodeType GetType() const {
		NodeType lt = left->GetType();
 		NodeType rt = right->GetType();

 		if( (lt == rt) && (lt != ERRTYPE) ){
			//cout << "passed if" << endl;
			return lt;
		}
		//cout << "PlusExpr ERRTYPE" << endl;
 		return ERRTYPE;
	}

	Value Eval(){
		NodeType type = left->GetType();
		if(type == STRTYPE){
			return Value(left->Eval().GetStrValue() + right->Eval().GetStrValue());
	 	}else if(type == INTTYPE){
			return Value(left->Eval().GetIntValue() + right->Eval().GetIntValue());
	 	}
		return Value();
	}
};

class MinusExpr : public ParseTree {
public:
	MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	NodeType GetType() const {
		NodeType lt = left->GetType();
 		NodeType rt = right->GetType();
 		if( lt == rt && lt != ERRTYPE )
 			return lt;
 		return ERRTYPE;
	}

	Value Eval(){
		NodeType type = left->GetType();
		if(type == INTTYPE){
			return Value(left->Eval().GetIntValue() - right->Eval().GetIntValue());
	 	}else if(type == STRTYPE){
      string original = left->Eval().GetStrValue();
      string sub = right->Eval().GetStrValue();
      string::size_type i = original.find(sub);

      if (i != std::string::npos){
        return Value(original.erase(i, sub.length()));
      }else{
        return Value(original);
      }
    }
		return Value();
	}
};

class TimesExpr : public ParseTree {
public:
	TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	NodeType GetType() const {
		NodeType lt = left->GetType();
    NodeType rt = right->GetType();
    if(lt == INTTYPE){
      if(rt == INTTYPE){
        return INTTYPE;
      }else if(rt == STRTYPE){
        return STRTYPE;
      }
    }else if(lt == STRTYPE){
      if(rt == INTTYPE){
        return STRTYPE;
      }else if(rt == STRTYPE){
        return ERRTYPE;
      }
    }
 		return ERRTYPE;
	}

	Value Eval(){
		NodeType type = left->GetType();
		if(type == INTTYPE){

		}else if(type == STRTYPE){
      int range = right->Eval().GetIntValue();
      string original = left->Eval().GetStrValue();
      string copy = original;
      for(int i= 1; i < range; i++){
        copy += original;
      }
      return Value(copy);
    }
    NodeType lt = left->GetType();
    NodeType rt = right->GetType();
    if(lt == INTTYPE){
      if(rt == INTTYPE){
        return Value(left->Eval().GetIntValue() * right->Eval().GetIntValue());
      }else if(rt == STRTYPE){
        int range = left->Eval().GetIntValue();
        string original = right->Eval().GetStrValue();
        string copy = original;
        for(int i= 1; i < range; i++){
          copy += original;
        }
        return Value(copy);
      }
    }else if(lt == STRTYPE){
      if(rt == INTTYPE){
        int range = right->Eval().GetIntValue();
        string original = left->Eval().GetStrValue();
        string copy = original;
        for(int i= 1; i < range; i++){
          copy += original;
        }
        return Value(copy);
      }else if(rt == STRTYPE){
        return Value();
      }
    }
		return Value();
	}
};

class SliceExpr : public ParseTree {
public:
	SliceExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	NodeType GetType() const{
		NodeType lt = left->GetType();
 		NodeType rt = right->GetType();
 		if(lt == STRTYPE && rt == INTTYPE)
 			return lt;
    cout << this->GetLineNumber() << ": Invalid type for slicing" << endl;
 		return ERRTYPE;
	}

	Value Eval(){
    if(this->GetType() == STRTYPE){
      string operand = left->Eval().GetStrValue();
      int begin = right->left->Eval().GetIntValue();
      unsigned int end = right->right->Eval().GetIntValue();
      if(begin >= 0 && (operand.size() - 1 >= end)) {
        return Value(operand.substr(begin, (end - begin + 1)));
      }else{
        cout << "RUNTIME ERROR: Slice expression is negative" << endl;
        runtimeError++;
      }
    }
		return Value();
	}
};

class SliceOperand : public ParseTree {
public:
	SliceOperand(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	NodeType GetType() const{
    NodeType lt = left->GetType();
    NodeType rt = right->GetType();
		if(lt == INTTYPE && lt==rt){
      return lt;
    }
		return ERRTYPE;
	}

	Value Eval(){
		return Value();
	}
};

class IConst : public ParseTree {
	int val;

public:
	IConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}

	NodeType GetType() const {
		//cout << "got type from IConst" << endl;
		return INTTYPE;
	}

	Value Eval(){
		return Value(val);
	}
};

class SConst : public ParseTree {
	string val;

public:
	SConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = t.GetLexeme();
	}

	NodeType GetType() const {
		//cout << "got type from SConst" << endl;
		return STRTYPE;
	}

	Value Eval(){
		return Value(val);
	}
};

class Ident : public ParseTree {
	string id;

public:
	Ident(Token& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}

	bool IsIdent() const { return true; }
	string GetId() const { return id; }

	NodeType GetType() const{
    string Iden = this->GetId();
		if(intTable.count(Iden) > 0){ return INTTYPE;}
    if(strTable.count(Iden) > 0){ return STRTYPE;}
		return ERRTYPE;
	}

	Value Eval(){
    string Iden = this->GetId();
		if(intTable.count(Iden) > 0){
      return Value(intTable[Iden]);
    }else if(strTable.count(Iden) > 0){
      return Value(strTable[Iden]);
    }
		return Value();
	}
};

#endif /* PARSETREE_H_ */
