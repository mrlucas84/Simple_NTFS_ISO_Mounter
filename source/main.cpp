#include "main.h"

void ftp_on(void);
void ftp_off(void);
void callback_sysutil_exit(uint64_t status, uint64_t param, void *userdata);
bool LoadModules(void);
bool UnloadModules(void);

bool ftp_service = false;

uint32_t screen_width, screen_height;
GLuint render_width, render_height;

struct PSGLdevice*	device;
struct PSGLcontext*	context;

bool bRun = true;
uint64_t nFrame = 0;

static int nSelInputFrame = 0;

int			nTextures;
c_tex**		textures;

FILE * fp_log = NULL;

#define REFERENCE_WIDTH		1920.0
#define REFERENCE_HEIGHT	1080.0

#define FONT_SIZE_1			25.0
#define FONT_SIZE_2			16.0
#define FONT_SIZE_3			14.0

#define FONT_SPACING		16.0

char szTitle[] = { "Simple NTFS ISO Mounter v1.02 (CaptainCPS-X, 2013)" };

static uint32_t temp_cpu = 0, temp_rsx = 0;

void input(void)
{
	InputFrameStart();

	if(((nFrame + nSelInputFrame) - nFrame) == 5)
	{
		// DPAD UP / LEFT ANALOG UP
		if((mIsButtPressed[BT_UP] && buttPressedNow[BT_UP]) || (mValLStickY < 50))
		{
			if(main_menu->nSelectedItem > 0 && main_menu->nSelectedItem <= main_menu->nTotalItem) 
			{
				main_menu->nSelectedItem--;
			}
		}

		// DPAD DOWN / LEFT ANALOG DOWN
		if	( (mIsButtPressed[BT_DOWN] && buttPressedNow[BT_DOWN]) || (mValLStickY > 200) )
		{
			if(main_menu->nSelectedItem >= 0 && main_menu->nSelectedItem < main_menu->nTotalItem-1) 
			{
				main_menu->nSelectedItem++;
			}
		}
		nSelInputFrame = 0;
	}
	nSelInputFrame++;

	// ------------------------------------------------------
	// 	

	if(!mIsButtPressed[BT_TRIANGLE] && buttPressedNow[BT_TRIANGLE])
	{
		// Rescan ISOs
		sys_game_process_exitspawn((char*)ISOLIST_SELF, NULL, NULL, NULL, 0, 1000, SYS_PROCESS_PRIMARY_STACK_SIZE_1M);
	}

	if(!mIsButtPressed[BT_CROSS] && buttPressedNow[BT_CROSS])
	{
		if(main_menu->nTotalItem >= 1)
		{
			int nType = -1;
			
			if(strstr(main_menu->item[main_menu->nSelectedItem]->szData_1, "PS3ISO")) nType = ISO_TYPE_PS3;
			if(strstr(main_menu->item[main_menu->nSelectedItem]->szData_1, "PSXISO")) nType = ISO_TYPE_PSX;
			if(strstr(main_menu->item[main_menu->nSelectedItem]->szData_1, "DVDISO")) nType = ISO_TYPE_DVD;
			if(strstr(main_menu->item[main_menu->nSelectedItem]->szData_1, "BDISO")) nType = ISO_TYPE_BD;
			
			if(nType != -1) {
				mount_ntfs_iso(main_menu->item[main_menu->nSelectedItem]->szData_1, nType);
			}
		}
	}

	InputFrameEnd();
}

double debug_font_size(double _px) {
	return (_px * 4.0 / 100.0);
}

