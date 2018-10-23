#ifndef ERR_H
#define ERR_H
#include<iostream>
using namespace std;

typedef enum ERR_T
{
	E_NONE,
	E_NULL,
	E_NOMEM,
	E_NO_SRC,
	E_NO_DEST,
	E_BADGRAPH
}ERR_T;

void printError(ERR_T err, ostream& out = cerr);

#endif
