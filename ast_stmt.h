/* File: ast_stmt.h
 * ----------------
 * The Stmt class and its subclasses are used to represent
 * statements in the parse tree.  For each statment in the
 * language (for, if, return, etc.) there is a corresponding
 * node class for that construct. 
 *
  * pp3: You will need to extend the Stmt classes to implement
  * semantic analysis for rules pertaining to statements.
  */


#ifndef _H_ast_stmt
#define _H_ast_stmt

#include "list.h"
#include "ast.h"
#include "symbolTable.h"

class Decl;
class VarDecl;
class Expr;
class IntConstant;
  
void yyerror(const char *msg);

class Program : public Node
{
  protected:
     List<Decl*> *decls;
     
  public:
     Program(List<Decl*> *declList);
     const char *GetPrintNameForNode() { return "Program"; }
     void PrintChildren(int indentLevel);
     void Check();
     //SymbolTable *table;
};

class Stmt : public Node
{
  public:
     Stmt() : Node() {}
     Stmt(yyltype loc) : Node(loc) {}
     virtual Type* Check(SymbolTable *table) =0;
};

class StmtBlock : public Stmt 
{
  protected:
    List<VarDecl*> *decls;
  public: //TODO added
    List<Stmt*> *stmts;
    
  public:
    StmtBlock(List<VarDecl*> *variableDeclarations, List<Stmt*> *statements);
    const char *GetPrintNameForNode() { return "StmtBlock"; }
    void PrintChildren(int indentLevel);
    Type* Check(SymbolTable *table);
    void Check(SymbolTable *table,List<VarDecl*>* formals);
};

class DeclStmt: public Stmt 
{
  protected:
    Decl* decl;
    
  public:
    DeclStmt(Decl *d);
    const char *GetPrintNameForNode() { return "DeclStmt"; }
    void PrintChildren(int indentLevel);
    Type* Check(SymbolTable *table);
};
  
class ConditionalStmt : public Stmt
{
  protected:
    Expr *test;
    Stmt *body;
  
  public:
    ConditionalStmt() : Stmt(), test(NULL), body(NULL) {}
    ConditionalStmt(Expr *testExpr, Stmt *body);
    Expr* getExpr(){return test;}
};

class LoopStmt : public ConditionalStmt 
{
  public:
    LoopStmt(Expr *testExpr, Stmt *body)
            : ConditionalStmt(testExpr, body) {}
};

class ForStmt : public LoopStmt 
{
  protected:
    Expr *init, *step;
  
  public:
    ForStmt(Expr *init, Expr *test, Expr *step, Stmt *body);
    const char *GetPrintNameForNode() { return "ForStmt"; }
    void PrintChildren(int indentLevel);
    Type* Check(SymbolTable *table);
};

class WhileStmt : public LoopStmt 
{
  public:
    WhileStmt(Expr *test, Stmt *body) : LoopStmt(test, body) {}
    const char *GetPrintNameForNode() { return "WhileStmt"; }
    void PrintChildren(int indentLevel);
    Type* Check(SymbolTable *table);
};

class IfStmt : public ConditionalStmt 
{
  protected:
    Stmt *elseBody;
  
  public:
    IfStmt() : ConditionalStmt(), elseBody(NULL) {}
    IfStmt(Expr *test, Stmt *thenBody, Stmt *elseBody);
    const char *GetPrintNameForNode() { return "IfStmt"; }
    void PrintChildren(int indentLevel);
    Type* Check(SymbolTable *table);
};

class IfStmtExprError : public IfStmt
{
  public:
    IfStmtExprError() : IfStmt() { yyerror(this->GetPrintNameForNode()); }
    const char *GetPrintNameForNode() { return "IfStmtExprError"; }
};

class BreakStmt : public Stmt 
{
  public:
    BreakStmt(yyltype loc) : Stmt(loc) {}
    const char *GetPrintNameForNode() { return "BreakStmt"; }
    Type* Check(SymbolTable *table);
};

class ContinueStmt : public Stmt 
{
  public:
    ContinueStmt(yyltype loc) : Stmt(loc) {}
    const char *GetPrintNameForNode() { return "ContinueStmt"; }
    Type* Check(SymbolTable *table);
};

class ReturnStmt : public Stmt  
{
  protected:
    Expr *expr;
  
  public:
    ReturnStmt(yyltype loc, Expr *expr = NULL);
    const char *GetPrintNameForNode() { return "ReturnStmt"; }
    void PrintChildren(int indentLevel);

    Type* Check(SymbolTable* table);
    Expr *getExpr(){return expr;}
};

class SwitchLabel : public Stmt
{
  protected:
    Expr     *label;
    Stmt     *stmt;

  public:
    SwitchLabel() { label = NULL; stmt = NULL; }
    SwitchLabel(Expr *label, Stmt *stmt);
    SwitchLabel(Stmt *stmt);
    void PrintChildren(int indentLevel);
    Stmt* getStmt(){return stmt;}
};

class Case : public SwitchLabel
{
  public:
    Case() : SwitchLabel() {}
    Case(Expr *label, Stmt *stmt) : SwitchLabel(label, stmt) {}
    const char *GetPrintNameForNode() { return "Case"; }
    Type* Check(SymbolTable *table);
};

class Default : public SwitchLabel
{
  public:
    Default(Stmt *stmt) : SwitchLabel(stmt) {}
    const char *GetPrintNameForNode() { return "Default"; }
    Type* Check(SymbolTable *table);
};

class SwitchStmt : public Stmt
{
  protected:
    Expr *expr;
    List<Stmt*> *cases;
    Default *def;

  public:
    SwitchStmt() : expr(NULL), cases(NULL), def(NULL) {}
    SwitchStmt(Expr *expr, List<Stmt*> *cases, Default *def);
    virtual const char *GetPrintNameForNode() { return "SwitchStmt"; }
    void PrintChildren(int indentLevel);
    Type* Check(SymbolTable *table);
};

class SwitchStmtError : public SwitchStmt
{
  public:
    SwitchStmtError(const char * msg) { yyerror(msg); }
    const char *GetPrintNameForNode() { return "SwitchStmtError"; }
};

#endif
