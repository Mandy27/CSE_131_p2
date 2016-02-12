#ifndef SYMBOLTABLE
# define SYMBOLTABLE

#include <vector>
#include <string>
#include "symbolTableLocal.h"

//class SymbolTableLocal;
using namespace std;

class SymbolTable{ 
    protected:
        vector<SymbolTableLocal *> *list ;
    public:
        SymbolTable(){
            list  = new vector<SymbolTableLocal*>();
        }
        void insert(SymbolTableLocal *s);
        bool lookup(string str);
};

#endif
