// ----------------------------------------------------------------
// "history.c" (Thanks to "user", from PSNope source)
// ----------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

// ----------------------------------------------------------------
// From "utils.c"
// ----------------------------------------------------------------
static uint8_t * read_file(char *path, uint32_t * file_size)
{
	uint8_t * buf;
	FILE * f = fopen(path, "rb");
	if(f)
	{
		uint32_t size = fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, 0, SEEK_SET);
		buf = malloc(size);
		fread(buf, size, 1, f);
		fclose(f);
		*(file_size) = size;
		return buf;
	}else{
		*(file_size) = 0;
		return NULL;
	}
}

static int write_file(uint8_t * buf, char *path, uint32_t size)
{
	FILE * f = fopen(path, "wb");
	int result;
	if(f)
	{
		result = fwrite(buf, size, 1, f);
		fclose(f);
		return result;
	}else{
		return 0;
	}
}
// ----------------------------------------------------------------

#define HOME_PATH					"/dev_hdd0/home/"
#define	BOOT_HISTORY_PATH			"/dev_hdd0/home/%s/etc/boot_history.dat"
#define GAME_DAT_PATH				"/dev_hdd0/vsh/pushlist/game.dat"
#define PATCH_DAT_PATH				"/dev_hdd0/vsh/pushlist/patch.dat"

char game_ids[][5] = {
	{"BLES"}, {"BCES"}, {"BCUS"}, {"BLUS"},
	{"BLJS"}, {"BCJS"}, {"BLAS"}, {"BCAS"}, 
	{"MRTC"}, {"NPEA"}, {"NPEB"}, {0}
};

static int id_is_legit(uint8_t * buffer);
static void clean_file(char * path, uint32_t entry_size, uint32_t id_offset);
static void clean_boot_history(void);
void delete_history(void);
void clean_history(void);

static int id_is_legit(uint8_t * buffer)
{
	int i=0;

	while(game_ids[i] && game_ids[i][0] > 0x40 && game_ids[i][0] < 0x5B)
	{
		if(memcmp(game_ids[i], buffer, 4) == 0)
			return 1;
		i++;
	}

	return 0;
}


static void clean_file(char * path, uint32_t entry_size, uint32_t id_offset)
{
	uint8_t * data = NULL;
	uint8_t * new_data = NULL;
	uint32_t in_size, out_size;
	uint32_t i;

	//printf("cleanup %s: ", path);
	data = read_file(path, &in_size);

	if(data)
	{
		new_data = malloc(in_size);
		memset(new_data, 0, in_size);
		out_size = 0;

		for(i=0;i<in_size;i+=entry_size)
		{
			if(id_is_legit(data+id_offset+i))
			{
				memcpy(new_data+out_size, data+i, entry_size);
				out_size+=entry_size;
			}
		}

		if(write_file(new_data, path, out_size)) {
			//printf("success\n");
		}else{
			//printf("cannot write file\n");
		}
		free(new_data);
		free(data);
	}else{
		//printf("cannot read file\n");
	}
}

static void clean_boot_history()
{
	DIR * d = opendir(HOME_PATH);
	struct dirent * dir;
	char boot_history_path[256];

	if(d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (dir->d_type == DT_DIR && dir->d_name[0] != '.')
			{
				sprintf(boot_history_path, BOOT_HISTORY_PATH, dir->d_name);
				clean_file(boot_history_path, 0x20, 1);
			}
		}

		closedir(d);
	}else{
		//printf("unable to open directory %s\n", HOME_PATH);
	}
}


void delete_history()
{
	DIR * d = opendir(HOME_PATH);
	struct dirent * dir;
	char boot_history_path[256];

	if(d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (dir->d_type == DT_DIR && dir->d_name[0] != '.')
			{
				sprintf(boot_history_path, BOOT_HISTORY_PATH, dir->d_name);
				remove(boot_history_path);
			}
		}

		closedir(d);
		remove(GAME_DAT_PATH);
		remove(PATCH_DAT_PATH);

		//printf("deleted history files\n");
	}else{
		//printf("unable to open directory %s\n", HOME_PATH);
	}
}

void clean_history()
{
	clean_file((char*)PATCH_DAT_PATH, 0x34, 0);
	clean_file((char*)GAME_DAT_PATH, 10, 0);
	clean_boot_history();
}