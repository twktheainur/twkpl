// Contient toutes les spécifications de classes nécessaires
// pour représenter l'arbre abstrait
#ifndef ARBRE_H_
#define ARBRE_H_

#include <vector>
#include <iostream>
#include <iomanip>
#include <map>
using namespace std;
#include "Symbole.h"
#include "Exception.h"
#include "Type.h"

////////////////////////////////////////////////////////////////////////////////

class Noeud
{
  // classe abstraite dont dériveront toutes les classes servant à représenter l'arbre abstrait
  // Remarque : la classe ne contient aucun constructeur
public:
  virtual Type * getValeur() = 0; // méthode pure (non implémentée) qui rend la classe abstraite
  virtual string getType()=0;
  virtual void afficher(unsigned short indentation = 0)
  {
    cout << setw(4 * indentation) << " ";
  }
  // pour afficher un noeud avec un décalage (indentation) proportionnel à son niveau dans l'arbre

  virtual ~Noeud()
  {
  } // présence d'un destructeur virtuel conseillée dans les classes abstraites
  //  virtual inline const type_info & getType(){return type;}
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class NoeudProgramme : public Noeud
{
  // classe pour représenter un noeud "Programme"
public:

  NoeudProgramme(Noeud * decl, Noeud * seqi)
  {
    this->seqInst = seqi;
    this->seqDecl=decl;
  }
  inline string getType(){return "<ENTIER>";}
  ~NoeudProgramme()
  {
  } // à cause du destructeur virtuel de la classe Noeud
  Type * getValeur(); // évalue chaque instruction de la séquence
  void afficher(unsigned short indentation = 0); // affiche la séquence d'instructions
  void ajouteInstruction(Noeud * instruction); // ajoute une instruction à la séquence

private:
  Noeud * seqInst;
  Noeud * seqDecl;
};
////////////////////////////////////////////////////////////////////////////////

class NoeudSeqInst : public Noeud
{
  // classe pour représenter un noeud "sequence d'instruction"
  // qui a autant de fils que d'instructions dans la séquence
public:
  NoeudSeqInst(); // construit une séquence d'instruction vide
  inline string getType(){return "<ENTIER>";}
  ~NoeudSeqInst()
  {
  } // à cause du destructeur virtuel de la classe Noeud
  Type * getValeur(); // évalue chaque instruction de la séquence
  void afficher(unsigned short indentation = 0); // affiche la séquence d'instructions
  void ajouteInstruction(Noeud * instruction); // ajoute une instruction à la séquence

private:
  vector < Noeud * >tabInst; // pour stocker les instructions de la séquence
};
////////////////////////////////////////////////////////////////////////////////

class NoeudDeclaration : public Noeud
{
  // classe pour représenter un noeud "sequence d'instruction"
  // qui a autant de fils que d'instructions dans la séquence
public:
  NoeudDeclaration(); // construit une séquence d'instruction vide

  ~NoeudDeclaration()
  {
  } // à cause du destructeur virtuel de la classe Noeud
  inline string getType(){return "<ENTIER>";}
  Type * getValeur(); // Declare chaque variable dans la table
  void afficher(unsigned short indentation = 0); // affiche la séquence d'instructions
  void ajouteDeclaration(string symbolName, string type); // ajoute une instruction à la séquence

private:
  map <string, string> mapDecl; // pour stocker les instructions de la séquence
};
////////////////////////////////////////////////////////////////////////////////

class NoeudAffectation : public Noeud
{
  // classe pour représenter un noeud "affectation"
  // composé de 2 fils : la variable et l'expression qu'on lui affecte
public:
  NoeudAffectation(Noeud * variable, Noeud* expression); // construit une affectation

  ~NoeudAffectation()
  {
  } // à cause du destructeur virtuel de la classe Noeud
  inline string getType()
  {
  	if(variable->getType()!=expression->getType())
  		throw ExTypeMismatch(variable->getType(),expression->getType());
  	return variable->getType();
  }
  Type * getValeur(); // évalue l'expression et affecte sa valeur à la variable
  void afficher(unsigned short indentation = 0); // affiche l'affectation

private:
  Noeud * variable;
  Noeud *expression;
};

////////////////////////////////////////////////////////////////////////////////

class NoeudOperateurBinaire : public Noeud
{
  // classe pour représenter un noeud "opération arithmétique" composé d'un opérateur (+-*/)
  // et de 2 fils : l'opérande gauche et l'opérande droit
public:
  NoeudOperateurBinaire(Symbole operateur, Noeud * operandeGauche,
                        Noeud * operandeDroit);
  // construit une opération binaire : operandeGauche operateur OperandeDroit

