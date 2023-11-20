/*
 *
 */

#ifndef STB_IMAGE_IMPLEMENTATION
   #define STB_TRUETYPE_IMPLEMENTATION
#endif

#ifndef STB_IMAGE_IMPLEMENTATION
   #define STB_IMAGE_IMPLEMENTATION
#endif

#include "hbgl.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// static
static void cursor_position_callback( GLFWwindow *window, double x, double y )
{
   HBGL *pHBGL = glfwGetWindowUserPointer( window );

   pHBGL->cursorX = x;
   pHBGL->cursorY = y;
}

static void key_callback( GLFWwindow *window, int key, int scancode, int action, int mods )
{
   HBGL *pHBGL = glfwGetWindowUserPointer( window );

   pHBGL->keyCode     = key;
   pHBGL->keyScancode = scancode;
   pHBGL->keyAction   = action;
   pHBGL->keyMods     = mods;
}

static void mouse_button_callback( GLFWwindow *window, int button, int action, int mods )
{
   HBGL *pHBGL = glfwGetWindowUserPointer( window );

   pHBGL->mouseButton = button;
   pHBGL->mouseAction = action;
   pHBGL->mouseMods   = mods;
}

static void window_maximize_callback( GLFWwindow *window, int maximized )
{
   HBGL *pHBGL = glfwGetWindowUserPointer( window );

   pHBGL->winMaximized = maximized;
}

static void window_size_callback( GLFWwindow *window, int width, int height )
{
   HBGL *pHBGL = glfwGetWindowUserPointer( window );

   pHBGL->width  = width;
   pHBGL->height = height;
}

