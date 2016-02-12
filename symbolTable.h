#ifndef SYMBOLTABLE
# define SYMBOLTABLE
#include <string>
#include <map>
#include "ast_decl.h"

using namespace std;

class SymbolTable :
{
    protected:
        List<map<string, Decl*>*> *list ;
    public:
        SymbolTable(){
            list = new List<map<string, Decl*>*>();
        }
        void insert(string str, Decl* d);
        void lookup();
        void makecopy();
};

#endif