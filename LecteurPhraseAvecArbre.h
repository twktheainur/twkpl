#ifndef LECTEURPHRASEAVECARBRE_H_
#define LECTEURPHRASEAVECARBRE_H_

#include "Symbole.h"
#include "LecteurSymbole.h"
#include "TableSymboles.h"
#include "Arbre.h"

#include <string>
using namespace std;

class LecteurPhraseAvecArbre
{
public:
  LecteurPhraseAvecArbre (string nomFich);	// Construit un lecteur de phrase pour interpreter
  //  le programme dans le fichier nomFich

  void analyse ();		// Si le contenu du fichier est conforme à la grammaire,
  //   cette méthode se termine normalement et affiche un message "Syntaxe correcte".
  //   la table des symboles (ts) et l'arbre abstrait (arbre) auront été construits
  // Sinon, le programme sera interrompu (exit).

  inline TableSymboles getTs ()
  {
    return ts;
  }				// accesseur
  inline Noeud *getArbre ()
  {
    return arbre;
  }				// accesseur


private:
  LecteurSymbole ls;		// le lecteur de symboles utilisé pour analyser le fichier
  TableSymboles ts;		// la table des symboles valués
  Noeud *arbre;			// l'arbre abstrait

  // implémentation de la grammaire
  Noeud *programme ();		//   <programme> ::= debut <seqInst> fin FIN_FICHIER
  Noeud *seqInst ();		//     <seqInst> ::= <inst> ; { <inst> ; }
  Noeud *inst ();		//        <inst> ::= <affectation>
  Noeud *affectation ();	// <affectation> ::= <variable> = <expression>
  Noeud *expression ();		//  <expression> ::= <facteur> { <opBinaire> <facteur> }
  Noeud *facteur ();	        //<entier> | <variable> | <opUnaire> <expBool> | ( <expBool> )
  Noeud *expBool();
  Noeud *terme ();                // <terme> ::= <facteur> { <opAdd> <facteur> }
  Noeud *relation ();              //<relation> ::= <expression> {<opRel> <expression>}
  Symbole opBinaire ();		//   <opBinaire> ::= + | - | *  | /
  Symbole opUnaire ();

  Noeud * instSi(bool rec=0);

  // outils pour se simplifier l'analyse syntaxique
  void testerSymCour (string ch);	// si symbole courant != ch, erreur : on arrete le programme, sinon rien
  void sauterSymCour (string ch);	// si symbole courant == ch, on passe au symbole suivant, sinon erreur : on arrete
  void erreur (string mess);	// affiche les message d'erreur mess et arrete le programme
};

#endif /* LECTEURPHRASEAVECARBRE_H_ */
