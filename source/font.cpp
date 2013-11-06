#include "main.h"

int font_init( uint32_t width, uint32_t height)
{
	CellDbgFontConfig cfg;
	memset(&cfg, 0, sizeof(CellDbgFontConfig));
	
	cfg.bufSize      = 1024 * 10;
	cfg.screenWidth  = width;
	cfg.screenHeight = height;
	
	int ret = cellDbgFontInit(&cfg);
	
	if (ret != CELL_OK) {
		// error msg here ...
		return 0;
	}

	return 1;
}

void font_exit(void)
{
	cellDbgFontExit();
}

void font_render(void)
{
	cellDbgFontDraw();
}
