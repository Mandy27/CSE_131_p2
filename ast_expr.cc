/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */

#include <string.h>
#include "ast_expr.h"
#include "ast_type.h"
#include "ast_decl.h"


IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
    value = val;
}
void IntConstant::PrintChildren(int indentLevel) { 
    printf("%d", value);
}

Type* IntConstant::Check(SymbolTable *table) { 
    return Type::intType;
}

Type* FloatConstant::Check(SymbolTable *table) { 
    return Type::floatType;
}

Type* BoolConstant::Check(SymbolTable *table) { 
    return Type::boolType;
}

FloatConstant::FloatConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
}
void FloatConstant::PrintChildren(int indentLevel) { 
    printf("%g", value);
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
}
void BoolConstant::PrintChildren(int indentLevel) { 
    printf("%s", value ? "true" : "false");
}

VarExpr::VarExpr(yyltype loc, Identifier *ident) : Expr(loc) {
    Assert(ident != NULL);
    this->id = ident;
}

void VarExpr::PrintChildren(int indentLevel) {
    id->Print(indentLevel+1);
}

Operator::Operator(yyltype loc, const char *tok) : Node(loc) {
    Assert(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}

void Operator::PrintChildren(int indentLevel) {
    printf("%s",tokenString);
}

CompoundExpr::CompoundExpr(Expr *l, Operator *o, Expr *r) 
  : Expr(Join(l->GetLocation(), r->GetLocation())) {
    Assert(l != NULL && o != NULL && r != NULL);
    (op=o)->SetParent(this);
    (left=l)->SetParent(this); 
    (right=r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Operator *o, Expr *r) 
  : Expr(Join(o->GetLocation(), r->GetLocation())) {
    Assert(o != NULL && r != NULL);
    left = NULL; 
    (op=o)->SetParent(this);
    (right=r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Expr *l, Operator *o) 
  : Expr(Join(l->GetLocation(), o->GetLocation())) {
    Assert(l != NULL && o != NULL);
    (left=l)->SetParent(this);
    (op=o)->SetParent(this);
}

void CompoundExpr::PrintChildren(int indentLevel) {
   if (left) left->Print(indentLevel+1);
   op->Print(indentLevel+1);
   if (right) right->Print(indentLevel+1);
}
   
  
ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
    (base=b)->SetParent(this); 
    (subscript=s)->SetParent(this);
}

void ArrayAccess::PrintChildren(int indentLevel) {
    base->Print(indentLevel+1);
    subscript->Print(indentLevel+1, "(subscript) ");
  }
     
FieldAccess::FieldAccess(Expr *b, Identifier *f) 
  : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
    Assert(f != NULL); // b can be be NULL (just means no explicit base)
    base = b; 
    if (base) base->SetParent(this); 
    (field=f)->SetParent(this);
}


  void FieldAccess::PrintChildren(int indentLevel) {
    if (base) base->Print(indentLevel+1);
    field->Print(indentLevel+1);
  }

Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr*> *a) : Expr(loc)  {
    Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
    (actuals=a)->SetParentAll(this);
}

 void Call::PrintChildren(int indentLevel) {
    if (base) base->Print(indentLevel+1);
    if (field) field->Print(indentLevel+1);
    if (actuals) actuals->PrintAll(indentLevel+1, "(actuals) ");
  }

Type* EmptyExpr::Check(SymbolTable *table){
  D("in empty expr");
  return Type::voidType;
}

Type* AssignExpr::Check(SymbolTable *table){
  D("\n*****AssingExpr*********\n");
  Type* l = left->Check(table);
  Type* r = right->Check(table);

   //this->Print(1);
   if(l->IsEquivalentTo(Type::errorType)|| r->IsEquivalentTo(Type::errorType)){
     return Type::errorType;
   }
   if(l&&r)
   if(!l->IsEquivalentTo(r))
   {
     D("mismatch");
     ReportError::IncompatibleOperands(op,l,r);
     return Type::errorType;
   } 
   
   return r;
}

