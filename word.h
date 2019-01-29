#ifndef WORD_H_INCLUDED
#define WORD_H_INCLUDED
#include<string>
using namespace std;
extern FILE *input_file_pointer;
extern FILE *output_words_pointer;
extern FILE *output_error;
extern char ch;
extern string token;
extern int num;
extern int k;
enum S{
    CONSTSY = 1, INTSY, CHARSY, VOIDSY, MAINSY, SCANFSY, PRINTFSY, IDSY,
    RETURNSY, IFSY, ELSESY, DOSY, WHILESY,SWITCHSY,CASESY,DEFAULTSY,
    LBRACESY = 30, RBRACESY, LPARSY, RPARSY, LBRACKETSY, RBRACKETSY, SEMISY, COMMASY, PLUSSY, MINUSSY, STARSY, SLASHSY,
    ISSY, LSSY, NBGSY, BGSY, NLSSY, NEQSY, EQUSY,COLONSY,
    INTCON = 60, CHARCON, STRINGCON,
    ENDSY = 99
};
extern enum S resultvalue;
extern enum S symbol0;
extern enum S symbol1;
extern enum S symbol2;
void error(int n);
char getch();
void ungetch(char c);
void catToken();
void clearToken();
S searchreserve();
int isColon(char c);
int isComma(char c);
int isSemi(char c);
int isEqu(char c);
int isDy(char c);
int isSy(char c);
int isPlus(char c);
int isMinus(char c);
int isMult(char c);
int isDiv(char c);
int Lpar(char c);
int Rpar(char c);
int Lzpar(char c);
int Rzpar(char c);
int Lhpar(char c);
int Rhpar(char c);
int Dyu(char c);
int Xyu(char c);
int Gth(char c);
int transNum(string s);
bool isletter(char c);
void getsym();

#endif // WORD_H_INCLUDED
