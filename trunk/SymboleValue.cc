
#include <cstdlib>
#include "SymboleValue.h"
////////////////////////////////////////////////////////////////////////////////

void SymboleValue::afficher (unsigned short indentation)
{
  Noeud::afficher (indentation);
  cout << "Feuille - Symbole value : " << getChaine () << endl;	// on affiche la chaine du symbole
}

////////////////////////////////////////////////////////////////////////////////
// Attention : cette fonction (operator) n'est pas membre de la classe SymboleValue
//ostream & operator <<(std::basic_ostream<char, std::char_traits<char> >& cout, SymboleValue * symb)
void SymboleValue::disp()
{

  cout << (Symbole)*this << "\t\t - Valeur=";
  if (defini)
  {
  	void * vald;
  	Type * toto =getValeur();
  	toto->getValeur(&vald);
  	if (toto->getType()=="<ENTIER>")
  	       cout<<"Entier:"<< *((int*) vald);
  	else if (toto->getType()=="<REEL>")
  	        cout<<"Reel:" << *((float*)vald);
  	else if (toto->getType()=="<CHAINE>")
  	        cout<<"Chaine:"<< *((string*) vald);
    cout << " ";
  }
  else
    cout << "indefinie ";
 // return cout;
}
