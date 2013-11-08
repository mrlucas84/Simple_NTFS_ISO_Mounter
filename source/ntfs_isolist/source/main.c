/* 
===================================================================================
NTFS ISO LIST GENERATOR - PLUGIN FOR "Simple NTFS ISO Mounter" (CaptainCPS-X, 2013)
===================================================================================

Thanks to "Eswald" for porting the NTFS library and "Cobra Dev" for improving it
and releasing the Cobra 7.00 CFW along with source code.

v1.00

- Support for PS3ISO, PSXISO, BDISO and DVDISO directories scanning and listing.
- Initial release.

NOTE: This application must be compiled with PSL1GHT since it depends on a 
library created on that SDK, maybe someday, someone will port it for Sony SDK too.

===================================================================================
*/
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <lv2/process.h>
#include "ntfs.h"

#define ZERO(x) \
	memset(&x, 0, sizeof(x));

#define SAFE_CLOSE(x) \
	if(x) { close(x); *&x = NULL; }

#define SAFE_FCLOSE(x) \
	if(x) { fclose(x); *&x = NULL; }

#define SAFE_FREE(x) \
	if(x) { free(x); *&x = NULL; }

#define SYS_PROCESS_SPAWN_STACK_SIZE_1M        0x70
#define SISOPATH "/dev_hdd0/game/SISO00123/USRDIR"

// =============================================
// PROTOTYPES
// =============================================

int		NTFS_Event_Mount(int id);
int		NTFS_UnMount(int id);
void	NTFS_UnMountAll(void);
int		NTFS_Test_Device(char *name);

int	generate_list(FILE* fp_list, char *path, int depth);

// =============================================
// DECLARATIONS
// =============================================

const DISC_INTERFACE *disc_ntfs[8]= {
    &__io_ntfs_usb000,
    &__io_ntfs_usb001,
    &__io_ntfs_usb002,
    &__io_ntfs_usb003,
    &__io_ntfs_usb004,
    &__io_ntfs_usb005,
    &__io_ntfs_usb006,
    &__io_ntfs_usb007
};