void render(bool bClearOnly)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(bClearOnly == false)
	{
		textures[0]->Render(0,0,0,0);

		// =============================================================================================
		// TITLE
		// =============================================================================================

		double xPos, yPos;

		xPos = DbgFontCenterX( 
			0,									// x left (as in RECT)
			render_width,						// x right (as in RECT)
			FONT_SIZE_1,						// font size (in pixels)		
			strlen(szTitle),					// text length (num. chars)
			REFERENCE_WIDTH, REFERENCE_HEIGHT,	// reference width / height (should not be changed)
			render_width, render_height			// currently active resolution
		);
		yPos = DbgFontY(50.0, render_height);

		cellDbgFontPrintf(xPos, yPos, debug_font_size(FONT_SIZE_1), 0xFFFFFFFF, "%s", szTitle);
		
		yPos += DbgFontY(FONT_SIZE_1 + FONT_SPACING, render_height);

		// =============================================================================================
		// DATE
		// =============================================================================================

		char szDate[128];
		char szTime[128];
		memset(&szDate, 0, sizeof(szDate));
		memset(&szTime, 0, sizeof(szTime));

		GetDateTimeStr((char*)szDate, (char*)szTime, false);

		xPos = DbgFontCenterX(
			0,									// x left (as in RECT)
			render_width,						// x right (as in RECT)
			FONT_SIZE_2,						// font size (in pixels)
			strlen(szDate),						// text length (num. chars)
			REFERENCE_WIDTH, REFERENCE_HEIGHT,	// reference width / height (should not be changed)
			render_width, render_height			// currently active resolution
		);
		
		cellDbgFontPrintf(xPos, yPos, debug_font_size(FONT_SIZE_2), 0xFFFFFFFF, "%s", szDate);
		
		yPos += DbgFontY(FONT_SIZE_2 + FONT_SPACING, render_height);
		
		// =============================================================================================
		// TIME
		// =============================================================================================

		xPos = DbgFontCenterX(
			0,									// x left (as in RECT)
			render_width,						// x right (as in RECT)
			FONT_SIZE_1,						// font size (in pixels)
			strlen(szTime),						// text length (num. chars)
			REFERENCE_WIDTH, REFERENCE_HEIGHT,	// reference width / height (should not be changed)
			render_width, render_height			// currently active resolution
		);
		
		cellDbgFontPrintf(xPos, yPos, debug_font_size(FONT_SIZE_1), 0xFFFFFFFF, "%s", szTime);
		
		yPos += DbgFontY(FONT_SIZE_1 + FONT_SPACING, render_height);

		// =============================================================================================
		
		static uint64_t nInterval = 30;				// 0.5 second @ 60 FPS
		static uint64_t nFrameCheckpoint = nFrame;
		
		if(nFrame >= (nFrameCheckpoint + nInterval)) 
		{
			nFrameCheckpoint = nFrame;
			
			sys_game_get_temperature(0, &temp_cpu);
			sys_game_get_temperature(1, &temp_rsx);
		}

		char szTemperature[256];
		memset(&szTemperature, 0, sizeof(szTemperature));

		sprintf(szTemperature, "Temperature: (CPU: %d' C) (RSX: %d' C)", temp_cpu, temp_rsx);

		xPos = DbgFontCenterX(
			0,									// x left (as in RECT)
			render_width,						// x right (as in RECT)
			FONT_SIZE_2,						// font size (in pixels)
			strlen(szTemperature),				// text length (num. chars)
			REFERENCE_WIDTH, REFERENCE_HEIGHT,	// reference width / height (should not be changed)
			render_width, render_height			// currently active resolution
		);
		
		cellDbgFontPrintf(xPos, yPos, debug_font_size(FONT_SIZE_2), 0xFFFFFFFF, "%s", szTemperature);
		
		yPos += DbgFontY(FONT_SIZE_2 + FONT_SPACING, render_height);

		// ===
		yPos += DbgFontY(FONT_SIZE_2 + FONT_SPACING, render_height);
		yPos += DbgFontY(FONT_SIZE_2 + FONT_SPACING, render_height);
		// ===

		//xPos = DbgFontCenterX(
		//	0,									// x left (as in RECT)
		//	render_width,						// x right (as in RECT)
		//	FONT_SIZE_2,						// font size (in pixels)
		//	strlen((char*)"..::[ ISO LIST ] [ PS3ISO / PSXISO / BDISO / DVDISO ] ::.."),				// text length (num. chars)
		//	REFERENCE_WIDTH, REFERENCE_HEIGHT,	// reference width / height (should not be changed)
		//	render_width, render_height			// currently active resolution
		//);
		//
		//cellDbgFontPrintf(xPos, yPos, debug_font_size(FONT_SIZE_2), 0xFFFFFFFF, "%s", (char*)"..::[ ISO LIST ] [ PS3ISO / PSXISO / BDISO / DVDISO ] ::..");
		//
		yPos += DbgFontY(FONT_SIZE_2 + FONT_SPACING, render_height);
		yPos += DbgFontY(FONT_SIZE_2 + FONT_SPACING, render_height);

		// ================

		xPos = DbgFontX(100.0, render_width);

		int nMenuItem = main_menu->UpdateTopItem();	

		while(nMenuItem <= (main_menu->nTopItem + main_menu->nListMax))
		{
			if(nMenuItem == main_menu->nTotalItem) break;
			if(main_menu->nTotalItem <= 0) break;

			// normal
			unsigned int nColor = 0xFFFFFFFF;

			// selected
			if(nMenuItem == main_menu->nSelectedItem) {
				nColor = 0xFF00CCFF;
			}
		
			//if(nFrameStep == 0) { nColor = g_opt_nFontShadowColor; } // Shadow color

			cellDbgFontPrintf(xPos, yPos, debug_font_size(FONT_SIZE_2), nColor, "%s", main_menu->item[nMenuItem]->szMenuLabel/*, main_menu->item[nMenuItem]->szData_1*/);
			yPos += DbgFontY(FONT_SIZE_2 + FONT_SPACING, render_height);

			nMenuItem++;
		}

		(void)font_render();		
	}
	
	nFrame++;
	psglSwap();
}


