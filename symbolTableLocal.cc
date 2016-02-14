#include "symbolTableLocal.h"

void SymbolTableLocal::insert(string str, Decl* d){
    /*printf("\nin symbollocal inserti\n");
    if(m->find(str)->second)
    {
      printf("\nFOUND");
      cout<<m->find(str)->second;
    } else print();
    */
    if(lookup(str))
      ReportError::DeclConflict(d,m->find(str)->second);

    m->insert(pair<string, Decl*> (d->getId(), d));
}

Decl* SymbolTableLocal::lookup(string str){
  return (m->find(str)->second);
}

void SymbolTableLocal::makecopy(){
    
}

void SymbolTableLocal::print(){
  for (std::map<string,Decl*>::iterator it=m->begin(); it!=m->end(); ++it)
    cout<< it->first<<" => "<<it->second<<endl;
}
