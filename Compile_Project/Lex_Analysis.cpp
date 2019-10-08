//�ʷ�����
#include"head.h"

/*ȫ�ֱ���*/
int state;        //�Զ���״̬����
char ch;          //�ַ���������ʱ������ַ�
char token[];     //���ʶ���е��ַ���
bool isKey        
/*��ʶʶ���е��ַ�����ʶ������ǹؼ���
iskey=0���ַ���Ϊʶ�����iskey=1��Ϊ�ؼ���*/;
char* begin;      //��ʼָ��
char* forward;    //��ǰָ��
char  buffer[];   //���뻺����
char keyWord[][20];//�ؼ��ʴ洢��

/*�Ӻ���*/
char getChar(char* forward);  //ȡ��ǰָ����ָ���ַ�
void ignoreSpace(char* forward); 
/*����ʱ�ַ������Ƿ�Ϊspace,����ǣ�������get_char
����ֱ����һ���ǿ��ַ�*/
void charCat(char ch, char* token);//(Ӧ�ÿ�����strcat����)
bool isLetter(char ch);    //�Ƿ�Ϊ��ĸ
bool isDigit(char ch);     //�Ƿ�Ϊ����
void backward(char* forward);           //��ǰ��ָ�����
char* checkKey(char* token);
/*�ж�token�еĵ����Ƿ�Ϊ�ؼ��֣����ǣ�
������Ǻţ���֮����'-1'*/
int strToInt(char* token);//��token�ַ���ת��Ϊ����
double strToF(char* token);//��token�ַ���ת��Ϊ������
char* tableInsert(char* token);
/*��token�еı�ʶ��������ű������ر�ʶ��λ��ָ��*/
void checkError();          //�����ֵĴ���

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