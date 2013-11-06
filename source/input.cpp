// ---------------------------------------------------------------------
// FB Alpha Retro Loader (CaptainCPS-X, 2013)
// ---------------------------------------------------------------------
#include "main.h"

bool		mIsButtPressed[16];
bool		buttPressedNow[16];
	
uint32_t	mValRStickX, mValRStickY;
uint32_t	mValLStickX, mValLStickY;

uint32_t	nButtons[16];

void InputFrameStart(void)
{
	InputPadRead();
}

void InputFrameEnd(void)
{
	for(int x = 0; x < 16; x++)	{
		mIsButtPressed[x] = buttPressedNow[x];
	}
}

int InputPadRead(void)
{
	int32_t nRet;

	// Get gamepads information
	CellPadInfo2 gpad_info;

	nRet = cellPadGetInfo2(&gpad_info);

	if(nRet != CELL_PAD_OK) {
		// error
		return 0;
	}	

	// Check if we have 1 - 7 controllers connected
	if(gpad_info.now_connect >= 1 && gpad_info.now_connect <= CELL_PAD_MAX_PORT_NUM)
	{
		bool bPadsConnected = false;
		
		for(int nController = 0; nController < CELL_PAD_MAX_PORT_NUM; nController++)
		{
			// Use controller 1 for operation (for now)
			if(gpad_info.port_status[nController] & CELL_PAD_STATUS_CONNECTED)
			{
				bPadsConnected = true;
				break;
			}
		}

		if(bPadsConnected)
		{
			// A controller is connected...

			// Get Buttons data...
			for(int nController = 0; nController < CELL_PAD_MAX_PORT_NUM; nController++)
			{
				CellPadData pPad;
				
				if(cellPadGetData(nController, &pPad) == CELL_PAD_OK)
				{
					if(pPad.len == 0) continue;

					GET_PAD_D1(BT_LEFT	, CELL_PAD_CTRL_LEFT);
					GET_PAD_D1(BT_RIGHT	, CELL_PAD_CTRL_RIGHT);
					GET_PAD_D1(BT_DOWN	, CELL_PAD_CTRL_DOWN);
					GET_PAD_D1(BT_UP	, CELL_PAD_CTRL_UP);
					GET_PAD_D1(BT_START	, CELL_PAD_CTRL_START);
					GET_PAD_D1(BT_R3	, CELL_PAD_CTRL_R3);
					GET_PAD_D1(BT_L3	, CELL_PAD_CTRL_L3);
					GET_PAD_D1(BT_SELECT, CELL_PAD_CTRL_SELECT);

					GET_PAD_D2(BT_SQUARE	, CELL_PAD_CTRL_SQUARE);
					GET_PAD_D2(BT_CROSS		, CELL_PAD_CTRL_CROSS);
					GET_PAD_D2(BT_CIRCLE	, CELL_PAD_CTRL_CIRCLE);
					GET_PAD_D2(BT_TRIANGLE	, CELL_PAD_CTRL_TRIANGLE);
					GET_PAD_D2(BT_R1		, CELL_PAD_CTRL_R1);
					GET_PAD_D2(BT_L1		, CELL_PAD_CTRL_L1);
					GET_PAD_D2(BT_R2		, CELL_PAD_CTRL_R2);
					GET_PAD_D2(BT_L2		, CELL_PAD_CTRL_L2);

					// analog controls

					mValRStickX = (uint32_t)pPad.button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X];
					mValRStickY = (uint32_t)pPad.button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y];
					mValLStickX = (uint32_t)pPad.button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X];
					mValLStickY = (uint32_t)pPad.button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_Y];
					
					cellPadClearBuf(nController);
					break;
				}
			}
		}

	} else {
		// todo: report user to connect a controller 
	}

	return 1;
}

int InputInit(void)
{
	// Init GamePad
	int ret;

	ret = cellPadInit( CELL_PAD_MAX_PORT_NUM );
	if ( ret != CELL_PAD_OK ) {
		return 0;
	}

	for(int nController = 0; nController < CELL_PAD_MAX_PORT_NUM; nController++) {
		cellPadSetPortSetting(nController, CELL_PAD_SETTING_PRESS_ON );
	}

	for(int n = 0; n < 16; n++)	{
		mIsButtPressed[n] = false;
		buttPressedNow[n] = false;
		nButtons[n] = (1 << n);		// Assign button values in order
	}
	
	mValRStickX = mValRStickY = 0;
	mValLStickX = mValLStickY = 0;

	return 1;
}

void InputExit(void)
{
	cellPadEnd();
}