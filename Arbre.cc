#include <stdlib.h>
#include "Arbre.h"
#include "SymboleValue.h"
#include <typeinfo>

////////////////////////////////////////////////////////////////////////////////
// NoeudDeclaration
////////////////////////////////////////////////////////////////////////////////

NoeudDeclaration::NoeudDeclaration() : mapDecl()
{
}

////////////////////////////////////////////////////////////////////////////////

Type *
NoeudDeclaration::getValeur()
{
  int size = mapDecl.size();
  Entier * res = new Entier();
  res->setValeur((void *) (&size));

  return res;
}

////////////////////////////////////////////////////////////////////////////////

void
NoeudDeclaration::afficher(unsigned short indentation)
{
  Noeud::afficher(indentation);
  cout << "Noeud - Sequence de " << mapDecl.size() << "Declaration(s)" <<
    endl;
  for (map<string, string>::iterator it = mapDecl.begin(); it != mapDecl.end(); it++)
  {
    cout << it->first << ":" << it->second;
  }
}

////////////////////////////////////////////////////////////////////////////////

void
NoeudDeclaration::ajouteDeclaration(string symbol, string type)
{
  mapDecl.insert(pair<string, string > (symbol, type));
}


////////////////////////////////////////////////////////////////////////////////
// NoeudSeqInst
////////////////////////////////////////////////////////////////////////////////

NoeudSeqInst::NoeudSeqInst() : tabInst()
{
}

////////////////////////////////////////////////////////////////////////////////

Type *
NoeudSeqInst::getValeur()
{
  Type * valeur=NULL;
  for (unsigned int i = 0; i < tabInst.size(); i++)
    valeur = tabInst[i]->getValeur(); // on evalue chaque instruction de la séquence
  return valeur; // par convention, resultat = valeur de la derniere instruction
}

////////////////////////////////////////////////////////////////////////////////

void
NoeudSeqInst::afficher(unsigned short indentation)
{
  Noeud::afficher(indentation);
  cout << "Noeud - Sequence de " << tabInst.size() << " instruction(s)" <<
    endl;
  for (unsigned int i = 0; i < tabInst.size(); i++)
    tabInst[i]->afficher(indentation + 1); // on affiche les fils en augmentant l'indentation
}

////////////////////////////////////////////////////////////////////////////////

void
NoeudSeqInst::ajouteInstruction(Noeud * instruction)
{
  tabInst.push_back(instruction);
}
////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////
Type *
NoeudProgramme::getValeur()
{
  this->seqDecl->getValeur();
  return this->seqInst->getValeur();
}

////////////////////////////////////////////////////////////////////////////////

void
NoeudProgramme::afficher(unsigned short indentation)
{
  Noeud::afficher(indentation);
  cout << "Noeud - Programme" << endl;
  seqDecl->afficher(indentation + 1); // on affiche variable et expression
  seqInst->afficher(indentation + 1); // en augmentant l'indentation
}

////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////

NoeudAffectation::NoeudAffectation(Noeud * variable, Noeud * expression)
{
  this->variable = variable;
  this->expression = expression;
}

////////////////////////////////////////////////////////////////////////////////

Type *
NoeudAffectation::getValeur()
{
  SymboleValue * svvar = (SymboleValue *) variable;
  Type * eval = expression->getValeur(); // on évalue l'expression
  svvar->setValeur(eval);
  return eval;
  // par convention, une affectation a pour valeur la valeur affectée
}

////////////////////////////////////////////////////////////////////////////////

void
NoeudAffectation::afficher(unsigned short indentation)
{
  Noeud::afficher(indentation);
  cout << "Noeud - Affectation" << endl;
  variable->afficher(indentation + 1); // on affiche variable et expression
  expression->afficher(indentation + 1); // en augmentant l'indentation
}

////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurBinaire::NoeudOperateurBinaire(Symbole operateur,
                                             Noeud * operandeGauche,
                                             Noeud * operandeDroit)
{
  this->operateur = operateur;
  this->operandeGauche = operandeGauche;
  this->operandeDroit = operandeDroit;
}

////////////////////////////////////////////////////////////////////////////////