void exit_app(void)
{
	(void)font_exit();
	(void)delete_history();
	(void)ftp_off();
	(void)InputExit();
	UnloadModules();
	exit(1);
}

int init_core(void)
{
	(void)delete_history();
	(void)ftp_on();
	//(void)CheckFWVersion();
	//if(!fan_init()) {
		// unsupported fw error...
	//}

	textures		= (c_tex**)malloc(sizeof(c_tex) * 10);
	memset(textures, 0, sizeof(c_tex) * 10);

	textures[0]		= new c_tex(0, (char*)"/dev_hdd0/game/SISO00123/USRDIR/gfx/BG.PNG");

	InitMainMenu();
	return 1;
}



int main(int argc, char* argv[])
{
	init_log();

	(void)argc;
	(void)argv;
	
	// [cobralib] unmount iso / eject
	cobra_send_fake_disc_eject_event();
	cobra_umount_disc_image();

	// Check if isolist.self is launching this app... 
	FILE* fp_flag = fopen("/dev_hdd0/game/SISO00123/USRDIR/isolist_finished", "r");
	if(fp_flag) 
	{
		// load normally...
		fclose(fp_flag);
		*&fp_flag = NULL;

		// remove the launch flag...
		cellFsUnlink("/dev_hdd0/game/SISO00123/USRDIR/isolist_finished");

	} else {
		// generate ISO list...
		sys_game_process_exitspawn((char*)ISOLIST_SELF, NULL, NULL, NULL, 0, 1000, SYS_PROCESS_PRIMARY_STACK_SIZE_1M);
	}
	
	if(!LoadModules()) 
	{
		// error...
		(void)exit_app();
		return 0;
	}
	
	// setup sys callback
	if(cellSysutilRegisterCallback(0, callback_sysutil_exit, NULL) != CELL_OK) 
	{
		// error...
		(void)exit_app();
		return 0;
	}

	InputInit();

	if(InitPSGLVideo(device, context, screen_width, screen_height, render_width, render_height))
	{
		// clear vid on startup
		(void)render(true);
	
		// init font
		if(!font_init(render_width, render_height)) { bRun = false; }

		// init app core modules, ftp, settings, etc...
		if(!init_core()) { bRun = false; }
		
		// loop
		while(bRun)
		{
			cellSysutilCheckCallback();			

			(void)input();			
			(void)render(false);			
		}

	} else {
		// error msg here...
	}
	
	(void)exit_app();
	return 0;
}

