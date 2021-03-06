/*
vid_vita.c - vitagl video backend
Copyright (C) 2018 fgsfds

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "common.h"
#if XASH_VIDEO == VIDEO_VITA
#include "client.h"
#include "gl_local.h"
#include "mod_local.h"
#include "input.h"
#include "gl_vidnt.h"

uint16_t *gl_vgl_indices = NULL;
float *gl_vgl_verts = NULL;
float *gl_vgl_texcoords = NULL;
float *gl_vgl_colors = NULL;

typedef enum
{
	rserr_ok,
	rserr_invalid_fullscreen,
	rserr_invalid_mode,
	rserr_unknown
} rserr_t;

extern qboolean vita_keyboard_on;

void vitaBeginFrame( void )
{
	vglStartRendering( );
	vglIndexPointer( GL_SHORT, 0, MAX_VGL_ARRAYSIZE, gl_vgl_indices );
}

void vitaEndFrame( void )
{
	if( vita_keyboard_on )
	{
		vglStopRenderingInit( );
		vglUpdateCommonDialog( );
		vglStopRenderingTerm( );
	}
	else
	{
		vglStopRendering( );
	}
}

/*
=================
GL_GetProcAddress
=================
*/
void *GL_GetProcAddress( const char *name )
{
	return NULL;
}

/*
=================
GL_CheckExtension
=================
*/
void GL_CheckExtension( const char *name, const dllfunc_t *funcs, const char *cvarname, int r_ext )
{
}

/*
===============
GL_UpdateGammaRamp
===============
*/
void GL_UpdateGammaRamp( void )
{
	if( !glConfig.deviceSupportsGamma ) return;

	// GL_BuildGammaTable();
}

/*
===============
GL_UpdateSwapInterval
===============
*/
void GL_UpdateSwapInterval( void )
{
	if( gl_swapInterval->modified )
	{
		gl_swapInterval->modified = false;
    MsgDev( D_INFO, "GL_UpdateSwapInterval: %d\n", gl_swapInterval->integer );
		//vglWaitVblankStart( gl_swapInterval->integer );
	}
}


/*
===============
GL_ContextError
===============
*/
static void GL_ContextError( void )
{

}

/*
==================
GL_SetupAttributes
==================
*/
void GL_SetupAttributes()
{

}

