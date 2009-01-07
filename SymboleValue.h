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
			defini=false;
			valeur=val;
		};
		//Returns the type of the value
		inline string getType()
		{
			if(valeur)
				return valeur->getType();
			else
				throw Exception();
		}

		inline Type * getValeur()//get the value
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
		inline void definir()
		{
			defini=true;
		}
		void disp();
	private:
		bool defini;  	// indique si la valeur du symbole est définie
		Type * valeur;


};

#endif /* SYMBOLEVALUE_H */
