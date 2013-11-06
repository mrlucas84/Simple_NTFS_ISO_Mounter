// CELL
#include <netex/libnetctl.h>
#include <netex/net.h>

#include <PSGL/psgl.h>
#include <PSGL/psglu.h>

#include <sysutil/sysutil_common.h>
#include <sysutil/sysutil_msgdialog.h>
#include <sysutil/sysutil_sysparam.h>		// used for cellVideoOutGetResolutionAvailability() and videoOutIsReady()
#include <sysutil/sysutil_screenshot.h>
#include <sys/timer.h>						// sys_timer_usleep()
#include <sys/return_code.h>				// CELL_OK 

#include <sys/spu_initialize.h>
#include <sys/memory.h>
#include <sys/paths.h>
#include <sys/process.h>

#include <sys/ppu_thread.h>

#include <cell/cell_fs.h>
#include <cell/sysmodule.h>
#include <cell/pad.h>
#include <cell/dbgfont.h>
#include <cell/audio.h>

#include <cell/usbd.h>

// STD
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <ctype.h>
#include <dirent.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>


#define CCORE_SELF "/dev_hdd0/game/CCRE00123/USRDIR/RELOAD.SELF"

#include "ftp.h"
#include "cfw_modules.h"
#include "cobra.h"

#define SAFE_FREE(x)	\
	if(x) {	free(x); *&x = NULL; }

#define SAFE_DELETE(x)	\
	if(x) { delete x; *&x = NULL; }

// rarch.cpp
bool rarch_load_png(const char *path, struct texture_image_png* img);

// ------------------------------------------------------------
// I use this reference resolution for design purposes
// all calculations are made for any resolution based on
// this, tested on 480p, 720p, 1080p...all OK.
// ------------------------------------------------------------
//#define REFERENCE_WIDTH		1920
//#define REFERENCE_HEIGHT	1080

// ------------------------------------------------------------
// OPENGL COORDINATE SYSTEM CALCULATIONS
// ------------------------------------------------------------

// X 
#define GL_PX_PCT_X(_x) \
	(double)((double)(_x) * 100.0 / (double)REFERENCE_WIDTH)

#define GL_PX_X(_x) \
	(double)(GL_PX_PCT_X(_x) * (double)app.renderWidth / 100.0)

// Y
#define GL_PX_PCT_Y(_y) \
	(double)((double)(_y) * 100.0 / (double)REFERENCE_HEIGHT)

#define GL_PX_Y(_y) \
	(double)(GL_PX_PCT_Y(_y) * (double)app.renderHeight / 100.0)

#define MAX_TEXTURES		20

#define TEX_TYPE_UNK		(-1)
#define TEX_TYPE_PNG		(0)
#define TEX_TYPE_JPG		(1)

struct texture_image_png
{
   unsigned width;
   unsigned height;
   uint32_t *pixels;
};

class c_tex
{
public:
	uint32_t					nTexture;
	char*						pszTexPath;
	GLuint						nTexId[1];
	struct texture_image_png	_texture_png;
	int							nImageType; // 0 = PNG , 1 = JPG, -1 = UNKNOWN
	bool						bTextureOK;	
	
	void	Render(GLuint x, GLuint y, GLsizei w, GLsizei h);
	int		BindTexture(char* pszPath);

	c_tex(uint32_t _nTexture, char* szPath);
	~c_tex();
};

class c_MenuItem 
{
public:	
	int			nIndex;
	char*		szMenuLabel;

	char*		szData_1;
	char*		szData_2;
	char*		szData_3;

	c_MenuItem(int nId);
	~c_MenuItem();
};

class c_Menu
{
public:
	int			nSelectedItem;
	int			nTopItem;
	int			nTotalItem;
	int			nListMax;
	c_MenuItem* item[2048];

	void AddItem(char* szItemLabel);
	void AddItemEx(char* szItemLabel, char* szData1, char* szData2, char* szData3);
	int UpdateTopItem();

