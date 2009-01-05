#include "LecteurPhraseAvecArbre.h"

#include <stdlib.h>
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////

LecteurPhraseAvecArbre::LecteurPhraseAvecArbre(string nomFich) :
ls(nomFich), ts()
{
}

////////////////////////////////////////////////////////////////////////////////

void
LecteurPhraseAvecArbre::analyse()
{
  arbre = programme();
  cout << "Syntaxe correcte." << endl;
}

////////////////////////////////////////////////////////////////////////////////

Noeud *
LecteurPhraseAvecArbre::programme()
{
  // <programme> ::= debut <seq_inst> fin FIN_FICHIER
  Noeud * decl = NULL;
  if (ls.getSymCour() == "declarer")
  {
    ls.suivant();
    decl = declaration();
  }
  cout << "before deb" << endl;
  sauterSymCour("debut");
  cout << "after deb" << endl;
  Noeud *si = seqInst();
  sauterSymCour("fin");
  testerSymCour("<FINDEFICHIER>");
  return new NoeudProgramme(decl, si);
}
////////////////////////////////////////////////////////////////////////////////

Noeud * LecteurPhraseAvecArbre::declaration()
{
  //<declaration>::= {<TYPE>:<VARIABLE>;}
  NoeudDeclaration * nd = new NoeudDeclaration();
  string type;
  string name;
  Symbole var;
  while (ls.getSymCour() == "<VARIABLE>")
  {
    var = ls.getSymCour();
    name = var.getChaine();
    ls.suivant();
    sauterSymCour(":");
    type = ls.getSymCour().getChaine();
    cout << type;
    if (type == "Entier" ||
            type == "Chaine" ||
            type == "Bool" ||
            type == "Reel")
      ls.suivant();
    else
      sauterSymCour("<VARIABLE>");
    ts.chercheAjoute(var, type);
    nd->ajouteDeclaration(name, type);
    sauterSymCour(";");
  }
  return nd;
}



////////////////////////////////////////////////////////////////////////////////

Noeud *
LecteurPhraseAvecArbre::seqInst()
{
  // <seqInst> ::= <inst> ; { <inst> ; }

  NoeudSeqInst *si = new NoeudSeqInst();
  do
  {
    si->ajouteInstruction(inst());
    sauterSymCour(";");
  }
  while (ls.getSymCour() == "<VARIABLE>" || ls.getSymCour() == "si" ||
          ls.getSymCour() == "tantque" || ls.getSymCour() == "repeter" ||
          ls.getSymCour() == "lire" || ls.getSymCour() == "ecrire");
  // tant que le symbole courant est un debut possible d'instruction...
  return si;
}

////////////////////////////////////////////////////////////////////////////////

Noeud *
LecteurPhraseAvecArbre::inst()
{
  // <inst> ::= <affectation> | <inst_condi>
  if (ls.getSymCour() == "<VARIABLE>")
  {
    cout << ls.getSymCour().getChaine();
    ts.cherche(ls.getSymCour());
    return affectation();
  }
  else if (ls.getSymCour() == "si")
    return instSi();
  else if (ls.getSymCour() == "tantque")
    return instTq();
  else if (ls.getSymCour() == "repeter")
    return instRepeter();
  else if (ls.getSymCour() == "pour")
    return instPour();
  else
    return NULL;
  //else if(ls.getSymCour () == "lire")
  //instLire();
  //else if(ls.getSymCour () == "ecrire")
  //instEcrire()

}

////////////////////////////////////////////////////////////////////////////////

Noeud *
LecteurPhraseAvecArbre::affectation()
{
  // <affectation> ::= <variable> = <expression>

  testerSymCour("<VARIABLE>");
  Noeud *var = ts.chercheAjoute(ls.getSymCour());
  ls.suivant();
  sauterSymCour("=");
  Noeud *exp = expression();
  //Type * t1 =var->getValeur();
  //Type * t2 =exp->getValeur();
  // string st1=t1->getType();
  // string st2=t2->getType();
  //cout <<st1<<"=="<<st2<<endl;
  //if(st1!=st2)
  //{
  //	cout <<"ho!"<<endl;
  //	throw new ExTypeMismatch(var->getValeur()->getType(),exp->getValeur()->getType());
  //}
  return new NoeudAffectation(var, exp);
}

////////////////////////////////////////////////////////////////////////////////

