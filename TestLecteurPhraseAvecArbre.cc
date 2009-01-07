#include <iostream>
using namespace std;
#include "LecteurPhraseAvecArbre.h"

int
main (int argc, char *argv[])
{
	char nomFich[255];
	if (argc != 2)
	{
		cout << "Usage : " << argv[0] << " nom_fichier_source" << endl << endl;
		cout << "Entrez le nom du fichier que voulez-vous interpreter : ";
		cin.getline (nomFich, sizeof (nomFich));
	}
	else
		strncpy (nomFich, argv[1], sizeof (nomFich));

	LecteurPhraseAvecArbre lp (nomFich);
	try
	{
		lp.analyse ();


		cout << endl << "Arbre Abstrait : " << endl;
		cout << "================" << endl;
		lp.getArbre ()->afficher ();
		cout << endl << "Table des symboles avant evaluation : " << lp.getTs ();
		cout << endl << "Evaluation de l'arbre (interpretation)..." << endl;
		lp.getArbre ()->getValeur ();
		cout << endl << "Table des symboles apres evaluation : " << lp.getTs ();
	}
	catch(Exception * e)
	{
		cout << "Caught an exception:"<<e->what()<<endl;
		delete e;
	}
	//Type * toto = new Entier();
	//	int valeur = 10;
	//	toto->setValeur(&valeur);
	//	void * val;
	//	toto->getValeur(&val);
	//	cout << *((int *)val);
	return 0;
}
