#ifndef HBGL_H_
#define HBGL_H_

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "stb_truetype.h"
#include "stb_image.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

#define FONT_TEXTURE_SIZE 1024

#define HBGL_VERSION_MAJOR   0
#define HBGL_VERSION_MINOR   0
#define HBGL_VERSION_RELEASE 0
#define HBGL_VERSION_STATUS  "dev"
#define HBGL_DESCRIPTION     "\"Harbour Graphics Library,\" written in the C programming language."
#define HBGL_COPYRIGHT       "Copyright (c) 2023 Harbour development, https://github.com/dev-harbour/hbgl"
#define HBGL_LICENSE         "MIT License"

typedef enum bool
{
   F = 0,
   T = ( ! 0 )
} bool;

typedef struct _HBGL         HBGL;
typedef struct _UserMetadata UserMetadata;
typedef struct _Font         Font;
typedef struct _Image        Image;
typedef struct _ImageButton  ImageButton;

struct _UserMetadata
{
   int         major;
   int         minor;
   int         release;
   const char *status;
   const char *description;
   const char *copyright;
   const char *license;
   char        compileDate[ 12 ]; // Format YYYY.MM.DD
   char        compileTime[ 6 ];  // Format HH:MM
};

struct _Font
{
   HBGL            *pHBGL;
   unsigned int     textureID;
   int              fontID;
   int              x;
   int              y;
   int              width;
   int              height;
   int              fontSize;
   stbtt_bakedchar  cdata[ 380 ];
};

struct _Image
{
   HBGL         *pHBGL;
   unsigned int  textureID;
   int           imageID;
   int           x;
   int           y;
   int           width;
   int           height;
   int           channels;
};

struct _ImageButton
{
   HBGL         *pHBGL;
   unsigned int  textureID;
   int           imageButtonID;
   int           x;
   int           y;
   int           width;
   int           height;
   int           channels;
   bool          state;
   bool          mouseOver;
   bool          clicked;
   void          ( *onClick )();
};

struct _HBGL
{
   GLFWwindow  *window;
   int          width;
   int          height;
   const char  *title;
   bool         closeFlag;
   double       cursorX;
   double       cursorY;
   int          keyCode;
   int          keyScancode;
   int          keyAction;
   int          keyMods;
   int          mouseButton;
   int          mouseAction;
   int          mouseMods;
   int          winMaximized;
   unsigned int background;

   Font **fonts;               // Pointer to an array of Font structures
   int    fontCount;           // Number of fonts loaded
   int    failedFontCount;     // Unloaded font counter

   Image **images;             // Pointer to an array of Image structures
   int     imageCount;         // Number of images loaded
   int     failedImageCount;   // Unloaded image counter

   ImageButton **imagesButton; // Pointer to an array of ImageButton structures
   int imageButtonCount;       // Number of images button loaded
   int failedImageButtonCount; // Unloaded image button counter
};

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// API functions
HBGL *WindowNew( int width, int height, const char *title );
bool  MainLoop( HBGL *pHBGL );
void  BeginDrawing( HBGL *pHBGL );
void  EndDrawing( const HBGL *pHBGL );
int   CloseAll( HBGL *pHBGL );
void  Background( HBGL *pHBGL, unsigned int color );
void  PollEvents();
void  WaitEvents();
void  WaitEventsTimeout( double timeout );
void  PrintHBGLMetadata();
void  PrintHBGLStruct( const HBGL *pHBGL );

//--- User Metadata
void SetUserMetadata( UserMetadata *metadata, int major, int minor, int release, const char *status, const char *description, const char *copyright, const char *license );
bool PrintUserMetadata( const UserMetadata *metadata );

//--- Font
Font *SystemFontNew( HBGL *pHBGL, const char *fontName, int fontSize );
Font *FontNew( HBGL *pHBGL, const char *fontPath, int fontSize );
void  DrawFont( Font *pFont, int x, int y, const char *text, unsigned int foreground );
void  DrawFontBg( Font *pFont, int x, int y, const char *text, unsigned int foreground, unsigned int background );
void  FreeFont( Font *pFont );
void  PrintFontStruct( const Font *pFont );

//--- Image
Image *ImageNew( HBGL *pHBGL, const char *imagePath );
void   DrawImage( Image *pImage, int x, int y, int width, int height );
void   FreeImage( Image *pImage );
void   PrintImageStruct( const Image *pImage );

//--- Image button
ImageButton *ImageButtonNew( HBGL *pHBGL, const char *imageButtonPath, void ( *onClick )() );
void         DrawImageButton( ImageButton *pImageButton, int x, int y, int width, int height );
void         FreeImageButton( ImageButton *pImageButton );

//--- Shapes
void Point( int x, int y, unsigned int color );
void PointSize( int x, int y, int pointSize, unsigned int color );
void Line( int x, int y, int width, int height, unsigned int color );
void LineWidth( int x, int y, int width, int height, int lineWidth, unsigned int color );
void Rect( int x, int y, int width, int height, unsigned int color );
void RectWidthToInside( int x, int y, int width, int height, int lineWidth, unsigned int color );
void RectWidthToCenter( int x, int y, int width, int height, int numberLines, unsigned int color );
void RectWidthToOutside(int x, int y, int width, int height, int numberLines, unsigned int color);
void FillRect( int x, int y, int width, int height, unsigned int color );

//--- HTTP functions
bool openURL( const char *url );
bool openEmailClient( const char *emailAddress );

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// internal
void check_open_gl_error( const char *stmt, const char *fname, int line, GLenum *errCode );
void set_color_from_hex( unsigned int hexColor );

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// macros
#define REPORT_OPENGL_ERROR( stmt ) \
   GLenum errCode; \
   check_open_gl_error( stmt, __FILE__, __LINE__, &errCode ); \
   if( errCode != GL_NO_ERROR ) return;

#define MAX( a, b ) ( ( a ) < ( b ) ? ( b ) : ( a ) )
#define MIN( a, b ) ( ( a ) < ( b ) ? ( a ) : ( b ) )
#define UNUSED( n ) ( ( void )( n ) )

#define BEGINDRAWING( pHBGL ) do { BeginDrawing( pHBGL )
#define ENDDRAWING( pHBGL ) EndDrawing( pHBGL ); } while( 0 )

#endif /* End HBGL_H_ */
