#include<iostream>
#include<string.h>
#include<string>
#include<sstream>
#include<stdio.h>
#include<vector>
#include<stdlib.h>
#include"word.h"
#include"syntax.h"
#include"mips.h"
using namespace std;
FILE * output_mips_pointer;
FILE * output_mid_pointer;
string mid;
string teststr;
bool enddeclare=false;
vector<int> fsize(LVL,0);
vector<int> fallsize(LVL,0);
void getallfuncsize(){
    int i;
    for(i=1;i<=LVL;i++){
        fsize.push_back(getrfuncsize(i));
        //printf("%d ",fsize[i-1]);
    }
    for(i=LVL-2;i>=0;i--){
        fsize[i]=fsize[i]+fsize[i+1];
    }
    for(i=0;i<LVL;i++){
        //printf("fsize is:%d\n",fsize[i]);
    }
    return;
}
void func_mid(){
	char c;
	c=fgetc(output_mid_pointer);//4 3  var 3 const 4
	//fprintf(output_mips_pointer,"rest2++++++++++%c %s\n",c,mid.c_str());
	mid.clear();
	for(int i=0;c!=' '&&c!='\n'&&c!=EOF;i++){//每次分析一个词
        mid+=c;
        c=fgetc(output_mid_pointer);
	}
	//fprintf(output_mips_pointer,"rest2++++++++++%s\n",mid.c_str());
    return;
}
void printfindex(){
    for(int i=0;i<10;i++){
        printf("%d ",TableIndex[i]);
    }
    printf("\n");
}
bool issame(string a,string b){
    if(a.size()!=b.size())return false;
    for(int i=0;i<a.size();i++){
        if(a[i]!=b[i])return false;
    }
    return true;
}
int getindex(string name2){//返回name在其所在层的位置
	int i=TableTop-1;
	int ans=INT_MAX;
	string str;
	int flag=-1;
	name2=strlwr2(name2);
	while(i>=0){
        str=symtable[i].name;
		if(strcmp(str.c_str(),name2.c_str())==0){//
            flag=i;
            break;
		}
		//if(i==11){fprintf(output_mips_pointer,"symbol[11].name is: %s  name2 is: %s\n",symtable[i].name.c_str(),name2.c_str());}
		//if(i==12){fprintf(output_mips_pointer,"symbol[12].name is: %s  name2 is: %s\n",symtable[i].name.c_str(),name2.c_str());}
		i--;
	}
	ans=symtable[flag].offs-TableIndex[symtable[flag].lev]+1;
	/*if(name2=="$varn1"){
            printftable();
            fprintf(output_mips_pointer,"$varn1.position==: %d offs: %d tableindex: %d  flag: %d name2: %s tabtop: %d\n",ans,symtable[i].offs,TableIndex[symtable[i].lev],flag,name2.c_str(),TableTop);
    }
    else if(name2=="$varn2"){
            fprintf(output_mips_pointer,"$varn2.position==: %d offs: %d tableindex: %d  i: %d name2: %s tabtop: %d\n",ans,symtable[i].offs,TableIndex[symtable[i].lev],i,name2.c_str(),TableTop);
    }*/
	return ans;
}
int getparalel(string name2){
    int i;
    bool flag=false;
    name2=strlwr2(name2);
    for(i=TableTop-1;i>=0;i--){
        if((strcmp(symtable[i].name.c_str(),name2.c_str())==0)&&symtable[i].kind=="parameter"){
            flag=true;
            break;
        }
    }
    if(flag)return symtable[i].lev;
    else
        return -1;

}