Type* VarExpr::Check(SymbolTable *table){
  D("\nin VarExpr::check\n");
  Decl* res = table->lookup(this->getId());
  if(res){
    VarDecl* var = dynamic_cast<VarDecl*>(res);
    if(var) return var->getType();
    //else D("in VarExpr::Check() Error");
  }
  
  ReportError::IdentifierNotDeclared(this->getIdentifier(),LookingForVariable);
  return Type::errorType;;
}
Type* EqualityCheck(Type* l,Operator *op,Type* r){
  D("\nLOGICAL EXPR\n");
  if(l->IsEquivalentTo(r) && (l->IsEquivalentTo(Type::boolType)||l->IsEquivalentTo(Type::intType)
   ||l->IsEquivalentTo(Type::floatType)||l->IsEquivalentTo(Type::vec2Type)||l->IsEquivalentTo(Type::vec3Type)
   ||l->IsEquivalentTo(Type::vec4Type)||l->IsEquivalentTo(Type::mat2Type)||l->IsEquivalentTo(Type::mat3Type)
   ||l->IsEquivalentTo(Type::mat4Type))){
    return Type::boolType;
  }else {
    ReportError::IncompatibleOperands(op,l,r);
    return Type::errorType;
  }
}

Type* LogicalCheck(Type* l,Operator *op,Type* r){
  D("\nLOGICAL EXPR\n");
  if(l->IsEquivalentTo(r) && l->IsEquivalentTo(Type::boolType)){
    return Type::boolType;
  }else {
    ReportError::IncompatibleOperands(op,l,r);
    return Type::errorType;
  }
}


Type* ArithmeticExpr::Check(SymbolTable *table){
  Type *l,*r;
  if(left){
    l = left->Check(table);
    if(l->IsEquivalentTo(Type::errorType)) 
      return Type::errorType;
  }
  if(right){
    r = right->Check(table);
    if(r->IsEquivalentTo(Type::errorType)) 
      return Type::errorType;
    }

  string oper = op->getString();
  D("\n%s: \n",oper.c_str());
  if(oper.compare("==")==0||oper.compare("!=")==0){
    D("\nEQUALITY\n");
    return EqualityCheck(l,op,r);
  }else if(oper.compare("||")==0||oper.compare("&&")==0||oper.compare("^^")==0){
    D("\nLOGICAL\n");
    return LogicalCheck(l,op,r);
  }

  D("\nARITHMETIC EXPR\n");
  if(left){
    D("\nUNARY\n");
    if(op ){
    if(l->IsEquivalentTo(r)&& (l->IsEquivalentTo(Type::intType) ||l->IsEquivalentTo(Type::floatType)
       ||l->IsEquivalentTo(Type::vec2Type)||l->IsEquivalentTo(Type::vec3Type)||l->IsEquivalentTo(Type::vec4Type))) 
      return l;
    else if((l->IsEquivalentTo(Type::floatType)&&(r->IsEquivalentTo(Type::vec2Type)))
           ||(r->IsEquivalentTo(Type::floatType)&&(l->IsEquivalentTo(Type::vec2Type))))
      return Type::vec2Type;
    else if((l->IsEquivalentTo(Type::floatType)&&(r->IsEquivalentTo(Type::vec3Type)))
           ||(r->IsEquivalentTo(Type::floatType)&&(l->IsEquivalentTo(Type::vec3Type))))
      return Type::vec3Type;
    else if((l->IsEquivalentTo(Type::floatType)&&(r->IsEquivalentTo(Type::vec4Type)))
           ||(r->IsEquivalentTo(Type::floatType)&&(l->IsEquivalentTo(Type::vec4Type))))
      return Type::vec4Type;
    else if((l->IsEquivalentTo(Type::floatType)&&(r->IsEquivalentTo(Type::mat2Type)))
           ||(r->IsEquivalentTo(Type::floatType)&&(l->IsEquivalentTo(Type::mat2Type))))
      return Type::mat2Type;
    else if((l->IsEquivalentTo(Type::floatType)&&(r->IsEquivalentTo(Type::mat3Type)))
           ||(r->IsEquivalentTo(Type::floatType)&&(l->IsEquivalentTo(Type::mat3Type))))
      return Type::mat3Type;
    else if((l->IsEquivalentTo(Type::floatType)&&(r->IsEquivalentTo(Type::mat4Type)))
           ||(r->IsEquivalentTo(Type::floatType)&&(l->IsEquivalentTo(Type::mat4Type))))
      return Type::mat4Type;
    else if((oper.compare("+")==0||oper.compare("-")==0||oper.compare("/")==0)&&l->IsEquivalentTo(r) &&
        (l->IsEquivalentTo(Type::mat2Type)||l->IsEquivalentTo(Type::mat3Type)||l->IsEquivalentTo(Type::mat4Type)))
	return l;
    else if(oper.compare("*")==0&&(((l->IsEquivalentTo(Type::vec2Type)&&r->IsEquivalentTo(Type::mat2Type)))|| 
          (l->IsEquivalentTo(Type::vec3Type)&& r->IsEquivalentTo(Type::mat3Type)) || 
	  (l->IsEquivalentTo(Type::vec4Type)&& r->IsEquivalentTo(Type::mat4Type)))) 
        return l;
    else if(oper.compare("*")==0&&((r->IsEquivalentTo(Type::vec2Type)&& l->IsEquivalentTo(Type::mat2Type))|| 
          (r->IsEquivalentTo(Type::vec3Type)&& l->IsEquivalentTo(Type::mat3Type)) || 
	  (r->IsEquivalentTo(Type::vec4Type)&& l->IsEquivalentTo(Type::mat4Type))))
	return r;
    else {
      ReportError::IncompatibleOperands(op,l,r);
      return Type::errorType;
    }
    }
  }else if(right){
    D("\nUNARYEXPR\n");
    Type *r = right->Check(table);
    if(r->IsEquivalentTo(Type::intType) ||r->IsEquivalentTo(Type::floatType))
      return r;
    else{
      ReportError::IncompatibleOperand(op,r);
      return Type::errorType;
    }
  }else{ //TODO both left and right are NULL
    D("left and right are NULL");
  }
  
  return Type::errorType;
}
Type* RelationalExpr::Check(SymbolTable *table){
  D("RELATIONALEXPR");
  Type* l = left->Check(table);
  Type* r = right->Check(table);
  if(l->IsEquivalentTo(Type::errorType)||r->IsEquivalentTo(Type::errorType))
    return Type::errorType;

  if(l->IsEquivalentTo(r) && (l->IsEquivalentTo(Type::intType) ||  l->IsEquivalentTo(Type::floatType)))
    return Type::boolType;
  else {
    ReportError::IncompatibleOperands(op,l,r);
    return Type::errorType;;
  }
}


