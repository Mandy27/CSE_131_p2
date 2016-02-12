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

