#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
typedef struct charlist
{
	char ch[5];//保存当前节点的字符 
	struct charlist* NextPtr;//保存下一节点的地址 
}LIST;
typedef LIST* LISTPtr;
LISTPtr CreateList();//创建链表 
void PrintList(LISTPtr headPtr,char ch);//打印链表 及其序列号 
void DestroyList(LISTPtr headPtr);//释放链表
void DevideList(LISTPtr headPtr0,LISTPtr headPtr1,LISTPtr headPtr2,LISTPtr headPtr3);//分离三个单链表
LISTPtr BubbleSortList(LISTPtr headPtr);//对链表进行冒泡排序  
int main()
{
	LISTPtr headPtr0=NULL,headPtr1=NULL,headPtr2=NULL,headPtr3=NULL;
	/*分别指向四个单链表L,A,B,C的头结点*/
	printf("Please input a list:"); 
	headPtr0=CreateList();//创建链表 
	headPtr1=malloc(sizeof(LIST));
	headPtr2=malloc(sizeof(LIST));
	headPtr3=malloc(sizeof(LIST));
	
	DevideList(headPtr0,headPtr1,headPtr2,headPtr3);//将链表L分配到对应的三链表中 
	headPtr1=BubbleSortList(headPtr1);	
	headPtr2=BubbleSortList(headPtr2);
	headPtr3=BubbleSortList(headPtr3);//对三个链表进行冒泡排序 
	PrintList(headPtr1,'A');
	PrintList(headPtr2,'B');
	PrintList(headPtr3,'C');//打印列表
	DestroyList(headPtr1);
	headPtr1=NULL;
	DestroyList(headPtr2);
	headPtr2=NULL;
	DestroyList(headPtr3);
	headPtr3=NULL;//释放链表
} 

void DevideList(LISTPtr headPtr0,LISTPtr headPtr1,LISTPtr headPtr2,LISTPtr headPtr3)
{
	LISTPtr currentPtr0;
	currentPtr0=headPtr0;
	while(currentPtr0!=NULL)
	{
		if(strcmp(currentPtr0->ch,"A")>=0&&strcmp(currentPtr0->ch,"Z")<=0||strcmp(currentPtr0->ch,"a")>=0&&strcmp(currentPtr0->ch,"z")<=0)
		/*对应字符串属于字母*/
		{
			headPtr1->NextPtr=currentPtr0;
			headPtr1=currentPtr0;
		}
		else if(strcmp(currentPtr0->ch,"0")>=0&&strcmp(currentPtr0->ch,"9")<=0)//对应字符串属于数字
		{
			headPtr2->NextPtr=currentPtr0;
			headPtr2=currentPtr0;
		}
		else//其他字符 
		{
			headPtr3->NextPtr=currentPtr0;
		    headPtr3=currentPtr0;
		}
		currentPtr0=currentPtr0->NextPtr;//对链表的下一节点进行比较 
	}
	headPtr1->NextPtr=NULL;headPtr2->NextPtr=NULL;headPtr3->NextPtr=NULL;//设置链表结束标记 
}
 
LISTPtr CreateList()
{
	char ch[5];//储存当前读取字符 
	LISTPtr headPtr=NULL,currentPtr=NULL,lastPtr=NULL;//储存链表头结点,当前节点，尾节点。 
	printf("PLease input a list:"); 
	scanf("%s",&ch);
	while(strcmp(ch,"-1")!=0)
	{
		currentPtr=malloc(sizeof(LIST));//分配节点内存 
		if(currentPtr!=NULL)//插入节点 
		{
			strcpy(currentPtr->ch,ch);
			if(headPtr==NULL)//若创建的是头结点 
			{
				headPtr=currentPtr;
				lastPtr=currentPtr;
			}
			else
			{
				lastPtr->NextPtr=currentPtr;//将当前节点与链表尾节点相连 
			    lastPtr=currentPtr;//使lastPtr指向当前链表的尾节点 
			}
		}
		scanf("%s",&ch);//读取下一字符 
	}
	lastPtr->NextPtr=NULL;//设置链表结束标记 
	return headPtr;//返回链表头节点 
}

LISTPtr BubbleSortList(LISTPtr headPtr) 
{
	LISTPtr IPtr=NULL;//相当于冒泡排序中的i
	LISTPtr JPtr=NULL;//相当于冒泡排序中的j
	LISTPtr tempPtr=NULL;//交换用的节点
	LIST temp;//交换用的结构体
    for(IPtr=headPtr;IPtr->NextPtr!=NULL;IPtr=IPtr->NextPtr)//类似冒泡排序
	{
	    for(JPtr=IPtr->NextPtr;JPtr!=NULL;JPtr=JPtr->NextPtr)
		{
		    if(strcmp(IPtr->ch,JPtr->ch)>0)
		    {
		     	temp=*IPtr;//第一步，交换指针域 
		     	*IPtr=*JPtr;
		     	*JPtr=temp;
		     	tempPtr=IPtr->NextPtr;//第二步，交换下一个指针的指向 
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
        headPtr=headPtr->NextPtr;//headPtr指向下一个要删除的结点
        free(tempPtr);//释放当前结点
	}
} 


