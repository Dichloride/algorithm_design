#pragma once
#include<iostream>
#include <string> 
#include <fstream> 
using namespace std;
#define HASHMAX 60013//1000003
#define seed  31 //31 131 1313 13131 131313 etc..
class page_File//页面文件
{
public:
	int doc_id;//网页编号
	string url;//网址
	page_File() {
		doc_id = 0;
		url = "NULL";
	}
};
typedef struct
{
	int doc_id;   //网页编号
	int count;    //出现频率
}WebPage;
typedef struct hashnode
{
	//链表中每个节点的结构
	hashnode()
	{
		term_id = 0;
		word = "NULL";
		w.doc_id = 0;
		w.count = 0;
		next = NULL;
		address = NULL;
		next_w = NULL;
	}
	int term_id;  //单词编号
	string word;  //单词
	WebPage w;
	struct hashnode* address;
	struct hashnode* next; //指向链表中具有相同word值的下个节点
	struct hashnode* next_w; //指向链表中具有相同hash值的下个节点
}hashNode, *hashNodePtr;

class HashLink
{
public:
	hashNodePtr *hashLink;
	int id_number;
	HashLink()
	{
		id_number = 0;
		hashLink = new hashNodePtr[HASHMAX];
		for (int i = 0; i < HASHMAX; i++)hashLink[i] = NULL;	
		
	}
	void insertWord(string pWord, int doc_id);//在hash表中插入单词，如果已经存在了，则增加单词的出现次数count
	//unsigned int BKDRHash(string str);       //hash函数得到唯一的单词编码
	void savefile();
	void Analyse(string letter, int leIndex, int leLen, int doc_id);
	void readfile(page_File* page);
    void generateIndex();
	~HashLink()
	{
		delete[]hashLink;
	}
};
unsigned int BKDRHash(string str);
//BKDR Hash Function
unsigned int BKDRHash(string str)//hash函数得到唯一的单词编码
{
	unsigned long hash = 0;
	unsigned int index = 0;
		while (index < str.length())
		{
			hash = hash * seed + str[index++];
		}
		
	return (hash & 0x7FFFFFFF) % HASHMAX;
}
//向hash表中插入单词
void HashLink::insertWord(string pWord, int doc_id)//在hash表中插入单词，如果已经存在了，则增加单词的出现次数count
{
	//number++;
	//cout << number << " " << id_number <<" " << pWord << endl;
	//cout << pWord << endl;
	int index = BKDRHash(pWord);        //用hash函数得到单词的hash值，也就是hash数组的下标
	//if (pWord == "Liangshan")
	//{
	//	cout << index << endl;
	//	cout << hashLink[269707]->word << endl;
	//}
	if (hashLink[index] == NULL)
	{
		id_number++;
		hashNode *p = new hashNode;
		p->term_id = id_number;
		p->word = pWord;
		p->w.doc_id = doc_id;
		p->w.count = 1;
		p->next = NULL;
		p->next_w = NULL;
		p->address = p;
		hashLink[index] = p;
		//cout << "1" << endl;
	}
	else
	{
		hashNode* p = hashLink[index];
		hashNode* top=hashLink[index];
		hashNode* temp = hashLink[index];
		int sign = 0;
		while (p!=NULL)
		{
			if (p->word == pWord)//查找单词是否已经在hash表中了
			{
				p = top->address;
				if (p->w.doc_id == doc_id)
				{
					(p->w.count)++;
					sign = 1;
					return;
				}
				else
				{
					hashNode* q = new hashNode;
					q->next = p->next;
					p->next = q;
					q->next_w = NULL;
					q->address = NULL;
					q->term_id = p->term_id;
					q->word = p->word;
					q->w.count = 1;
					q->w.doc_id = doc_id;
					sign = 1;
					top->address = q;
					//cout << "2" << endl;
				}
			}
			else
			{
				temp = top;
				top = top->next_w;
				p = top;
			}
		}
		if (sign==0)
		{
			hashNode* q = new hashNode;
			q->next_w = temp->next_w;
			temp->next_w = q;
			id_number++;
			q->term_id = id_number;
			q->word = pWord;
			q->w.doc_id = doc_id;
			q->w.count = 1;
			q->next = NULL;
			q->address = q;
			//if (pWord == "Liangshan")//269707
			//{
			//	cout << q->word << endl;
			//	cout << q->term_id << endl;
			//	cout << index << endl;
			//}
			//cout << "3" << endl;
		}
	}
}
void  HashLink::savefile()
{
	ofstream ofs1;
	ofstream ofs2;
	ofs1.open("temp_Index.txt", ios_base::out | ios_base::trunc);
	ofs2.open("term_Id.txt", ios_base::out | ios_base::trunc);
	if (!ofs1)
	{
		cout << "文件temp_Index写入打开失败！！" << endl;
		return;
	}
	if (!ofs2)
	{
		cout << "文件term_Id写入打开失败！！" << endl;
		return;
	}
	for (int i = 0; i < HASHMAX; i++)
	{
		if (hashLink[i] != NULL)
		{
			hashNode * p= hashLink[i];
			hashNode* top = hashLink[i];
				while (top != NULL)
				{
					p = top;
					ofs2 << top->term_id << " " << top->word << endl;
					while (p != NULL)
					{
						ofs1 << p->term_id << " " << p->w.doc_id << " " << p->w.count << endl;
						p = p->next;
					}
					top = top->next_w;
				}
			}	
	}
	ofs1.close();
	ofs2.close();
}
void HashLink::Analyse(string letter, int leIndex, int leLen, int doc_id)
{

	while (leIndex < leLen)
	{
		//处理换行符
		while (letter[leIndex] == '\n' || letter[leIndex] == '\t' || letter[leIndex] == '\r')
		{
			leIndex++;
			if (leIndex >= leLen)return;
		}
		//处理空格
		while (letter[leIndex] == ' ')
		{
			leIndex++;
			if (leIndex >= leLen)return;
		}
		if (letter[leIndex] & 0x80)
		{
			leIndex += 2;
		}
		//处理单词
		 else if (isalpha(letter[leIndex]) || isdigit(letter[leIndex]))
		{
			string str;
			if (isalpha(letter[leIndex]))
			{
				while (isalpha(letter[leIndex])|| letter[leIndex]=='-' || isdigit(letter[leIndex])|| letter[leIndex] == '\'')
				{
					if (leIndex >= leLen)break;
					str.push_back(letter[leIndex]);
					leIndex++;
					if (letter[leIndex] & 0x80)
					{
						leIndex += 2;
						break;
					}
				}
			}
			else
			{
				while (isdigit(letter[leIndex]))
				{
		            if (leIndex >= leLen)break;
					else if (letter[leIndex] == ',')
					{
						leIndex++;
					}
					else
					{
						str.push_back(letter[leIndex]);
						leIndex++;
					}
				}
			}
			insertWord(str, doc_id);
			//cout << ++number << " " << str << endl;
			//cout << "!!" << endl;
		}
		 else
		 {
			 leIndex++;//其他符号
		 }
	}
}




void HashLink::readfile(page_File* page)
{
	ifstream ifs;
	ofstream ofs;
	ifs.open("news.csv", ios::in);
	ofs.open("doc_id.txt", ios_base::out | ios_base::trunc);
	string str;
	if (!ifs)
	{
		cout << "fail to read news" << endl;
		return;
	}
	if (!ofs)
	{
		cout << "fail to write doc_id" << endl;
		return;
	}
	int index;//读取的每一行字符串当前字符位置
	while (getline(ifs, str))
	{
		index = 0;
		if ((str.find("http:") != string::npos) && str.find("html") != string::npos)
		{
			index = str.find("html") + 4;
			page->url = str.substr(0, index);
			page->doc_id++;
			ofs << page->doc_id << " " << page->url << endl;
		}
		Analyse(str, index, str.length(), page->doc_id);
	}
	ifs.close();
	ofs.close();
	return;
}
