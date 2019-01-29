#include<iostream>
#include<string.h>
#include<string>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<stack>
#include"word.h"
#include"syntax.h"
#include"mips.h"
using namespace std;
FILE * output_grammar_pointer;
FILE * output_midcoder_pointer;
string v0;
string v1;
string v2;
string arrayname;
int TableTop=0;						// 符号表栈顶指针
int LVL=0;						    // 层号
int TableIndex[SYMTOP];				// 分程序索引表
int Labeln=0;
int Labelab=1;
int Labelstr=0;
int nstr=0;
int offset=0;
bool expprintflag=false;
bool returnflag=false;
bool mainflag=false;
bool arrayflag=false;
bool minusarray=false;
string newlabel;
string whilestr;
int defaultcount=-1;
stack <string> defaultlabel;
stack <string> whilelabel;
string strlabel;
vector<string> strlabelarr;
int nvpara=0;
int strcount=0;
/*struct symTable{                    //符号表
	string name;			        // 名字
	int type;						// 类型0 int 1 char
	string kind;					// 种类,常量、变量、函数、数组等
	string val;					    // 常量的值
	int lev;						// 层号
	int range;						// 数组大小、范围
	int offs;						//偏移量
};*/
symTable symtable[SYMTOP];		// 符号表
string name;        //名字
string val;         //常量的值
string sname;
string leftterm;
string rightterm;
string leftname;
int types;
void insert(string name2,string kind2,int type2,string value,int currange){
	int i=0;
	if(TableTop>=SYMTOP)error(37);
	if(name2!=""){
		if(repeattable(name2)==true)
			error(38);
	}
	symtable[TableTop].kind=kind2;
	symtable[TableTop].type=type2;
	symtable[TableTop].lev=LVL;
	symtable[TableTop].range=currange;
	symtable[TableTop].val=value;
	symtable[TableTop].offs=TableTop;//计算偏移量
	name=strlwr2(name);//不区分大小写
	symtable[TableTop].name=name2;
	if(kind2=="array"){
        addarr(currange,type2);//分配数组空间
		TableTop--;
	}
	TableTop++;
	if(kind2=="func"){
        TableIndex[++LVL]=TableTop;
	}
}
string strlwr2(string str){
    string res=str;
    for(int i=0;i<res.size();i++){
        if(res[i]>='A'&&res[i]<='Z'){
            res[i]+=32;
        }
    }
    return res;
}
bool repeattable(string name2){
    int index=TableTop-1;
    int i;
    name2=strlwr2(name2);
    for(i=TableTop-1;i>=0;i--){
        if(symtable[i].name==name2&&symtable[i].lev==LVL){
            //cout<<name<<" "<<name2<<" "<<LVL<<endl;
            return true;
        }
    }
    return false;
}
void addarr(int k,int type){
	int i=0;
	TableTop++;
	while(i<k){
		insert("","",type,"",-1);
		i++;
	}
}
void printftable(){
	int i=TableTop-1;
	while (i>=0){
		printf("%s %s %d %s %d %d  %d\n",symtable[i].name.c_str(),symtable[i].kind.c_str(),symtable[i].type,symtable[i].val.c_str(),symtable[i].lev,symtable[i].offs,symtable[i].range);
		i--;
	}
}
/*int countarray(string name2,int offset){
    int ans=1,level;
    level=symtable[offset].lev+1;
	int i=offset+1;
	while(symtable[i].lev==level){
        if(symtable[i].kind=="array"){
            ans+=symtable[i].range;
        }
	}
    return ans;
}*/
void  funcsize(string name2){
	int func_range=0,i;
	for(i=TableTop-1;i>=0;i--){
        if(symtable[i].name==name2&&symtable[i].kind=="func")break;
	}
	func_range=(TableTop-i)*4;
	symtable[i].range=func_range;
}
string newvar(){
    string tmp(10,'\0');
    string counter;
	int i=0;
	char *ch;
	tmp[0]='$';tmp[1]='v';tmp[2]='a';tmp[3]='r';tmp[4]='n';
	//itoa(Labeln,ch,10);//const char * 与 char *不兼容
    counter=to_string(Labeln);
	for(i=0;i<counter.size();i++){
        tmp[i+5]=counter[i];
	}
	Labeln++;
	return tmp;
}
string newstrlable(){
    string str;
    strlabel="string";
    //itoa(Labelstr,ch,10);
    str=to_string(Labelstr);
    for(int i=0;i<str.size();i++){
        strlabel+=str[i];
    }
    Labelstr++;
    return strlabel;
}