Type *
NoeudOperateurBinaire::getValeur()
{
  Type * valeur=NULL;
  Type * og = operandeGauche->getValeur();
  Type * od = operandeDroit->getValeur();
  if (this->operateur == "+" ||
    this->operateur == "-" ||
    this->operateur == "*" ||
    this->operateur == "/")
  {
    if (og->getType() == "<ENTIER>")
    {
      void * valg;
      void * vald;
      int resval;
      og->getValeur(&valg);
      od->getValeur(&vald);
      valeur = new Entier();
      if (this->operateur == "+")
        resval = *((int *) valg) + *((int *) vald);
      else if (this->operateur == "-")
        resval = *((int *) valg) - *((int *) vald);
      else if (this->operateur == "*")
        resval = *((int *) valg) * *((int *) vald);
      else if (this->operateur == "/")
      {
        if (*((int *) vald) != 0)
          resval = *((int *) valg) / *((int *) vald);
        //else
        //TODO:throw something here
      }
      valeur->setValeur(&resval);
    }
    else if (og->getType() == "<REEL>")
    {
      void * valg;
      void * vald;
      float resval;
      og->getValeur(&valg);
      od->getValeur(&vald);
      valeur = new Reel();
      if (this->operateur == "+")
        resval = *((float *) valg) + *((float *) vald);
      else if (this->operateur == "-")
        resval = *((float *) valg) - *((float *) vald);
      else if (this->operateur == "*")
        resval = *((float *) valg) * *((float *) vald);
      else if (this->operateur == "/")
      {
        if (*((float *) vald) != 0)
          resval = *((float *) valg) / *((float *) vald);
        //else
        //TODO:throw something here
      }
      valeur->setValeur(&resval);
    }
    else if (og->getType() == "<CHAINE>" && this->operateur == "+")
    {
      void * valg;
      void * vald;
      string resval;
      og->getValeur(&valg);
      od->getValeur(&vald);
      valeur = new Chaine();
      resval = *((string *) valg) + *((string *) vald);
      cout <<"str concat:" << resval<<endl;
      valeur->setValeur(&resval);
    }
  }
  else
  {
    int resval;
    void * valg;
    void *vald;
    og->getValeur(&valg);
    od->getValeur(&vald);
    try
    {
    	this->getType();
    }
    catch(Exception * e)
    {
    	throw;
    }

    valeur = new Entier();
    if (this->operateur == "et")
      resval = *((int*)valg) && *((int*)vald);
    else if (this->operateur == "ou")
      resval = *((int*)valg) || *((int*)vald);
    else if (this->operateur == "==")
    {
      if (og->getType() == "<ENTIER>")
        resval = *((int*) valg) == *((int*) vald);
      else if (og->getType() == "<REEL>")
        resval = *((float*) valg) == *((float*) vald);
      else if (og->getType() == "<CHAINE>")
        resval = *((string*) valg) == *((string*) vald);
    }
    else if (this->operateur == "!=")
    {
    	cout << "=="<<endl;
      if (og->getType() == "<ENTIER>")
        resval = *((int*) valg) != *((int*) vald);
      else if (og->getType() == "<REEL>")
        resval = *((float*) valg) != *((float*) vald);
      else if (og->getType() == "<CHAINE>")
        resval = *((string*) valg) != *((string*) vald);
    }

    else if (this->operateur == ">")
    {
      if (og->getType() == "<ENTIER>")
        resval = *((int*) valg) > *((int*) vald);
      else if (og->getType() == "<REEL>")
        resval = *((float*) valg) > *((float*) vald);
      else if (og->getType() == "<CHAINE>")
        resval = *((string*) valg) > *((string*) vald);
    }
    else if (this->operateur == "<")
    {
      if (og->getType() == "<ENTIER>")
      {
        resval = *((int*) valg)< *((int*) vald);
      }
      else if (og->getType() == "<REEL>")
        resval = *((float*) valg) < *((float*) vald);
      else if (og->getType() == "<CHAINE>")
        resval = *((string*) valg) < *((string*) vald);
    }
    else if (this->operateur == ">=")
    {
      if (og->getType() == "<ENTIER>")
        resval = *((int*) valg) >= *((int*) vald);
      else if (og->getType() == "<REEL>")
        resval = *((float*) valg) >= *((float*) vald);
      else if (og->getType() == "<CHAINE>")
        resval = *((string*) valg) >= *((string*) vald);
    }
    else if (this->operateur == "<=")
    {
      if (og->getType() == "<ENTIER>")
        resval = *((int*) valg)< *((int*) vald);
      else if (og->getType() == "<REEL>")
        resval = *((float*) valg) < *((float*) vald);
      else if (og->getType() == "<CHAINE>")
        resval = *((string*) valg) < *((string*) vald);
    }
    valeur->setValeur(&resval);
  }
  return valeur;
}

