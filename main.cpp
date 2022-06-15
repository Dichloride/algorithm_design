#include "index.h"
#include "inverted_index.h"
#include <time.h>
int main()
{
	page_File* page = new page_File;
	HashLink* hashLink = new HashLink;
	hashLink->readfile(page);
	hashLink->savefile();
	delete page; 
	delete hashLink;
    Createfile();
}