int gettype(string str){
    int i=TableTop-1;
	int find=0;
	str=strlwr2(str);
	if(symtable[i].lev==LVL){
        for(i=TableTop-1;i>=0;i--){
            if(symtable[i].name==str){find=1;break;}
        }
	}
	if(find==0){
        if(symtable[i].lev==0){
            for(i=0;i<TableTop;i++){
                if(symtable[i].name==str){
                    find=1;
                    break;
                }
            }
        }
	}
	if(find==1){
		return symtable[i].type;
	}
	return 0;
}
int getfunctype(string name2){
    int flag=-1;//0:int 1:char
    name2=strlwr2(name2);
    for(int i=TableTop-1;i>=0;i--){
        if(symtable[i].name==name2&&symtable[i].kind=="func"){
            flag=symtable[i].type;
            break;
        }
    }
    return flag;
}
int insymtable(string str){
    int res=0;
    for(int i=TableTop-1;i>=0;i--){
        if(symtable[i].name==str){
            res=1;
            break;
        }
    }
    return res;
}
int getarrange(string name2){
    int i;
    for(i=TableTop-1;i>=0;i--){
        if(symtable[i].name==name2&&symtable[i].kind=="array"&&symtable[i].lev==LVL)return symtable[i].range;//局部数组
    }
    for(i=TableTop-1;i>=0;i--){
        if(symtable[i].name==name2&&symtable[i].kind=="array"&&symtable[i].lev==0)return symtable[i].range;//全局数组
    }
}
string newlab(){
    string counter;
	int i=0;
	string str="lable";
	newlabel=str;
	char *ch;
	//itoa(Labelab,ch,10);
	counter=to_string(Labelab);
	for(i=0;i<counter.size();i++){
        newlabel+=counter[i];
	}
	Labelab++;//标签加1
	return newlabel;
}
string divzero(string str){
    string ans="";
    int i=0;
    while(str[i]=='0'&&i<str.size()){
        i++;
    }
    //cout<<"i is **********"<<i<<endl;
    if(i==str.size()-1){
        if(str[i]!='0')
            ans+=str[i];
        else
            ans="0";
        return ans;
    }
    else if(i==str.size()){
        return "0";
    }
    while(i<str.size()){
        ans+=str[i];
        i++;
    }
    //cout<<"ans is **********"<<ans<<endl;
    return ans;
}
void read(){
    symbol0=symbol1;
    v0=v1;
    symbol1=symbol2;
    v1=v2;
    getsym();
    symbol2=resultvalue;
    v2=token;
}
//程序定义
void program(){
    fprintf(output_grammar_pointer,"这是一个程序。\n");
    printf("这是一个程序。\n");
    if (symbol0 == CONSTSY){
		con_dcl(); //调用常量分析函数
	}
    if ((symbol0 == CHARSY || symbol0 == INTSY) && symbol1 == IDSY&&symbol2 != LPARSY&&symbol2!=LBRACESY){
		var_dcl();
    }
    fprintf(output_midcoder_pointer,"ENDDECLARE\n");
	while ((symbol0 == CHARSY || symbol0 == INTSY || symbol0 == VOIDSY) && symbol1 == IDSY&&(symbol2 == LPARSY||symbol2==LBRACESY)){
		if ((symbol0 == CHARSY || symbol0 == INTSY) && symbol1 == IDSY&&(symbol2 == LPARSY||symbol2==LBRACESY))
			r_func_def();
		if (symbol0 == VOIDSY&& symbol1 == IDSY&&(symbol2 == LPARSY||symbol2==LBRACESY))
			v_func_def();
	}
	if (symbol0 == VOIDSY&&symbol1 == MAINSY){
		maindef();
	}
	else{
		error(6);
		read();
	}
}
void con_dcl(){//常量说明
	read();
	con_def();
	if (symbol0 == SEMISY){
        fprintf(output_grammar_pointer,"这是一个常量分析语句。\n");
		printf("常量分析语句\n");
		read();
	}
	else
		error(7);
	while (symbol0 == CONSTSY){
		read();
		con_def();
		if (symbol0 == SEMISY){
            fprintf(output_grammar_pointer,"这是一个常量分析语句。\n");
			printf("常量分析语句\n");
			read();
		}
		else
			error(7);
	}
}
void con_def(){
	if (symbol0 == CHARSY){
		read();
		if (symbol0 == IDSY&&symbol1 == ISSY&&symbol2==CHARCON){
            //cout<<"233333333333"<<v0<<" "<<v1<<" "<<v2<<endl;
            name=strlwr2(v0);
            val=v2;
			read(); read(); read();
			insert(name,"const",1,val,-1);//非函数范围是-1
			fprintf(output_midcoder_pointer,"const %s char %s\n",name.c_str(),val.c_str());
			while (symbol0 == COMMASY&&symbol1==IDSY&&symbol2==ISSY){
                name=strlwr2(v1);
				read(); read(); read();
				if (symbol0 != CHARCON)
					error(8);
                //cout<<"135625s262"<<symbol2<<" "<<v2<<" "<<v1<<" "<<v0<<endl;
                val=v0;
                insert(name,"const",1,val,-1);
                fprintf(output_midcoder_pointer,"const %s char %s\n",name.c_str(),val.c_str());
				read();
			}
		}
		else
			error(9);
   }
	else if (symbol0 == INTSY){
		read();
		if (symbol0 == IDSY&&symbol1 == ISSY){
            name=strlwr2(v0);
			read(); read();
			integer();//要考虑带有±符号的情况
			insert(name,"const",0,val,-1);
			fprintf(output_midcoder_pointer,"const %s int %s\n",name.c_str(),val.c_str());
			while (symbol0 == COMMASY&&symbol1 == IDSY&&symbol2==ISSY){
                name=strlwr2(v1);
				read(); read(); read();
				integer();
                insert(name,"const",0,val,-1);
                fprintf(output_midcoder_pointer,"const %s int %s\n",name.c_str(),val.c_str());
			}
		}
		else
			error(8);
	}
	else
		error(9);
    fprintf(output_grammar_pointer,"这是一个常量定义语句\n");
	printf("常量定义语句\n");
}
void integer(){//处理±符号的情况
	if (symbol0 == PLUSSY || symbol0 == MINUSSY){
		if(symbol0==PLUSSY){
            read();
            if (symbol0 == INTCON){//无符号整数
                val=v0;
                read();
            }
            else
                error(10);
		}
		else if(symbol0==MINUSSY){
            read();
            if(symbol0==INTCON){
                val='-'+v0;
                read();
            }
            else
                error(10);
		}
	}
	else if (symbol0 == INTCON){
        val=divzero(v0);//去前导0
        read();
	}
    else
		error(10);
}
void var_dcl(){
	var_def();
	if (symbol0 == SEMISY){
        read();
        fprintf(output_grammar_pointer,"这是一个变量说明语句\n");
        printf("变量说明语句\n");
    }
	else
		error(11);
	while ((symbol0 == INTSY || symbol0 == CHARSY) && symbol2 != LPARSY&&symbol2!=LBRACESY)
	{
		var_def();
		if (symbol0 == SEMISY){
            read();
            fprintf(output_grammar_pointer,"这是一个变量说明语句\n");
            printf("变量说明语句\n");
        }
		else
			error(11);
	}
}
void var_def(){
    int num=0;
	if (symbol0 == INTSY){
		read();
		if (symbol0 == IDSY&&symbol1 != LBRACKETSY){//非数组
            name=strlwr2(v0);
            insert(name,"var",0,"",-1);//val为空
			fprintf(output_midcoder_pointer,"var int %s\n",name.c_str());
			read();
			fprintf(output_grammar_pointer,"这是一个int变量定义\n");
			printf("int变量定义\n");
		}
		else if (symbol0 == IDSY&&symbol1 == LBRACKETSY&&symbol2 == INTCON){//数组
		    name=strlwr2(v0);
		    val=v2;
		    num=atoi(val.c_str());
		    insert(name,"array",0,"",num);//num是size
		    fprintf(output_midcoder_pointer,"array int %s %s\n",name.c_str(),val.c_str());
			read(); read(); read();
			if (symbol0 == RBRACKETSY){
                fprintf(output_grammar_pointer,"这是一个int数组变量定义\n");
				printf("int数组变量定义\n");
				read();
			}
			else
				error(12);
		}
		else
			error(13);
		while (symbol0 == COMMASY){
			read();
			if (symbol0 == IDSY&&symbol1 != LBRACKETSY){
                name=strlwr2(v0);
                insert(name,"var",0,"",-1);
                fprintf(output_midcoder_pointer,"var int %s\n",name.c_str());
				read();
				fprintf(output_grammar_pointer,"这是一个int变量定义\n");
				printf("int变量定义\n");
			}
			else if (symbol0 == IDSY&&symbol1 == LBRACKETSY&&symbol2 == INTCON){
			    name=strlwr2(v0);
			    //cout<<"v2 is ********"<<v2<<endl;
			    val=divzero(v2);
			    num=atoi(val.c_str());//前导0
                insert(name,"array",0,"",num);
                fprintf(output_midcoder_pointer,"array int %s %s\n",name.c_str(),val.c_str());
				read(); read(); read();
				if (symbol0 == RBRACKETSY){
					fprintf(output_grammar_pointer,"这是一个int数组变量定义\n");
                    printf("int数组变量定义\n");
					read();
				}
				else
					error(12);
			}
			else
				error(13);
		}
    }
	else if (symbol0 == CHARSY){
		read();
		if (symbol0 == IDSY&&symbol1 != LBRACKETSY){//非数组
            name=strlwr2(v0);
            insert(name,"var",1,"",-1);
            fprintf(output_midcoder_pointer,"var char %s\n",name.c_str());
			read();
			fprintf(output_grammar_pointer,"这是一个char变量定义\n");
			printf("char变量定义\n");
		}
		else if (symbol0 == IDSY&&symbol1 == LBRACKETSY&&symbol2 == INTCON){
            name=strlwr2(v0);
            val=v2;
            num=atoi(val.c_str());
            insert(name,"array",1,"",num);
            fprintf(output_midcoder_pointer,"array char %s %s\n",name.c_str(),val.c_str());
			read(); read(); read();
			if (symbol0 == RBRACKETSY){
                fprintf(output_grammar_pointer,"这是一个char数组变量定义\n");
				printf("char数组变量定义\n");
				read();
			}
			else
				error(12);
		}
		else
			error(13);
		while (symbol0 == COMMASY){
			read();
			if (symbol0==IDSY&&symbol1!=LBRACKETSY){
                name=strlwr2(v0);
                insert(name,"var",1,"",-1);
                fprintf(output_midcoder_pointer,"var char %s\n",name.c_str());
				read();
				fprintf(output_grammar_pointer,"这是一个char变量定义\n");
				printf("char变量定义\n");
			}
			else if (symbol0 == IDSY&&symbol1 == LBRACKETSY&&symbol2 == INTCON){
                name=strlwr2(v0);
                val=v2;
                num=atoi(v2.c_str());
                insert(name,"array",1,"",num);
                fprintf(output_midcoder_pointer,"array char %s %s\n",name.c_str(),val.c_str());
				read(); read(); read();
				if (symbol0 == RBRACKETSY){
                    fprintf(output_grammar_pointer,"这是一个char数组变量定义\n");
					printf("char数组变量定义\n");
					read();
				}
				else
					error(12);
			}
			else
				error(13);
		}
	}
	else
		error(13);
}
void r_func_def(){//＜有返回值函数定义＞ ::= ＜声明头部＞‘(’＜参数表＞‘)’ ‘{’＜复合语句＞‘}’|＜声明头部＞‘{’＜复合语句＞‘}’  //第一种选择为有参数的情况，第二种选择为无参数的情况
	fprintf(output_grammar_pointer,"这是一个有返回值函数定义\n");
    printf("有返回值函数定义\n");
    string str;
	head_statement();
	str=name;
	if (symbol0 == LPARSY&&symbol1 == RPARSY){ error(16); }
	else if (symbol0 == LPARSY&&symbol1 != RPARSY){
		read();
		paratable();
		if (symbol0 != RPARSY){ error(14); }
		read();
	}
    if (symbol0 == LBRACESY&&symbol1 == SEMISY&&symbol2==RBRACESY){ //空语句报错
            read();
            read();
            if (symbol0 != RBRACESY){
                error(15);
		    }
		    read();
		    error(44);
    }
	else if (symbol0 == LBRACESY){//非空语句
		read();
		comstatement();//复合语句
		if(returnflag==false)
            error(39);
        fprintf(output_midcoder_pointer,"END %s\n",str.c_str());//函数结束
        returnflag=false;//重置，否则一直为真，存在bug
		if (symbol0 != RBRACESY){
			error(15);
		}
		read();
	}
	else
		error(17);
    funcsize(str);//计算函数大小
}
void head_statement(){
    fprintf(output_grammar_pointer,"这是一个声明头部\n");
    printf("声明头部\n");
	if (symbol0 == INTSY || symbol0 == CHARSY){
        if(symbol0==INTSY){
            read();
            if (symbol0 != IDSY)
                error(18);
            name=strlwr2(v0);
            insert(name,"func",0,"",-1);
            fprintf(output_midcoder_pointer,"func int %s\n",name.c_str());
        }
        else if(symbol0==CHARSY){
            read();
            if (symbol0 != IDSY)
                error(18);
            name=strlwr2(v0);
            insert(name,"func",1,"",-1);
            fprintf(output_midcoder_pointer,"func char %s\n",name.c_str());
        }

	}
	else
		error(18);
    read();
}
void paratable(){//参数表
    fprintf(output_grammar_pointer,"这是一个参数表\n");
    printf("参数表\n");
    int flag=0;
    string str;
	if (symbol0 == INTSY || symbol0 == CHARSY){
		if(symbol0==INTSY){flag=0;str="INTSY";}
		else if(symbol0==CHARSY){flag=1;str="CHARSY";}
		read();
		if (symbol0 != IDSY){ error(19);}
		name=strlwr2(v0);
		insert(name,"parameter",flag,"",-1);
		fprintf(output_midcoder_pointer,"parameter %s %s\n",str.c_str(),name.c_str());
		read();/////////////////////////////////////////后加了read()
		while (symbol0 == COMMASY){
			read();
			if (symbol0 == INTSY || symbol0 == CHARSY){
                if(symbol0==INTSY){str="INTSY";flag=0;}
                else if(symbol0==CHARSY){str="CHARSY";flag=1;}
				read();
				if (symbol0 != IDSY){ error(19);}
				name=strlwr2(v0);
				insert(name,"parameter",flag,"",-1);
				fprintf(output_midcoder_pointer,"parameter %s %s\n",str.c_str(),name.c_str());
				read();
			}
			else
                error(19);
		}
	}
	else
		error(19);
}
void comstatement(){//复合语句    逻辑有错误   ＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
    fprintf(output_grammar_pointer,"这是一个复合语句\n");
    printf("复合语句\n");
	if (symbol0 == CONSTSY){ con_dcl(); }
    if (symbol0 == CHARSY || symbol0 == INTSY){
		var_dcl();
	}
	statementlist(); //语句列
}
void statementlist(){
	while (symbol0==IFSY||symbol0==DOSY||symbol0==SWITCHSY||symbol0==LBRACESY||(symbol0==IDSY&&(symbol1!=ISSY&&symbol1!=LPARSY))
    ||(symbol0==IDSY&&(symbol1==ISSY||symbol1==LPARSY))||symbol0==SCANFSY||symbol0==PRINTFSY||symbol0==RETURNSY||symbol0==SEMISY)

	{ statement(); }
}
void statement(){
    string str;
    string name;
    int type=0;
    if (symbol0 == IDSY&&symbol1 == LPARSY){//需要引入符号表
		Rfcallstate(name,&type);
		if (symbol0 == SEMISY){ read(); }
		else { error(20); }
	}
	else if (symbol0 == IDSY&&symbol1!=LPARSY&&symbol1!=ISSY&&symbol1==SEMISY){
		Rfcallstate(name,&type);
		if (symbol0 == SEMISY){ read(); }
		else { error(20); }
	}//无返回值函数调用没写；
	else if (symbol0 == IFSY){ read();ifstate(); }
	else if (symbol0 == DOSY){
        newlab();
        whilestr=newlabel;
        whilelabel.push(whilestr);
        fprintf(output_midcoder_pointer,"LABEL %s\n",whilestr.c_str());
        read();
        whilestate();//do{}while()  ;
    }
	else if (symbol0 == SWITCHSY){ read();switchstate(); }
	else if (symbol0 == LBRACESY){
        cout<<"********语句列!*******"<<endl;
		read();
        //cout<<"string is:***********"<<v0<<" "<<v1<<" "<<v2<<" "<<token<<" "<<ch<<endl;//mark [ i
        //cout<<symbol0<<" "<<symbol1<<" "<<symbol2<<endl;//8:id 34:[ 8:id
		statementlist();
		if (symbol0 == RBRACESY){ read(); }
		else error(20);
	}
	else if(symbol0 == IDSY&&(symbol1 == ISSY||symbol1==LBRACKETSY)){
        asstate();
        if (symbol0 == SEMISY){ read(); }
		else { error(20); }
	}
	else if (symbol0 == SCANFSY){
		scanfstate();
		if (symbol0 == SEMISY){ read(); }
		else{error(20); }
	}
	else if (symbol0 == PRINTFSY){
		printfstate();
		if (symbol0 == SEMISY){ read(); }
		else{ error(20); }
	}
	else if (symbol0 == RETURNSY){
		returnstate();
		if (symbol0 == SEMISY){ read(); }
		else { error(20); }
	}
	else if (symbol0 == SEMISY){ read(); }
	else{ error(21); }
}
void v_func_def(){
    fprintf(output_grammar_pointer,"无返回值函数定义\n");
    printf("无返回值函数定义\n");
    string str;
	if (symbol0 == VOIDSY&&symbol1 == IDSY){
        name=strlwr2(v1);
        str=strlwr2(v1);
        insert(name,"func",2,"",-1);//2:void
		fprintf(output_midcoder_pointer,"func void %s\n",name.c_str());
		read(); read();
		if (symbol0 == LPARSY&&symbol1 == RPARSY){ error(16);}
        else if (symbol0 == LPARSY&&symbol1 != RPARSY){
			read();
			paratable();
			if (symbol0 != RPARSY){ error(36); }
			read();
            if (symbol0 == LBRACESY&&symbol1 == SEMISY&&symbol2==RBRACESY){
                    read(); read();read();
                    printf("定义了空语句的带有参数的无返回值函数!\n");
                    fprintf(output_error,"定义了空语句的带有参数的无返回值函数!\n");
            }
            else if (symbol0 == LBRACESY){
                read();
                comstatement();
                fprintf(output_midcoder_pointer,"END %s\n",str.c_str());
                if (symbol0 != RBRACESY){ error(36); }
                read();
            }
            else
                error(36);
		}
		else if (symbol0 == LBRACESY&&symbol1 == SEMISY&&symbol2==RBRACESY){//void empty{;{};{;};;;{{;}}}void empty{{};{;};;;{{;}}}
                read();
                read();
                if (symbol0 != RBRACESY){ error(36); }
                read();
                printf("定义了空语句的无返回值函数!\n");
                fprintf(output_error,"定义了空语句的无返回值函数!\n");
        }
		else if (symbol0 == LBRACESY){
			read();
			comstatement();
			fprintf(output_midcoder_pointer,"END %s\n",str.c_str());
			if (symbol0 != RBRACESY){ error(36); }
			read();
		}
		else
			error(36);
	}
	else
		error(36);
    funcsize(str);
}
void maindef(){
    fprintf(output_grammar_pointer,"主函数定义\n");
    printf("主函数\n");
    mainflag=true;//只有一个主函数在最后声明时不需要重置flag，但是先声明主函数不符合文法，综上不需要重置该变量。
	if (symbol0 == VOIDSY&&symbol1 == MAINSY){
        insert("main","func",2,"",-1);//2:void
        fprintf(output_midcoder_pointer,"func void main\n");
		read(); read();
		if (symbol0 == LPARSY&&symbol1 == RPARSY&&symbol2 == LBRACESY){
			read(); read(); read();
			comstatement();
			funcsize("main");
			if (symbol0 == RBRACESY){ read(); }
			else{ error(26); printf("695\n");}
		}
		else { error(26); printf("697\n");}
	}
	else{ error(26); printf("699\n");}
}
void exp(string &expn,int *expt){
    string termname1;
    string termname2;//左值 右值
	int termt1,termt2;
	int flag=1;
	string str;
	expprintflag=false;
    fprintf(output_grammar_pointer,"表达式\n");
    printf("表达式\n");
	if (symbol0 == PLUSSY || symbol0 == MINUSSY){//写语句中，字符串原样输出，单个字符类型的变量或常量输出字符，其他表达式按整型输出
            expprintflag=true;
            if (symbol0==MINUSSY) {
                flag=-1;
                if(arrayflag==true){
                    minusarray=true;
                }
            }
            read();
    }
	term(termname1,&termt1);
	//cout<<"asas"<<termname1<<endl;printf("%s\n",termname1.c_str());//fprintf(output_midcoder_pointer,"255expname is: %s \n",termname1.c_str());
	leftterm=rightterm;
	if(flag==-1){
		fprintf(output_midcoder_pointer,"neg %s\n",termname1.c_str());
	}
	while (symbol0 == PLUSSY || symbol0 == MINUSSY){
        expprintflag=true;
        str=v0;
		read();
        term(termname2,&termt2);
        if(str=="+")fprintf(output_midcoder_pointer,"+ %s %s %s\n",termname1.c_str(),termname2.c_str(),termname1.c_str());//printf("termname1:%s termname2:%s",termname1,termname2);}////////////bug
        if(str=="-")fprintf(output_midcoder_pointer,"- %s %s %s\n",termname1.c_str(),termname2.c_str(),termname1.c_str());////////////bug
	}
	expn=termname1;
	*expt=termt1;
}
void term(string &termn,int *termt){
    string factorname1;
    string factorname2;
	int factor1=0,factor2=0;
	string str;
	//string leftname;
    fprintf(output_grammar_pointer,"项\n");
    printf("项\n");
	factor(factorname1,&factor1);
	leftname=sname;
	//cout<<"xiang factor"<<factorname1<<endl;//$var0
	while (symbol0 == STARSY || symbol0 == SLASHSY){
        expprintflag=true;
        str=v0;
		read();
        factor(factorname2,&factor2);
		if(str=="*"){
            fprintf(output_midcoder_pointer,"* %s %s %s\n",factorname1.c_str(),factorname2.c_str(),factorname1.c_str());
            //fprintf(output_midcoder_pointer,"fact1: %s  fact2: %s",factorname1,factorname2);
		}
        if(str=="/")fprintf(output_midcoder_pointer,"/ %s %s %s\n",factorname1.c_str(),factorname2.c_str(),factorname1.c_str());
	}
	termn=factorname1;
	rightterm=termn;/////////////////added line
	//cout<<"term"<<termn<<endl;
	*termt=factor1;
}
void factor(string &factorn,int *factort){//＜因子＞::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’
	string varname;
	string str;
	int type;
	string expname;
	string strv0;
	int arrayedge;
	int exptype=0;
	int n=0;
    if (symbol0 == IDSY&&symbol1 != LBRACKETSY&&symbol1 != LPARSY){//标识符也可以是调用函数
        if(insymtable(strlwr2(v0))==0)//fprintf(output_midcoder_pointer,"JUMP %s\n",str.c_str());
           error(40);//未定义标识符
        varname=newvar();             //fprintf(output_midcoder_pointer,"LODR %s $v0\n",name.c_str())
        factorn=varname;//string -> char * insert(s,"call",*type,"",-2);//调用类型
        if(getfunctype(v0)==-1){//不是函数调用
            type=gettype(strlwr2(v0));
            *factort=type;
            sname=varname;
            insert(varname,"newvar",type,"",-1);
            fprintf(output_midcoder_pointer,"LODV %s %s %d\n",varname.c_str(),strlwr2(v0).c_str(),LVL);//加载变量
        }
        else{
            type=getfunctype(v0);
            insert(varname,"call",type,"",-2);//调用类型
            fprintf(output_midcoder_pointer,"JUMP %s\n",strlwr2(v0).c_str());//标识符不区分大写小写，应该化为小写
            fprintf(output_midcoder_pointer,"LODR %s $v0\n",varname.c_str());
        }
		read();
        printf("因子标识符\n");
	}
	else if (symbol0 == IDSY&&symbol1 == LBRACKETSY){
        arrayname=strlwr2(v0);
        if(insymtable(strlwr2(v0))==0)
            error(40);//未定义标识符
        //varname=strlwr2(v0);
        type=gettype(strlwr2(v0));
        strv0=strlwr2(v0);
        types=type;
		read(); read();
		varname=newvar();
		factorn =varname;
		str=factorn;
        sname=str;
        arrayflag=true;
		exp(expname,&exptype);
		arrayflag=false;
		insert(str,"newvar",type,"",-1);
		fprintf(output_midcoder_pointer,"LODA %s %s %s %d\n",strv0.c_str(),expname.c_str(),factorn.c_str(),LVL);
		if (symbol0 == RBRACKETSY){ read(); printf("因子: ＜标识符＞[＜表达式＞]\n"); }
		else { error(28); }
	}
	else if (symbol0 == LPARSY){
		read(); exp(factorn,factort);
		if (symbol0 == RPARSY){ read(); printf("因子: (＜表达式＞) \n"); }
		else { error(28); }
	}
	else if (symbol0 == PLUSSY || symbol0 == MINUSSY || symbol0 == INTCON){//i=0;
		integer();//read();
		arrayedge=atoi(val.c_str());
		//fprintf(output_error,"arrayedge is **%d range:%d %d\n",arrayedge,getarrange(arrayname),arrayflag);
		//fprintf(output_midcoder_pointer,"***************%s %d %d\n",arrayname.c_str(),getarrange(arrayname),arrayedge);
		if(arrayflag==true){
            if(minusarray==true){
                arrayedge=0-arrayedge;
            }
            if(arrayedge<0 || arrayedge>=getarrange(arrayname)){
                error(45);
                fprintf(output_error,"arrayedge is **%s %d range:%d %d\n",arrayname.c_str(),arrayedge,getarrange(arrayname),arrayflag);
                //fprintf(output_midcoder_pointer,"ARRAYINT out the range\n");
            }
		}
		minusarray=false;
		varname=newvar();
		factorn=varname;//生成寄存器变量
		//str=*factorn;
		sname=varname;
		types=0;
		//printftable();
		//cout<<"insert name str is:"<<" "<<*factorn<<endl;
		insert(varname.c_str(),"newvar",0,"",-1);
		//printftable();
		fprintf(output_midcoder_pointer,"LODI %s %s\n",factorn.c_str(),val.c_str());
		printf("因子  整数 \n");
	}
	else if (symbol0 == CHARCON){
        varname=newvar();
        factorn=varname;
        //str=*factorn;
        sname=varname;
        types=1;
		insert(varname.c_str(),"newvar",1,"",-1);
		fprintf(output_midcoder_pointer,"LODC %s %s\n",factorn.c_str(),v0.c_str());
        read();
        printf("因子: 字符 \n");
    }
	else if (symbol0 == IDSY&&symbol1 == LPARSY){
		Rfcallstate(factorn,factort); printf("因子有返回值函数调用语句 \n");
	}
	else { error(27); }
}
void asstate(){//赋值语句
    fprintf(output_grammar_pointer,"赋值语句\n");
    printf("赋值语句\n");
    string expnameleft;
	string expnameright;
	int exptype=0;
	string str;
	if (symbol0 == IDSY&&symbol1 == ISSY){
        string sexp;
        str=strlwr2(v0);
        if(insymtable(str)==0)error(40);
		read(); read();
        exp(expnameleft,&exptype);//调用表达式生成$varn    i=i*i+i
        fprintf(output_midcoder_pointer,"= %s %s %d\n",expnameleft.c_str(),str.c_str(),LVL);//str.c_str()=expnameleft sname全局变量会更新
	}
	else if (symbol0 == IDSY&&symbol1 == LBRACKETSY){//数组赋值
        str=strlwr2(v0);
        arrayname=str;
        if(insymtable(str)==0)error(40);
		read(); read();
		arrayflag=true;
		exp(expnameleft,&exptype);
		arrayflag=false;
		if (symbol0 == RBRACKETSY&&symbol1 == ISSY){
			read(); read();
			exp(expnameright,&exptype);
			fprintf(output_midcoder_pointer,"eql %s %s %s %d\n",str.c_str(),expnameleft.c_str(),expnameright.c_str(),LVL);
		}
		else{error(29); }
	}
	else{ error(29); }
}
void ifstate(){//设置标签label
    string str;
    string elsestr;
    fprintf(output_grammar_pointer,"if语句\n");
    printf("if语句\n");
	if (symbol0 == LPARSY){
        newlab();
		str=newlabel;
		newlab();
        elsestr=newlabel;
		read();
		condition(1,str,elsestr);//
		if (symbol0 == RPARSY){
			read();
			//cout<<"string is:***********"<<v0<<" "<<v1<<" "<<v2<<" "<<token<<" "<<ch<<endl;
			statement();//////////bug if (){if(i==2)i=3;else i=4;}else
			fprintf(output_midcoder_pointer,"JUMP %s\n",elsestr.c_str());
            if(symbol0==ELSESY){//bug if() else 不执行
                fprintf(output_midcoder_pointer,"LABEL %s\n",str.c_str());
                read();
                statement();
                fprintf(output_midcoder_pointer,"LABEL %s\n",elsestr.c_str());
            }
            else
                error(22);
		}
		else{ error(22); }
	}
	else{ error(22); }
}
void condition(int flag,string label,string elsestr){
    fprintf(output_grammar_pointer,"条件\n");
	printf("条件\n");
	string expright;
	string expleft;
	string str;
	string leftname;
	int expt=0;
	exp(expleft,&expt);
	leftname=sname;
	/*if (symbol0 == LSSY || symbol0 == NBGSY || symbol0 == BGSY || symbol0 == NLSSY || symbol0 == NEQSY || symbol0 == EQUSY){//LSSY, NBGSY, BGSY, NLSSY, NEQSY, EQUSY
		read(); exp();
	}*/

	if(flag==1){
        if(symbol0==LSSY){//<
            read();
            exp(expright,&expt);
            fprintf(output_midcoder_pointer,">= %s %s %s\n",expleft.c_str(),expright.c_str(),label.c_str());
        }
        else if(symbol0==NBGSY){//<=
            read();
            exp(expright,&expt);
            fprintf(output_midcoder_pointer,"> %s %s %s\n",expleft.c_str(),expright.c_str(),label.c_str());
        }
        else if(symbol0==BGSY){//>
            read();
            exp(expright,&expt);
            fprintf(output_midcoder_pointer,"<= %s %s %s\n",expleft.c_str(),expright.c_str(),label.c_str());
        }
        else if(symbol0==NLSSY){//>=
            read();
            exp(expright,&expt);
            fprintf(output_midcoder_pointer,"< %s %s %s\n",expleft.c_str(),expright.c_str(),label.c_str());
        }
        else if(symbol0==NEQSY){// !=
            read();
            exp(expright,&expt);
            fprintf(output_midcoder_pointer,"== %s %s %s\n",expleft.c_str(),expright.c_str(),label.c_str());
        }
        else if(symbol0==EQUSY){//==
            //printf("打印出了==！*****************\n");
            read();
            exp(expright,&expt);
            fprintf(output_midcoder_pointer,"!= %s %s %s\n",expleft.c_str(),expright.c_str(),label.c_str());
        }
        else{
			fprintf(output_midcoder_pointer,"eql0 %s %s %s\n",expleft.c_str(),"0",label.c_str());
		}
	}
	else if(flag==0){
        if (symbol0 == LSSY || symbol0 == NBGSY || symbol0 == BGSY || symbol0 == NLSSY || symbol0 == NEQSY || symbol0 == EQUSY){
            str=v0;
            read();
            exp(expright,&expt);
            fprintf(output_midcoder_pointer,"%s %s %s %s\n",str.c_str(),expleft.c_str(),expright.c_str(),label.c_str());
        }//while(0/i)单个表达式
        else{
            fprintf(output_midcoder_pointer,"!= %s 0 %s\n",expleft.c_str(),label.c_str());
        }
	}
}
void whilestate(){
        fprintf(output_grammar_pointer,"while循环语句\n");
        printf("while循环语句\n");
        statement();
        string elsestr;
        string whilestr2;
		if (symbol0 == WHILESY&&symbol1 == LPARSY){
			read(); read();
			whilestr2=whilelabel.top();
			whilelabel.pop();
			condition(0,whilestr2,elsestr);
			if (symbol0 == RPARSY){ read();}
			else{ error(23); }
		}
	    else{ error(23); }
}
void constant(){
	if (symbol0 == CHARCON){ read(); }
	else if (symbol0 == PLUSSY || symbol0 == MINUSSY || symbol0 == INTCON){
		integer();
	}
	else{ error(24); }
}
void switchstate(){//default
    fprintf(output_grammar_pointer,"这是switch-case语句\n");
    printf("这是switch-case语句\n");
    string expname;
	//char *casevar=NULL;
	int expt=0;
	string str;
	newlab();
	str=newlabel;//设置标签
	defaultlabel.push(str);
	if(symbol0==LPARSY){
        //cout<<symbol0<<" "<<symbol1<<" "<<symbol2<<endl;//32 8 33(i)
        read();
        exp(expname,&expt);
        if(symbol0!=RPARSY){error(25);printf("871\n");}
        read();
        if(symbol0!=LBRACESY){error(25);printf("873\n");}
        read();
	}
    else
        {error(25);printf("502\n");}
	switchtable(expname);
	if(symbol0==DEFAULTSY){
        defaultstate();
	}
    else error(25);
    if(symbol0==RBRACESY){
            read();
            //fprintf(output_midcoder_pointer,"LABEL %s\n",str.c_str());
    }
    else error(25);
}
void switchtable(string &casevar){
	string str;
	newlab();
	str=newlabel;
	casestate(casevar);
	fprintf(output_midcoder_pointer,"JUMP %s\n",defaultlabel.top().c_str());//bug
	fprintf(output_midcoder_pointer,"LABEL %s\n",str.c_str());
    while(symbol0==CASESY){
		 newlab();
		 str=newlabel;
		 casestate(casevar);
		 fprintf(output_midcoder_pointer,"JUMP %s\n",defaultlabel.top().c_str());
		 fprintf(output_midcoder_pointer,"LABEL %s\n",str.c_str());
	 }
}
void casestate(string &casevar){
    string casev;
	casev=newvar();
	string str;
	if (symbol0 == CASESY){
		read();
		if(symbol0==CHARCON){
            str=casev;
			insert(str,"case",1,val,-1);
			fprintf(output_midcoder_pointer,"LODCASE %s %s 1\n",casev.c_str(),v0.c_str());//v0:case 后跟的字符  1:type
			fprintf(output_midcoder_pointer,"!= %s %s %s\n",casevar.c_str(),casev.c_str(),newlabel.c_str());//不符合条件跳转到str,下一个case或者是default
			read();
		}
		else if(symbol0==PLUSSY||symbol0==MINUSSY||symbol0==INTCON){
			integer();//val 赋值
			str=casev;
			insert(str,"case",0,val,-1);
			fprintf(output_midcoder_pointer,"LODCASE %s %s 0\n",casev.c_str(),val.c_str());
			fprintf(output_midcoder_pointer,"!= %s %s %s\n",casevar.c_str(),casev.c_str(),newlabel.c_str());
		}
		else{
			read();
			error(30);
		}
		if (symbol0 == COLONSY){
			read();
			statement();//读取下一个字符
		}
		else{ error(30); }
	}
	else{error(30); }
}
void defaultstate(){//jne , , ,label指令（不等，则跳到下一个case或者default位置）   生成语句指令  jump default_label_final 执行该分支后，直接跳到default结束的位置
    read();
    if(symbol0==COLONSY){
        read();
        statement();
        fprintf(output_midcoder_pointer,"LABEL %s\n",defaultlabel.top().c_str());
        defaultlabel.pop();
    }
    else error(31);
}
void Rfcallstate(string &name,int *type){
    fprintf(output_grammar_pointer,"这是有返回值函数调用语句\n");
    printf("有返回值函数调用语句\n");
    string str;
    string s;
    //cout<<token<<" "<<ch<<" "<<symbol0<<endl;
	if (symbol0 == IDSY&&symbol1 == LPARSY){
        str=strlwr2(v0);
        name=newvar();
        s=name;
        *type=getfunctype(str);
        if(*type==-1)error(41);
        insert(s,"call",*type,"",-2);//调用类型
		if (symbol2 != RPARSY){ read(); read();Vparatable(str); }//值参数表
		else{error(32);}
		if (symbol0 == RPARSY){
            read();
            fprintf(output_midcoder_pointer,"JUMP %s\n",str.c_str());
			fprintf(output_midcoder_pointer,"LODR %s $v0\n",name.c_str());
        }
		else{ error(32); }
	}
	else if(symbol0==IDSY&&symbol1!=LPARSY){
        str=strlwr2(v0);
        name=newvar();
        s=name;
        *type=getfunctype(str);
        if(*type==-1)error(41);
        insert(s,"call",*type,"",-2);
        fprintf(output_midcoder_pointer,"JUMP %s\n",str.c_str());
        fprintf(output_midcoder_pointer,"LODR %s $v0\n",name.c_str());
        read();
	}
	else{
		error(32);
	}
}
void Ufcallstate(){
    fprintf(output_grammar_pointer,"这是无返回值函数调用语句\n");
    printf("无返回值函数调用语句\n");
    string str,str1;
    int hasvoid=0;
	if ( symbol0 == IDSY && symbol1 == LPARSY){
        str=strlwr2(v0);
        str1=newvar();
        hasvoid=getfunctype(str);
        if(hasvoid==-1)error(41);
        insert(str1,"call",2,"",-2);//2:type->void
		if (symbol2 != RPARSY){
            read();
            read();
            Vparatable(str);
        }
		else{ error(33);}//值参数表为空
		if (symbol0 == RPARSY){
            read();
            fprintf(output_midcoder_pointer,"JUMP %s\n",str.c_str());
			fprintf(output_midcoder_pointer,"LODR %s $v0\n",str1.c_str());
        }
		else{ error(33); }
	}
	else if(symbol0==IDSY&&symbol1!=LPARSY){
        str=strlwr2(v0);
        str1=newvar();
        hasvoid=getfunctype(str);
        if(hasvoid==-1)error(41);
        insert(str1,"call",2,"",-2);
        fprintf(output_midcoder_pointer,"JUMP %s\n",str.c_str());
        fprintf(output_midcoder_pointer,"LODR %s $v0\n",str1.c_str());
        read();
	}
	else{ error(33); }
}
void Vparatable(string paraname){
    fprintf(output_grammar_pointer,"这是值参数表\n");
    printf("值参数表\n");
    string  expname;
    int exptype=0;
    int paranums=0;
    int truepara=0;
    int formpara=0;
    exp(expname,&exptype);
    fprintf(output_midcoder_pointer,"Vpara %s %s %d\n",expname.c_str(),paraname.c_str(),paranums);//参数名  所属函数名   参数次序
    paranums++;//参数类型检查
    truepara=getvartype2(expname,LVL);
    formpara=getfuncpara(paraname,paranums);
    if(truepara!=formpara){
        printf("%s函数参数类型不匹配\n",paraname.c_str());
        fprintf(output_error,"%s函数参数类型不匹配\n",paraname.c_str());
    }
	while (symbol0 == COMMASY){
		read();
		exp(expname,&exptype);
		fprintf(output_midcoder_pointer,"Vpara %s %s %d\n",expname.c_str(),paraname.c_str(),paranums);//参数名  所属函数名   参数次序
        paranums++;
        truepara=getvartype2(expname,LVL);
        formpara=getfuncpara(paraname,paranums);
        if(truepara!=formpara){
            printf("%s函数参数类型不匹配\n",paraname.c_str());
            fprintf(output_error,"%s函数参数类型不匹配\n",paraname.c_str());
        }
	}
	//printf("%d %d\n",getparan2(paraname),paranums);
	if(paranums!=getparan2(paraname)){
        printf("参数个数出错\n");
        fprintf(output_error,"参数个数出错\n");
	}
}
void printfstate(){
    fprintf(output_grammar_pointer,"这是输出语句\n");
    printf("输出语句\n");
    string expname;
    int exptype=0;
    string str;
	if (symbol0 == PRINTFSY&&symbol1 == LPARSY){
		read(); read();
		if (symbol0 == STRINGCON&&symbol1 == COMMASY){//字符串，表达式
            str=newstrlable();
            fprintf(output_midcoder_pointer,"PRINTFSTR %s\n",str.c_str());
            strcount++;
            strlabelarr.push_back(v0);//保存字符串一一对应的  str[k]==stringk k>=0
			read(); read();
			exp(expname,&exptype);
			if(expprintflag==false){
                fprintf(output_midcoder_pointer,"PRINTFEXP %s\n",expname.c_str());
			}
			else{
                fprintf(output_midcoder_pointer,"PRINTFEXPINT %s\n",expname.c_str());//运算式
			}
			if (symbol0 == RPARSY){ read();}
			else{ error(33); }
		}
		else if (symbol0 == STRINGCON&&symbol1 != COMMASY){
			str=newstrlable();
			fprintf(output_midcoder_pointer,"PRINTFSTR %s\n",str.c_str());
			strcount++;
            strlabelarr.push_back(v0);
			read();
			if (symbol0 == RPARSY){ read();}
			else{ error(33); }
		}
		else{
			exp(expname,&exptype);
			if(expprintflag==false){
                fprintf(output_midcoder_pointer,"PRINTFEXP %s\n",expname.c_str());
			}
			else{
                fprintf(output_midcoder_pointer,"PRINTFEXPINT %s\n",expname.c_str());
			}
			if (symbol0 == RPARSY){ read(); }
			else{error(33); }
		}
	}
	else{ error(33); }
}

