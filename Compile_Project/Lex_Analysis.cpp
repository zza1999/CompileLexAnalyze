//词法分析
#include"head.h"

/*全局变量*/
int state;        //自动机状态变量
char ch;          //字符变量，此时读入的字符
char token[];     //存放识别中的字符串
bool isKey        
/*标识识别中的字符串是识别符还是关键字
iskey=0，字符串为识别符；iskey=1，为关键字*/;
char* begin;      //开始指针
char* forward;    //向前指针
char  buffer[];   //输入缓冲区
char keyWord[][20];//关键词存储区

/*子函数*/
char getChar(char* forward);  //取向前指针所指的字符
void ignoreSpace(char* forward); 
/*检查此时字符变量是否为space,如果是，则利用get_char
令其直到另一个非空字符*/
void charCat(char ch, char* token);//(应该可以用strcat代替)
bool isLetter(char ch);    //是否为字母
bool isDigit(char ch);     //是否为数字
void backward(char* forward);           //令前进指针后退
char* checkKey(char* token);
/*判断token中的单词是否为关键字，若是，
返回其记号，反之返回'-1'*/
int strToInt(char* token);//将token字符串转换为整数
double strToF(char* token);//将token字符串转换为浮点数
char* tableInsert(char* token);
/*将token中的标识符加入符号表，并返回标识符位置指针*/
void checkError();          //处理发现的错误

void lexAnalysis(char* sourceCode) {

}

char getChar(char* forward) {
     return *forward;
}

void ignoreSpace(char* forward){
	while (*forward==' ') {
		forward++;
	 }
}

void charCat(char ch, char* token) {
	char* tempStr = (char*)malloc(sizeof(char));
	tempStr[0] = ch;
	strcat_s(token,50, tempStr);
	free(tempStr);
}

bool isLetter(char ch) {
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) return true;
	else return false;
}

bool isDigit(char ch) {
	if (ch >= '0' && ch <= '9') return true;
	else return false;
}

void backward(char* forward) {
	forward--;
}

char* checkKey(char* token) {
	int flag = 0;
	char* tempStr = (char*)malloc(sizeof(char) * 100);
	for (int i=0; keyWord[i] !='\0'; i++) {
		if (strcmp(token, keyWord[i])) {
		    flag=1;
			strcpy_s(tempStr,100,keyWord[i]);
			break;
		}
	}
	return tempStr;
}

int strToInt(char* token) {
	int sum = 0;
	for (int i = 0; i < strlen(token); i++) {
		sum = 10 * sum + (int)(token[i] - '0');
	}
	return sum;
}

double strToF(char* token) {
	double sum = 0;
	int pointLoc;
	int flag = 0;
	for (int i = 0; i < strlen(token); i++) {
		if (token[i] != '.' && flag == 0) sum = 10 * sum + (int)(token[i] - '0');
		else if (token[i] == '.') {
			flag = 1;
			pointLoc = i;
		}
		else sum += pow(0.1, (i - pointLoc)) * (int)(token[i] - '0');
	}
	return sum;
}

void tableInsert(char* token) {
	int i;
	for (i = 0; keyWord[i] != "\0"; i++) {}
	strcpy_s(keyWord[i], 100, token);
}

void checkError() {

}