  ~NoeudOperateurBinaire()
  {
  } // à cause du destructeur virtuel de la classe Noeud
  Type * getValeur(); // évalue l'operande gauche, l'operande droit et applique l'opérateur
  void afficher(unsigned short indentation = 0); // affiche l'opération
  inline string getType()
  {
  	if(operandeDroit->getType()!=operandeGauche->getType())
  		throw ExTypeMismatch(operandeDroit->getType(),operandeGauche->getType());
  	return operandeDroit->getType();
  }
  inline Symbole getSymbole()
  {
    return operateur;
  }

private:
  Symbole operateur;
  Noeud *operandeGauche;
  Noeud *operandeDroit;
};

////////////////////////////////////////////////////////////////////////////////

class NoeudOperateurUnaire : public Noeud
{
  // classe pour représenter un noeud "opération arithmétique" composé d'un opérateur (+-*/)
  // et de 2 fils : l'opérande gauche et l'opérande droit
public:
  NoeudOperateurUnaire(Symbole operateur, Noeud * operandeDroit);
  // construit une opération binaire : operandeGauche operateur OperandeDroit
  inline string getType(){return operandeGauche->getType();}
  ~NoeudOperateurUnaire()
  {
  } // à cause du destructeur virtuel de la classe Noeud
  Type * getValeur(); // évalue l'operande gauche, l'operande droit et applique l'opérateur
  void afficher(unsigned short indentation = 0); // affiche l'opération

private:
  Symbole operateur;
  Noeud *operandeGauche;
};

class NoeudSi : public Noeud
{
public:
  NoeudSi(Noeud *condition, Noeud * seqVrai, Noeud * seqFaux);

  ~NoeudSi()
  {
  }
  inline string getType()
  {
  	if(condition->getType()!="<BOOL>")
  		throw ExTypeMismatch(condition->getType(),"<BOOL>");
  	return condition->getType();
  }
  Type * getValeur();
  void afficher(unsigned short indentation = 0);

private:
  Noeud *condition;
  Noeud *seqVrai;
  Noeud *siFaux;
};

class NoeudLire : public Noeud
{
public:
  NoeudLire(Noeud *facteur);
  ~NoeudLire()
  {
  }
  inline string getType()
  {
  	return facteur->getType();
  }
  Type * getValeur();
  void afficher(unsigned short indentation = 0);
private:
  Noeud *facteur;
};

class NoeudEcrire : public Noeud
{
public:
  NoeudEcrire(Noeud *facteur);
  ~NoeudEcrire()
  {
  }
  inline string getType()
  {
  	return facteur->getType();
  }
  Type * getValeur();
  void afficher(unsigned short indentation = 0);
private:
  Noeud *facteur;
};

class NoeudBoucle : public Noeud
{
public:
  NoeudBoucle(Noeud *condition, Noeud * seq);

  ~NoeudBoucle()
  {
  }
  virtual Type * getValeur() = 0;
  inline string getType(){return "<ENTIER>";}
  void afficher(unsigned short indentation = 0);

  inline Noeud* getCondition()
  {
    return condition;
  }

  inline Noeud* getSeq()
  {
    return seq;
  }
private:
  Noeud *condition;
  Noeud *seq;
};

class NoeudTantque : public NoeudBoucle
{
public:
  NoeudTantque(Noeud *condition, Noeud * seq);

  ~NoeudTantque()
  {
  }
  Type * getValeur();
  void afficher(unsigned short indentation = 0);
};

class NoeudRepeter : public NoeudBoucle
{
public:
  NoeudRepeter(Noeud *condition, Noeud * seq);

  ~NoeudRepeter()
  {
  }
  Type * getValeur();
  void afficher(unsigned short indentation = 0);
};

class NoeudPour : public NoeudBoucle
{
public:

  NoeudPour(Noeud * init, Noeud *condition, Noeud * affect, Noeud * seq)
  : NoeudBoucle(condition, seq)
  {
    this->affectation = affect;
    this->init = init;
  }

  ~NoeudPour()
  {
  }
  Type * getValeur();
  void afficher(unsigned short indentation = 0);
private:
  Noeud * affectation;
  Noeud * init;
};
#endif /* ARBRE_H_ */
