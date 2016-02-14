#ifndef SYMBOLTABLELOCAL
#define SYMBOLTABLELOCAL

#include <map>
#include<string>
#include<iterator>
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
        ~SymbolTableLocal(){delete m;};
        void insert(string str, Decl* d);
        Decl* lookup(string str);
        void makecopy();
        
        void print();
};

#endif
