#include "search.h"
#include <ctime>
int main()
{
    clock_t start,end;
    start = clock();
    page_File* page = new page_File;
	HashLink* hashLink = new HashLink;
	hashLink->readfile(page);
	hashLink->savefile();
    end = clock();
    cout<<"time = "<<double(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
    string searchtext;
    while(getline(cin,searchtext))
    {
        start = clock();
        search s;
        s.text=searchtext;
        s.display(hashLink);
        end = clock();
        cout<<"time = "<<double(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
    }
	delete page;
	delete hashLink;
}