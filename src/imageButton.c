/*
 *
 */

#include "hbgl.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// static
static int is_point_inside( ImageButton *pImageButton, int x, int y )
{
   return x >= pImageButton->x && x <= ( pImageButton->x + ( int ) pImageButton->width ) &&
          y >= pImageButton->y && y <= ( pImageButton->y + ( int ) pImageButton->height );
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
ImageButton *ImageButtonNew( HBGL *pHBGL, const char *imageButtonPath, void ( *onClick )() )
{
   ImageButton *pImageButton = malloc( sizeof( ImageButton ) );
   if( ! pImageButton )
   {
      fprintf( stderr, "Failed to allocate memory for new image.\n" );
      return NULL;
   }

   memset( pImageButton, 0, sizeof( ImageButton ) );

   pImageButton->pHBGL = pHBGL;
   pImageButton->onClick = onClick;

   pImageButton->pHBGL->imageButtonCount++;
   pImageButton->pHBGL->imagesButton = realloc( pImageButton->pHBGL->imagesButton, pImageButton->pHBGL->imageButtonCount * sizeof( ImageButton * ) );
   if( ! pImageButton->pHBGL->imagesButton )
   {
      fprintf( stderr, "Failed to reallocate memory for image array.\n" );
      free( pImageButton );
      return NULL;
   }
   pImageButton->pHBGL->imagesButton[ pImageButton->pHBGL->imageButtonCount - 1 ] = pImageButton;

   pImageButton->imageButtonID = pImageButton->pHBGL->imageButtonCount;

   unsigned char *data;
   data = stbi_load( imageButtonPath, &pImageButton->width, &pImageButton->height, &pImageButton->channels, 0 );
   if( ! data )
   {
      fprintf( stderr, "Failed to load image: %s\n", imageButtonPath );

      // Increment the counter of failed image loads
      pImageButton->pHBGL->failedImageButtonCount++;

      // Cleanup if image loading failed
      pImageButton->pHBGL->imageButtonCount--;
      if( pImageButton->pHBGL->imageButtonCount > 0 )
      {
         // Set the last valid element of the array to NULL
         pImageButton->pHBGL->imagesButton[ pImageButton->pHBGL->imageButtonCount ] = NULL;
      }
      else
      {
         free( pImageButton->pHBGL->imagesButton );
         pImageButton->pHBGL->imagesButton = NULL;
      }

      // Returning ImageButton with zeroed dimensions to avoid errors with unloaded imagesButton.
      memset( pImageButton, 0, sizeof( ImageButton ) );
      return pImageButton;
   }

   glGenTextures( 1, &pImageButton->textureID );
   glBindTexture( GL_TEXTURE_2D, pImageButton->textureID );

   glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );  // Recommended by NVIDIA

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   if( pImageButton->channels == 3 )
   {
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, pImageButton->width, pImageButton->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
   }
   else if( pImageButton->channels == 4 )
   {
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, pImageButton->width, pImageButton->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
   }

   stbi_image_free( data );

   return pImageButton;
}

void DrawImageButton( ImageButton *pImageButton, int x, int y, int width, int height )
{
   if( pImageButton && pImageButton->pHBGL && pImageButton->pHBGL->imagesButton )
   {
      pImageButton->x = x;
      pImageButton->y = y;

      if( is_point_inside( pImageButton, pImageButton->pHBGL->cursorX, pImageButton->pHBGL->cursorY ) )
      {
         pImageButton->mouseOver = T;
      }
      else
      {
         pImageButton->mouseOver = F;
      }

      if( pImageButton->pHBGL->mouseButton == GLFW_MOUSE_BUTTON_LEFT && pImageButton->pHBGL->mouseAction == GLFW_PRESS )
      {
         if( pImageButton->mouseOver )
         {
            pImageButton->clicked = T;
            if( pImageButton->onClick )
            {
               pImageButton->onClick();
            }
         }
      }
      else if( pImageButton->pHBGL->mouseButton == GLFW_MOUSE_BUTTON_LEFT && pImageButton->pHBGL->mouseAction == GLFW_RELEASE )
      {
         pImageButton->clicked = F; // Reset the 'clicked' flag after releasing the mouse image button
      }

      if( pImageButton->clicked )
      {
         glColor3f( 1.0f, 1.0f, 1.0f );  // Increase brightness by 10%
      }
      else if( pImageButton->mouseOver )
      {
         glColor3f( 0.9f, 0.9f, 0.9f );  // Reduce brightness by 10%
      }

      bool wasEnabled = glIsEnabled( GL_TEXTURE_2D );
      if( ! wasEnabled )
      {
         glEnable( GL_TEXTURE_2D );
      }

      glBindTexture( GL_TEXTURE_2D, pImageButton->textureID );

      glBegin( GL_QUADS );
         glTexCoord2f( 0.0f, 1.0f ); glVertex2f( ( float )   x, ( float ) ( y + height ) );
         glTexCoord2f( 1.0f, 1.0f ); glVertex2f( ( float ) ( x + width ), ( float ) ( y + height ) );
         glTexCoord2f( 1.0f, 0.0f ); glVertex2f( ( float ) ( x + width ), ( float ) y );
         glTexCoord2f( 0.0f, 0.0f ); glVertex2f( ( float )   x, ( float ) y );
      glEnd();

      glBindTexture( GL_TEXTURE_2D, 0 );

      if( pImageButton->clicked )
      {
         Rect( x, y, width, height,  0x323232 );
      }
      else if( pImageButton->mouseOver )
      {
         Rect( x, y, width, height,  0xFFFFFF );
      }

      if( ! wasEnabled )
      {
         glDisable( GL_TEXTURE_2D );
      }
   }
   else
   {
      fprintf( stderr, "Failed to draw image button, missing required data (pImageButton, pHBGL or imagesButton) for the operation." );
      return;
   }
}

void FreeImageButton( ImageButton *pImageButton )
{
   if( pImageButton )
   {
      if( pImageButton->textureID )
      {
         glDeleteTextures( 1, &pImageButton->textureID );
      }
      free( pImageButton );
   }
}

