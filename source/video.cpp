#include "main.h"

static int chooseBestResolution(const uint32_t *resolutions, uint32_t numResolutions)
{
	uint32_t bestResolution = 0;

	for (uint32_t i = 0; bestResolution == 0 && i < numResolutions; i++) 
	{
		if(cellVideoOutGetResolutionAvailability(CELL_VIDEO_OUT_PRIMARY, resolutions[i], CELL_VIDEO_OUT_ASPECT_AUTO, 0))
		{
			bestResolution = resolutions[i];
		}
	}
	return bestResolution;
}

static int getResolutionWidthHeight(const uint32_t resolutionId, uint32_t &w, uint32_t &h)
{
	switch(resolutionId) {
		case CELL_VIDEO_OUT_RESOLUTION_480:			w=720;  h=480;	return 1;
		case CELL_VIDEO_OUT_RESOLUTION_576:			w=720;  h=576;	return 1;
		case CELL_VIDEO_OUT_RESOLUTION_720:			w=1280; h=720;	return 1;
		case CELL_VIDEO_OUT_RESOLUTION_1080:		w=1920; h=1080;	return 1;
		case CELL_VIDEO_OUT_RESOLUTION_1600x1080:	w=1600; h=1080;	return 1;
		case CELL_VIDEO_OUT_RESOLUTION_1440x1080:	w=1440; h=1080;	return 1;
		case CELL_VIDEO_OUT_RESOLUTION_1280x1080:	w=1280; h=1080;	return 1;
		case CELL_VIDEO_OUT_RESOLUTION_960x1080:	w=960;  h=1080;	return 1;
	};
	// error...
	return 0;
}

static bool videoOutIsReady()
{
	CellVideoOutState videoState;
	cellVideoOutGetState(CELL_VIDEO_OUT_PRIMARY, 0, &videoState);
	return(videoState.state == CELL_VIDEO_OUT_OUTPUT_STATE_ENABLED);
}

int InitPSGLVideo(
		struct PSGLdevice* _device, 
		struct PSGLcontext* _context, 
		uint32_t &_width, 
		uint32_t &_height,
		uint32_t &_render_width,
		uint32_t &_render_height )
{
	while(!videoOutIsReady()) { /* ... */ }

	// Initialize 6 SPUs but reserve 1 SPU as a raw SPU for PSGL
	sys_spu_initialize(6, 1);

	PSGLinitOptions options = 
	{
		enable:					PSGL_INIT_MAX_SPUS | PSGL_INIT_INITIALIZE_SPUS,
		maxSPUs:				1,
		initializeSPUs:			GL_FALSE,
		persistentMemorySize:	0,
		transientMemorySize:	0,
		errorConsole:			0,
		fifoSize:				0,  
		hostMemorySize:			128*1024*1024,  // 128 mbs for host memory 
	};

#if CELL_SDK_VERSION < 0x340000
	options.enable |=	PSGL_INIT_HOST_MEMORY_SIZE;
#endif

	psglInit(&options);

	const unsigned int resolutions[] = { 
		CELL_VIDEO_OUT_RESOLUTION_1080, 
		CELL_VIDEO_OUT_RESOLUTION_960x1080, 
		CELL_VIDEO_OUT_RESOLUTION_720, 
		CELL_VIDEO_OUT_RESOLUTION_480 
	};

	const int numResolutions	= sizeof(resolutions) / sizeof(resolutions[0]);
	int bestResolution			= chooseBestResolution(resolutions,numResolutions);
	getResolutionWidthHeight(bestResolution, _width, _height);

	if(bestResolution)
	{
		PSGLdeviceParameters		params;

		params.enable				= PSGL_DEVICE_PARAMETERS_COLOR_FORMAT |
									  PSGL_DEVICE_PARAMETERS_DEPTH_FORMAT |
									  PSGL_DEVICE_PARAMETERS_MULTISAMPLING_MODE;
		params.colorFormat			= GL_ARGB_SCE;
		params.depthFormat			= GL_DEPTH_COMPONENT24;
		params.multisamplingMode	= GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE;
		params.enable				|= PSGL_DEVICE_PARAMETERS_WIDTH_HEIGHT;
		params.width				= _width;
		params.height				= _height;

		_device						= psglCreateDeviceExtended(&params);
		_context					= psglCreateContext();

		

		psglMakeCurrent(_context, _device);
		psglResetCurrentContext();

		psglGetRenderBufferDimensions(_device, &_render_width, &_render_height);
		
		glEnable(GL_VSYNC_SCE);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_EQUAL, 1.0);

		glViewport(0, 0, _render_width, _render_height);

	} else {
		return 0;
	}

	return 1;
}
