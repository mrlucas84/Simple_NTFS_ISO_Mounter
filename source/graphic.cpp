#include "main.h"

GLfloat _normals[12] = 
{ 
	0.0, 0.0, 1.0, 
	0.0, 0.0, 1.0, 
	0.0, 0.0, 1.0, 
	0.0, 0.0, 1.0
};

GLfloat _squareVertices[8] = 
{
	-1.0, -1.0, // - -
	 1.0, -1.0, // + -
	-1.0,  1.0, // - +
	 1.0,  1.0  // + +
};

GLfloat _texCoords[8] = 
{
	0.0, 1.0,
	1.0, 1.0,
	0.0, 0.0,
	1.0, 0.0
};

c_tex::c_tex(uint32_t _nTexture, char* szPath) 
{
	nTexture	= _nTexture;
	nImageType	= TEX_TYPE_UNK;
	glGenTextures(1, &nTexId[0]);
	bTextureOK = false;
	
	pszTexPath	= (char*)malloc(2048);
	memset(pszTexPath, 0, 2048);
	strcpy(pszTexPath, szPath);
	
	BindTexture(pszTexPath);
}

c_tex::~c_tex()
{
	if(bTextureOK)
	{
		glDeleteTextures(1, &nTexId[0]);
		*&nTexture = NULL;			
		*&nTexId[0] = NULL;
		*&nImageType = NULL;
		*&bTextureOK = NULL;
	}
	SAFE_FREE(pszTexPath)
}

// TODO: Use QUADS instead of Triangle Strips
void c_tex::Render(GLuint x, GLuint y, GLsizei w, GLsizei h)
{
	if(!bTextureOK) return;	

	if(w == 0 || h == 0)	{
		w = render_width;
		h = render_height;
	}

	glViewport(x, y, w, h);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, nTexId[0]);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer		(2, GL_FLOAT, 0, _squareVertices);
	glNormalPointer		(	GL_FLOAT, 0, _normals		);
	glTexCoordPointer	(2, GL_FLOAT, 0, _texCoords		);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}

int c_tex::BindTexture(char* pszPath)
{
	if(FileExist(pszPath))
	{
		// check extension
		int nLen = strlen(pszPath);
		char szExt[12] = { 0 };
		
		strncpy(szExt, pszPath+(nLen-4), 4);

		// PNG
		if( strcmp(szExt, ".png")==0 ||	strcmp(szExt, ".PNG")==0) {
			nImageType = TEX_TYPE_PNG;
		}
		
		bool bImageLoaded = false;
		
		switch(nImageType)
		{
			case TEX_TYPE_PNG:
				bImageLoaded = rarch_load_png(pszPath, &_texture_png);
				break;
		}

		if(!bImageLoaded || nImageType == TEX_TYPE_UNK)
		{
			glDeleteTextures(1, &nTexId[0]);		
			return 0;
		} else {

			bTextureOK = true;
			
			glBindTexture(GL_TEXTURE_2D, nTexId[0]);

			if(nImageType == TEX_TYPE_PNG) {
			
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_ARGB_SCE,
					_texture_png.width,
					_texture_png.height,
					0,
					GL_BGRA, 
					GL_UNSIGNED_INT_8_8_8_8_REV, 
					_texture_png.pixels
				);
				SAFE_FREE(_texture_png.pixels)
			} 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			return 1;
		}
	}
	return 0;
}
