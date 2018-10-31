#include<iostream>
#include "err.h"
using namespace std;


void printError(ERR_T err, ostream& out)
{
	switch(err)
	{
		case E_NONE: 		out << "Error: 	No Error"		<<endl; break;
		case E_NULL: 		out << "Error: 	Null Reference"		<<endl; break;
		case E_NOMEM: 		out << "Error: 	Memory Alloc"		<<endl; break;
		case E_NO_SRC: 		out << "Error:	No SRC Node"		<<endl; break;
		case E_NO_DEST: 	out << "Error:	No DEST Node"		<<endl; break;
		case E_NO_EDGE: 	out << "Error:	No Edge"		<<endl; break;
		case E_BADGRAPH:	out << "Error:	Courrupted Graph"	<<endl; break;
		default:		out << "Error: 	Unidentified Error"	<<endl; break;
	}
	return;
}
