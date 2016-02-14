#include "symbolTable.h"

void SymbolTable::insert(SymbolTableLocal *s){
    list.push_back(s);
}

void SymbolTable::insert(string str, Decl* d){
  //printf("\n size: %d\n",end());
  list[end()]->insert(str,d);
}
Decl* SymbolTable::lookup(string str){
 /// D("symboltable lookup");
  int size = list.size();
  Decl* res;
  
  for(int i = size -1;i >=0;i--){
    //D("\ni =%d", i);
    //list[i]->print();
    res = list[i]->lookup(str);
    if(res) return res;
  }
  
  return NULL; //TODO
}

int SymbolTable::end(){
  return (list.size()-1);
}

void SymbolTable::print(){
  D("\ntable number: %d\n", end());
  list[end()]->print(); 
}

void SymbolTable::newScope(){
  SymbolTableLocal *tmp = new SymbolTableLocal();
  list.push_back(tmp);
}

void SymbolTable::deleteScope(){
  SymbolTableLocal *tmp = list[end()];
  delete tmp;
  list.pop_back();
}