void GL_InitExtensions( void )
{
	// initialize gl extensions
	GL_SetExtension( GL_OPENGL_110, true );

	// get our various GL strings
	glConfig.vendor_string = pglGetString( GL_VENDOR );
	glConfig.renderer_string = pglGetString( GL_RENDERER );
	glConfig.version_string = pglGetString( GL_VERSION );
	glConfig.extensions_string = pglGetString( GL_EXTENSIONS );
	MsgDev( D_INFO, "Video: %s\n", glConfig.renderer_string );

	// initalize until base opengl functions loaded

	GL_SetExtension( GL_DRAW_RANGEELEMENTS_EXT, false );
	GL_SetExtension( GL_ENV_COMBINE_EXT, false );
	GL_SetExtension( GL_DOT3_ARB_EXT, false );
	GL_SetExtension( GL_TEXTURE_3D_EXT, false );
	GL_SetExtension( GL_SGIS_MIPMAPS_EXT, false );

	// SCE_GXM_MAX_TEXTURE_UNITS is 16, but 4 is enough
	//GL_SetExtension( GL_ARB_MULTITEXTURE, true ); // TODO: make this shit work
	//glConfig.max_texture_units = 4;
	GL_SetExtension( GL_ARB_MULTITEXTURE, false ); 
	glConfig.max_texture_units = 1;

	// there's no hardware cubemaps
	GL_SetExtension( GL_TEXTURECUBEMAP_EXT, false ); 
	GL_SetExtension( GL_ARB_SEAMLESS_CUBEMAP, false );

	GL_SetExtension( GL_EXT_POINTPARAMETERS, false );
	GL_SetExtension( GL_ARB_TEXTURE_NPOT_EXT, false ); // TODO: check for NPOT support

	GL_SetExtension( GL_TEXTURE_COMPRESSION_EXT, false );
	GL_SetExtension( GL_CUSTOM_VERTEX_ARRAY_EXT, false );
	GL_SetExtension( GL_CLAMPTOEDGE_EXT, true ); // vitaGL supports GL_CLAMP_TO_EDGE
	GL_SetExtension( GL_ANISOTROPY_EXT, false );
	GL_SetExtension( GL_TEXTURE_LODBIAS, false );
	GL_SetExtension( GL_CLAMP_TEXBORDER_EXT, false );
	GL_SetExtension( GL_BLEND_MINMAX_EXT, false );
	GL_SetExtension( GL_BLEND_SUBTRACT_EXT, false );
	GL_SetExtension( GL_SEPARATESTENCIL_EXT, false );
	GL_SetExtension( GL_STENCILTWOSIDE_EXT, false );
	GL_SetExtension( GL_ARB_VERTEX_BUFFER_OBJECT_EXT, false );
	GL_SetExtension( GL_TEXTURE_ENV_ADD_EXT,false  );
	GL_SetExtension( GL_SHADER_OBJECTS_EXT, false );
	GL_SetExtension( GL_SHADER_GLSL100_EXT, false );
	GL_SetExtension( GL_VERTEX_SHADER_EXT, false );
	GL_SetExtension( GL_FRAGMENT_SHADER_EXT, false );
	GL_SetExtension( GL_SHADOW_EXT, false );
	GL_SetExtension( GL_ARB_DEPTH_FLOAT_EXT, false );
	GL_SetExtension( GL_OCCLUSION_QUERIES_EXT, false );
	GL_SetExtension( GL_DEPTH_TEXTURE, false );

	glConfig.texRectangle = glConfig.max_2d_rectangle_size = 0; // no rectangle
	glConfig.max_2d_texture_size = 2048;

	Cvar_Get( "gl_max_texture_size", "0", CVAR_INIT, "opengl texture max dims" );
	Cvar_Set( "gl_max_texture_size", va( "%i", glConfig.max_2d_texture_size ) );

	// SCE_GXM_MAX_UNIFORM_BUFFERS is 8, but whatever
	glConfig.max_vertex_uniforms = 0;
	// SCE_GXM_MAX_VERTEX_ATTRIBUTES is 16, but whatever
	glConfig.max_vertex_attribs = 0;

	glw_state.initialized = true;

	tr.framecount = tr.visframecount = 1;
}


/*
=================
GL_CreateContext
=================
*/
qboolean GL_CreateContext( void )
{
	return true;
}

/*
=================
GL_UpdateContext
=================
*/
qboolean GL_UpdateContext( void )
{
	return true;
}

/*
=================
GL_DeleteContext
=================
*/
qboolean GL_DeleteContext( void )
{
	return false;
}

/*
=================
VID_*
=================
*/

uint VID_EnumerateInstances( void )
{
	return 1;
}

void VID_StartupGamma( void )
{
	// Device supports gamma anyway, but cannot do anything with it.
	fs_offset_t gamma_size;
	byte *savedGamma;
	size_t gammaTypeSize = sizeof( glState.stateRamp );

	// init gamma ramp
	Q_memset( glState.stateRamp, 0, gammaTypeSize );

	// force to set cvar
	Cvar_FullSet( "gl_ignorehwgamma", "1", CVAR_GLCONFIG );

	glConfig.deviceSupportsGamma = false;	// even if supported!
	BuildGammaTable( vid_gamma->value, vid_texgamma->value );
	MsgDev( D_NOTE, "VID_StartupGamma: software gamma initialized\n" );
}

void VID_RestoreGamma( void )
{
}

qboolean VID_CreateWindow( int width, int height, qboolean fullscreen )
{
	return true;
}

