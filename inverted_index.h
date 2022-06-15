#include<iostream>
#include<stdlib.h>
#include<string>
using namespace std;
class Term//一个单词与其出现网页 
{
	public:
	int id;//单词编号 
	string did;//关联网页编号
};
Term term[50000];
Term readline(FILE* fp)     //读一整行文件内容 
{
	Term a;
	int number;
	int frequency;
	char str[20];
	fscanf(fp,"%d %d %d ",&a.id,&number,&frequency);
	itoa(number,str,10);
	a.did=str;
	return a;
}
void Readfile()//读取索引文件 
{

	FILE *fp;
	fp=fopen("./temp_Index.txt","r");
	term[0]=readline(fp);
	Term temp;
	int i=1;
	while(!feof(fp)&&i<50000)
	{
        temp=readline(fp);
        //cout<<temp.id<<ends; 
        //cout<<temp.did<<endl; 
        if(temp.id!=term[i-1].id)
        {
          //cout<<"i:"<<i<<"id:"<<term[i-1].id<<endl; 
          term[i]=temp;
          i++;
		}
		else//将文件中连续相同ID的单词整合在一起，便于排序
        {
          term[i-1].did+=","; 
          term[i-1].did+=temp.did;
		}
	}
	fclose(fp);
}
void swap(Term& a, Term& b)
{
	Term temp = a;
	a = b;
	b = temp;
}

int Partition(Term a[], int p, int r)
{
	int x = a[r].id;
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		if (a[j].id <= x)
		{
			swap(a[j], a[++i]);
		}
	}
	swap(a[r], a[i + 1]);
	return i + 1;
}

void QuickSort(Term a[], int p, int r)
{
	if (p < r)
	{
		int q = Partition(a, p, r);
		QuickSort(a, p, q - 1);
		QuickSort(a, q + 1, r);
	}
}
void Createfile()//创建倒排索引文件
{
	Readfile();
	QuickSort(term,0,45974);
	FILE *fp;
	fp=fopen("Tra_TempIndex.txt","w+");
	int i=0;
	while(i<45975)
	{
		const char* str=NULL;
		if(term[i].id!=0)
		{
			str=term[i].did.c_str();
			fprintf(fp,"%d %s %s",term[i].id,str,"\n");
		}
		i++;
	}
	fclose(fp);
	cout<<"file generated"<<endl; 
	return; 
 } 

