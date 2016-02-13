#ifndef SYMBOLTABLE
# define SYMBOLTABLE

#include <vector>
#include <string>
#include "symbolTableLocal.h"

//class SymbolTableLocal;
using namespace std;

class SymbolTable{ 
   protected:
        vector<SymbolTableLocal *> list ;
   public:
        SymbolTable(){
            //list  = new vector<SymbolTableLocal*>();
            SymbolTableLocal* temp = new SymbolTableLocal();
            this->list.push_back(temp);
        }
        void insert(SymbolTableLocal *s);
        void insert(string str, Decl *d);
        void newScope();
        bool lookup(string str);
        int end();
        
        void print();
};

#endif
