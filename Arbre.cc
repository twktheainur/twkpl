#include <stdlib.h>
#include "Arbre.h"
#include "Symbole.h"
#include "SymboleValue.h"

////////////////////////////////////////////////////////////////////////////////
// NoeudSeqInst
////////////////////////////////////////////////////////////////////////////////

NoeudSeqInst::NoeudSeqInst ():tabInst ()
{
}

////////////////////////////////////////////////////////////////////////////////
int
NoeudSeqInst::getValeur ()
{
  int valeur = 0;
  for (unsigned int i = 0; i < tabInst.size (); i++)
    valeur = tabInst[i]->getValeur ();	// on evalue chaque instruction de la séquence
  return valeur;		// par convention, resultat = valeur de la derniere instruction
}

////////////////////////////////////////////////////////////////////////////////
void
NoeudSeqInst::afficher (unsigned short indentation)
{
  Noeud::afficher (indentation);
  cout << "Noeud - Sequence de " << tabInst.size () << " instruction(s)" <<
    endl;
  for (unsigned int i = 0; i < tabInst.size (); i++)
    tabInst[i]->afficher (indentation + 1);	// on affiche les fils en augmentant l'indentation
}

////////////////////////////////////////////////////////////////////////////////
void
NoeudSeqInst::ajouteInstruction (Noeud * instruction)
{
  tabInst.push_back (instruction);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////

NoeudAffectation::NoeudAffectation (Noeud * variable, Noeud * expression)
{
  this->variable = variable;
  this->expression = expression;
}

////////////////////////////////////////////////////////////////////////////////
int
NoeudAffectation::getValeur ()
{
  int valeur = expression->getValeur ();	// on évalue l'expression
  ((SymboleValue *) variable)->setValeur (valeur);	// on affecte la variable
  return valeur;		// par convention, une affectation a pour valeur la valeur affectée
}

////////////////////////////////////////////////////////////////////////////////
void
NoeudAffectation::afficher (unsigned short indentation)
{
  Noeud::afficher (indentation);
  cout << "Noeud - Affectation" << endl;
  variable->afficher (indentation + 1);	// on affiche variable et expression
  expression->afficher (indentation + 1);	// en augmentant l'indentation
}

////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurBinaire::NoeudOperateurBinaire (Symbole operateur,
					      Noeud * operandeGauche,
					      Noeud * operandeDroit)
{
  this->operateur = operateur;
  this->operandeGauche = operandeGauche;
  this->operandeDroit = operandeDroit;
}

////////////////////////////////////////////////////////////////////////////////
int
NoeudOperateurBinaire::getValeur ()
{
  int valeur = 0;
  int og = operandeGauche->getValeur ();
  int od = operandeDroit->getValeur ();
  if (this->operateur == "+")
    valeur = og + od;
  else if (this->operateur == "-")
    valeur = og - od;
  else if (this->operateur == "*")
    valeur = og * od;
  else if (this->operateur == "et")
      valeur = og && od;
  else if (this->operateur == "ou")
      valeur = og || od;
  else if (this->operateur == "==")
      valeur = og == od;
  else if (this->operateur == "!=")
      valeur = og != od;
  else if (this->operateur == ">")
        valeur = og > od;
  else if (this->operateur == "<")
        valeur = og < od;
  else if (this->operateur == ">=")
        valeur = og >= od;
  else if (this->operateur == "<=")
        valeur = og <= od;
  else if(this->operateur == "/")
    {
      if (od != 0)
        valeur = og / od;
      else
        {
          cout << "Erreur pendant l'interpretation : division par zero" << endl;
          exit (0);			// plus tard on levera une exception
        }
    }
  return valeur;
}

////////////////////////////////////////////////////////////////////////////////
void
NoeudOperateurBinaire::afficher (unsigned short indentation)
{
  Noeud::afficher (indentation);
  cout << "Noeud - Operateur Binaire \"" << this->
    operateur.getChaine () << "\" applique a : " << endl;
  operandeGauche->afficher (indentation + 1);	// on affiche fils gauche et fils droit
  operandeDroit->afficher (indentation + 1);	// en augmentant l'indentation
}

