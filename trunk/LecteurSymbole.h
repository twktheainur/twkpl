#ifndef LECTEURSYMBOLE_H_
#define LECTEURSYMBOLE_H_

#include <fstream>
#include <string>
using namespace std;

#include "LecteurCaractere.h"
#include "Symbole.h"

class LecteurSymbole
{
public:
  LecteurSymbole (string nomFich);	// résultat : symCour = premier symbole du fichier nomFich
  void suivant ();		// passe au symbole suivant du fichier
  inline Symbole getSymCour ()
  {
    return symCour;
  }				// accesseur
  inline unsigned int getLigne ()
  {
    return ligne;
  }				// accesseur
  inline unsigned int getColonne ()
  {
    return colonne;
  }				// accesseur

private:
  LecteurCaractere lc;		// le lecteur de caracteres
  Symbole symCour;		// le symbole courant du lecteur de symboles
  unsigned int ligne, colonne;	// coordonnees, dans le fichier, du symbole courant
  void sauterSeparateurs ();	// saute dans lc une suite de separateurs consecutifs
  bool sauterCommentaire ();	// saute dans lc une ligne de commentaire
  string motSuivant ();		// lit dans lc la chaine du prochain symbole et la renvoie en resultat
};

#endif /* LECTEURSYMBOLE_H_ */