// mounts from /dev_usb000 to 007
ntfs_md *mounts[8]		= {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
int mountCount[8]		= {0, 0, 0, 0, 0, 0, 0, 0};
int automountCount[8]	= {0, 0, 0, 0, 0, 0, 0, 0};

uint32_t ports_cnt		= 0;
uint32_t old_ports_cnt	= 0;

//int max_list			= 0;	// directory listing counter (disabled limitation)

// =============================================
// MAIN MODULE
// =============================================
int main(int argc, const char* argv[])
{
	int i;
	int r				= -1;
	int nTry			= 0;	
	char* cur_device	= NULL;
	FILE* fp_isolist	= NULL;
	char path[PATH_MAX];
	ZERO(path);

	fp_isolist = fopen(SISOPATH"/iso_list.txt", "w"); // always overwrite with clean list...

	cur_device = (char*)malloc(strlen("/ntfs0:")+1);
	memset(cur_device, 0, strlen("/ntfs0:")+1);

	// Try to mount devices ...
	while(nTry < 1000)
	{
		// __io_ntfs_usb000 ... __io_ntfs_usb007		
				
		for(i = 0; i < 8 ; i++) 
		{
			int rr = NTFS_Event_Mount(i);

			if(rr == 1) 
			{ 
				// mount device            
				NTFS_UnMount(i);

				mounts[i] = NULL;
				mountCount[i] = 0;

				mountCount[i] = ntfsMountDevice (disc_ntfs[i], &mounts[i], NTFS_DEFAULT | NTFS_RECOVER);
            
				if(mountCount[i] > 0) {
					// ...
				} 
			} else if(rr == -1) { 
				// unmount device
				NTFS_UnMount(i);
			}
		}

		r = -1;		

		r = NTFS_Test_Device("ntfs0");
		if(r >= 0) { strcpy(cur_device, (char*)"/ntfs0:"); break; }

		r = NTFS_Test_Device("ntfs1");
		if(r >= 0) { strcpy(cur_device, (char*)"/ntfs1:"); break; }
		
		r = NTFS_Test_Device("ntfs2");
		if(r >= 0) { strcpy(cur_device, (char*)"/ntfs2:"); break; }
		
		r = NTFS_Test_Device("ntfs3");
		if(r >= 0) { strcpy(cur_device, (char*)"/ntfs3:"); break; }
		
		nTry++;
	}

	if(r>=0 && cur_device) 
	{
		sprintf(path, "%s/PS3ISO"	, cur_device);
		generate_list(fp_isolist, path, 0);

		sprintf(path, "%s/PSXISO"	, cur_device);
		generate_list(fp_isolist, path, 0);

		sprintf(path, "%s/BDISO"	, cur_device);
		generate_list(fp_isolist, path, 0);

		sprintf(path, "%s/DVDISO"	, cur_device);
		generate_list(fp_isolist, path, 0);
	}

	if(fp_isolist) { fclose(fp_isolist); *&fp_isolist = NULL; }

	NTFS_UnMountAll();

	// This flag will let "Simple NTFS ISO Mounter" that this was already launched,
	// allowing it to continue booting normally...

	FILE* fp = fopen(SISOPATH"/isolist_finished", "w");
	SAFE_FCLOSE(fp);

	sysProcessExitSpawn2( SISOPATH"/RELOAD.SELF", NULL, NULL, NULL, 0, 1001, SYS_PROCESS_SPAWN_STACK_SIZE_1M );	

	return 0;
}

// =============================================
// MODULES
// =============================================

int generate_list(FILE* fp_list, char *path, int depth)
{
    DIR_ITER *pdir;
    struct stat st;
    char new_path[PATH_MAX];
	ZERO(new_path);
    char filename[PATH_MAX];
	ZERO(filename);

    // Open the directory
    pdir = ps3ntfs_diropen(path);

    if (pdir) 
	{
        // List the contents of the directory
        while (ps3ntfs_dirnext(pdir, filename, &st) == 0) 
		{          
            if ((strcmp(filename, ".") == 0) || (strcmp(filename, "..") == 0)) {
                continue;
			}

            sprintf(new_path, "%s/%s", path, filename);
            
			// List the entry
            if (S_ISDIR(st.st_mode)) 
			{           
				// ...
            } else if (S_ISREG(st.st_mode)) {
				
				int nLen = strlen(filename);

				if(strstr(path, "PS3ISO") || strstr(path, "DVDISO") || strstr(path, "BDISO")) 
				{
					if(strcmp(filename+(nLen-4), ".iso") == 0 || strcmp(filename+(nLen-4), ".ISO") == 0) {
						fprintf(fp_list, "<iso>%s;%lu;%s;%s;</iso>\n", filename, (uint64_t)st.st_size, filename+(nLen-4), path );
					}
				}

				if(strstr(path, "PSXISO"))
				{
					if(strcmp(filename+(nLen-4), ".bin") == 0 || strcmp(filename+(nLen-4), ".BIN") == 0) {
						fprintf(fp_list, "<iso>%s;%lu;%s;%s;</iso>\n", filename, (uint64_t)st.st_size, filename+(nLen-4), path);
					}
				}
            } else {
                //...
            }

        }

        // Close the directory
        ps3ntfs_dirclose(pdir);

    } else {
        // error: failed to open directory...
		return 0;
    }

    return 1;
}

int NTFS_Event_Mount(int id) 
{
    int r = 0;

    ports_cnt &= ~(1<<id);
    if(PS3_NTFS_IsInserted(id)) ports_cnt |= 1<<id;

    if( ((ports_cnt>>id) & 1) && !((old_ports_cnt>>id) & 1)) automountCount[id] = 300; // enable delay event

    if(automountCount[id]>0) { // if delay counter ticks...
        automountCount[id]--; if(automountCount[id]==0) r = 1; // mount device
    }

    if( !((ports_cnt>>id) & 1) && ((old_ports_cnt>>id) & 1)) { // unmount device
        automountCount[id]=0; r = -1; 
    }

    old_ports_cnt = ports_cnt;

    return r;
}

int NTFS_UnMount(int id)
{
    int ret = 0;

    if (mounts[id]) {
    int k;
    for (k = 0; k < mountCount[id]; k++)
        if((mounts[id]+k)->name[0]) 
            {ret = 1; ntfsUnmount((mounts[id]+k)->name, true); (mounts[id]+k)->name[0] = 0;}

    free(mounts[id]); 
    mounts[id]= NULL;
    mountCount[id] = 0;
    }
    
    PS3_NTFS_Shutdown(id);

    return ret;
}

void NTFS_UnMountAll(void)
{ 
    int i;

    for(i = 0; i < 8; i++) {
        NTFS_UnMount(i);
    }
}

int NTFS_Test_Device(char *name)
{
    int k, i;

    for(k = 0; k < 8; k++) {
        for (i = 0; i < mountCount[k]; i++)
        if(!strncmp((mounts[k]+i)->name, name, 5 - 1 *( name[0] == 'e'))) 
            return ((mounts[k] + i)->interface->ioType & 0xff) - '0';
    }

    return -1;
    
}
