#include<iostream>
#include<string.h>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>
#include"syntax.h"
using namespace std;
enum S{
    CONSTSY = 1, INTSY, CHARSY, VOIDSY, MAINSY, SCANFSY, PRINTFSY, IDSY,
    RETURNSY, IFSY, ELSESY, DOSY, WHILESY,SWITCHSY,CASESY,DEFAULTSY,
    LBRACESY = 30, RBRACESY, LPARSY, RPARSY, LBRACKETSY, RBRACKETSY, SEMISY, COMMASY, PLUSSY, MINUSSY, STARSY, SLASHSY,
    ISSY, LSSY, NBGSY, BGSY, NLSSY, NEQSY, EQUSY,COLONSY,
    INTCON = 60, CHARCON, STRINGCON,
    ENDSY = 99
};
/*
1:const 2.int 3.char 4.void 5.main 6.scanf 7.printf 8.��ʶ�� 9.return 10.if 11.else 12.do
13.while
30.{ 31.} 32.( 33.) 34.[ 35.] 36.; 37., 38.+ 39.- 40.* 41./ 42.= 43.< 44.<=
45.> 46.>= 47.!= 48.== 49.:
50.���� 51.�ַ� 52.�ַ���
*/
enum S resultvalue;
enum S symbol0;
enum S symbol1;
enum S symbol2;
int num=0;
int k=0;
string token;
char ch;
FILE *input_file_pointer;
FILE *output_words_pointer;
FILE *output_error;
void error(int n){
    switch(n){
        case 1:printf("!������˷Ƿ��ַ�Ӧ����=\n");fprintf(output_error,"!������˷Ƿ��ַ�Ӧ����=\n");break;
        case 2:printf("�������ݳ���\n");fprintf(output_error,"�������ݳ���\n");break;
        case 3:printf("ȱ�ٵ����Ż����ַ����ȴ���һ\n");fprintf(output_error,"ȱ�ٵ����Ż����ַ����ȴ���һ\n");break;
        case 4:printf("�ַ����г��ֲ��ɱ�ʶ�ַ�������ļ�β\n");fprintf(output_error,"�ַ����г��ֲ��ɱ�ʶ�ַ�������ļ�β\n");break;
        case 5:printf("���ֲ���ʶ���ַ�\n");fprintf(output_error,"���ֲ���ʶ���ַ�\n");break;
        case 6:printf("������ڴ���\n");cout<<token<<" "<<ch;fprintf(output_error,"������ڴ���\n");break;
        case 7:printf("����˵������ȱ�ٷֺ�\n");fprintf(output_error,"����˵������ȱ�ٷֺ�\n");break;
        case 8:printf("�������岿��char��δ���ַ�\n");fprintf(output_error,"�������岿��char��δ���ַ�\n");break;
        case 9:printf("�����������\n");fprintf(output_error,"�����������\n");break;
        case 10:printf("��������\n");fprintf(output_error,"��������\n");break;
        case 11:printf("����˵������ȱ�ٷֺ�\n");fprintf(output_error,"����˵������ȱ�ٷֺ�\n");break;
        case 12:printf("�����������ȱ�� ] \n");fprintf(output_error,"�����������ȱ�� ] \n");break;
        case 13:printf("�����������\n");fprintf(output_error,"�����������\n");break;
        case 14:printf("�з���ֵ�����������ȱ��������\n");fprintf(output_error,"�з���ֵ�����������ȱ��������\n");break;
        case 15:printf("�з���ֵ�����������ȱ���һ�����\n");fprintf(output_error,"�з���ֵ�����������ȱ���һ�����\n");break;
        case 16:printf("������Ϊ��\n");fprintf(output_error,"����������������Ӧ�ò�Ϊ��\n");break;
        case 17:printf("�з���ֵ�����������\n");fprintf(output_error,"�з���ֵ�����������\n");break;
        case 18:printf("����ͷ������\n");fprintf(output_error,"����ͷ������\n");break;
        case 19:printf("���������\n");fprintf(output_error,"���������\n");break;
        case 20:printf("������\n");cout<<token<<" "<<ch<<endl;fprintf(output_error,"������\n");break;
        case 21:printf("������δʶ���ַ�\n");fprintf(output_error,"������δʶ���ַ�\n");break;
        case 22:printf("if������\n");cout<<token<<" "<<ch<<endl;fprintf(output_error,"if������\n");break;
        case 23:printf("while������\n");fprintf(output_error,"while������\n");break;
        case 24:printf("��������\n");fprintf(output_error,"��������\n");break;
        case 25:printf("switch-case������\n");cout<<token<<" "<<ch;fprintf(output_error,"switch-case������\n");break;
        case 26:printf("����������\n");fprintf(output_error,"����������\n");break;
        case 27:printf("δƥ�䵽����\n");fprintf(output_error,"δƥ�䵽����\n");break;
        case 28:printf("���ӳ���\n");fprintf(output_error,"���ӳ���\n");break;
        case 29:printf("��ֵ������\n");fprintf(output_error,"��ֵ������\n");break;
        case 30:printf("case������\n");fprintf(output_error,"case������\n");break;
        case 31:printf("default������\n");fprintf(output_error,"default������\n");break;
        case 32:printf("�з���ֵ��������������\n");fprintf(output_error,"�з���ֵ��������������\n");break;
        case 33:printf("���������\n");fprintf(output_error,"���������\n");break;
        case 34:printf("��������\n");fprintf(output_error,"��������\n");break;
        case 35:printf("����������\n");fprintf(output_error,"����������\n");break;
        case 36:printf("�޷���ֵ�����������\n");fprintf(output_error,"�޷���ֵ�����������\n");break;
        case 37:printf("���ű����\n");fprintf(output_error,"���ű����\n");break;
        case 38:printf("�ظ�������ʶ��\n");fprintf(output_error,"�ظ�������ʶ��\n");break;
        case 39:printf("�з���ֵ����ȱ��return���\n");fprintf(output_error,"�з���ֵ����ȱ��return���\n");break;
        case 40:printf("δ����˱�ʶ��\n");fprintf(output_error,"δ����˱�ʶ��\n");break;
        case 41:printf("δ����˺���\n");fprintf(output_error,"δ����˺���\n");break;
        case 42:printf("const midcode����\n");fprintf(output_error,"const midcode����\n");break;
        case 43:printf("û�ж���ú���\n");fprintf(output_error,"û�ж���ú���\n");break;
        case 44:printf("�з���ֵ������Ϊ�����\n");fprintf(output_error,"�з���ֵ������Ϊ�����\n");break;
        case 45:printf("����Խ��\n");fprintf(output_error,"����Խ��\n");break;
        default: ;
     }
    cout<<"near"<<" "<<token<<endl;
}
char getch(){
    return fgetc(input_file_pointer);
}
void ungetch(char ch){
    ungetc(ch, input_file_pointer);
}
void catToken(){//ÿ�ε��ý�ch���ַ���token�ַ�������
    /*char c_t[2] = {ch,'\0'};
    strcat(token,c_t);*/
    token+=ch;
}
void clearToken(){
	/*int i;
	for( i=0;i<100;i++){
	   token[i]='\0';//���token����
	}*/
	token.clear();
}
S searchreserve(){
    if(strlwr2(token)=="const")
        return CONSTSY;
    else if(strlwr2(token)=="int")
        return INTSY;
    else if(strlwr2(token)=="char")
        return CHARSY;
    else if(strlwr2(token)=="void")
        return VOIDSY;
    else if(strlwr2(token)=="main")
        return MAINSY;
    else if(strlwr2(token)=="scanf")
        return SCANFSY;
    else if(strlwr2(token)=="printf")
        return PRINTFSY;
    else if(strlwr2(token)=="return")
        return RETURNSY;
    else if(strlwr2(token)=="if")
        return IFSY;
    else if(strlwr2(token)=="else")
        return ELSESY;
    else if(strlwr2(token)=="do")
        return DOSY;
    else if(strlwr2(token)=="while")
        return WHILESY;
    else if(strlwr2(token)=="switch")
        return SWITCHSY;
    else if(strlwr2(token)=="case")
        return CASESY;
    else if(strlwr2(token)=="default")
        return DEFAULTSY;
    else
        return IDSY;
}
int isColon(char c){//ð��
  if(c==':')
	  return 1;
  return 0;
}
int isComma(char c){//����
	if(c==',')
		return 1;
	return 0;
}
int isSemi(char c){//�ֺ�
	if(c==';')
		return 1;
	return 0;
}
int isEqu(char c){//�Ⱥ�
	if(c=='=')
		return 1;
	return 0;
}
int isDy(char c){//'������
	if(c=='\'')
		return 1;
	return 0;
}
int isSy(char c){//"˫����
	if(c=='\"')
		return 1;
	return 0;
}
int isPlus(char c){//+
    if(c=='+')
		return 1;
	return 0;
}
int isMinus(char c){//-
	if(ch=='-')
		return 1;
	return 0;
}
int isMult(char c){//*
	if(c=='*')
		return 1;
	return 0;
}
int isDiv(char c){//-
   if(c=='/')
	   return 1;
   return 0;
}
int Lpar(char c){//(
    if(c=='(')
      return 1;
	return 0;
}
int Rpar(char c){//)
    if(c==')')
      return 1;
	return 0;
}
int Lzpar(char c){//[
    if(c=='[')
      return 1;
	return 0;
}
int Rzpar(char c){//]
    if(c==']')
      return 1;
	return 0;
}
int Lhpar(char c){//{
    if(c=='{')
      return 1;
	return 0;
}
int Rhpar(char c){//}
	if(c=='}')
      return 1;
	return 0;
}
int Dyu(char c){//>
	if(c=='>')
		return 1;
	return 0;
}
int Xyu(char c){//<
	if(c=='<')
		return 1;
	return 0;
}
int Gth(char c){
    if(c=='!')
		return 1;
	return 0;
}

