/* File: ast_decl.cc
 * -----------------
 * Implementation of Decl node classes.
 */
#include "ast_decl.h"
#include "ast_type.h"
#include "ast_stmt.h"
        
         
Decl::Decl(Identifier *n) : Node(*n->GetLocation()) {
    Assert(n != NULL);
    (id=n)->SetParent(this); 
}


VarDecl::VarDecl(Identifier *n, Type *t) : Decl(n) {
    Assert(n != NULL && t != NULL);
    (type=t)->SetParent(this);
}
  
void VarDecl::PrintChildren(int indentLevel) { 
   if (type) type->Print(indentLevel+1);
   if (id) id->Print(indentLevel+1);
}

void VarDecl::Check(SymbolTable *table){
  D("\nIn VarDecl Check\n");
}

FnDecl::FnDecl(Identifier *n, Type *r, List<VarDecl*> *d) : Decl(n) {
    Assert(n != NULL && r!= NULL && d != NULL);
    (returnType=r)->SetParent(this);
    (formals=d)->SetParentAll(this);
    body = NULL;
}

void FnDecl::SetFunctionBody(Stmt *b) { 
    (body=b)->SetParent(this);
}

void FnDecl::PrintChildren(int indentLevel) {
    if (returnType) returnType->Print(indentLevel+1, "(return type) ");
    if (id) id->Print(indentLevel+1);
    if (formals) formals->PrintAll(indentLevel+1, "(formals) ");
    if (body) body->Print(indentLevel+1, "(body) ");
}

void FnDecl::Check(SymbolTable *table){ //TODO added
    printf("\nIn FnDecl check\n");
    if (formals){
    for (int i = 0; i < formals->NumElements(); i++)
      formals->Nth(i)->Print(1, "formals"); 
      //printf("var: \n");//formals->Nth(i));
    }
    if(body){
      StmtBlock* stmtblock = (StmtBlock*)  body;
      body->Check(table);
      //stmtblock->PrintChildren(1);
      //if(!l) printf("list is null");
    //for (int i = 0; i < l->NumElements(); i++)
      //l->Nth(i)->Print(1, "formals"); 
    }
    //if (body) body->Print(1, "(body) ");
}
