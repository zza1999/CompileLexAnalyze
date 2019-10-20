#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct student
{
	int data;//保存当前节点的数值 
	struct student* NextPtr;//保存下一节点的地址 
}STU;
typedef STU* StuPtr;
StuPtr CreateFIFOList();//创建链表 
int MaxNum(StuPtr headPtr);//求最大值 
int MinNum(StuPtr headPtr);//求最小值 
int SumNum(StuPtr headPtr);//求和 
void DestroyList(StuPtr headPtr);//释放链表 
int main()
{
	int max,min,sum;//储存最大值,最小值和数值之和 
	printf("Please input a series of integers:");
	StuPtr headPtr=CreateFIFOList();//储存链表的头结点 
    max=MaxNum(headPtr);
    min=MinNum(headPtr);
    sum=SumNum(headPtr);
    printf("The maximum,minmum and the total are:%d %d %d\n",max,min,sum); 
	DestroyList(headPtr);
	headPtr=NULL;
	return 0;
}

StuPtr CreateFIFOList()
{
	int num;//储存当前读取数值 
	StuPtr headPtr=NULL,currentPtr=NULL,lastPtr=NULL;//储存链表头结点,尾节点,当前节点 
	scanf("%d",&num);
	while(num!=-1)//数值输入 
	{
		currentPtr=malloc(sizeof(STU));//分配节点内存 
		if(currentPtr!=NULL)//插入节点 
		{
			currentPtr->data=num;
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
		scanf("%d",&num);//读取下一数值 
	}
	lastPtr->NextPtr=NULL;//设置链表结束标记 
	return headPtr;//返回链表头节点 
}

int MaxNum(StuPtr headPtr)
{
	int n;//储存当前数值 
	int max=headPtr->data;//储存最大值并初始化 
	while(headPtr!=NULL)//链表不为空 
	{
		if(headPtr->data>max)//比较数值 
		{
			max=headPtr->data; 
		}
		headPtr=headPtr->NextPtr;//指向下一节点 
	}
	return max;
}

int MinNum(StuPtr headPtr)
{
	int n;//储存当前数值
	int min=headPtr->data;//储存最小值并初始化
	while(headPtr!=NULL)//链表不为空
	{
		if(headPtr->data<min)//比较数值
		{
			min=headPtr->data;
		}
		headPtr=headPtr->NextPtr;//指向下一节点 
	}
	return min;
}

int SumNum(StuPtr headPtr)
{
	int sum=0;//储存数值之和 
	while(headPtr!=NULL)//链表不为空 
	{
		sum+=headPtr->data;
		headPtr=headPtr->NextPtr;//指向下一节点 
	}
	return sum;
}
		
void DestroyList(StuPtr headPtr)
{
	StuPtr tempPtr;//储存链表 
	while(headPtr)
	{
		tempPtr=headPtr;
	    headPtr=headPtr->NextPtr;//指向下一个要删除的节点 
	    free(tempPtr);//释放节点 
	}
}
		