Noeud *
LecteurPhraseAvecArbre::expression()
{
  // <expression> ::= <terme> { <opAdd> <terme> }

  Noeud *fact = facteur();
  while (ls.getSymCour() == "+" || ls.getSymCour() == "-")
  {
    Symbole operateur = opBinaire(); // on stocke le symbole de l'opérateur
    Noeud *factDroit = facteur(); // lecture de l'operande droit
    fact = new NoeudOperateurBinaire(operateur, fact, factDroit); // const. du noeud
  }
  return fact;
}

Noeud *
LecteurPhraseAvecArbre::terme()
{
  // <expression> ::= <facteur> { <opMult> <facteur> }

  Noeud *fact = facteur();
  while (ls.getSymCour() == "*" || ls.getSymCour() == "/")
  {
    Symbole operateur = opBinaire(); // on stocke le symbole de l'opérateur
    Noeud *factDroit = facteur(); // lecture de l'operande droit
    fact = new NoeudOperateurBinaire(operateur, fact, factDroit); // const. du noeud
  }
  return fact;
}

////////////////////////////////////////////////////////////////////////////////

Noeud *
LecteurPhraseAvecArbre::expBool()
{
  //<expBool>  ::=  <relation> { <opBool> <relation> }

  Noeud *fact = relation();
  while (ls.getSymCour() == "et" || ls.getSymCour() == "ou")
  {
    Symbole operateur = opBinaire(); // on stocke le symbole de l'opérateur
    Noeud *factDroit = relation(); // lecture de l'operande droit
    fact = new NoeudOperateurBinaire(operateur, fact, factDroit); // const. du noeud
  }
  return fact;
}

////////////////////////////////////////////////////////////////////////////////

Noeud *
LecteurPhraseAvecArbre::relation()
{
  //<relation>::= <expression> { <opRel> <expression> }


  Noeud *fact = expression();
  while (ls.getSymCour() == "==" || ls.getSymCour() == "!=" ||
          ls.getSymCour() == "<" || ls.getSymCour() == ">" ||
          ls.getSymCour() == "<=" || ls.getSymCour() == ">=")
  {
    Symbole operateur = opBinaire(); // on stocke le symbole de l'opérateur
    Noeud *factDroit = relation(); // lecture de l'operande droit
    fact = new NoeudOperateurBinaire(operateur, fact, factDroit); // const. du noeud
  }
  return fact;
}

////////////////////////////////////////////////////////////////////////////////

Noeud *
LecteurPhraseAvecArbre::facteur()
{
  // <facteur> ::= <entier>  |  <variable>  |  - <facteur>  |  ( <expression> )

  Noeud *fact = NULL;

  if (ls.getSymCour() == "<VARIABLE>" || ls.getSymCour() == "<ENTIER>" || ls.getSymCour() == "<CHAINE>")
  {


    if (ls.getSymCour() == "<VARIABLE>")
    {
      fact = ts.cherche(ls.getSymCour());
    }
    else
    {
      fact = ts.chercheAjoute(ls.getSymCour());
    }
    ls.suivant();
  }
  else if (ls.getSymCour() == "-" || ls.getSymCour() == "non")
  {
    ls.suivant();
    // on représente le moins unaire (- facteur) par une soustractin binaire (0 - facteur)
    fact =
            new NoeudOperateurUnaire(ls.getSymCour(), expBool());
  }
  else if (ls.getSymCour() == "(")
  {
    ls.suivant();
    fact = expBool();
    sauterSymCour(")");
  }
  else
    erreur(E_SYNTAX, "<facteur>");
  return fact;
}
////////////////////////////////////////////////////////////////////////////////

Noeud *
LecteurPhraseAvecArbre::instSi(bool rec)
{//<instSi> ::= si ( <expBool> ) <seqInst> { sinonsi ( <expBool> ) <seqInst> }
  //  [ sinon <seqInst> ] finsi


  Noeud * cond = NULL;
  Noeud * seqVrai = NULL;
  Noeud * siFaux = NULL;
  if (ls.getSymCour() == "si" || (rec && ls.getSymCour() == "sinonsi"))
  {
    ls.suivant();
    if (ls.getSymCour() == "(")
    {
      ls.suivant();
      cond = expBool();
      sauterSymCour(")");
    }
    seqVrai = seqInst();
    if (ls.getSymCour() == "sinon")
    {
      ls.suivant();
      siFaux = seqInst();
      sauterSymCour("finsi");
    }
    else if (ls.getSymCour() == "sinonsi")
      siFaux = instSi(true);
    else
    {
      siFaux = new NoeudSeqInst();
      sauterSymCour("finsi");
    }
  }
  return new NoeudSi(cond, seqVrai, siFaux);
}
////////////////////////////////////////////////////////////////////////////////

