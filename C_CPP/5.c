
#include "spoj.h"
#include <stdlib.h>
 
using namespace std;
 
// <0-255> normal, - 1 white, -2 eof
int getChar(FILE *f, bool ignWhite) {
	bool white;
	int ch;
	do {
		if ((ch = getc(f))==EOF)
			return -2;
                if (ch=='\r') continue;
		if (ch==' ' || ch=='\t')
			white=true;
		else white=false;
	} while (ignWhite && white);
	if (white)
		return -1;
	return ch;
}
 
int main(void) {
	spoj_init();
	int ch1 = getChar(spoj_t_out, true);
	int ch2 = getChar(spoj_p_out, true);
	while (ch1 == ch2) {
		if (ch1==-2)
			exit(SPOJ_RV_POSITIVE);
		bool ignWhite;
		if (ch1==-1)
			ignWhite =true;
		else
			ignWhite=false;
		ch1 = getChar(spoj_t_out, ignWhite);
		ch2 = getChar(spoj_p_out, ignWhite);
	};
	if (ch1 == -2 && ch2 == -1 && getChar(spoj_p_out, true)==-2 )
		return SPOJ_RV_POSITIVE;
	if (ch2 == -2 && ch1 == -1 && getChar(spoj_t_out, true)==-2)
		return SPOJ_RV_POSITIVE;
	return SPOJ_RV_NEGATIVE;
}