void callback_sysutil_exit(uint64_t status, uint64_t param, void *userdata)
{
	(void)param;
	(void)userdata;

	switch(status) 
	{
		case CELL_SYSUTIL_REQUEST_EXITGAME:
			//printf("CELL_SYSUTIL_REQUEST_EXITGAME\n");
			bRun = false;
			break;
		case CELL_SYSUTIL_SYSTEM_MENU_OPEN:			
			//printf("CELL_SYSUTIL_SYSTEM_MENU_OPEN\n");
			break;
		case CELL_SYSUTIL_SYSTEM_MENU_CLOSE:
			//printf("CELL_SYSUTIL_SYSTEM_MENU_CLOSE\n");
			break;
		case CELL_SYSUTIL_DRAWING_BEGIN:
			//printf("CELL_SYSUTIL_DRAWING_BEGIN (Pause game)\n");
			break;
		case CELL_SYSUTIL_DRAWING_END:
			//printf("CELL_SYSUTIL_DRAWING_END (Unpause game)\n");
			break;
		case CELL_SYSUTIL_BGMPLAYBACK_PLAY:
		case CELL_SYSUTIL_BGMPLAYBACK_STOP:
		default:
			//printf("system notification: unknown status\n");
			break;
	}
}

bool LoadModules(void)
{
	cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_SCREENSHOT);
	cellScreenShotEnable();

	int ret = cellSysmoduleLoadModule( CELL_SYSMODULE_FS );
	if ( ret < 0 )	{
		//printf( "\nError loading module FS!!!\n" );
		return false;
	}

	

	ret = cellSysmoduleLoadModule( CELL_SYSMODULE_USBD );
	if ( ret < 0 )	{
		//printf( "\nError loading module USBD!!!\n" );
		return false;
	}
  

	ret = cellSysmoduleLoadModule(CELL_SYSMODULE_HTTP);
	if ( ret < 0 )	{
		//printf( "\nError loading module HTTP!!!\n" );
		return false;
	}
	sys_net_initialize_network();

    //printf( "\nLoading libnet\n" );
	ret = cellSysmoduleLoadModule( CELL_SYSMODULE_NET );
	if ( ret < 0 )	{
		//printf( "\nError loading module NET!!!\n" );
		return false;
	}

	cellNetCtlInit();

    //printf( "\nLoading libio\n" );
	ret = cellSysmoduleLoadModule( CELL_SYSMODULE_IO );
	if ( ret < 0 )	{
		//printf( "\nError loading module IO!!!\n" );
		return false;
	}

    //printf( "\nLoading libaudio\n" );
	ret = cellSysmoduleLoadModule( CELL_SYSMODULE_AUDIO );
	if ( ret < 0 )	{
		//printf( "\nError loading module AUDIO!!!\n" );
		return false;
	}

	ret = cellSysmoduleLoadModule( CELL_SYSMODULE_RESC );
	if ( ret < 0 )	{
		//printf( "\nError loading module RESC!!!\n" );
		return false;
	}

	

    return true;
}

bool UnloadModules(void)
{
    cellSysmoduleUnloadModule( CELL_SYSMODULE_FS );
 	cellSysmoduleUnloadModule( CELL_SYSMODULE_SYSUTIL_SCREENSHOT );
	cellSysmoduleUnloadModule( CELL_SYSMODULE_USBD );
	cellSysmoduleUnloadModule( CELL_SYSMODULE_NET );
	cellSysmoduleUnloadModule(CELL_SYSMODULE_HTTP);
	cellSysmoduleUnloadModule( CELL_SYSMODULE_IO );
	cellSysmoduleUnloadModule( CELL_SYSMODULE_AUDIO );
    cellSysmoduleUnloadModule( CELL_SYSMODULE_RESC );
    return true;
}

void ftp_on(void) 
{
	if(ftp_service) { return; }
	ftp_service = true;
	//ftp_clients = 0;
	main_ftp(0);
}

void ftp_off(void) 
{
	if(!ftp_service) { return; }
	ftp_service = false;
	//ftp_clients = 0;
	main_ftp(1);
}
