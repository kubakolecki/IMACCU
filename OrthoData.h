#include "pch.h"

struct OrthoData
{
	GLdouble left;
	GLdouble right;
	GLdouble top;
	GLdouble bottom;
	GLdouble nearr; //Pisane przez 'rr' bo nazwa near jest ju¿ zastrze¿ona dyrektyw¹ #define. By³y przez to jaja.
	GLdouble farr; //Pisane przez 'rr' bo nazwa near jest ju¿ zastrze¿ona dyrektyw¹ #define
};
