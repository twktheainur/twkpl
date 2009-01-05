#include "TableSymboles.h"
#include "Exception.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
TableSymboles::TableSymboles ():table ()
// Construit une table des symboles valués vide
{
}

////////////////////////////////////////////////////////////////////////////////
SymboleValue *
TableSymboles::chercheAjoute (Symbole  s,string vtype)
// si s est identique à un symbole valué déjà présent dans la table,
// on renvoie un pointeur sur ce symbole valué.
// Sinon, on insère un nouveau symbole valué correspondant à s
// et on renvoie un pointeur sur le nouveau symbole valué inséré.
{
  vector < SymboleValue * >::iterator i;
  Type * svv;
  SymboleValue * sv;
  i = table.begin ();
  while (i < table.end () && (**i).getChaine () < s.getChaine ())
    i++;
  if (i == table.end () || (**i).getChaine () != s.getChaine ())// si pas trouvé...
  {
    if(s=="<ENTIER>")
    {
    	int val;
      istringstream str(s.getChaine());
      str>>val;
      svv=new Entier();
      svv->setValeur(&val);
//      cout <<"int"<<endl;
    }
    else if(s=="<CHAINE>")
    {
    	string val(s.getChaine());
      svv=new Chaine();
      svv->setValeur(&val);
//      cout <<"str"<<endl;
    }
    else if(s=="<REEL>")
    {
        float val;
        istringstream str(s.getChaine());
        str>>val;
        svv=new Reel();
        svv->setValeur(&val);
//        cout <<"float"<<endl;
    }
    else if(s=="<VARIABLE>")
    {
    	if(vtype=="Entier")
    		svv = new Entier();
    	else if(vtype=="Reel")
    		svv = new Reel();
    	else if(vtype=="Chaine")
    		svv=new Chaine();
    }
    sv = new SymboleValue(s,svv);
    if(s!="<VARIABLE>")
    	sv->setValeur(svv);
    i = table.insert (i, sv);

  }
  return *i;
}


SymboleValue *
TableSymboles::cherche(Symbole s)
// si s est identique à un symbole valué déjà présent dans la table,
// on renvoie un pointeur sur ce symbole valué.
// Sinon, on revoie une exception
{
  vector < SymboleValue * >::iterator i;

  i = table.begin ();
  while (i < table.end () && (**i).getChaine () < s.getChaine ())
    i++;
  if (i == table.end () || (**i).getChaine () != s.getChaine ())	// si pas trouvé...
   throw new ExVarUndef(s.getChaine());
  return *i;
}

////////////////////////////////////////////////////////////////////////////////
ostream & operator << (ostream & cout, TableSymboles ts)
// affiche ts sur cout
{
  cout << endl << "Contenu de la Table des Symboles Values :" << endl
    << "=========================================" << endl << endl;
  for (unsigned int i = 0; i < ts.table.size (); i++)
  {
    cout << "  ";
    SymboleValue sv =*(ts.table[i]);
    sv.disp();
    cout << endl;
  }
  cout << endl;
  return cout;
}
