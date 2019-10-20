#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct student
{
	int data;//���浱ǰ�ڵ����ֵ 
	struct student* NextPtr;//������һ�ڵ�ĵ�ַ 
}STU;
typedef STU* StuPtr;
StuPtr CreateFIFOList();//�������� 
int MaxNum(StuPtr headPtr);//�����ֵ 
int MinNum(StuPtr headPtr);//����Сֵ 
int SumNum(StuPtr headPtr);//��� 
void DestroyList(StuPtr headPtr);//�ͷ����� 
int main()
{
	int max,min,sum;//�������ֵ,��Сֵ����ֵ֮�� 
	printf("Please input a series of integers:");
	StuPtr headPtr=CreateFIFOList();//���������ͷ��� 
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
	int num;//���浱ǰ��ȡ��ֵ 
	StuPtr headPtr=NULL,currentPtr=NULL,lastPtr=NULL;//��������ͷ���,β�ڵ�,��ǰ�ڵ� 
	scanf("%d",&num);
	while(num!=-1)//��ֵ���� 
	{
		currentPtr=malloc(sizeof(STU));//����ڵ��ڴ� 
		if(currentPtr!=NULL)//����ڵ� 
		{
			currentPtr->data=num;
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
		scanf("%d",&num);//��ȡ��һ��ֵ 
	}
	lastPtr->NextPtr=NULL;//�������������� 
	return headPtr;//��������ͷ�ڵ� 
}

int MaxNum(StuPtr headPtr)
{
	int n;//���浱ǰ��ֵ 
	int max=headPtr->data;//�������ֵ����ʼ�� 
	while(headPtr!=NULL)//����Ϊ�� 
	{
		if(headPtr->data>max)//�Ƚ���ֵ 
		{
			max=headPtr->data; 
		}
		headPtr=headPtr->NextPtr;//ָ����һ�ڵ� 
	}
	return max;
}

int MinNum(StuPtr headPtr)
{
	int n;//���浱ǰ��ֵ
	int min=headPtr->data;//������Сֵ����ʼ��
	while(headPtr!=NULL)//����Ϊ��
	{
		if(headPtr->data<min)//�Ƚ���ֵ
		{
			min=headPtr->data;
		}
		headPtr=headPtr->NextPtr;//ָ����һ�ڵ� 
	}
	return min;
}

int SumNum(StuPtr headPtr)
{
	int sum=0;//������ֵ֮�� 
	while(headPtr!=NULL)//����Ϊ�� 
	{
		sum+=headPtr->data;
		headPtr=headPtr->NextPtr;//ָ����һ�ڵ� 
	}
	return sum;
}
		
void DestroyList(StuPtr headPtr)
{
	StuPtr tempPtr;//�������� 
	while(headPtr)
	{
		tempPtr=headPtr;
	    headPtr=headPtr->NextPtr;//ָ����һ��Ҫɾ���Ľڵ� 
	    free(tempPtr);//�ͷŽڵ� 
	}
}
		
