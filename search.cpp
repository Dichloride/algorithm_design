#include"search.h"
#include <ctype.h>
int vis[MAX_OF_WEBS];               //存储某网页编号在web数组中的地址
void search::textcut()
{
    //int len = text.length();
    string temp;
    temp+=" ";
    temp+=text;
    temp+=" ";
    int len=temp.length();
    //cout<<len<<endl;
    //int i = 0;
    int count=0;
    int delim[MAX_OF_TEXT];
    for(int i=0;i<len;i++)
    {
        if(!(isalnum(temp[i])||temp[i]=='\''))
        {
            delim[count++]=i;
            i++;
        }
    }
    for(int i=0;i<count-1;i++)
    {
        if(delim[i+1]-delim[i]>0)
        {
            string sub=temp.substr(delim[i]+1,delim[i+1]-delim[i]);
            word[word_count++]=sub;
        }
    }
}

void search::InitwebList(HashLink* hashLink)
{
    for(int i=0;i<MAX_OF_WEBS;i++)
        vis[i]=-1;                                      //每次生成webList时将vis数组初始化，每个对应地址值为-1
    for(int i=0;i<word_count;i++)
    {
        int index=BKDRHash(word[i]);
        hashNode *p=hashLink->hashLink[index];
        while(p->word!=word[i]&&p->next_w)              //防止HASH碰撞
            p=p->next_w;
        //索引到哈希链表 得到网页ID，将网页ID作为vis数组下标 由vis数组判断改网页是否访问过，访问过的话freq增加，未访问过的话添加到网页数组里面
        if(vis[p->w.doc_id]==-1)                        //未访问过
        {
            web[web_count].freq=p->w.count;
            web[web_count].web_id=p->w.doc_id;          //网页编号
            vis[p->w.doc_id]=web_count;
            web_count++;
        }
        else                                            //已访问过
        {
            web[vis[p->w.doc_id]].freq+=p->w.count;     //网页编号不改变，只增加网页出现次数
        }
    }
}

//对web数组根据freq进行快排，从大到小排序
void search::SortWeb()
{
    QuickSort(web,0,web_count-1);
}

//将网页id与url对应并输出
/*
读取doc_id文件，对应到网页id的具体行，得到网址
*/
void search::disweb()
{
    for(int i=0;i<web_count;i++)
    {
        int line=web[web_count].web_id-1;
        cout<<"line is"<<line<<endl;
        //cout<<ReadLine(ivt_index_path,line);
    }
}

void swap(Web& a, Web& b)
{
	Web temp = a;
	a = b;
	b = temp;
}

int Partition(Web a[], int p, int r)
{
	int x = a[r].freq;
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		if (a[j].freq > x)
		{
			swap(a[j], a[++i]);
		}
	}
	swap(a[r], a[i + 1]);
	return i + 1;
}

void QuickSort(Web a[], int p, int r)
{
	if (p < r)
	{
		int q = Partition(a, p, r);
		QuickSort(a, p, q - 1);
		QuickSort(a, q + 1, r);
	}
}

int CountLines(string filename)
{
    ifstream ReadFile;
    int n=0;
    string tmp;
    ReadFile.open(filename.c_str());//ios::in 表示以只读的方式读取文件
    if(ReadFile.fail())//文件打开失败:返回0
    {
        return 0;
    }
    else//文件存在
    {
        while(getline(ReadFile,tmp,'\n'))
        {
            n++;
        }
        ReadFile.close();
        return n;
    }
}
 
string ReadLine(string filename,int line)
{
    int lines,i=0;
    string temp;
    fstream file;
    file.open(filename.c_str());
    lines=CountLines(filename);
 
    if(line<=0)
    {
        return "Error 1: line number error";
    }
    if(file.fail())
    {
        return "Error 2: file does not exist";
    }
    if(line>lines)
    {
        return "Error 3: line out of bounds";
    }
    while(getline(file,temp) && i < line-1)
    {
        i++;
    }
    file.close();
    return temp;
}

int main()
{
    page_File* page = new page_File;
	HashLink* hashLink = new HashLink;
	hashLink->readfile(page);
	hashLink->savefile();
    string searchtext;
    while(getline(cin,searchtext))
    {
        search s;
        s.text=searchtext;
        s.disweb();
    }
	delete page; 
	delete hashLink;
}