int getparan(string name){
	int i=TableTop-1;
	int ans=0;
	int flag=0;
	int lvl=getparalel(name);
	while((i>=0)){
		strlwr2(name);
		if(symtable[i].kind=="parameter"&&symtable[i].lev==lvl){
			flag=1;
			ans++;
		}
		i--;
	}
	if(flag==1)return ans;
	else{
		error(43);
		return -1;
	}
}
int getparan2(string name2){
    int i=TableTop-1;
	int vparacount=0;
	int flag=0;
	name2=strlwr2(name2);
	int lel=getfunclvl(name2)+1;
	for(int i=TableTop-1;i>=0;i--){
		if(symtable[i].kind=="parameter"&&symtable[i].lev==lel){
			flag=1;
			vparacount++;
		}
	}
	if(flag==1)
        return vparacount;
    error(41);
    return -1;

}
int getfuncpara(string funcname,int paranums){
    funcname=strlwr2(funcname);
    int type=0;
    int paracount=0;
    int lel=getfunclvl(funcname)+1;
    for(int i=TableTop-1;i>=0;i--){
		if(symtable[i].kind=="parameter"&&symtable[i].lev==lel){
			paracount++;
			if(paracount==paranums)type=symtable[i].type;
		}
	}
	return type;
}
int getfunclvl(string name2){
    name2=strlwr2(name2);
    for(int i=TableTop-1;i>=0;i--){
        if((strcmp(symtable[i].name.c_str(),name2.c_str())==0)&&symtable[i].kind=="func")return symtable[i].lev;
    }
    error(41);
    return -1;
}
int getindex2(string name2,int level){//返回name2在其所在层的位置  var1 level:1
	int i=TableTop-1;
	int flag=0;
	int res=-1;
	name2=strlwr2(name2);
	for(i=TableTop-1;i>=0;i--){
		if((strcmp(name2.c_str(),symtable[i].name.c_str())==0) && symtable[i].lev==level){
                /*if(name2=="var1"){
                    fprintf(output_mips_pointer,"var1.position==: %d offs: %d tableindex: %d  flag: %d name2: %s i:%d\n",res,symtable[i].offs,TableIndex[symtable[i].lev],flag,name2.c_str(),i);
                }*/
                flag=1;
				break;
		}
	}
	if(flag==1){
        res=symtable[i].offs-TableIndex[symtable[i].lev]+1;
	}
	else if(flag==0){
		res=findingobel(name2);
	}
	/*if(name2=="var1"){
        fprintf(output_mips_pointer,"var1.position==: %d offs: %d tableindex: %d  flag: %d name2: %s i:%d\n",res,symtable[i].offs,TableIndex[symtable[i].lev],flag,name2.c_str(),i);
    }*/
	return res; //-1  -2
}
int findingobel(string name2){
	int i=TableTop-1;
	bool flag=false;
	name2=strlwr2(name2);
	for(i=TableTop-1;i>=0;i--){
		if((strcmp(symtable[i].name.c_str(),name2.c_str())==0)&&symtable[i].lev==0){
                flag=true;
				break;
		}
	}
	if(flag==true){
		return -2;//全局变量
	}
	else {
		return -1;
	}
}
int getlevel(string name2){
    int ans=0;
    int i;
    name2=strlwr2(name2);
    for(i=TableTop-1;i>=0;i--){
        if((strcmp(symtable[i].name.c_str(),name2.c_str())==0))
            return symtable[i].lev;
    }
    error(40);
    return -1;
}

