#include "search.h"
int main()
{
    page_File* page = new page_File;
	HashLink* hashLink = new HashLink;
	hashLink->readfile(page);
	hashLink->savefile();
    string searchtext;
    while(getline(cin,searchtext))
    {
		//cout << searchtext.length() << endl;
        search s;
        s.text=searchtext;
        //cout<<s.text<<endl;
        s.display(hashLink);
    }
	delete page; 
	delete hashLink;
}