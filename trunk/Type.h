/*
 * Type.h
 *
 *  Created on: Dec 28, 2008
 *      Author: twk
 */

#ifndef TYPE_H_
#define TYPE_H_
#include <iostream>
#include <string>
using namespace std;
class Type
{
public:
	Type(){}
	virtual string getType()=0;
	virtual void getValeur(void ** val)=0;
	virtual void setValeur(void * val)=0;
};

class Entier: public Type
{
public:
	Entier(){}
  inline string getType(){return "<ENTIER>";}
  void getValeur(void ** val);
  void setValeur(void * val);
  void plus(Type * a,Type * b);
  friend ostream & operator<<(ostream & cout,Entier* ch);
private:
	int valeur;
};


class Chaine: public Type
{
public:
	inline string getType(){return "<CHAINE>";}
  void getValeur(void ** val);
  void setValeur(void * val);
  friend ostream & operator<<(ostream & cout,Chaine* ch);
private:
	string valeur;
};

class Reel: public Type
{
public:
	inline string getType(){return "<REEL>";}
  void getValeur(void ** val);
  void setValeur(void * val);
  friend ostream & operator<<(ostream & cout,Reel* ch);
private:
	float valeur;
};
/*class Bool: public Type
{
public:
	inline string getType(){return "<Bool>";}
  void getValeur(void ** val);
  void setValeur(void * val);
  friend ostream & operator<<(ostream & cout,Bool* ch);
private:
	bool valeur;
};
*/
#endif /* TYPE_H_ */