static void set_clear_color_from_hex( unsigned int hexColor )
{
   if( ( hexColor <= 0xffffff ) )
   {
      float r, g, b;
      r = ( ( hexColor >> 16 ) & 0xFF ) / 255.0;
      g = ( ( hexColor >> 8 )  & 0xFF ) / 255.0;
      b = (   hexColor         & 0xFF ) / 255.0;
      glClearColor( r, g, b, 1.0 );
   }
   else if( ( hexColor <= 0xffffffff ) )
   {
      float r, g, b, a;
      r = ( ( hexColor >> 24 ) & 0xFF ) / 255.0;
      g = ( ( hexColor >> 16 ) & 0xFF ) / 255.0;
      b = ( ( hexColor >> 8 )  & 0xFF ) / 255.0;
      a = (   hexColor         & 0xFF ) / 255.0;
      glClearColor( r, g, b, a );
   }
   else
   {
      fprintf( stderr, "An invalid hexadecimal color value was passed." );
   }
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// internal
void check_open_gl_error( const char *stmt, const char *fname, int line, GLenum *errCode )
{
   GLenum err = glGetError();
   if( err != GL_NO_ERROR )
   {
      printf( "OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt );
      *errCode = err;
   }
   else
   {
     *errCode = GL_NO_ERROR;
   }
}

void set_color_from_hex( unsigned int hexColor )
{
   if( ( hexColor <= 0xffffff ) )
   {
      float r, g, b;
      r = ( ( hexColor >> 16 ) & 0xFF ) / 255.0f;
      g = ( ( hexColor >> 8 )  & 0xFF ) / 255.0f;
      b = (   hexColor         & 0xFF ) / 255.0f;
      glColor3f( r, g, b );
   }
   else if( ( hexColor <= 0xffffffff ) )
   {
      float r, g, b, a;
      r = ( ( hexColor >> 24 ) & 0xFF ) / 255.0f;
      g = ( ( hexColor >> 16 ) & 0xFF ) / 255.0f;
      b = ( ( hexColor >> 8 )  & 0xFF ) / 255.0f;
      a = (   hexColor         & 0xFF ) / 255.0f;
      glColor4f( r, g, b, a );
   }
   else
   {
      fprintf( stderr, "An invalid hexadecimal color value was passed." );
   }
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// API functions
HBGL *WindowNew( int width, int height, const char *title )
{
   if( ! glfwInit() )
   {
      exit( EXIT_FAILURE );
   }

   HBGL *pHBGL = malloc( sizeof( HBGL ) );
   if( ! pHBGL )
   {
      fprintf( stderr, "Memory allocation failed for HBGL structure.\n" );
      return NULL;
   }

   memset( pHBGL, 0, sizeof( HBGL ) );

   pHBGL->width  = width;
   pHBGL->height = height;
   pHBGL->title  = title;

   pHBGL->background = 0x323232;

   pHBGL->window = glfwCreateWindow( pHBGL->width, pHBGL->height, pHBGL->title, NULL, NULL );
   if( ! pHBGL->window )
   {
      glfwTerminate();
      return NULL;
   }

   glfwMakeContextCurrent( pHBGL->window );
   glfwSetWindowUserPointer( pHBGL->window, pHBGL );
   glfwSwapInterval( 1 );

   glfwSetCursorPosCallback( pHBGL->window, cursor_position_callback );
   glfwSetKeyCallback( pHBGL->window, key_callback );
   glfwSetMouseButtonCallback( pHBGL->window, mouse_button_callback );
   glfwSetWindowMaximizeCallback( pHBGL->window, window_maximize_callback );
   glfwSetWindowSizeCallback( pHBGL->window, window_size_callback );

   glfwSetWindowSizeLimits( pHBGL->window, pHBGL->width, pHBGL->height, GLFW_DONT_CARE , GLFW_DONT_CARE );

   if( ! gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress ) )
   {
      fprintf( stderr, "Failed to initialize the GLAD library.\n" );
      glfwTerminate();
      return NULL;
   }

   return pHBGL;
}

bool MainLoop( HBGL *pHBGL )
{
   bool state;

   pHBGL->closeFlag = glfwWindowShouldClose( pHBGL->window );
   glfwSetWindowShouldClose( pHBGL->window, GLFW_FALSE );

   state = glfwGetWindowAttrib( pHBGL->window, GLFW_ICONIFIED );
   if( state == T )
   {
      while( state == T )
      {
         state = glfwGetWindowAttrib( pHBGL->window, GLFW_ICONIFIED );

         glfwWaitEventsTimeout( 0.5 );
      }
   }
   return pHBGL->closeFlag;
}

void BeginDrawing( HBGL *pHBGL )
{
   glfwGetFramebufferSize( pHBGL->window, &pHBGL->width, &pHBGL->height );

   glViewport( 0, 0, pHBGL->width, pHBGL->height );
   set_clear_color_from_hex( pHBGL->background );
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glDisable( GL_CULL_FACE );
   glDisable( GL_DEPTH_TEST );
   glDisable( GL_BLEND );

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glOrtho( 0, pHBGL->width, pHBGL->height, 0, -1, 1 );
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();

   glEnable( GL_TEXTURE_2D );
   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void EndDrawing( const HBGL *pHBGL )
{
   REPORT_OPENGL_ERROR( "End drawing: " );
   glfwSwapBuffers( pHBGL->window );
}

void Background( HBGL *pHBGL, unsigned int color )
{
   pHBGL->background = color;
}

int CloseAll( HBGL *pHBGL )
{
   if( pHBGL )
   {
      //--- Font
      if( pHBGL->fonts )
      {
         for( int i = 0; i < pHBGL->fontCount; ++i )
         {
            FreeFont( pHBGL->fonts[ i ] );
         }
         free( pHBGL->fonts );
      }
      //--- Image
      if( pHBGL->images )
      {
         for( int i = 0; i < pHBGL->imageCount; ++i )
         {
            FreeImage( pHBGL->images[ i ] );
         }
         free( pHBGL->images );
      }
      //--- Image button
      if( pHBGL->imagesButton )
      {
         for( int i = 0; i < pHBGL->imageButtonCount; ++i )
         {
            FreeImageButton( pHBGL->imagesButton[ i ] );
         }
         free( pHBGL->imagesButton );
      }

      glfwDestroyWindow( pHBGL->window );
      free( pHBGL );
      glfwTerminate();
      exit( EXIT_SUCCESS );
   }
   else
   {
      fprintf( stderr, "Error: CloseWindow called with NULL HBGL pointer.\n" );
      exit( EXIT_FAILURE );
   }
}

void PollEvents()
{
   glfwPollEvents();
}

void WaitEvents()
{
   glfwWaitEvents();
}

void WaitEventsTimeout( double timeout )
{
   glfwWaitEventsTimeout( timeout );
}

void PrintHBGLMetadata()
{
   printf( "\033[2J" );
   printf( "\033[H" );

   printf( "HBGL Metadata\n" );
   printf( "[\n" );
   printf( "   Major      : %d\n", HBGL_VERSION_MAJOR );
   printf( "   Minor      : %d\n", HBGL_VERSION_MINOR );
   printf( "   Release    : %d\n", HBGL_VERSION_RELEASE );
   printf( "   Status     : %s\n", HBGL_VERSION_STATUS );
   printf( "   Description: %s\n", HBGL_DESCRIPTION );
   printf( "   Copyright  : %s\n", HBGL_COPYRIGHT );
   printf( "   License    : %s\n", HBGL_LICENSE );
   printf( "]\n" );

   fflush( stdout );
}

void PrintHBGLStruct( const HBGL *pHBGL )
{
   printf( "\033[2J" );
   printf( "\033[H" );

   if( pHBGL )
   {
      printf( "HBGL Structure\n" );
      printf( "[\n" );
      printf( "   Window          : %p\n", pHBGL->window );
      printf( "   Width           : %d\n", pHBGL->width );
      printf( "   Height          : %d\n", pHBGL->height );
      printf( "   Title           : %s\n", pHBGL->title );
      printf( "   Close Flag      : %d\n", pHBGL->closeFlag );
      printf( "   Cursor X        : %f\n", pHBGL->cursorX );
      printf( "   Cursor Y        : %f\n", pHBGL->cursorY );
      printf( "   Key Code        : %d\n", pHBGL->keyCode );
      printf( "   Key Scancode    : %d\n", pHBGL->keyScancode );
      printf( "   Key Action      : %d\n", pHBGL->keyAction );
      printf( "   Key Mods        : %d\n", pHBGL->keyMods );
      printf( "   Mouse Button    : %d\n", pHBGL->mouseButton );
      printf( "   Mouse Action    : %d\n", pHBGL->mouseAction );
      printf( "   Mouse Mods      : %d\n", pHBGL->mouseMods );
      printf( "   Window Maximized: %d\n", pHBGL->winMaximized );
      printf( "   Background Color: 0x%X\n", pHBGL->background );
      printf( "]\n" );
   }
   else
   {
      printf( "HBGL structure is NULL\n" );
      return;
   }

   fflush( stdout );
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// tmp