Noeud *
LecteurPhraseAvecArbre::instTq()
{
  //<instTq> ::= tantque ( <expBool> ) <seqInst> fintantque
  Noeud * nexpBool = NULL;
  Noeud * nseqInst = NULL;
  sauterSymCour("tantque");
  if (ls.getSymCour() == "(")
  {
    ls.suivant();
    nexpBool = expBool();
    sauterSymCour(")");
  }
  nseqInst = seqInst();
  sauterSymCour("fintantque");
  return new NoeudTantque(nexpBool, nseqInst);
}

Noeud *
LecteurPhraseAvecArbre::instRepeter()
{
  //<instTq> ::= repeter <seqInst>  tantque ( <expBool> )
  Noeud * nexpBool = NULL;
  Noeud * nseqInst = NULL;
  sauterSymCour("repeter");
  nseqInst = seqInst();
  sauterSymCour("tantque");
  if (ls.getSymCour() == "(")
  {
    ls.suivant();
    nexpBool = expBool();
    sauterSymCour(")");
  }
  return new NoeudRepeter(nexpBool, nseqInst);
}

Noeud *
LecteurPhraseAvecArbre::instPour()
{
  //<instPour> ::= pour (<affectation>;<expBool>;<affectation>>  ) <seqInst> finpour
  Noeud * init = NULL;
  Noeud * affect = NULL;
  Noeud * condition = NULL;
  Noeud * seq;
  sauterSymCour("pour");
  if (ls.getSymCour() == "(")
  {
    ls.suivant();
    init = affectation();
    sauterSymCour(";");
    condition = expBool();
    sauterSymCour(";");
    affect = affectation();
    sauterSymCour(")");
  }
  seq = seqInst();
  sauterSymCour("fintantque");
  return new NoeudPour(init, condition, affect, seq);
}
////////////////////////////////////////////////////////////////////////////////

Symbole LecteurPhraseAvecArbre::opBinaire()
{
  // <opBinaire> ::= + | - | *  | / | et | ou | == | != |< |>|<=|>=
  Symbole operateur;
  if (ls.getSymCour() == "+" || ls.getSymCour() == "-" ||
          ls.getSymCour() == "*" || ls.getSymCour() == "/" ||
          ls.getSymCour() == "et" || ls.getSymCour() == "ou" ||
          ls.getSymCour() == "==" || ls.getSymCour() == "!=" ||
          ls.getSymCour() == "<" || ls.getSymCour() == ">" ||
          ls.getSymCour() == "<=" || ls.getSymCour() == ">=")
  {
    operateur = ls.getSymCour();
    ls.suivant();
  }
  else
  {
    erreur(E_SYNTAX, "<opBinaire>");
  }
  return operateur;
}

Symbole LecteurPhraseAvecArbre::opUnaire()
{
  Symbole operateur;
  if (ls.getSymCour() == "-" || ls.getSymCour() == "non")
  {
    operateur = ls.getSymCour();
    ls.suivant();
  }
  else
    erreur(E_SYNTAX, "<opUnaire>");
  return operateur;

}

////////////////////////////////////////////////////////////////////////////////

void
LecteurPhraseAvecArbre::testerSymCour(string ch)
{
  if (ls.getSymCour() != ch)
  {
    throw new ExSyntaxError(ch, ls.getSymCour().getChaine(), ls.getLigne(), ls.getColonne());
  }
}

////////////////////////////////////////////////////////////////////////////////

void
LecteurPhraseAvecArbre::sauterSymCour(string ch)
{
  testerSymCour(ch);
  ls.suivant();
}

////////////////////////////////////////////////////////////////////////////////

void
LecteurPhraseAvecArbre::erreur(ExType_t type, string msg, string msg1)
{
  int line = ls.getLigne();
  int col = ls.getColonne();
  switch (type)
  {
  case E_UNHANDLED: throw new Exception(line, col);
    break;
  case E_SYNTAX: throw new ExSyntaxError(ls.getSymCour().getChaine(), msg, line, col);
    break;
  case E_UNDEFVAR:throw new ExVarUndef(msg, line, col);
    break;
  case E_TYPE: throw new ExTypeMismatch(msg, msg1, line, col);
    break;
  }
}
