#ifndef MIPS_H_INCLUDED
#define MIPS_H_INCLUDED
void func_mid();
int getindex(string name2);
int getparalel(string name2);
int getparan(string name);
int getindex2(string name2,int level);
int findingobel(string name2);
int getlevel(string name2);
int getvartype2(string name2,int level);
int getvartype(string name2);
int getrfuncsize(int level);
int getfuncsize(string name2);
int getparan2(string name2);
int getfunclvl(string name2);
void getallfuncsize();
int getfuncpara(string funcname,int paranums);
#endif // MIPS_H_INCLUDED
