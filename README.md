# BUAA_compile
BUAA_compile

北航计算机系大三编译器设计作业，功能：将符合c0文法的类c语言编译成为汇编语言，但是该设计没有完成优化部分，所以编译成的汇编指令有进一步精简的空间。

c0文法如文件c0_compile所示，现将附加说明贴出来。
附加说明：
（1）char类型的变量或常量，用字符的ASCII码对应的整数参加运算
（2）标识符不区分大小写字母
（3）写语句中，字符串原样输出，单个字符类型的变量或常量输出字符，其他表达式按整型输出 
（4）数组的下标从0开始
（5）情况语句中，switch后面的表达式和case后面的常量只允许出现int和char类型；每个情况子语句执行完毕后，不继续执行后面的情况子语句
（6）语句后并不一定都跟分号，单独的分号表示空语句
（7）函数定义用左右圆括号来区分参数表的有无
（8）函数调用可以只出现标识符
