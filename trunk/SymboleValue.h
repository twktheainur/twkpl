#ifndef SYMBOLEVALUE_H_
#define SYMBOLEVALUE_H_

#include <cstring>
#include <iostream>
#include <typeinfo>
using namespace std;
#include "Symbole.h"
#include "Arbre.h"

class SymboleValue:public Noeud,public Symbole
{
public:
  SymboleValue(Symbole s,Type * val=NULL)
  :Symbole(s.getChaine ())
  {
  	//if(val==NULL)
   //   defini = false;
  	//else
  		defini=false;
    valeur=val;
  };
  inline string getType()
  {
  	if(valeur)
  	  return valeur->getType();
  	else
      throw Exception();
  }

  inline Type * getValeur()
  {
    return valeur;
  }
  inline void setValeur(Type * val)
  {
    valeur=val;
    defini=true;
  }

  void afficher(unsigned short indentation);
  inline bool estDefini ()
  {
    return defini;
  }
  //friend ostream & operator <<(std::basic_ostream<char, std::char_traits<char> >& cout, SymboleValue symb);
  void disp();
private:
  bool defini;  	// indique si la valeur du symbole est définie
  Type * valeur;


};

#endif /* SYMBOLEVALUE_H */