////////////////////////////////////////////////////////////////////////////////

void
NoeudOperateurBinaire::afficher(unsigned short indentation)
{
  Noeud::afficher(indentation);
  cout << "Noeud - Operateur Binaire \"" << this->
    operateur.getChaine() << "\" applique a : " << endl;
  operandeGauche->afficher(indentation + 1); // on affiche fils gauche et fils droit
  operandeDroit->afficher(indentation + 1); // en augmentant l'indentation
}

////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurUnaire::NoeudOperateurUnaire(Symbole operateur,
                                           Noeud * operandeGauche)
{
  this->operateur = operateur;
  this->operandeGauche = operandeGauche;
}

////////////////////////////////////////////////////////////////////////////////

Type *
NoeudOperateurUnaire::getValeur()
{
	//cout << "eval opun"<<endl;
  Type * valeur=NULL;
  Type * og = operandeGauche->getValeur();
  void * val;
  og->getValeur(&val);
  if (this->operateur == "-")
  {
    if (og->getType() == "<ENTIER>")
    {
      valeur = new Entier();
      *((int *) val) = -*((int *) val);
    }
    else if (og->getType() == "<REEL>")
    {
      valeur = new Reel();
      *((float *) val) = -*((float *)val);
    }
    else
      throw new ExTypeMismatch(string(string(typeid (Entier).name()) + " ou " + string(typeid (Reel).name())), typeid (og).name());
  }
  else if (this->operateur == "non")
  {
    if (og->getType() == "<ENTIER>")
    {
      valeur = new Entier();
      *((float *) val) = -*((float *)val);
    }
    else
      throw new ExTypeMismatch(string(string(typeid (Entier).name()) + " ou " + string(typeid (Reel).name())), typeid (og).name());
  }
  valeur->setValeur(&val);
  return valeur;
}

////////////////////////////////////////////////////////////////////////////////

void
NoeudOperateurUnaire::afficher(unsigned short indentation)
{
  Noeud::afficher(indentation);
  cout << "Noeud - Operateur Unaire \"" << this->
    operateur.getChaine() << "\" applique a : " << endl;
  operandeGauche->afficher(indentation + 1); // on affiche fils gauche et fils droit
}
////////////////////////////////////////////////////////////////////////////////
// NoeudSi
////////////////////////////////////////////////////////////////////////////////

NoeudSi::NoeudSi(Noeud *condition, Noeud * seqVrai, Noeud * siFaux)
{
  this->condition = condition;
  this->seqVrai = seqVrai;
  this->siFaux = siFaux;
}

Type * NoeudSi::getValeur()
{

  void * val;
  if (condition->getType() == "<BOOL>" || condition->getType() == "<ENTIER>")
  {
    condition->getValeur()->getValeur(&val);
    if (*((bool *)val))
      return seqVrai->getValeur();
    else
      return siFaux->getValeur();
  }
  else
    throw new ExTypeMismatch("<BOOL>|<ENTIER>",condition->getType());
}

void NoeudSi::afficher(unsigned short indentation)
{
  cout << "Noeud - Operateur Si:" << endl;
  this->condition->afficher(indentation);
  cout << "Si Vrai:" << endl;
  this->seqVrai->afficher(indentation + 1);
  cout << "siFaux" << endl;
  this->siFaux->afficher(indentation + 1);
  cout << endl;
}
////////////////////////////////////////////////////////////////////////////////
// NoeudLire
////////////////////////////////////////////////////////////////////////////////

