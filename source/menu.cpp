#include "main.h"

c_Menu*	main_menu;

c_MenuItem::c_MenuItem(int nId) 
{
	szMenuLabel = (char*)malloc(1024);	
	szData_1	= (char*)malloc(1024);
	szData_2	= (char*)malloc(1024);
	szData_3	= (char*)malloc(1024);

	memset(szMenuLabel, 0, 1024);
	memset(szData_1, 0, 1024);
	memset(szData_2, 0, 1024);
	memset(szData_3, 0, 1024);

	nIndex		= nId;
}

c_MenuItem::~c_MenuItem()
{
	SAFE_FREE(szMenuLabel)
	SAFE_FREE(szData_1)
	SAFE_FREE(szData_2)
	SAFE_FREE(szData_3)
	*&nIndex = NULL;
}

c_Menu::c_Menu(int nListMaximum)
{
	nSelectedItem	= 0;
	nTopItem		= 0;
	nTotalItem		= 0;
	nListMax		= nListMaximum;
}

c_Menu::~c_Menu()
{
	for(int n = 0; n < nTotalItem; n++)
	{
		SAFE_DELETE(item[n])
	}
	*&nSelectedItem = NULL;
	*&nTopItem		= NULL;
	*&nTotalItem	= NULL;
	*&nListMax		= NULL;
}

void c_Menu::AddItem(char* szItemLabel) 
{
	if(!szItemLabel) return;

	item[nTotalItem] =  new c_MenuItem(nTotalItem);

	strcpy(item[nTotalItem]->szMenuLabel, szItemLabel);

	nTotalItem++;
}

void c_Menu::AddItemEx(char* szItemLabel, char* szData1, char* szData2, char* szData3) 
{
	if(!szItemLabel) return;

	item[nTotalItem] =  new c_MenuItem(nTotalItem);
	strcpy(item[nTotalItem]->szMenuLabel, szItemLabel);

	if(szData1) strcpy(item[nTotalItem]->szData_1, szData1);
	if(szData2) strcpy(item[nTotalItem]->szData_2, szData2);
	if(szData3) strcpy(item[nTotalItem]->szData_3, szData3);

	nTotalItem++;
}

int c_Menu::UpdateTopItem()
{
	if(nSelectedItem > nListMax || nTopItem > 0)
	{
		if(nTopItem < (nSelectedItem - nListMax)) {
			nTopItem = nSelectedItem - nListMax;
		}
		if(nTopItem > 0 && nSelectedItem < nTopItem) {
			nTopItem = nSelectedItem;
		} else {
			nTopItem = nSelectedItem - nListMax;
		}
	} else {
		nTopItem = nSelectedItem - nListMax;
	}
	if(nTopItem < 0) {
		nTopItem = 0;
	}
	return nTopItem;
}

void InitMainMenu()
{
	main_menu = new c_Menu(20);
	parse_iso_list();
}

void EndMainMenu()
{
	SAFE_DELETE(main_menu)
}
