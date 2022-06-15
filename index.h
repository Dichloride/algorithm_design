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