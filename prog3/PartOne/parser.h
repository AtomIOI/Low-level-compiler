/*
 * parse.cpp
 */

#include "parse.h"
#include "Lex.h"

namespace Parser {
	bool pushed_back = false;
	Token	pushed_token;

	static Token GetNextToken(istream *in, int *line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(Token& t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;
	}
}

static int error_count = 0;

void ParseError(int line, string msg){
 cout << line << ": " << msg << endl;
 error_count++;
}

ParseTree *Prog(istream *in, int *line){
	ParseTree *sl = Slist(in, line);

	if( sl == 0 )
		ParseError(*line, "No statements in program");

	if( error_count )
		return 0;

	return sl;
}

// Slist is a Statement followed by a Statement List
ParseTree *Slist(istream *in, int *line) {
   ParseTree *s = Stmt(in, line);
   if( s == 0 )
     return 0;

   if(Parser::GetNextToken(in, line) != SC ) {
     ParseError(*line, "Missing semicolon");
     delete s;
     return 0;
   }

	 return new StmtList(s, Slist(in,line));
}

ParseTree *Stmt(istream *in, int *line) {
	Token t = Parser::GetNextToken(in, line);
	if(t == VAR){
		return VarStmt(in, line);
	}else if(t == SET){
		return SetStmt(in, line);
	}else if(t == PRINT){
		return PrintStmt(in, line);
	}else if(t == REPEAT){
		return RepeatStmt(in, line);
	}else if(t == DONE){
		return 0;
	}else{
		ParseError(*line, "Invaild key word");
		return 0;
	}
}

ParseTree *VarStmt(istream *in, int *line) {
	int l = *line;
	Token t = Parser::GetNextToken(in, line);

	if(t == IDENT){
		ParseTree *ex = Expr(in, line);
		if( ex == 0){
			ParseError(*line, "Missing expression after identifier");
			return 0;
		}
		return new VarDecl(l, ex, t.GetLexeme());
	}else{
		ParseError(*line, "Missing identifier after var");
		return 0;
	}
}

ParseTree *SetStmt(istream *in, int *line) {
	int l = *line;
	Token t = Parser::GetNextToken(in, line);

	if(t == IDENT){
		ParseTree *ex = Expr(in, line);
		if(ex == 0){
			ParseError(*line, "Missing expression after identifier");
			return 0;
		}
			return new Assignment(l, ex, t.GetLexeme());
	}else{
		ParseError(*line, "Missing identifier after set");
		return 0;
	}
}

ParseTree *PrintStmt(istream *in, int *line) {
	int l = *line;

	ParseTree *ex = Expr(in, line);
	if( ex == 0 ) {
		ParseError(*line, "Missing expression after print");
		return 0;
	}
	return new Print(l, ex);
}

ParseTree *RepeatStmt(istream *in, int *line) {
	ParseTree *ex = Expr(in, line);

	if(ex == 0){
		ParseError(*line, "Missing expression after repeat");
		return 0;
	}
	ParseTree *s = Stmt(in, line);

	if(s == 0){
		ParseError(*line, "Missing statement after expression");
		return 0;
	}
	return new Repeat(*line, ex, s);
}

ParseTree *Expr(istream *in, int *line) {
	ParseTree *t1 = Term(in, line);
	if( t1 == 0 ) {
		return 0;
	}

	while ( true ) {
		Token t = Parser::GetNextToken(in, line);

		if( t != PLUS && t != MINUS ) {
			Parser::PushBackToken(t);
			return t1;
		}

		ParseTree *t2 = Term(in, line);
		if( t2 == 0 ) {
			ParseError(*line, "Missing expression after operator");
			return 0;
		}

		if( t == PLUS )
			t1 = new PlusExpr(t.GetLinenum(), t1, t2);
		else
			t1 = new MinusExpr(t.GetLinenum(), t1, t2);
	}
}

ParseTree *Term(istream *in, int *line) {
	ParseTree *f1 = Factor(in, line);
	if( f1 == 0 ) {
		return 0;
	}

	while ( true ) {
		Token t = Parser::GetNextToken(in, line);

		if( t != STAR) {
			Parser::PushBackToken(t);
			return f1;
		}

		ParseTree *f2 = Factor(in, line);
		if( f2 == 0 ) {
			ParseError(*line, "Missing expression after operator");
			return 0;
		}
		f1 = new TimesExpr(t.GetLinenum(), f1, f2);
	}
}

ParseTree *Factor(istream *in, int *line) {
	int l = *line;
	ParseTree *p1 = Primary(in, line);
	if(p1 == 0){
		return 0;
	}

	Token t = Parser::GetNextToken(in, line);
	if(t == LSQ){
		ParseTree *lexpr = Expr(in, line);
		if (lexpr == 0){
			ParseError(*line, "Missing expression after left bracket");
			return 0;
		}

		t = Parser::GetNextToken(in, line);
		if(t != COLON){
			ParseError(*line, "Missing colon after expression");
			return 0;
		}

		ParseTree *rexpr = Expr(in, line);
		if (rexpr == 0){
			ParseError(*line, "Missing expression after left bracket");
			return 0;
		}

		t = Parser::GetNextToken(in, line);
		if(t != RSQ){
			ParseError(*line, "Missing right bracket");
			return 0;
		}
		p1 = new SliceOperand(l, lexpr, rexpr); //fill in after creating in parsetree.h
	}else{
		Parser::PushBackToken(t);
	}
	return p1;
}

ParseTree *Primary(istream *in, int *line) {
	Token t = Parser::GetNextToken(in, line);
	if(t == IDENT){
		return new Ident(t);
	}else if(t == ICONST){
		return new IConst(t);
	}else if(t == SCONST){
		return new SConst(t);
	}else if(t == LPAREN){
		ParseTree *ex = Expr(in, line);
		if(ex == 0){
			ParseError(*line, "Missing expression after left paren");
			return 0;
		}
		Token t = Parser::GetNextToken(in, line);
		if(t != RPAREN){
			ParseError(*line, "Missing right paren after expression");
			return 0;
		}
		return ex;
	}else{
		ParseError(*line, "Missing Primary");
		return 0;
	}
}
