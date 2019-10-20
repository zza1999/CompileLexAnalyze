#include<stdio.h>
int main()
{
	FILE *fin,*fout;
	fin=fopen("input.txt","rb");
	fout=fopen("output.txt","wb");
	int max,min,sum=0,n=0,x;
	fscanf(fin,"%d",&x);
	min=x;
	max=x;
	while(x!=0)
	{
		sum+=x;
		n++;
		if(x<min)
		min=x;
		if(x>max)
		max=x;
		fscanf(fin,"%d",&x);
	}
	fprintf(fout,"%d %d %.3f\n",max,min,(float)sum/n);
	fclose(fin);
	fclose(fout);
	return 0;
}
