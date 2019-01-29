#ifndef SYNTAX_H_INCLUDED
#define SYNTAX_H_INCLUDED
#define	SYMTOP	10000
#include<vector>
#include<stack>
using namespace std;
void func_mid();
string divzero(string str);
void read();
void program();
void con_dcl();
void con_def();
void integer();
void var_dcl();
void var_def();
void r_func_def();
void head_statement();
void paratable();
void comstatement();
void statementlist();
void statement();
void v_func_def();
void maindef();
void exp(string &expname,int *expt);//name type
void term(string &termname,int *termt);
void factor(string &factorname,int *facotrt);
void asstate();
void ifstate();
void condition(int flag,string label,string elsestr);
void whilestate();
void constant();
void switchstate();
void switchtable(string &casevar);
void casestate(string &casevar);
void defaultstate();
void Rfcallstate(string &factorn,int *factortype);
void Ufcallstate();
void Vparatable(string paraname);
void printfstate();
void scanfstate();
void returnstate();
void insert(string name2,string kind2,int type2,string value,int currange);
void addarr(int k,int type);
void printftable();
void funcsize(string name2);
string newvar();
string newstrlable();
int gettype(string str);
int getfunctype(string name);
int insymtable(string str);
string newlab();
bool repeattable(string name);
extern string strlwr2(string name);
extern FILE * output_grammar_pointer;
extern FILE * output_midcoder_pointer;
extern string v0;
extern string v1;
extern string v2;
extern int TableTop;						// 符号表栈顶指针
extern int LVL;						    // 层号
extern int TableIndex[SYMTOP];				// 分程序索引表
extern int Labeln;
extern int Labelab;
extern int Labelstr;
extern int offset;
extern bool returnflag;
extern bool mainflag;
extern string newlabel;
extern string whilestr;
extern stack<string> defaultlabel;
extern string strlabel;
extern vector <string> strlabelarr;
extern int nvpara;
extern int strcount;
extern string name;        //名字
extern string val;         //常量的值
extern string sname;
extern string leftterm;
extern string rightterm;
extern string leftname;
extern int types;
struct symTable{                    //符号表
	 string name;			        // 名字
	 int type;						// 类型0 int 1 char
	 string kind;					// 种类,常量、变量、函数、数组等
	 string val;					    // 常量的值
	 int lev;						// 层号
	 int range;						// 数组大小、范围
	 int offs;						//偏移量
};
extern symTable symtable[SYMTOP];
#endif // SYNTAX_H_INCLUDED