void scanfstate(){
    fprintf(output_grammar_pointer,"这是读语句\n");
    printf("读语句\n");
    string str;
	if (symbol0 == SCANFSY&&symbol1 == LPARSY){
		read(); read();
		if (symbol0 == IDSY){
            str=strlwr2(v0);
            if(insymtable(str)==0)
                error(40);
            fprintf(output_midcoder_pointer,"SCANF %s %d\n",str.c_str(),LVL);//lvl变量所属层次
			read();
			while (symbol0 == COMMASY){
				read();
				if (symbol0 == IDSY){
				    str=strlwr2(v0);
				    if(insymtable(str)==0)//未声明报错
                        error(40);
                    fprintf(output_midcoder_pointer,"SCANF %s %d\n",str.c_str(),LVL);
					read();
				}
				else{ error(34); }
			}
		}
		else{ error(34); }
		if (symbol0 == RPARSY){ read();}
		else{ error(34); }
	}
	else{ error(34); }
}
void returnstate(){
    fprintf(output_grammar_pointer,"这是返回语句\n");
    printf("返回语句\n");
    string expname;
    int exptype;
	if (symbol0 == RETURNSY&&symbol1 == LPARSY){
        returnflag=true;
		read(); read();
		exp(expname,&exptype);
		if(mainflag==false)
            fprintf(output_midcoder_pointer,"RETURN1 %s %d %d\n",expname.c_str(),exptype,LVL);
        else if(mainflag==true)
            fprintf(output_midcoder_pointer,"JUMPEND\n");
		if (symbol0 == RPARSY){ read();}
		else{ error(35); }
	}
	else if (symbol0 == RETURNSY&&symbol1 == SEMISY){
            read();
            if(mainflag==false)
                fprintf(output_midcoder_pointer,"RETURN2 %d\n",LVL);
            else if(mainflag==true)
                fprintf(output_midcoder_pointer,"JUMPEND\n");
    }
	else{ error(35); }
}
/*int main(){//存在死循环  debug
        ch=' ';
        input_file_pointer = fopen("test.txt","r");
        output_grammar_pointer = fopen("grammer.txt","w");
        output_words_pointer = fopen("output.txt","w");
        output_midcoder_pointer = fopen("midcode.txt","w");
        cout<<"11111111111"<<endl;
        getsym();
        symbol0 = resultvalue;
        v0=token;
        getsym();
        symbol1 = resultvalue;
        v1=token;
        getsym();
        //cout<<token<<endl;
        symbol2 = resultvalue;
        v2=token;
        while(!feof(input_file_pointer)){
            program();
        }
        fclose(input_file_pointer);
        fclose(output_grammar_pointer);
        fclose(output_words_pointer);
        fclose(output_midcoder_pointer);
        return 0;
}*/
