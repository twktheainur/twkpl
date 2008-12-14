#include "LecteurPhraseAvecTable.h"

#include <stdlib.h>
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
LecteurPhraseAvecTable::LecteurPhraseAvecTable (string nomFich):
ls (nomFich), ts ()
{
}
void
LecteurPhraseAvecTable::analyse ()
{
  programme ();
  cout << "Syntaxe correcte." << endl;
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseAvecTable::programme ()
{
// <programme> ::= debut <seqInst> fin FIN_FICHIER

  sauterSymCour ("debut");
  seqInst ();
  sauterSymCour ("fin");
  testerSymCour ("<FINDEFICHIER>");
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseAvecTable::seqInst ()
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
LecteurPhraseAvecTable::inst ()
{
// <inst> ::= <affectation>

  if(ls.getSymCour () == "<VARIABLE>")
    {
      ts.chercheAjoute (ls.getSymCour ());
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
LecteurPhraseAvecTable::affectation ()
{
// <affectation> ::= <variable> = <expression>

  testerSymCour ("<VARIABLE>");
  ts.chercheAjoute (ls.getSymCour ());
  ls.suivant();
  sauterSymCour ("=");
  expression ();
  //relation();
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseAvecTable::expression ()
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
LecteurPhraseAvecTable::expBool ()
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
LecteurPhraseAvecTable::relation ()
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
LecteurPhraseAvecTable::terme ()
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
LecteurPhraseAvecTable::facteur ()
{
// <facteur> ::= <entier>  |  <variable>  |chaine |  <opUnaire> <expBool>  |  ( <expBool> )

  if (ls.getSymCour () == "<VARIABLE>" || ls.getSymCour () == "<ENTIER>"
      || ls.getSymCour () == "<CHAINE>")
    {
      ts.chercheAjoute (ls.getSymCour ());
      ls.suivant ();
    }
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
LecteurPhraseAvecTable::instSi()
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
LecteurPhraseAvecTable::instTq()
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
LecteurPhraseAvecTable::instRepeter()
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
LecteurPhraseAvecTable::instLire()
{
  sauterSymCour("lire");
  sauterSymCour("(");
  testerSymCour("<VARIABLE>");
  ts.chercheAjoute (ls.getSymCour ());
  ls.suivant ();
  sauterSymCour(")");
}
void
LecteurPhraseAvecTable::instEcrire()
{
  sauterSymCour("ecrire");
  sauterSymCour("(");
  if(ls.getSymCour()=="<CHAINE>")
    {
      ts.chercheAjoute (ls.getSymCour ());
      ls.suivant();
    }
  else
    expression();
  sauterSymCour(")");
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseAvecTable::opAdd ()
// <opAdd> ::= + | -
{
  if (ls.getSymCour () == "+" || ls.getSymCour () == "-")
    ls.suivant ();
  else
    erreur ("<opAdd>");
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseAvecTable::opMult ()
// <opMult> ::=  *  | /
{
  if (ls.getSymCour () == "*" || ls.getSymCour () == "/")
    ls.suivant ();
  else
    erreur ("<opMult>");
}

void
LecteurPhraseAvecTable::opBool ()
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
LecteurPhraseAvecTable::opRel ()
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
LecteurPhraseAvecTable::opUnaire ()
//<opUnaire>::= - | non
{
  if (ls.getSymCour () == "-" || ls.getSymCour () == "non")
    ls.suivant ();
  else
    erreur ("<opUnaire>");
}


////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseAvecTable::testerSymCour (string ch)
{
  if (ls.getSymCour () != ch)
    {
      cout << endl << "-------- Erreur ligne " << ls.getLigne ()
        << " - Colonne " << ls.getColonne ()<< "|" << endl << "   Attendu : "
        << ch << endl << "   Trouve  : " << ls.getSymCour ()<<"|" << endl << endl;
      exit (0);                 // plus tard, on levera une exception
    }
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseAvecTable::sauterSymCour (string ch)
{
  testerSymCour (ch);
  ls.suivant ();
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseAvecTable::erreur (string mess)
{
  cout << endl << "-------- Erreur ligne " << ls.getLigne () << " - Colonne "
    << ls.getColonne () << endl << "   Attendu : " << mess << endl
    << "   Trouve  : " << ls.getSymCour () << endl << endl;
  exit (0);                     // plus tard, on levera une exception
}
