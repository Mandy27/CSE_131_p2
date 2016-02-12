#ifndef MAP
# define MAP

#include <string>
#include <map>
//#include "ast.h"
//#include "ast_decl.h"
#include "errors.h"
#include "list.h"

using namespace std;

class Map{
    protected:
        List<map<string, Decl*>*> *list ;
    public:
        Map(){
            list = new List<map<string, Decl*>*>();
        }
        void insert(string str, Decl* d);
        bool lookup(string str);
        void makecopy();
};

#endif