int getvartype2(string name2,int level){//全局变量还会被同名局部变量屏蔽
    name2=strlwr2(name2);
    for(int i=TableTop-1;i>=0;i--){
        if((strcmp(symtable[i].name.c_str(),name2.c_str())==0)&&symtable[i].lev==level)return symtable[i].type;
    }
    for(int i=0;i<=TableTop-1;i++){
        if((strcmp(symtable[i].name.c_str(),name2.c_str())==0)&&symtable[i].lev==0)return symtable[i].type;
    }
    error(40);
    return -1;
}
int getvartype(string name2){
    name2=strlwr2(name2);
    for(int i=TableTop-1;i>=0;i--){
        if((strcmp(symtable[i].name.c_str(),name2.c_str())==0))return symtable[i].type;
    }
    error(40);
    return -1;
}
int getrfuncsize(int level){
    for(int i=TableTop-1;i>=0;i--){
        if(symtable[i].lev==level-1&&symtable[i].kind=="func")return symtable[i].range;//-1
    }
    printf("getrfuncsize出错\n");
    return -1;
}
int getfuncsize(string name2){
    name2=strlwr2(name2);
    for(int i=TableTop-1;i>=0;i--){
        if((strcmp(symtable[i].name.c_str(),name2.c_str())==0)&&symtable[i].kind=="func")return symtable[i].range;
    }
    return -1;
}
string number(string str){
    string ans;
    if(str[0]!='+'&&str[0]!='-'){
        ans=divzero(str);//去前导0
    }
    else{
        if(str[0]=='+'){
            ans=str.substr(1,str.size());
            ans=divzero(ans);
        }
        else{//-
            ans=str.substr(1,str.size());
            ans=divzero(ans);
            ans='-'+ans;
        }
    }
    return ans;
}
int mips(){
    string mid1;
    string mid2;
    string mid3;
    string mid4;
    string mid5;
    int trans;
    int paranum=0;
    int index=0;
    int index2=0;
    int offset2=0;
    int scanftype=0;
    char asch;
    func_mid();//读取mid.txt
    mid1=mid;
    //fprintf(output_mips_pointer,"%s %d\n",mid1.c_str(),enddeclare);
    if(mid1=="const"&&enddeclare==false){//从const开始分析  const复合语句中也有
        func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        func_mid();
        mid4=mid;
        if(mid3=="int"){
            fprintf(output_mips_pointer,"%s: .word  %s\n",mid2.c_str(),mid4.c_str());
        }
        else if(mid3=="char"){
            asch=mid4[0]+0;
            fprintf(output_mips_pointer,"%s: .word '%c'\n",mid2.c_str(),asch);
        }
    }
    else if(mid1=="const"&&enddeclare==true){
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        func_mid();
        mid4=mid;
		if(mid3=="int"){
			fprintf(output_mips_pointer,"li $t1,%d\n",transNum(mid4));
			fprintf(output_mips_pointer,"sw $t1,%d($sp)\n",getindex(mid2)*4);
		}
		else if(mid3=="char"){//转为assic
		    asch=mid4[0]+0;
			fprintf(output_mips_pointer,"li $t1,%d\n",asch);
			fprintf(output_mips_pointer,"sw $t1,%d($sp)\n",getindex(mid2)*4);
		}
        else
            error(42);
	}
    else if(mid1=="var"&&enddeclare==false){
        func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        fprintf(output_mips_pointer,"%s: .space 4\n",mid3.c_str());//int 4字节
        // fprintf(output_mips_pointer,"test+++++++++++%s %s %s\n",mid1.c_str(),mid2.c_str(),mid3.c_str());
    }
    else if(mid1=="var"){//////局部变量
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
	}
    else if(mid1=="array"){
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        func_mid();
        mid4=mid;//size
        if(enddeclare==false)
            fprintf(output_mips_pointer,"%s: .word 1:%s\n",mid3.c_str(),mid4.c_str());
        /*else if(enddeclare==true){//局部数组声明

        }*/
	}
    else if(mid1=="ENDDECLARE"){
        for(int i=0;i<strlabelarr.size();i++){
            fprintf(output_mips_pointer,"string%d: .asciiz \"%s\"\n",i,strlabelarr[i].c_str());
        }
        enddeclare=true;//常量/变量声明结束
        fprintf(output_mips_pointer,".text\n");
        fprintf(output_mips_pointer,"jal main\n");
        fprintf(output_mips_pointer,"nop\n");
    }
    else if(mid1=="func"){
        func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        fprintf(output_mips_pointer,"%s:\n",mid3.c_str());
        fprintf(output_mips_pointer,"subi $sp,$sp,%d\n",fsize[getfunclvl(mid3)]);//getfuncsize(mid3)
        //fprintf(output_mips_pointer,"#func[i] is*******%d\n",fsize[getfunclvl(mid3)]);
        fprintf(output_mips_pointer,"sw $ra,0($sp)\n");//$ra:返回地址
    }
    else if(mid1=="parameter"){
        func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        paranum=getparan(mid3);//查看有几个参数
		index=getindex(mid3);
		//fprintf(output_mips_pointer,"parameter index: %d\n",index);
    }
    else if(mid1=="Vpara"){
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        func_mid();
        mid4=mid;
		if(transNum(mid4)>=getparan2(mid3)){//参数表 Vpara $varn14 f1 1
			printf("%s函数参数过多\n",mid3.c_str());
		}
		fprintf(output_mips_pointer,"lw $a0,%d($sp)\n",getindex(mid2)*4);
		fprintf(output_mips_pointer,"subi $sp,$sp,%d\n",fsize[getfunclvl(mid3)]);//回到函数入口getfuncsize(mid3)
		//fprintf(output_mips_pointer,"#func[i] is*******%d\n",fsize[getfunclvl(mid3)]);
		fprintf(output_mips_pointer,"sw $a0,%d($sp)\n",(transNum(mid4)+1)*4);//mid4*4的是形参
		fprintf(output_mips_pointer,"addi $sp,$sp,%d\n",fsize[getfunclvl(mid3)]);//getfuncsize(mid3)
		//fprintf(output_mips_pointer,"#func[i] is*******%d\n",fsize[getfunclvl(mid3)]);
	}
    else if(mid1=="LODI"){
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;//midcoder val
		fprintf(output_mips_pointer,"li $t0,%s\n",number(mid3).c_str());//不能用transnum
		fprintf(output_mips_pointer,"sw $t0,%d($sp)\n",getindex(mid2)*4);
	}
	else if(mid1=="LODR"){
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
		fprintf(output_mips_pointer,"sw $v0,%d($sp)\n",getindex(mid2)*4);
	}
	else if(mid1=="LODC"){
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        asch=mid3[0];
		fprintf(output_mips_pointer,"li $t8,%d\n",asch);//转为ascii
		fprintf(output_mips_pointer,"sw $t8,%d($sp)\n",getindex(mid2)*4);
	}
	else if(mid1=="LODV"){
		func_mid();
        mid2=mid;
        teststr=mid2;
        func_mid();
        mid3=mid;
        func_mid();
        mid4=mid;
		num=transNum(mid4);
		index2=getindex2(mid3,num);//bug  parameter
		//fprintf(output_mips_pointer,"getlel is:%d\n",getlevel(mid3));
		if((getlevel(mid3)==0)||(index2==-2)){//全局变量   char变量要化成整型
			fprintf(output_mips_pointer,"la $t5,%s\n",mid3.c_str());
			fprintf(output_mips_pointer,"lw $t1,0($t5)\n");
			fprintf(output_mips_pointer,"sw $t1,%d($sp)\n",getindex(mid2)*4);
		}
		else{
			fprintf(output_mips_pointer,"lw $t1,%d($sp)\n",index2*4);
			fprintf(output_mips_pointer,"sw $t1,%d($sp)\n",getindex(mid2)*4);
		}
	}
    else if(mid1=="LODCASE"){
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        func_mid();
        mid4=mid;
		if(mid4=="1"){//字符
		    asch=mid3[0];
			fprintf(output_mips_pointer,"li $t1,%d\n",asch);
            fprintf(output_mips_pointer,"sw $t1,%d($sp)\n",getindex(mid2)*4);//写入内存
		}
		else {//整型
            fprintf(output_mips_pointer,"li $t1,%s\n",mid3.c_str());
            fprintf(output_mips_pointer,"sw $t1,%d($sp)\n",getindex(mid2)*4);//写入内存
        }
    }
    else if(mid1=="LABEL"){
		func_mid();
		mid2=mid;
		fprintf(output_mips_pointer,"%s:\n",mid2.c_str());
	}
    else if(mid1=="JUMP"){
		func_mid();
		mid2=mid;
		fprintf(output_mips_pointer,"jal %s\n",mid2.c_str());
		fprintf(output_mips_pointer,"nop\n");//加nop
	}
	else if(mid1=="=="||mid1=="!="){// != $varn 0 label
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        func_mid();
        mid4=mid;
        if(!isdigit(mid3[0])){//$开头或是字母开头
            fprintf(output_mips_pointer,"lw $t1,%d($sp)\n",getindex(mid2)*4);//获取地址offset+$sp
            fprintf(output_mips_pointer,"lw $t2,%d($sp)\n",getindex(mid3)*4);
            if(mid1=="==")fprintf(output_mips_pointer,"beq $t2,$t1,%s\n",mid4.c_str());
            if(mid1=="!=")fprintf(output_mips_pointer,"bne $t2,$t1,%s\n",mid4.c_str());
        }
        else{//在exp部分完成了前导0 + -
            fprintf(output_mips_pointer,"lw $t1,%d($sp)\n",getindex(mid2)*4);//获取地址offset+$sp
            if(mid1=="==")fprintf(output_mips_pointer,"beq $t1,%s,%s\n",mid3.c_str(),mid4.c_str());
            if(mid1=="!=")fprintf(output_mips_pointer,"bne $t1,%s,%s\n",mid3.c_str(),mid4.c_str());
        }
		fprintf(output_mips_pointer,"nop\n");
	}
    else if(mid1=="="){//"= expnameleft.c_str(),str.c_str(),LVL"
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        func_mid();
        mid4=mid;
		//index=getindex(mid2);
		index2=getindex2(mid3,transNum(mid4));
		if((getlevel(mid3)==0)||(index2==-2)){//全局变量赋值
			fprintf(output_mips_pointer,"lw $t5,%d($sp)\n",getindex(mid2)*4);
			fprintf(output_mips_pointer,"la $t4,%s\n",mid3.c_str());//取地址
			fprintf(output_mips_pointer,"sw $t5,0($t4)\n");
		}
		else {
			fprintf(output_mips_pointer,"lw $t1,%d($sp)\n",getindex(mid2)*4);//局部变量赋值
			fprintf(output_mips_pointer,"sw $t1,%d($sp)\n",index2*4);
		}
	}
    else if(mid1=="neg"){//'-'<项>
		func_mid();
        mid2=mid;
		fprintf(output_mips_pointer,"lw $t1,%d($sp)\n",getindex(mid2)*4);
		fprintf(output_mips_pointer,"sub $t2,$0,$t1\n");
		fprintf(output_mips_pointer,"sw $t2,%d($sp)\n",getindex(mid2)*4);//mid2=-1*mid;
	}
	else if(mid1=="+"||mid1=="-"||mid1=="*"||mid1=="/"){
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        func_mid();
        mid4=mid;
		fprintf(output_mips_pointer,"lw $t2,%d($sp)\n",getindex(mid2)*4);
		fprintf(output_mips_pointer,"lw $t3,%d($sp)\n",getindex(mid3)*4);
		if(mid1=="+"){
				fprintf(output_mips_pointer,"add $t3,$t2,$t3\n");
				fprintf(output_mips_pointer,"sw $t3,%d($sp)\n",getindex(mid2)*4);
		}
		else if(mid1=="-"){
			fprintf(output_mips_pointer,"sub $t2,$t2,$t3\n");/////////////////////add fix
			fprintf(output_mips_pointer,"sw $t2,%d($sp)\n",getindex(mid2)*4);
		}
		else if(mid1=="*"){
			fprintf(output_mips_pointer,"mult $t2,$t3\n");
			fprintf(output_mips_pointer,"mflo $t3\n");//取值
			fprintf(output_mips_pointer,"sw $t3,%d($sp)\n",getindex(mid2)*4);
		}
		else if(mid1=="/"){
			fprintf(output_mips_pointer,"div $t2,$t3\n");
			fprintf(output_mips_pointer,"mflo $t3\n");//取值
			fprintf(output_mips_pointer,"sw $t3,%d($sp)\n",getindex(mid2)*4);
        }
	}
	else if(mid1=="eql0"){
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        func_mid();
        mid4=mid;
		fprintf(output_mips_pointer,"lw $t7,%d($sp)\n",getindex(mid2)*4); //if(i)没有左值
		fprintf(output_mips_pointer,"c\n",mid4.c_str());
	}
	else if(mid1==">"||mid1=="<"||mid1==">="||mid1=="<="){
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        func_mid();
        mid4=mid;
		//index1=getindex(op3);
		fprintf(output_mips_pointer,"lw $t1,%d($sp)\n",getindex(mid2)*4);
		fprintf(output_mips_pointer,"lw $t2,%d($sp)\n",getindex(mid3)*4);
		fprintf(output_mips_pointer,"sub $t3,$t1,$t2\n");/////////////////////////sub    mid2
		if(mid1==">")fprintf(output_mips_pointer,"bgtz $t3,%s\n",mid4.c_str()); //0
		else if(mid1=="<")fprintf(output_mips_pointer,"bltz $t3,%s\n",mid4.c_str());
		else if(mid1=="<=")fprintf(output_mips_pointer,"blez $t3,%s\n",mid4.c_str());
		else if(mid1==">=")fprintf(output_mips_pointer,"bgez $t3,%s\n",mid4.c_str());
		fprintf(output_mips_pointer,"nop\n");
	}
    else if(mid1=="JUMPEND"){//return
		  fprintf(output_mips_pointer,"jal endlabel\n");
	}
    else if(mid1=="END"){//////////////////////doubt
		func_mid();
        mid2=mid;
		fprintf(output_mips_pointer,"lw $ra,0($sp)\n");
		fprintf(output_mips_pointer,"addi $sp,$sp,%d\n",fsize[getfunclvl(mid)]);//栈顶指针到函数结尾getfuncsize(mid2)
		//fprintf(output_mips_pointer,"#fsize[i] is %d\n",fsize[getfunclvl(mid)]);
		fprintf(output_mips_pointer,"jr $ra\n");
	}
	else if(mid1=="LODA"){
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        func_mid();
        mid4=mid;
        func_mid();
        mid5=mid;//level
		index2=getindex2(mid2,transNum(mid5));
		if((getlevel(mid2)==0)||((getindex2(mid2,transNum(mid5))==-2))){//全局数组LODA ,strlwr2(v0).c_str(),expname.c_str(),factorn.c_str(),LVL
			fprintf(output_mips_pointer,"la $t5,%s\n",mid2.c_str());
			fprintf(output_mips_pointer,"lw $t3,%d($sp)\n",getindex(mid3)*4);
			fprintf(output_mips_pointer,"li $t2,4\n");
			fprintf(output_mips_pointer,"mult $t2,$t3\n");
			fprintf(output_mips_pointer,"mflo $t2\n");
			fprintf(output_mips_pointer,"add $t4,$t5,$t2\n");
			fprintf(output_mips_pointer,"lw $t1,0($t4)\n");
			fprintf(output_mips_pointer,"sw $t1,%d($sp)\n",getindex(mid4)*4);
		}
		else{//局部数组
			fprintf(output_mips_pointer,"addi $t5,$sp,%d\n",index2*4);//取局部数组首地址
			fprintf(output_mips_pointer,"lw $t3,%d($sp)\n",getindex(mid3)*4);
			fprintf(output_mips_pointer,"li $t2,4\n");
			fprintf(output_mips_pointer,"mult $t2,$t3\n");
			fprintf(output_mips_pointer,"mflo $t3\n");
			fprintf(output_mips_pointer,"add $t4,$t5,$t3\n");//目标数组地址
			fprintf(output_mips_pointer,"lw $t1,0($t4)\n");
			fprintf(output_mips_pointer,"sw $t1,%d($sp)\n",getindex(mid4)*4);// printfexp
		}
	}
	else if(mid1=="eql"){
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        func_mid();
        mid4=mid;
        func_mid();
        mid5=mid;//level
		index2=getindex2(mid2,transNum(mid5)); //if(cmp(op2,"a"))printf("%d %d %s\n",index2,getlel(op2),op2);
		if((getlevel(mid2)==0)||(index2==-2)){
			fprintf(output_mips_pointer,"la $t5,%s\n",mid2.c_str());
			fprintf(output_mips_pointer,"lw $t3,%d($sp)\n",getindex(mid3)*4);
			fprintf(output_mips_pointer,"li $t2,4\n");
			fprintf(output_mips_pointer,"mult $t2,$t3\n");
			fprintf(output_mips_pointer,"mflo $t2\n");
			fprintf(output_mips_pointer,"add $t4,$t5,$t2\n");
			fprintf(output_mips_pointer,"lw $t1,%d($sp)\n",getindex(mid4)*4);
			fprintf(output_mips_pointer,"sw $t1,0($t4)\n");
		}
		else{
			fprintf(output_mips_pointer,"addi $t5,$sp,%d\n",index2*4);
			fprintf(output_mips_pointer,"lw $t3,%d($sp)\n",getindex(mid3)*4);
			fprintf(output_mips_pointer,"li $t2,4\n");
			fprintf(output_mips_pointer,"mult $t2,$t3\n");
			fprintf(output_mips_pointer,"mflo $t2\n");
			fprintf(output_mips_pointer,"add $t4,$t5,$t2\n");
			fprintf(output_mips_pointer,"lw $t1,%d($sp)\n",getindex(mid4)*4);
			fprintf(output_mips_pointer,"sw $t1,0($t4)\n");
		}
	}
    else if(mid1=="SCANF"){
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
		index2=getindex2(mid2,transNum(mid3));
		scanftype=getvartype2(mid2,transNum(mid3));
		//fprintf(output_mips_pointer,"scanftype is :************%d %s\n",scanftype,mid2.c_str());
		if((getlevel(mid2)==0)||(index2==-2)){//全局变量
			if(scanftype==0){//int
				fprintf(output_mips_pointer,"la $t5,%s\n",mid2.c_str());
				fprintf(output_mips_pointer,"li $v0,5\n");
				fprintf(output_mips_pointer,"syscall\n");
				fprintf(output_mips_pointer,"sw $v0,0($t5)\n");
			}
			else if(scanftype==1){//char
				fprintf(output_mips_pointer,"la $t5,%s\n",mid2.c_str());
				fprintf(output_mips_pointer,"li $v0,12\n");
				fprintf(output_mips_pointer,"syscall\n");
				fprintf(output_mips_pointer,"sw $v0,0($t5)\n");
			}
		}
		else{//局部变量
			if(scanftype==0){
				fprintf(output_mips_pointer,"li $v0,5\n");
				fprintf(output_mips_pointer,"syscall\n");
				fprintf(output_mips_pointer,"sw $v0,%d($sp)\n",index2*4);
			}
			else if(scanftype==1){
				fprintf(output_mips_pointer,"li $v0,12\n");
				fprintf(output_mips_pointer,"syscall\n");
				fprintf(output_mips_pointer,"sw $v0,%d($sp)\n",index2*4);
			}
		}
    }
    else if(mid1=="PRINTFSTR"){
		func_mid();
        mid2=mid;
		fprintf(output_mips_pointer,"li $v0,4\n");
		fprintf(output_mips_pointer,"la $a0,%s\n",mid2.c_str());
		fprintf(output_mips_pointer,"syscall\n");
	}
	else if(mid1=="PRINTFEXPINT"){
        func_mid();
        mid2=mid;
        fprintf(output_mips_pointer,"li $v0,1\n");
        fprintf(output_mips_pointer,"lw $a0,%d($sp)\n",getindex(mid2)*4);
        fprintf(output_mips_pointer,"syscall\n");
	}
    else if(mid1=="PRINTFEXP"){
		func_mid();
        mid2=mid;
		if(getvartype(mid2)==0){//int  表达式都是以int形式输出
			fprintf(output_mips_pointer,"li $v0,1\n");
			fprintf(output_mips_pointer,"lw $a0,%d($sp)\n",getindex(mid2)*4);
			fprintf(output_mips_pointer,"syscall\n");
		}
		else if(getvartype(mid2)==1){//char
			fprintf(output_mips_pointer,"li $v0,11\n");
			fprintf(output_mips_pointer,"lw $a0,%d($sp)\n",getindex(mid2)*4);
			fprintf(output_mips_pointer,"syscall\n");
		}
		else{
			printf("不可打印此类型的数据\n");
			fprintf(output_error,"不可打印此类型的数据\n");
		}
	}
	else if(mid1=="RETURN1"){//()
		func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        func_mid();
        mid4=mid;
		offset2=getrfuncsize(transNum(mid4));
		fprintf(output_mips_pointer,"lw $v0,%d($sp)\n",getindex(mid2)*4);
		fprintf(output_mips_pointer,"lw $ra,0($sp)\n");
		fprintf(output_mips_pointer,"addi $sp,$sp,%d\n",fsize[transNum(mid4)-1]);//offset2
		//fprintf(output_mips_pointer,"#func[i] is*******%d\n",fsize[transNum(mid4)-1]);
		fprintf(output_mips_pointer,"jr $ra\n");
		fprintf(output_mips_pointer,"nop\n");
	}
	else if(mid1=="RETURN2"){//;
		func_mid();
        mid2=mid;//lvl
		offset2=getrfuncsize(transNum(mid2));
		fprintf(output_mips_pointer,"lw $ra,0($sp)\n");
		fprintf(output_mips_pointer,"addi $sp,$sp,%d\n",fsize[transNum(mid2)-1]);//offset2
		//fprintf(output_mips_pointer,"#func[i] is*******%d\n",fsize[transNum(mid2)-1]);
		fprintf(output_mips_pointer,"jr $ra\n");
		fprintf(output_mips_pointer,"nop\n");
	}
	/*else if(mid1=="ARRAYINT"){
        func_mid();
        mid2=mid;
        func_mid();
        mid3=mid;
        func_mid();
        mid4=mid;
        fprintf(output_mips_pointer,"数组越界 array out the range\n");
        fprintf(output_error,"数组越界 array out the range\n");
        return 0;
	}*/
	else
        return 0;
}

