#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
typedef struct charlist
{
	char ch[5];//���浱ǰ�ڵ���ַ� 
	struct charlist* NextPtr;//������һ�ڵ�ĵ�ַ 
}LIST;
typedef LIST* LISTPtr;
LISTPtr CreateList();//�������� 
void PrintList(LISTPtr headPtr,char ch);//��ӡ���� �������к� 
void DestroyList(LISTPtr headPtr);//�ͷ�����
void DevideList(LISTPtr headPtr0,LISTPtr headPtr1,LISTPtr headPtr2,LISTPtr headPtr3);//��������������
LISTPtr BubbleSortList(LISTPtr headPtr);//���������ð������  
int main()
{
	LISTPtr headPtr0=NULL,headPtr1=NULL,headPtr2=NULL,headPtr3=NULL;
	/*�ֱ�ָ���ĸ�������L,A,B,C��ͷ���*/
	printf("Please input a list:"); 
	headPtr0=CreateList();//�������� 
	headPtr1=malloc(sizeof(LIST));
	headPtr2=malloc(sizeof(LIST));
	headPtr3=malloc(sizeof(LIST));
	
	DevideList(headPtr0,headPtr1,headPtr2,headPtr3);//������L���䵽��Ӧ���������� 
	headPtr1=BubbleSortList(headPtr1);	
	headPtr2=BubbleSortList(headPtr2);
	headPtr3=BubbleSortList(headPtr3);//�������������ð������ 
	PrintList(headPtr1,'A');
	PrintList(headPtr2,'B');
	PrintList(headPtr3,'C');//��ӡ�б�
	DestroyList(headPtr1);
	headPtr1=NULL;
	DestroyList(headPtr2);
	headPtr2=NULL;
	DestroyList(headPtr3);
	headPtr3=NULL;//�ͷ�����
} 

void DevideList(LISTPtr headPtr0,LISTPtr headPtr1,LISTPtr headPtr2,LISTPtr headPtr3)
{
	LISTPtr currentPtr0;
	currentPtr0=headPtr0;
	while(currentPtr0!=NULL)
	{
		if(strcmp(currentPtr0->ch,"A")>=0&&strcmp(currentPtr0->ch,"Z")<=0||strcmp(currentPtr0->ch,"a")>=0&&strcmp(currentPtr0->ch,"z")<=0)
		/*��Ӧ�ַ���������ĸ*/
		{
			headPtr1->NextPtr=currentPtr0;
			headPtr1=currentPtr0;
		}
		else if(strcmp(currentPtr0->ch,"0")>=0&&strcmp(currentPtr0->ch,"9")<=0)//��Ӧ�ַ�����������
		{
			headPtr2->NextPtr=currentPtr0;
			headPtr2=currentPtr0;
		}
		else//�����ַ� 
		{
			headPtr3->NextPtr=currentPtr0;
		    headPtr3=currentPtr0;
		}
		currentPtr0=currentPtr0->NextPtr;//���������һ�ڵ���бȽ� 
	}
	headPtr1->NextPtr=NULL;headPtr2->NextPtr=NULL;headPtr3->NextPtr=NULL;//�������������� 
}
 
LISTPtr CreateList()
{
	char ch[5];//���浱ǰ��ȡ�ַ� 
	LISTPtr headPtr=NULL,currentPtr=NULL,lastPtr=NULL;//��������ͷ���,��ǰ�ڵ㣬β�ڵ㡣 
	printf("PLease input a list:"); 
	scanf("%s",&ch);
	while(strcmp(ch,"-1")!=0)
	{
		currentPtr=malloc(sizeof(LIST));//����ڵ��ڴ� 
		if(currentPtr!=NULL)//����ڵ� 
		{
			strcpy(currentPtr->ch,ch);
			if(headPtr==NULL)//����������ͷ��� 
			{
				headPtr=currentPtr;
				lastPtr=currentPtr;
			}
			else
			{
				lastPtr->NextPtr=currentPtr;//����ǰ�ڵ�������β�ڵ����� 
			    lastPtr=currentPtr;//ʹlastPtrָ��ǰ�����β�ڵ� 
			}
		}
		scanf("%s",&ch);//��ȡ��һ�ַ� 
	}
	lastPtr->NextPtr=NULL;//�������������� 
	return headPtr;//��������ͷ�ڵ� 
}

LISTPtr BubbleSortList(LISTPtr headPtr) 
{
	LISTPtr IPtr=NULL;//�൱��ð�������е�i
	LISTPtr JPtr=NULL;//�൱��ð�������е�j
	LISTPtr tempPtr=NULL;//�����õĽڵ�
	LIST temp;//�����õĽṹ��
    for(IPtr=headPtr;IPtr->NextPtr!=NULL;IPtr=IPtr->NextPtr)//����ð������
	{
	    for(JPtr=IPtr->NextPtr;JPtr!=NULL;JPtr=JPtr->NextPtr)
		{
		    if(strcmp(IPtr->ch,JPtr->ch)>0)
		    {
		     	temp=*IPtr;//��һ��������ָ���� 
		     	*IPtr=*JPtr;
		     	*JPtr=temp;
		     	tempPtr=IPtr->NextPtr;//�ڶ�����������һ��ָ���ָ�� 
                IPtr->NextPtr=JPtr->NextPtr;
                JPtr->NextPtr=tempPtr;
            }
        }
    }
    return headPtr;
}

 void PrintList(LISTPtr headPtr,char ch) 
 {
	if(headPtr==NULL)
	{
		printf("There is no item in %c list.",ch);
	}
	else
	{
		printf("The list%c is:",ch);
	    while(headPtr->NextPtr->NextPtr!=NULL)
		{
	        printf("%s ",headPtr->ch);
		    headPtr=headPtr->NextPtr;
	    } 
	    printf("%s\n",headPtr->ch);
	}
}

void DestroyList(LISTPtr headPtr)
 {
 	LISTPtr tempPtr;
	while (headPtr!=NULL)
	{
        tempPtr=headPtr;
        headPtr=headPtr->NextPtr;//headPtrָ����һ��Ҫɾ���Ľ��
        free(tempPtr);//�ͷŵ�ǰ���
	}
} 


