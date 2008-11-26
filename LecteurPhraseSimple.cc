#include "LecteurPhraseSimple.h"

#include <stdlib.h>
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
LecteurPhraseSimple::LecteurPhraseSimple (string nomFich):
ls (nomFich)
{
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::analyse ()
{
  programme ();
  cout << "Syntaxe correcte." << endl;
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::programme ()
{
// <programme> ::= debut <seqInst> fin FIN_FICHIER

  sauterSymCour ("debut");
  seqInst ();
  sauterSymCour ("fin");
  testerSymCour ("<FINDEFICHIER>");
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::seqInst ()
{
// <seqInst> ::= <inst> ; { <inst> ; }

  do
    {
      inst ();
      sauterSymCour (";");
    }
  while (ls.getSymCour () == "<VARIABLE>");
  // tant que le symbole courant est un debut possible d'instruction...
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::inst ()
{
// <inst> ::= <affectation>

  affectation ();
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::affectation ()
{
// <affectation> ::= <variable> = <expression>

  sauterSymCour ("<VARIABLE>");
  sauterSymCour ("=");
  expression ();
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::expression ()
{
// <expression> ::= <terme> { <opAdd> <terme> }

  terme ();
  while (ls.getSymCour () == "+" || ls.getSymCour () == "-")
    {
      opAdd ();
      terme ();
    }
}

void
LecteurPhraseSimple::terme ()
{
// <expression> ::= <facteur> { <opAdd> <facteur> }

  terme ();
  while (ls.getSymCour () == "*" || ls.getSymCour () == "/")
    {
      opAdd ();
      terme ();
    }
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::facteur ()
{
// <facteur> ::= <entier>  |  <variable>  |  - <facteur>  |  ( <expression> )

  if (ls.getSymCour () == "<VARIABLE>" || ls.getSymCour () == "<ENTIER>" || ls.getSymCour () =="<CHAINE>")
    ls.suivant ();
  else if (ls.getSymCour () == "-")
    {
      ls.suivant ();
      facteur ();
    }
  else if (ls.getSymCour () == "(")
    {
      ls.suivant ();
      expression ();
      sauterSymCour (")");
    }
  else
    erreur ("<facteur>");
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::opAdd ()
// <opAdd> ::= + | -
{
  if (ls.getSymCour () == "+" || ls.getSymCour () == "-")
    ls.suivant ();
  else
    erreur ("<opAdd>");
}
////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::opMult ()
// <opMult> ::=  *  | /
{
  if (ls.getSymCour () == "*" || ls.getSymCour () == "/")
    ls.suivant ();
  else
    erreur ("<opMult>");
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::testerSymCour (string ch)
{
  if (ls.getSymCour () != ch)
    {
      cout << endl << "-------- Erreur ligne " << ls.getLigne ()
	<< " - Colonne " << ls.getColonne () << endl << "   Attendu : "
	<< ch << endl << "   Trouve  : " << ls.getSymCour () << endl << endl;
      exit (0);			// plus tard, on levera une exception
    }
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::sauterSymCour (string ch)
{
  testerSymCour (ch);
  ls.suivant ();
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::erreur (string mess)
{
  cout << endl << "-------- Erreur ligne " << ls.getLigne () << " - Colonne "
    << ls.getColonne () << endl << "   Attendu : " << mess << endl
    << "   Trouve  : " << ls.getSymCour () << endl << endl;
  exit (0);			// plus tard, on levera une exception
}