int main(){//存在死循环  debug
        ch=' ';
        int flag=INT_MAX;
        string fname;
        printf("输入文件名\n");
        cin>>fname;
        //fname="testwrong.txt";
        input_file_pointer = fopen(fname.c_str(),"r");
        output_grammar_pointer = fopen("grammer.txt","w");
        output_words_pointer = fopen("output.txt","w");
        output_midcoder_pointer = fopen("midcode.txt","w");
        output_mips_pointer = fopen("mips.txt","w");
        output_error =fopen("error.txt","w");
        //cout<<"11111111111"<<endl;
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
        fclose(output_midcoder_pointer);
        getallfuncsize();
        output_mid_pointer = fopen("midcode.txt","r");//读取中间代码
        fprintf(output_mips_pointer,".data\n");
        while(flag!=0){
            flag=mips();
        }
        fprintf(output_mips_pointer,"endlabel:\n");
        fclose(input_file_pointer);
        fclose(output_grammar_pointer);
        fclose(output_words_pointer);
        fclose(output_mid_pointer);
        fclose(output_mips_pointer);
        fclose(output_error);
        printf("lvl is%d\n",LVL);
        //printftable();
        //printfindex();
        //printf("LVL IS:******%d %d\n",symtable[TableTop-1].lev,LVL);//main LVL-1层
        return 0;
}
