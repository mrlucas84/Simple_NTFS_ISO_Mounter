#include "main.h"

void parse_iso_list()
{
	FILE* fp = NULL;
	fp = fopen(ISO_LIST, "r");
	
	if(fp) 
	{
		char* pszLine = NULL;
		pszLine = (char*)malloc(2048+1);
		memset(pszLine, 0, 2048+1);
	
		while (!feof(fp)) 
		{
			memset(pszLine, 0, 2048+1);
			fgets(pszLine, 2048, fp);

			char* pch = NULL;
			pch = strstr(pszLine, "<iso>");

			if(pch) 
			{
				char* pszName =  NULL;
				pszName = (char*)malloc(2048+1);
				memset(pszName, 0, 2048+1);

				char* pszSize =  NULL;
				pszSize = (char*)malloc(2048+1);
				memset(pszSize, 0, 2048+1);

				char* pszExt =  NULL;
				pszExt = (char*)malloc(2048+1);
				memset(pszExt, 0, 2048+1);

				char* pszPath =  NULL;
				pszPath = (char*)malloc(2048+1);
				memset(pszPath, 0, 2048+1);
				
				int nEntryPos = strlen("<iso>");
				
				// ISO NAME
				strncpy(pszName, pch + nEntryPos, 2048);	
				pch = strchr(pszName, ';');
				pszName[(int)(pch-pszName)] = 0;				
				nEntryPos += strlen(pszName);
				nEntryPos++;

				// ISO SIZE
				pch = strstr(pszLine, "<iso>");

				strncpy(pszSize, pch + nEntryPos, 2048);	
				pch = strchr(pszSize, ';');
				pszSize[(int)(pch-pszSize)] = 0;				
				nEntryPos += strlen(pszSize);
				nEntryPos++;

				// EXTENSION
				pch = strstr(pszLine, "<iso>");

				strncpy(pszExt, pch + nEntryPos, 2048);	
				pch = strchr(pszExt, ';');
				pszExt[(int)(pch-pszExt)] = 0;			
				nEntryPos += strlen(pszExt);
				nEntryPos++;

				// PATH
				pch = strstr(pszLine, "<iso>");

				strncpy(pszPath, pch + nEntryPos, 2048);	
				pch = strchr(pszPath, ';');
				pszPath[(int)(pch-pszPath)] = 0;			
				nEntryPos += strlen(pszPath);
				nEntryPos++;
				
				strcat(pszPath, "/");
				strcat(pszPath, pszName);

				main_menu->AddItemEx(
					pszName, 
					pszPath + strlen("/ntfs0:"), // Ex. "/PS3ISO/ISONAME.iso"
					pszSize, 
					pszPath
				);

				if(pszName) { free(pszName);	*&pszName	= NULL; }
				if(pszSize) { free(pszSize);	*&pszSize	= NULL; }
				if(pszExt)	{ free(pszExt);		*&pszExt	= NULL; }
				if(pszPath) { free(pszPath);	*&pszPath	= NULL; }
			}
		}

		if(pszLine) { free(pszLine); *&pszLine = NULL; }
		fclose(fp);
	}
}

void mount_ntfs_iso(char* szGamePath, int nType)
{
	char szType[][8] = { {"EMU_PS3"},{"EMU_PSX"},{"EMU_BD"},{"EMU_DVD"} };
	char *argv[3];

	argv[0] = (char*)malloc(strlen(szGamePath)+1);
	memset(argv[0], 0, strlen(szGamePath)+1);
	strcpy(argv[0], szGamePath);

	argv[1] = (char*)malloc(8);
	strcpy(argv[1], szType[nType]);

	argv[2] = NULL;

	sys_game_process_exitspawn(ntfs_ext_iso_self_path, (const char**) argv, NULL, NULL, 0, 1001, SYS_PROCESS_PRIMARY_STACK_SIZE_1M);
}
