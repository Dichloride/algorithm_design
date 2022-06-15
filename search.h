#include"index.h"
#define MAX_OF_TEXT 1000                        //输入的字符串长度最大值
#define MAX_OF_WEBS 65536                       //网页表中网页最大值及web数组最大值
const string ivt_index_path="./Tra_TempIndex.txt"; 
typedef struct 
{
    int web_id;             //网页编号
    int freq;               //出现次数
}Web;
class search
{
public:
    Web web[MAX_OF_WEBS];                     //网址
    string text;                              //输入的语句
    int web_count;
    int word_count;
    string word[MAX_OF_TEXT];
    search()
    {
        web_count=0;
        word_count=0;
    }
    //int wordindex(HashLink* hashLink);  //单词ID查询
    //int urlindex(HashLink* hashLink);   //网页ID查询
    void InitwebList(HashLink* hashLink);//网址数组
    void textcut();                 //分词
    void disweb();                  //网址输出
    void SortWeb();                 //对web数组按照从大到小进行排序
};
void swap(Web& a, Web& b);
int Partition(Web a[], int p, int r);
void QuickSort(Web a[], int p, int r);
int CountLines(string filename);
string ReadLine(string filename,int line);
