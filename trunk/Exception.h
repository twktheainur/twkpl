//
// File:   Exception.h
// Author: twk
//
// Created on December 25, 2008, 6:54 PM
//
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#ifndef _EXCEPTION_H
#define	_EXCEPTION_H
using namespace std;

typedef enum ExType
{
  E_UNHANDLED,
  E_SYNTAX,
  E_TYPE,
  E_UNDEFVAR
}ExType_t;

class Exception: public exception
{
public:
  Exception()
  :line(-1),col(-1)
   {}
  Exception(int line,int col)
  :line(line),col(col)
  { }
 ~Exception() throw()
 {}
virtual const char * what()
{
  stringstream err;
  if(col>=0 || line>=0)
  {
    err << "Erreur";
    err <<": l" << line << "-c"<<col;
  }
  return err.str().data();
}
inline int getLine(){return line;}
inline int getCol(){return col;}
private:
  int line;
  int col;
};

class ExVarUndef:Exception
{
public:
  ExVarUndef(std::string symbol,int line=-1,int col=-1)
          :Exception(line,col),symbol(symbol)
  {}
  ~ExVarUndef() throw()
  {}
  inline const char * what()
  {
    stringstream err;
    err << Exception::what() << "Reference indefinie vers:"
          << symbol;
    return err.str().data();
  }
private:
  string symbol;
};

class ExTypeMismatch:public Exception
{
public:
  ExTypeMismatch(string expected, string found,int line=-1,int col=-1) throw()
  :Exception(line,col)
  {
     this->expected =expected;
     this->found=found;
  }
  inline const char * what() throw()
  {
  	stringstream strdisp;
  	strdisp << "Expected: "<<expected<<" Found: "<<found;
  	return strdisp.str().data();
  }
  ~ExTypeMismatch() throw()
  {}
private:
	string expected;
	string found;
};

class ExSyntaxError: public Exception
{
public:
  ExSyntaxError(string expected,string found,int col,int line) throw()
  :Exception(col,line)
  {
  	this->expected=expected;
  	this->found=found;
  }
  ~ExSyntaxError() throw(){}
  inline const char * what() throw()
  {
    stringstream strdisp;
    strdisp << "Syntax Error l"<<getLine()<<"c"<<getCol() <<"- Expected: "<<expected << " Found: " << found;
    return strdisp.str().data();
  }
private:
	string expected;
	string found;

};


#endif	/* _EXCEPTION_H */