void VID_DestroyWindow( void )
{
	if( glState.fullScreen )
	{
		glState.fullScreen = false;
	}
}

/*
==================
R_ChangeDisplaySettingsFast

Change window size fastly to custom values, without setting vid mode
==================
*/
void R_ChangeDisplaySettingsFast( int width, int height )
{
	//Cvar_SetFloat("vid_mode", VID_NOMODE);
	Cvar_SetFloat( "width", width );
	Cvar_SetFloat( "height", height );
	MsgDev( D_NOTE, "R_ChangeDisplaySettingsFast(%d, %d)\n", width, height);

	glState.width = width;
	glState.height = height;
	host.window_center_x = width / 2;
	host.window_center_y = height / 2;

	glState.wideScreen = true; // V_AdjustFov will check for widescreen

	SCR_VidInit();
}


rserr_t R_ChangeDisplaySettings( int width, int height, qboolean fullscreen )
{
	R_SaveVideoMode( width, height );

	host.window_center_x = width / 2;
	host.window_center_y = height / 2;
	
	glState.wideScreen = true; // V_AdjustFov will check for widescreen

	return rserr_ok;
}


qboolean VID_SetScreenResolution( int width, int height )
{
	return false;
}

void VID_RestoreScreenResolution( void )
{

}


/*
==================
VID_SetMode

Set the described video mode
==================
*/
qboolean VID_SetMode( void )
{
	int width = 960, height = 544;
	MsgDev( D_NOTE, "VID_SetMode(%d, %d)\n", width, height);
	R_ChangeDisplaySettings( width, height, false );
	return true;
}

/*
==================
R_Init_OpenGL
==================
*/
qboolean R_Init_OpenGL( void )
{
	VID_StartupGamma();
	MsgDev( D_NOTE, "R_Init_OpenGL()\n");

	gl_vgl_indices = malloc( sizeof( uint16_t ) * MAX_VGL_ARRAYSIZE );
	for( uint16_t i = 0; i < MAX_VGL_ARRAYSIZE; ++i ) gl_vgl_indices[i] = i;

	gl_vgl_verts = calloc( MAX_VGL_ARRAYSIZE * 3, sizeof( GLfloat ) );
	gl_vgl_colors = calloc( MAX_VGL_ARRAYSIZE * 4, sizeof( GLfloat ) );
	gl_vgl_texcoords = calloc( MAX_VGL_ARRAYSIZE * 2, sizeof( GLfloat ) );

	vglInit( 0x2000000 );
	vglUseVram( GL_TRUE );
	vglWaitVblankStart( GL_TRUE );

	return VID_SetMode();
}


/*
==================
R_Free_OpenGL
==================
*/
void R_Free_OpenGL( void )
{
	MsgDev( D_NOTE, "R_Free_OpenGL()\n");

	VID_RestoreGamma ();

	GL_DeleteContext ();

	VID_DestroyWindow ();

	vglEnd( );

	// now all extensions are disabled
	Q_memset( glConfig.extension, 0, sizeof( glConfig.extension[0] ) * GL_EXTCOUNT );
	glw_state.initialized = false;
}

/* GL WRAPPERS */

// hackfix alpha test bugs and texture format inconsistencies
// backported from FWGS' nanogl
void pglTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels )
{
	uint8_t *data = (uint8_t*)pixels;

	if( internalformat == GL_RGB && format == GL_RGBA ) // strip alpha from texture
	{
		uint8_t *in = data, *out;
		int i = 0, size = width * height * 4;

		data = out = malloc( size );
	
		for( i = 0; i < size; i += 4, in += 4, out += 4 )
		{
			memcpy( out, in, 3 );
			out[3] = 255;
		}
	}

	internalformat = format;
	glTexImage2D( target, level, internalformat, width, height, border, format, type, data );

	if( data != pixels ) free( data );
}

#endif // XASH_VIDEO