////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurUnaire::NoeudOperateurUnaire (Symbole operateur,
                                              Noeud * operandeGauche)
{
  this->operateur = operateur;
  this->operandeGauche = operandeGauche;
}

////////////////////////////////////////////////////////////////////////////////
int
NoeudOperateurUnaire::getValeur ()
{
  int valeur = 0;
  int og = operandeGauche->getValeur ();
  if (this->operateur == "-")
    valeur =-og;
  else if (this->operateur == "non")
    valeur = !og;
  return valeur;
}

////////////////////////////////////////////////////////////////////////////////
void
NoeudOperateurUnaire::afficher (unsigned short indentation)
{
  Noeud::afficher (indentation);
  cout << "Noeud - Operateur Unaire \"" << this->
    operateur.getChaine () << "\" applique a : " << endl;
  operandeGauche->afficher (indentation + 1);   // on affiche fils gauche et fils droit
}
////////////////////////////////////////////////////////////////////////////////
// NoeudSi
////////////////////////////////////////////////////////////////////////////////
NoeudSi::NoeudSi (Noeud *condition,Noeud * seqVrai, Noeud * siFaux)
{
  this->condition=condition;
  this->seqVrai=seqVrai;
  this->siFaux=siFaux;
}

int NoeudSi::getValeur ()
{
  if(condition->getValeur())
    return seqVrai->getValeur();
  else
    return siFaux->getValeur();
}
void NoeudSi::afficher (unsigned short indentation )
{
  cout << "Noeud - Operateur Si:"<<endl;
  this->condition->afficher (indentation);
  cout << "Si Vrai:" << endl;
  this->seqVrai->afficher (indentation +1);
  cout << "siFaux" << endl;
  this->siFaux->afficher(indentation+1);
  cout <<endl;
}
////////////////////////////////////////////////////////////////////////////////
// NoeudBoucle
////////////////////////////////////////////////////////////////////////////////
NoeudBoucle::NoeudBoucle (Noeud *condition,Noeud * seq)
{
  this->condition=condition;
  this->seq=seq;
}
void NoeudBoucle::afficher (unsigned short indentation )
{
  cout << "Noeud Boucle";
}

////////////////////////////////////////////////////////////////////////////////
// NoeudBoucle
////////////////////////////////////////////////////////////////////////////////
NoeudRepeter::NoeudRepeter (Noeud *condition,Noeud * seq)
:NoeudBoucle(condition,seq){}
int NoeudRepeter::getValeur()
{
  int valeur=0;
  do
    {
      valeur=getSeq()->getValeur();
    }
  while(getCondition()->getValeur());
  return valeur;
}

void NoeudRepeter::afficher (unsigned short indentation )
{
  NoeudBoucle::afficher(indentation);
  cout << "Noeud Repeter" << endl;
  this->getCondition()->afficher(indentation);
  this->getSeq()->afficher(indentation+1);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudTantque
////////////////////////////////////////////////////////////////////////////////
NoeudTantque::NoeudTantque (Noeud *condition,Noeud * seq)
:NoeudBoucle(condition,seq)
{}

int NoeudTantque::getValeur()
{
  int valeur=0;
  while(getCondition()->getValeur())
    {
      valeur=getSeq()->getValeur();
    }
  return valeur;
}
void NoeudTantque::afficher (unsigned short indentation )
{
  NoeudBoucle::afficher();
  cout << "Noeud Tantque" << endl;
  this->getCondition()->afficher(indentation);
  this->getSeq()->afficher(indentation+1);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudPour
////////////////////////////////////////////////////////////////////////////////

int NoeudPour::getValeur()
{
  int valeur=0;
  int i;
 for(getCondition()->getValeur();getCondition()->getValeur();affectation->getValeur())
 {
   valeur=getSeq()->getValeur();
 }
  return valeur;
}
void NoeudPour::afficher (unsigned short indentation )
{
  NoeudPour::afficher();
  
}

