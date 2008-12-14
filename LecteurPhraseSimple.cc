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
  while (ls.getSymCour () == "<VARIABLE>" || ls.getSymCour () == "si" ||
         ls.getSymCour () == "tantque" || ls.getSymCour () == "repeter" ||
         ls.getSymCour () == "lire" || ls.getSymCour () == "ecrire");
  // tant que le symbole courant est un debut possible d'instruction...
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::inst ()
{
// <inst> ::= <affectation>

  if(ls.getSymCour () == "<VARIABLE>")
    {
      affectation ();
    }
  else if(ls.getSymCour () == "si")
      instSi();
  else if(ls.getSymCour () == "tantque")
    instTq();
  else if(ls.getSymCour () == "repeter")
    instRepeter();
  else if(ls.getSymCour () == "lire")
    instLire();
  else if(ls.getSymCour () == "ecrire")
    instEcrire();
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::affectation ()
{
// <affectation> ::= <variable> = <expression>

  sauterSymCour ("<VARIABLE>");
  sauterSymCour ("=");
  expression ();
  //relation();
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
LecteurPhraseSimple::expBool ()
// <expBool> ::= <relation> {<opBool> <relation>}
{
  relation ();
  while (ls.getSymCour () == "et" || ls.getSymCour () == "ou")
    {
      opBool ();
      relation ();
    }

}

void
LecteurPhraseSimple::relation ()
//<relation> ::= <expression> {<opRel> <expression>}
{
  expression ();
  while (ls.getSymCour () == "==" || ls.getSymCour () == "!="||
       	 ls.getSymCour () == "<" || ls.getSymCour () == ">" ||
         ls.getSymCour () == "<=" || ls.getSymCour () == ">=" )
    {
      opRel ();
      expression ();
    }
}

void
LecteurPhraseSimple::terme ()
{
// <terme> ::= <facteur> { <opMult> <facteur> }

  facteur ();
  while (ls.getSymCour () == "*" || ls.getSymCour () == "/")
    {
      opMult ();
      facteur ();
    }
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::facteur ()
{
// <facteur> ::= <entier>  |  <variable>  |chaine |  <opUnaire> <expBool>  |  ( <expBool> )

  if (ls.getSymCour () == "<VARIABLE>" || ls.getSymCour () == "<ENTIER>"
      || ls.getSymCour () == "<CHAINE>")
    ls.suivant ();
  else if (ls.getSymCour () == "-" || ls.getSymCour () == "not")
    {
      ls.suivant ();
      expBool ();
    }
  else if (ls.getSymCour () == "(")
    {
      ls.suivant ();
      expBool ();
      sauterSymCour (")");
    }
  else
    erreur ("<facteur>");
}
void
LecteurPhraseSimple::instSi()
{//<instSi> ::= si ( <expBool> ) <seqInst> { sinonsi ( <expBool> ) <seqInst> }
//  [ sinon <seqInst> ] finsi
    sauterSymCour("si");
    //ls.suivant();
    if(ls.getSymCour ()=="(")
      {
        ls.suivant();
        expBool();
        sauterSymCour(")");
      }
    else
      erreur("si");
    seqInst();
    while(ls.getSymCour ()=="sinonsi")
      {
        ls.suivant();
        if(ls.getSymCour ()=="(")
          {
            ls.suivant();
            expBool();
            sauterSymCour(")");
          }
        seqInst();
      }
    if(ls.getSymCour ()=="sinon")
      {
        ls.suivant();
        seqInst();
      }
    sauterSymCour("finsi");
}

void
LecteurPhraseSimple::instTq()
{
  //<instTq> ::= tantque ( <expBool> ) <seqInst> fintantque
    sauterSymCour("tantque");
    if(ls.getSymCour ()=="(")
      {
        ls.suivant();
        expBool();
        sauterSymCour(")");
      }
    seqInst();
    sauterSymCour("fintantque");
}

void
LecteurPhraseSimple::instRepeter()
{
  //<instRepeter> ::= repeter <seqInst> jusqua ( <expBool> )
    sauterSymCour("repeter");
    seqInst();
    sauterSymCour("jusqua");
    if(ls.getSymCour ()=="(")
          {
            ls.suivant();
            expBool();
            sauterSymCour(")");
          }
}
void
LecteurPhraseSimple::instLire()
{
  sauterSymCour("lire");
  sauterSymCour("(");
  sauterSymCour("<VARIABLE>");
  sauterSymCour(")");
}
void
LecteurPhraseSimple::instEcrire()
{
  sauterSymCour("ecrire");
  sauterSymCour("(");
  if(ls.getSymCour()=="<CHAINE>")
    ls.suivant();
  else
    expression();
  sauterSymCour(")");
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

void
LecteurPhraseSimple::opBool ()
//<opBool>::= et | ou
{
  if (ls.getSymCour () == "et" || ls.getSymCour () == "ou")
  {
    ls.suivant ();
  }
  else
    erreur ("<opBool>");
}



void
LecteurPhraseSimple::opRel ()
//<opRel>::= == | != | < | <= | > |>=
{
  if (ls.getSymCour () == "==" || ls.getSymCour () == "!=" ||
      ls.getSymCour () == "<" || ls.getSymCour () == "<=" ||
      ls.getSymCour () == ">" || ls.getSymCour () == ">=")
    ls.suivant ();
  else
    erreur ("<opRel>");
}

void
LecteurPhraseSimple::opUnaire ()
//<opUnaire>::= - | non
{
  if (ls.getSymCour () == "-" || ls.getSymCour () == "non")
    ls.suivant ();
  else
    erreur ("<opUnaire>");
}


////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseSimple::testerSymCour (string ch)
{
  if (ls.getSymCour () != ch)
    {
      cout << endl << "-------- Erreur ligne " << ls.getLigne ()
	<< " - Colonne " << ls.getColonne ()<< "|" << endl << "   Attendu : "
	<< ch << endl << "   Trouve  : " << ls.getSymCour ()<<"|" << endl << endl;
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
