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
        ~SymbolTable(){}
        void deleteScope();
        void insert(SymbolTableLocal *s);
        void insert(string str, Decl *d);
        void newScope();
        Decl* lookup(string str);
        int end();
        
        void print();
};

#endif
