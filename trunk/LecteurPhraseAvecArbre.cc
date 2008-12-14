#include "LecteurPhraseAvecArbre.h"

#include <stdlib.h>
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
LecteurPhraseAvecArbre::LecteurPhraseAvecArbre (string nomFich):
ls (nomFich), ts ()
{
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseAvecArbre::analyse ()
{
  arbre = programme ();
  cout << "Syntaxe correcte." << endl;
}

////////////////////////////////////////////////////////////////////////////////
Noeud *
LecteurPhraseAvecArbre::programme ()
{
  // <programme> ::= debut <seq_inst> fin FIN_FICHIER

  sauterSymCour ("debut");
  Noeud *si = seqInst ();
  sauterSymCour ("fin");
  testerSymCour ("<FINDEFICHIER>");
  return si;
}

////////////////////////////////////////////////////////////////////////////////
Noeud *
LecteurPhraseAvecArbre::seqInst ()
{
  // <seqInst> ::= <inst> ; { <inst> ; }

  NoeudSeqInst *si = new NoeudSeqInst ();
  do
    {
      si->ajouteInstruction (inst ());
      sauterSymCour (";");
    }
  while (ls.getSymCour () == "<VARIABLE>" || ls.getSymCour () == "si" ||
      ls.getSymCour () == "tantque" || ls.getSymCour () == "repeter" ||
      ls.getSymCour () == "lire" || ls.getSymCour () == "ecrire");
  // tant que le symbole courant est un debut possible d'instruction...
  return si;
}

////////////////////////////////////////////////////////////////////////////////
Noeud *
LecteurPhraseAvecArbre::inst ()
{
// <inst> ::= <affectation> | <inst_condi>
  if(ls.getSymCour () == "<VARIABLE>")
    {
      ts.chercheAjoute (ls.getSymCour ());
      return affectation ();
    }
  else if(ls.getSymCour () == "si")
      return instSi();
  else if(ls.getSymCour () == "tantque");
    //return instTq();
  else if(ls.getSymCour () == "repeter");
    //return instRepeter();
  //else if(ls.getSymCour () == "lire")
    //instLire();
  //else if(ls.getSymCour () == "ecrire")
    //instEcrire();

//  return affectation ();
}

////////////////////////////////////////////////////////////////////////////////
Noeud *
LecteurPhraseAvecArbre::affectation ()
{
// <affectation> ::= <variable> = <expression>

  testerSymCour ("<VARIABLE>");
  Noeud *var = ts.chercheAjoute (ls.getSymCour ());
  ls.suivant ();
  sauterSymCour ("=");
  Noeud *exp = expression ();
  return new NoeudAffectation (var, exp);
}

////////////////////////////////////////////////////////////////////////////////
Noeud *
LecteurPhraseAvecArbre::expression ()
{
// <expression> ::= <terme> { <opAdd> <terme> }

  Noeud *fact = facteur ();
  while (ls.getSymCour () == "+" || ls.getSymCour () == "-")
    {
      Symbole operateur = opBinaire ();	// on stocke le symbole de l'opérateur
      Noeud *factDroit = facteur ();	// lecture de l'operande droit
      fact = new NoeudOperateurBinaire (operateur, fact, factDroit);	// const. du noeud
    }
  return fact;
}

Noeud *
LecteurPhraseAvecArbre::terme ()
{
// <expression> ::= <facteur> { <opMult> <facteur> }

  Noeud *fact = facteur ();
  while (ls.getSymCour () == "*" || ls.getSymCour () == "/")
    {
      Symbole operateur = opBinaire (); // on stocke le symbole de l'opérateur
      Noeud *factDroit = facteur ();    // lecture de l'operande droit
      fact = new NoeudOperateurBinaire (operateur, fact, factDroit);    // const. du noeud
    }
  return fact;
}

////////////////////////////////////////////////////////////////////////////////
Noeud *
LecteurPhraseAvecArbre::expBool()
{
  //<expBool>  ::=  <relation> { <opBool> <relation> }

  Noeud *fact = relation ();
  while (ls.getSymCour () == "et" || ls.getSymCour () == "ou")
    {
      Symbole operateur = opBinaire (); // on stocke le symbole de l'opérateur
      Noeud *factDroit = relation ();    // lecture de l'operande droit
      fact = new NoeudOperateurBinaire (operateur, fact, factDroit);    // const. du noeud
    }
  return fact;
}

////////////////////////////////////////////////////////////////////////////////
Noeud *
LecteurPhraseAvecArbre::relation()
{
  //<relation>::= <expression> { <opRel> <expression> }


  Noeud *fact = expression ();
  while (ls.getSymCour () == "==" || ls.getSymCour () == "!="||
         ls.getSymCour () == "<" || ls.getSymCour () == ">" ||
         ls.getSymCour () == "<=" || ls.getSymCour () == ">=")
    {
      Symbole operateur = opBinaire (); // on stocke le symbole de l'opérateur
      Noeud *factDroit = relation ();    // lecture de l'operande droit
      fact = new NoeudOperateurBinaire (operateur, fact, factDroit);    // const. du noeud
    }
  return fact;
}

////////////////////////////////////////////////////////////////////////////////
Noeud *
LecteurPhraseAvecArbre::facteur ()
{
// <facteur> ::= <entier>  |  <variable>  |  - <facteur>  |  ( <expression> )

  Noeud *fact = NULL;

  if (ls.getSymCour () == "<VARIABLE>" || ls.getSymCour () == "<ENTIER>")
    {
      fact = ts.chercheAjoute (ls.getSymCour ());
      ls.suivant ();
    }
  else if (ls.getSymCour () == "-" || ls.getSymCour () == "non")
    {
      ls.suivant ();
      // on représente le moins unaire (- facteur) par une soustractin binaire (0 - facteur)
      fact =
	new NoeudOperateurUnaire (ls.getSymCour (),expBool());
    }
  else if (ls.getSymCour () == "(")
    {
      ls.suivant ();
      fact = expBool();
      sauterSymCour (")");
    }
  else
    erreur ("<facteur>");
  return fact;
}
////////////////////////////////////////////////////////////////////////////////
Noeud *
LecteurPhraseAvecArbre::instSi(bool rec)
{//<instSi> ::= si ( <expBool> ) <seqInst> { sinonsi ( <expBool> ) <seqInst> }
//  [ sinon <seqInst> ] finsi


    Noeud * cond;
    Noeud * seqVrai;
    Noeud * siFaux;
    if(ls.getSymCour ()=="si" || (rec && ls.getSymCour ()=="sinonsi"))
      {
        ls.suivant();
        if(ls.getSymCour ()=="(")
          {
            ls.suivant();
            cond=expBool();
            sauterSymCour(")");
          }
        seqVrai=seqInst();
        siFaux=instSi(true);
      }
    else if(rec && ls.getSymCour ()=="sinon")
      {
        ls.suivant();
        siFaux=seqInst();
      }
    else
      {
        siFaux=new NoeudSeqInst();
        sauterSymCour("finsi");
      }
    return new NoeudSi(cond,seqVrai,siFaux);
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
Symbole LecteurPhraseAvecArbre::opBinaire ()
{
// <opBinaire> ::= + | - | *  | / | et | ou | == | != |< |>|<=|>=
  Symbole operateur;
  if (ls.getSymCour () == "+" || ls.getSymCour () == "-" ||
      ls.getSymCour () == "*" || ls.getSymCour () == "/" ||
      ls.getSymCour () == "et"|| ls.getSymCour () == "ou"||
      ls.getSymCour () == "=="|| ls.getSymCour () == "!="||
      ls.getSymCour () == "<" || ls.getSymCour () == ">" ||
      ls.getSymCour () == "<="|| ls.getSymCour () == ">=")
    {
      operateur = ls.getSymCour ();
      ls.suivant ();
    }
  else
    {
      erreur ("<opBinaire>");
    }
  return operateur;
}

Symbole LecteurPhraseAvecArbre::opUnaire ()
{
  Symbole operateur;
  if(ls.getSymCour () == "-" || ls.getSymCour () == "non" )
    {
      operateur=ls.getSymCour ();
      ls.suivant();
    }
  else
    erreur("<opUnaire>");
  return operateur;

}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseAvecArbre::testerSymCour (string ch)
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
LecteurPhraseAvecArbre::sauterSymCour (string ch)
{
  testerSymCour (ch);
  ls.suivant ();
}

////////////////////////////////////////////////////////////////////////////////
void
LecteurPhraseAvecArbre::erreur (string mess)
{
  cout << endl << "-------- Erreur ligne " << ls.getLigne () << " - Colonne "
    << ls.getColonne () << endl << "   Attendu : " << mess << endl
    << "   Trouve  : " << ls.getSymCour () << endl << endl;
  exit (0);			// plus tard, on levera une exception
}
