/*
 * Type.cpp
 *
 *  Created on: Dec 28, 2008
 *      Author: twk
 */

#include "Type.h"
//Accesseurs Entier

void Entier::getValeur(void ** val)
{
  void * tmp = &valeur;
  *val = tmp;
}

void Entier::setValeur(void * val)
{
  valeur = *((int *) val);
}
//Accesseurs Chaine

void Chaine::getValeur(void ** val)
{
  void * tmp = &valeur;
  *val = tmp;
}

void Chaine::setValeur(void * val)
{
  valeur = *((string *) val);
}
//Accesseurs Reels

void Reel::getValeur(void ** val)
{
  void * tmp = &valeur;
  *val = tmp;
}

void Reel::setValeur(void * val)
{
  valeur = *((float *) val);
}

//  void Bool::getValeur(void ** val)
//  {
//    void * tmp = &valeur;
//    val = &tmp;
//  }
//  void Bool::setValeur(void * val)
//  {
//    valeur=*((bool *)val);
//  }

//Operateurs de flux pour chaque type

ostream & operator<<(ostream & cout, Chaine* ch)
{
  void * val;
  ch->getValeur(&val);
  return cout << *((string *) val);
}

ostream & operator<<(ostream & cout, Entier* ch)
{
  void * val;
  ch->getValeur(&val);
  return cout << *((int *) val);
}

ostream & operator<<(ostream & cout, Reel* ch)
{
  void * val;
  ch->getValeur(&val);
  return cout << *((float *) val);
}