NoeudLire::NoeudLire(Noeud *facteur)
{
  this->facteur = facteur;
}

Type * NoeudLire::getValeur()
{
  void * val;
  if (facteur->getType() == "<ENTIER>")
  {
  	int iv;
  	cin >>iv;
  	val = &iv;
  }
  else if (facteur->getType() == "<REEL>")
  {
  	float iv;
  	cin >>iv;
  	val = &iv;
  }
  else if(facteur->getType() == "<CHAINE>")
  {
   	float iv;
    cin >>iv;
    val = &iv;
  }
  facteur->getValeur()->setValeur(val);
  ((SymboleValue*)facteur)->definir();
  return facteur->getValeur();
}

void NoeudLire::afficher(unsigned short indentation)
{
  cout << "Noeud - Lire:"<< facteur->getType() << endl;
}
////////////////////////////////////////////////////////////////////////////////
// NoeudEcrire
////////////////////////////////////////////////////////////////////////////////

NoeudEcrire::NoeudEcrire(Noeud *facteur)
{
  this->facteur = facteur;
}

Type * NoeudEcrire::getValeur()
{
  void * val;
  facteur->getValeur()->getValeur(&val);
  if (facteur->getType() == "<ENTIER>")
    cout<< *((int *)val);
  else if (facteur->getType() == "<REEL>")
  	cout<< *((float *)val);
  else if(facteur->getType() == "<CHAINE>")
  	cout<< *((string *)val);
  return facteur->getValeur();
}

void NoeudEcrire::afficher(unsigned short indentation)
{
  cout << "Noeud - Ecrire:"<< facteur->getType() << endl;
}
////////////////////////////////////////////////////////////////////////////////
// NoeudBoucle
////////////////////////////////////////////////////////////////////////////////

NoeudBoucle::NoeudBoucle(Noeud *condition, Noeud * seq)
{
  this->condition = condition;
  this->seq = seq;
}

void NoeudBoucle::afficher(unsigned short indentation)
{
  indentation++;
  cout << "Noeud Boucle";
}

////////////////////////////////////////////////////////////////////////////////
// NoeudBoucle
////////////////////////////////////////////////////////////////////////////////

NoeudRepeter::NoeudRepeter(Noeud *condition, Noeud * seq)
: NoeudBoucle(condition, seq)
{
}

Type *
NoeudRepeter::getValeur()
{
  Type * valeur;
  void * val;
  do
  {
    getCondition()->getValeur()->getValeur(&val);
    valeur = getSeq()->getValeur();
  }
  while (*((int *)val));
  return valeur;
}

void NoeudRepeter::afficher(unsigned short indentation)
{
  NoeudBoucle::afficher(indentation);
  cout << "Noeud Repeter" << endl;
  this->getCondition()->afficher(indentation);
  this->getSeq()->afficher(indentation + 1);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudTantque
////////////////////////////////////////////////////////////////////////////////

NoeudTantque::NoeudTantque(Noeud *condition, Noeud * seq)
: NoeudBoucle(condition, seq)
{
}

Type * NoeudTantque::getValeur()
{
  Type * valeur=NULL;
  void * val;
  getCondition()->getValeur()->getValeur(&val);
  while (*((int *)val))
  {
    valeur = getSeq()->getValeur();
    getCondition()->getValeur()->getValeur(&val);
  }
  return valeur;
}

void NoeudTantque::afficher(unsigned short indentation)
{
  NoeudBoucle::afficher();
  cout << "Noeud Tantque" << endl;
  this->getCondition()->afficher(indentation);
  this->getSeq()->afficher(indentation + 1);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudPour
////////////////////////////////////////////////////////////////////////////////

Type * NoeudPour::getValeur()
{
  Type * valeur=NULL;
  void * cond;
  this->getCondition()->getValeur()->getValeur(&cond);
  for (init->getValeur(); *((int *)cond); affectation->getValeur())
  {
    valeur = getSeq()->getValeur();
  }
  return valeur;
}

void NoeudPour::afficher(unsigned short indentation)
{
  indentation++;
  NoeudPour::afficher();

}

