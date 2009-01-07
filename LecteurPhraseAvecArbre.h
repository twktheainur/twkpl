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
		Noeud *programme ();		//  declarer <declaration >debut <seq_inst> fin FIN_FICHIER
		Noeud *declaration();  //<declaration>::= {<TYPE>:<VARIABLE>;}
		Noeud *seqInst ();		//     <seqInst> ::= <inst> ; { <inst> ; }
		Noeud *inst ();		    // <inst> ::= <affectation> | <instSi>|<instTantque>|<instRepeter>|<instPour>|<instLire>|<instEcrire>
		Noeud *affectation ();	// <affectation> ::= <variable> = <expression>


		Noeud *expression ();		//  <expression> ::= <facteur> { <opBinaire> <facteur> }
		Noeud *facteur ();	       // <facteur> ::= <entier>  |<chaine>|<reel>|  <variable>  |  - <facteur>  |  ( <expression> )
		Noeud *expBool();// <facteur> ::= <entier>  |<chaine>|<reel>|  <variable>  |  - <facteur>  |  ( <expression> )
		Noeud *terme ();                // <terme> ::= <facteur> { <opAdd> <facteur> }
		Noeud *relation ();              //<relation> ::= <expression> {<opRel> <expression>}
		Symbole opBinaire ();		//   <opBinaire> ::= + | - | *  | /
		Symbole opUnaire ();        //<opUnaire>::==-|non

		Noeud * instSi(bool rec=0);//<instSi> ::= si ( <expBool> ) <seqInst> { sinonsi ( <expBool> ) <seqInst> }
		//  [ sinon <seqInst> ] finsi
		Noeud * instTq();//<instTq> ::= tantque ( <expBool> ) <seqInst> fintantque
		Noeud * instRepeter();//<instTq> ::= repeter <seqInst>  tantque ( <expBool> )
		Noeud * instPour();//<instPour> ::= pour (<affectation>;<expBool>;<affectation>>  ) <seqInst> finpour

		Noeud * instLire();//<instLire> ::= lire(<variable>|<entier>|<reel>|chaine)
		Noeud * instEcrire();//<instEcrire> ::= ecrire(<facteur>)
		Noeud * instEcrireln();//<instEcrireln> ::= ecrire(<facteur>)


		// outils pour se simplifier l'analyse syntaxique
		void testerSymCour (string ch);	// si symbole courant != ch, erreur : on throw une exception, sinon rien
		void sauterSymCour (string ch);	// si symbole courant == ch, on passe au symbole suivant, sinon erreur : exception
		void erreur (ExType_t type,string msg="",string msg1="");// affiche les message d'erreur mess et jette une exception
};

#endif /* LECTEURPHRASEAVECARBRE_H_ */