int transNum(string s){//ʮ�����ַ���ת��Ϊ���� -2
    int res = 0;
	for(unsigned int i=0;i<s.size();i++)
	{
	 res= res*10 + (s[i] - '0');
    }
	return res;
}

bool isletter(char c){
    if(c=='_'||isalpha(c))return true;
    else return false;
}
void getsym(){ //����ȥ���ո�,tab,����
    clearToken();
    ch=getch();
    while(ch==' '||ch=='\n'||ch=='\t')
        ch=getch();
    if(isletter(ch)){
        while(isletter(ch)||isdigit(ch)){
            catToken();
            //cout<<token<<" "<<ch;
            ch=getch();
        }
        ungetch(ch);//�����ַ�
        resultvalue=searchreserve();
    }
    else if(isdigit(ch)){
        while(isdigit(ch)){
		   catToken();
		   ch=getch();
		}
		ungetch(ch);
	    num=transNum(token);
	    resultvalue=INTCON;
    }
    else if(Dyu(ch)){//> >=
        catToken();//��>����string token
        ch=getch();
        if(ch=='='){
            resultvalue=NLSSY;
            catToken();
		}
		else{
            resultvalue=BGSY;
            ungetch(ch);
		}
    }
    else if(Xyu(ch)){//< <=
        catToken();
        ch=getch();
        if(ch=='='){
            resultvalue=NBGSY;
            catToken();
        }
        else{
            resultvalue=LSSY;
            ungetch(ch);
        }
    }
    else if(Gth(ch)){//! !=
         catToken();
	     ch=getch();
		 if(ch=='='){
			 resultvalue=NEQSY;
			 //catToken();
		 }
		 else{
			 error(1);//���� ��=
		     ungetch(ch);
		 }
		 catToken();
		 resultvalue=NEQSY;
	}
	else if(isEqu(ch)){//= ==
        catToken();
        ch=getch();
		if(ch=='='){
			resultvalue=EQUSY;
		    catToken();
		}
		else{
            resultvalue=ISSY;
            ungetch(ch);
		}
	}
	else if(isDy(ch)){//////////bug ���ȴ���һ
        ch=getch();
        if(!isdigit(ch)&&!isletter(ch)&&!isMinus(ch)&&!isMult(ch)&&!isPlus(ch)&&!isDiv(ch))
            error(2);//�������ݳ���
        else{
            catToken();
            resultvalue=CHARCON;
        }
        ch=getch();
        if(!isDy(ch))
            error(3);//ȱ�ٵ�����
	}
	else if(isSy(ch)){
        ch=getch();
        while(!isSy(ch)){
            if((int)ch != 32 && (int)ch != 33 && ((int)ch < 35 || (int)ch > 126)){
                error(4);//����4���ַ����г��ֲ��ɱ�ʶ�ַ�������ļ�β
                break;
            }
            catToken();
            ch = getch();
        }
        resultvalue = STRINGCON;
	}
	else if(isColon(ch))
	{resultvalue=COLONSY;catToken();}
	else if(isMinus(ch))//������
	{
	    resultvalue=MINUSSY;
	    catToken();
    }
	else if(isPlus(ch))//������
	{
	    resultvalue=PLUSSY;
	    catToken();
    }
	else if(isMult(ch))
	{resultvalue=STARSY;catToken();}
	else if(isDiv(ch))
	{resultvalue=SLASHSY;catToken();}
	else if(Lpar(ch))
	{resultvalue=LPARSY;catToken();}
	else if(Rpar(ch))
	{resultvalue=RPARSY;catToken();}
	else if(Lzpar(ch))
	{resultvalue=LBRACKETSY;catToken();}
	else if(Rzpar(ch))
	{resultvalue=RBRACKETSY;catToken();}
	else if(Lhpar(ch))
	{resultvalue=LBRACESY;catToken();}
	else if(Rhpar(ch))
	{resultvalue=RBRACESY;catToken();}
	else if(isComma(ch))
	{resultvalue=COMMASY;catToken();}
	else if(isSemi(ch))
	{resultvalue=SEMISY;catToken();}
    else if(feof(input_file_pointer)) resultvalue = ENDSY;
    else error(5);//���ֲ���ʶ���ַ�
    switch(resultvalue){
            case 1: fprintf(output_words_pointer,"CONSTSY "); break;
            case 2: fprintf(output_words_pointer,"INTSY "); break;
            case 3: fprintf(output_words_pointer,"CHARSY "); break;
            case 4: fprintf(output_words_pointer,"VOIDSY "); break;
            case 5: fprintf(output_words_pointer,"MAINSY "); break;
            case 6: fprintf(output_words_pointer,"SCANFSY "); break;
            case 7: fprintf(output_words_pointer,"PRINTFSY "); break;
            case 8: fprintf(output_words_pointer,"IDSY "); break;
            case 9: fprintf(output_words_pointer,"RETURNSY "); break;
            case 10: fprintf(output_words_pointer,"IFSY "); break;
            case 11: fprintf(output_words_pointer,"ELSESY "); break;
            case 12: fprintf(output_words_pointer,"DOSY "); break;
            case 13: fprintf(output_words_pointer,"WHILESY ");break;
            case 14: fprintf(output_words_pointer,"SWITCHSY "); break;
            case 15: fprintf(output_words_pointer,"CASESY"); break;
            case 16: fprintf(output_words_pointer,"DEFAULTSY"); break;
            case 30: fprintf(output_words_pointer,"LBRACESY "); break;
            case 31: fprintf(output_words_pointer,"RBRACESY "); break;
            case 32: fprintf(output_words_pointer,"LPARSY "); break;
            case 33: fprintf(output_words_pointer,"RPARSY "); break;
            case 34: fprintf(output_words_pointer,"LBRACKETSY "); break;
            case 35: fprintf(output_words_pointer,"RBRACKETSY "); break;
            case 36: fprintf(output_words_pointer,"SEMISY "); break;
            case 37: fprintf(output_words_pointer,"COMMASY "); break;
            case 38: fprintf(output_words_pointer,"PLUSSY "); break;
            case 39: fprintf(output_words_pointer,"MINUSSY "); break;
            case 40: fprintf(output_words_pointer,"STARSY "); break;
            case 41: fprintf(output_words_pointer,"SLASHSY "); break;
            case 42: fprintf(output_words_pointer,"ISSY "); break;
            case 43: fprintf(output_words_pointer,"LSSY "); break;
            case 44: fprintf(output_words_pointer,"NBGSY "); break;
            case 45: fprintf(output_words_pointer,"BGSY "); break;
            case 46: fprintf(output_words_pointer,"NLSSY "); break;
            case 47: fprintf(output_words_pointer,"NEQSY "); break;
            case 48: fprintf(output_words_pointer,"EQUSY "); break;
            case 49: fprintf(output_words_pointer,"COLONSY "); break;
            case 60: fprintf(output_words_pointer,"INTCON "); break;
            case 61: fprintf(output_words_pointer,"CHARCON "); break;
            case 62: fprintf(output_words_pointer,"STRINGCON "); break;
            default: ;
        }
        fprintf(output_words_pointer,"%s\n",token.c_str());
        //cout<<token;
}
/*int main(){
       ch=' ';
       //cout<<token;
       input_file_pointer = fopen("test.txt","r");
       output_words_pointer = fopen("output.txt","w");
       while(!feof(input_file_pointer)){
         getsym();
       }
       fclose(input_file_pointer);
       fclose(output_words_pointer);
       return 0;
}*/