	c_Menu(int nListMaximum);
	~c_Menu();
};

// main.cpp

extern bool					ftp_service;
extern uint32_t				screen_width, screen_height;
extern GLuint				render_width, render_height;
extern struct PSGLdevice*	device;
extern struct PSGLcontext*	context;
extern bool					bRun;
extern uint64_t				nFrame;
extern int					nTextures;
extern c_tex**				textures;
extern FILE*				fp_log;

// video.cpp

int InitPSGLVideo(struct PSGLdevice*, struct PSGLcontext*, uint32_t &, uint32_t &, GLuint &, GLuint &);

// input.cpp

// Button organized by values ( ex. (1<<0), (1<<1), etc... )
#define BT_SELECT		0
#define BT_L3			1
#define BT_R3			2
#define BT_START		3
#define BT_UP			4
#define BT_RIGHT		5
#define BT_DOWN			6
#define BT_LEFT			7
#define BT_L2			8
#define BT_R2			9
#define BT_L1			10
#define BT_R1			11
#define BT_TRIANGLE		12
#define BT_CIRCLE		13
#define BT_CROSS		14
#define BT_SQUARE		15

#define GET_PAD_D1(BT_X, CELL_PAD_CTRL_X)	\
	if(pPad.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_X)	\
	{	\
		buttPressedNow[BT_X] = true;	\
	} else {	\
		buttPressedNow[BT_X] = false;	\
	}

#define GET_PAD_D2(BT_X, CELL_PAD_CTRL_X)	\
	if(pPad.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_X)	\
	{	\
		buttPressedNow[BT_X] = true;	\
	} else {	\
		buttPressedNow[BT_X] = false;	\
	}

extern bool		mIsButtPressed[16];
extern bool		buttPressedNow[16];
extern uint32_t	mValRStickX, mValRStickY;
extern uint32_t	mValLStickX, mValLStickY;
extern uint32_t	nButtons[16];
int InputInit(void);
void InputExit(void);
int InputPadRead(void);
void InputFrameStart(void);
void InputFrameEnd(void);

// font.cpp

int font_init(uint32_t width, uint32_t height);
void font_exit(void);
void font_render(void);

// history.c

extern "C" 
{
void clean_history(void);
void delete_history(void);
}

// utils.cpp

double NumToPercent(double number, double ref_number);
double PercentToNum(double percent, double ref_number);
double CalcXW(double xw, double ref_width, double display_width);
double CalcYH(double yh, double ref_height, double display_height);
double DbgFontX(double x, double display_width);
double DbgFontY(double y, double display_height);
double GetCenterX(double width, double max_width);
double DbgFontCenterX(double left_x, double right_x, double fontsize, unsigned int len, double ref_width, double ref_height, double display_width, double display_height);

void GetDateTimeStr(char* szDate, char* szTime, bool b24hour);

bool DirExist(char* szDir);
bool FileExist(char* szFile);
uint64_t GetFileSize(char* szFilePath);

void init_log();

#define my_printf(...) \
	fp_log = fopen("/dev_hdd0/game/SISO00123/USRDIR/log.txt", "a"); \
	if(fp_log) { fprintf(fp_log, __VA_ARGS__); fclose(fp_log); fp_log = NULL; }

// menu.cpp
extern c_Menu*	main_menu;
void InitMainMenu();
void EndMainMenu();

// iso_mount.cpp
void parse_iso_list();
void mount_ntfs_iso(char* szGamePath, int nType);

#define ISO_LIST				"/dev_hdd0/game/SISO00123/USRDIR/iso_list.txt"

#define ntfs_ext_iso_self_path	"/dev_hdd0/game/SISO00123/USRDIR/ntfs_ext_iso.self"
#define ISOLIST_SELF			"/dev_hdd0/game/SISO00123/USRDIR/isolist.self"

#define ISO_TYPE_PS3	0
#define ISO_TYPE_PSX	1
#define ISO_TYPE_BD		2
#define ISO_TYPE_DVD	3
