#ifndef SYMBOLTABLELOCAL
#define SYMBOLTABLELOCAL

#include <map>
#include<string>
#include "ast_decl.h"
#include "errors.h"

using namespace std;
class Decl;
class ReportError;

class SymbolTableLocal{
    protected:
        map<string, Decl*> *m;
    public:
        SymbolTableLocal(){
            m = new map<string, Decl*>();
        }
        void insert(string str, Decl* d);
        bool lookup(string str);
        void makecopy();
};

#endif