Type* EqualityExpr::Check(SymbolTable *table){
  D("\nEQUALITY EXPR\n");
  return Type::errorType;
}
Type* PostfixExpr::Check(SymbolTable *table){
  D("\nPOSTFIXEXPR CHECK\n");
  Type* temp = left->Check(table);
  if(!temp->IsEquivalentTo(Type::intType)){
    ReportError::IncompatibleOperand(op,temp);
    return Type::errorType;
  }
  return Type::intType;
}

bool checkVec(string name, char* list){
  string newList = list;
  for(int i = 0 ; i<  name.length();i++){
    if(newList.find(name[i]) == string::npos)
      return false;
  }
  return true;
}
Type* FieldAccess::Check(SymbolTable *table){
  D("FIELDACCESS");
  Type *temp = base->Check(table);
  char v2[] = "xy", v3[] ="xyz", v4[] ="xyzw";
  char *l;
  l = v4;

  if(!(temp->IsEquivalentTo(Type::vec2Type) || temp->IsEquivalentTo(Type::vec3Type)
     ||temp->IsEquivalentTo(Type::vec4Type))){
     ReportError::InaccessibleSwizzle(field,base);
     return Type::errorType;
  }


  if (!(checkVec(field->getName(),l))){
    ReportError::InvalidSwizzle(field,base);
    return Type::errorType;
  } 
  else{
  
   if(temp->IsEquivalentTo(Type::vec2Type)) l =v2;
   if(temp->IsEquivalentTo(Type::vec3Type)) l =v3;
   if (!(checkVec(field->getName(),l))){
    ReportError::SwizzleOutOfBound(field,base);
    return Type::errorType;
   }
   else if(field->getName().length()>4){
    ReportError::OversizedVector(field,base);
    return Type::errorType;
  }
  }



  return Type::errorType;;
}

