#include"index.h"
#include <ctype.h>
#include <string>
#define MAX_OF_TEXT 1000                        //输入的字符串长度最大值
#define MAX_OF_WEBS 65536                       //网页表中网页最大值及web数组最大值
const string ivt_index_path="./doc_id.txt"; 
int vis[MAX_OF_WEBS];               //存储某网页编号在web数组中的地址
typedef struct wordList
{
    string word;
    struct wordList *next;
}*WordLink,Wordptr;
typedef struct 
{
    int web_id;             //网页编号
    int freq;               //出现次数
    int word_count;         //匹配的单词数
    Wordptr *wordList;
}Web;
class search
{
public:
    Web web[MAX_OF_WEBS];                     //网址
    string text;                              //输入的语句
    int web_count;
    int word_count;
    int effective_web_count;
    string word[MAX_OF_TEXT];
    search()
    {
        web_count=0;
        word_count=0;
        effective_web_count=0;
    }
    //int wordindex(HashLink* hashLink);  //单词ID查询
    //int urlindex(HashLink* hashLink);   //网页ID查询
    void InitwebList(HashLink* hashLink);//网址数组
    void textcut();                 //分词
    void disweb();                  //网址输出
    void SortWeb();                 //对web数组按照从大到小进行排序
    void display(HashLink* hashLink);
};

void swap(Web& a, Web& b);
int Partition(Web a[], int p, int r);
void QuickSort(Web a[], int p, int r);
int CountLines(string filename);
string ReadLine(string filename,int line);

void search::textcut()
{
    //int len = text.length();
    string temp;
    temp+=" ";
    temp+=text;
    temp+=" ";
    //cout << "init temp:" << temp <<endl;
    int len=temp.length();
    //cout<<len<<endl;
    //int i = 0;
    int count=0;
    int delim[MAX_OF_TEXT];
    for(int i=0;i<len;i++)
    {
        if(temp[i]=='"')
            temp[i]=' ';
    }
    //cout << "now temp:" << temp <<endl;
    for(int i=0;i<len;i++)
    {
        if(!(isalnum(temp[i])||temp[i]=='\''||temp[i]=='-'))
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
    /*
    for(int i=0;i<word_count;i++)
        cout<<word[i]<<endl;
    */    
}

void search::InitwebList(HashLink* hashLink)
{
    for(int i=0;i<MAX_OF_WEBS;i++)
        vis[i]=-1;                                      //每次生成webList时将vis数组初始化，每个对应地址值为-1
    for(int i=0;i<word_count;i++)
    {
        int lent=word[i].length();
        string w=word[i].substr(0,lent-1);
        int index=BKDRHash(w);
        hashNode *p=hashLink->hashLink[index];
        //cout<<"web id is "<<p->w.doc_id<<endl;
        //cout<<"web freq is "<<p->w.count<<endl;
        //cout<<vis[p->w.doc_id]<<endl;
        /*
        int length=w.length();
        cout<<"w length:"<<length<<endl;
        for(int j=0;j<length;j++)
            cout<<"letter "<<j<<":"<<w[j]<<endl;
        cout<<"word "<<i<<" "<<"word:"<<w<<"index:"<<index<<endl;
        if(w=="China")
            cout<<"equal"<<endl;
        else
            cout<<"different"<<endl;
        hashNode *p=hashLink->hashLink[index];
        if(p)
            cout<<"p exists"<<endl;
        else
            cout<<"p does not exist"<<endl;
            */
        //cout<<"the word is "<<p->word<<endl;
        while(p->word!=w&&p->next_w)              //防止HASH碰撞
            p=p->next_w;
        //索引到哈希链表 得到网页ID，将网页ID作为vis数组下标 由vis数组判断改网页是否访问过，访问过的话freq增加，未访问过的话添加到网页数组里面
        if(p->word==w)
        {
            //cout<<"compare"<<endl;
            while(p->next)
            {
                if(vis[p->w.doc_id]==-1)                        //未访问过
                {
                    web_count++;
                    //cout<<"new page"<<endl;
                    //cout<<"web count:"<<web_count<<endl;
                    web[web_count].freq=p->w.count;
                    web[web_count].web_id=p->w.doc_id;          //网页编号
                    //web[web_count].wordList->word=w;
                    web[web_count].word_count=1;
                    vis[p->w.doc_id]=web_count;
                    
                    Wordptr *wordptr=new Wordptr;
                    wordptr->word=w;
                    wordptr->next=NULL;
                    web[web_count].wordList=wordptr;
                    
                }
                else                                            //已访问过
                {
                    web[vis[p->w.doc_id]].freq+=p->w.count;     //网页编号不改变，只增加网页出现次数
                    
                    Wordptr *wordptr=web[vis[p->w.doc_id]].wordList;
                    while(wordptr->word!=w&&wordptr->next)
                        wordptr=wordptr->next;
                    
                    if(wordptr->word!=w)                        //未找到匹配项
                    {
                        Wordptr *newptr=new Wordptr;
                        newptr->word=w;
                        newptr->next=NULL;
                        wordptr->next=newptr;
                        web[vis[p->w.doc_id]].word_count++;
                    }
                    
                }
                p=p->next;
            }
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
        //cout<<web[i].word_count<<endl;
        if(web[i].word_count>0.75*word_count)
        {
            int line=web[i].web_id;
            //cout<<"line is"<<line<<endl;
            string id_url;
            id_url=ReadLine(ivt_index_path,line);
            int pos;
            for(int i=0;i<id_url.length();i++)
            {
                if(id_url[i]==' ')
                    {
                        pos=i;
                        break;
                    }
            }
            //cout<<pos<<endl;
            string url=id_url.substr(pos+1);
            cout<<url<<endl;
            effective_web_count++;
        }
    }
}

void search::display(HashLink* hashLink)
{
    if(text.length()>MAX_OF_TEXT)
        cout<<"Search sentence is too long"<<endl;
    else
    {
        //cout << "start" << endl;
        textcut();
        InitwebList(hashLink);
        SortWeb();
        disweb();
        if(!effective_web_count)
            cout << "no matching pages" <<endl;
        else
            cout<<effective_web_count<<" pages in all"<<endl;
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
	int x = a[r].word_count;
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		if (a[j].word_count <= x)
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