#include "symbolTableLocal.h"

void SymbolTableLocal::insert(string str, Decl* d){
    if(lookup(str))
      ReportError::DeclConflict(d,m->find(str)->second);

    m->insert(pair<string, Decl*> (d->getId(), d));
}

bool SymbolTableLocal::lookup(string str){
  return m->find(str)->second;
}

void SymbolTableLocal::makecopy(){
    
}

void SymbolTableLocal::print(){
  for (std::map<string,Decl*>::iterator it=m->begin(); it!=m->end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';
}
