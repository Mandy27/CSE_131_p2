#include "Map.h"

using namespace std;

void Map::insert(string str, Decl* d){
    if(lookup(str))
      ReportError::DeclConflict(d,list->back()->find(str)->second);

    list->back()->insert(pair<string, Decl*> (d->getId(), d));
}

bool Map::lookup(string str){
  return list->back()->find(str)->second;
}

void Map::makecopy(){
    
}

