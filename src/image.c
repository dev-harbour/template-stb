/*
 *
 */

#include "hbgl.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
Image *ImageNew( HBGL *pHBGL, const char *imagePath )
{
   Image *pImage = malloc( sizeof( Image ) );
   if( ! pImage )
   {
      fprintf( stderr, "Failed to allocate memory for new image.\n" );
      return NULL;
   }

   memset( pImage, 0, sizeof( Image ) );

   pImage->pHBGL = pHBGL;

   pImage->pHBGL->imageCount++;
   pImage->pHBGL->images = realloc( pImage->pHBGL->images, pImage->pHBGL->imageCount * sizeof( Image * ) );
   if( ! pImage->pHBGL->images )
   {
      fprintf( stderr, "Failed to reallocate memory for image array.\n" );
      free( pImage );
      return NULL;
   }
   pImage->pHBGL->images[ pImage->pHBGL->imageCount - 1 ] = pImage;

   pImage->imageID = pImage->pHBGL->imageCount;

   unsigned char *data;
   data = stbi_load( imagePath, &pImage->width, &pImage->height, &pImage->channels, 0 );
   if( ! data )
   {
      fprintf( stderr, "Failed to load image: %s\n", imagePath );

      // Increment the counter of failed image loads
      pImage->pHBGL->failedImageCount++;

      // Cleanup if image loading failed
      pImage->pHBGL->imageCount--;
      if( pImage->pHBGL->imageCount > 0 )
      {
         // Set the last valid element of the array to NULL
         pImage->pHBGL->images[ pImage->pHBGL->imageCount ] = NULL;
      }
      else
      {
         free( pImage->pHBGL->images );
         pImage->pHBGL->images = NULL;
      }

      // Returning Image with zeroed dimensions to avoid errors with unloaded images.
      memset( pImage, 0, sizeof( Image ) );
      return pImage;
   }

   glGenTextures( 1, &pImage->textureID );
   glBindTexture( GL_TEXTURE_2D, pImage->textureID );

   glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );  // Recommended by NVIDIA

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   if( pImage->channels == 3 )
   {
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, pImage->width, pImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
   }
   else if( pImage->channels == 4 )
   {
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, pImage->width, pImage->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
   }

   stbi_image_free( data );

   return pImage;
}

void DrawImage( Image *pImage, int x, int y, int width, int height )
{
   if( pImage && pImage->pHBGL && pImage->pHBGL->images )
   {
      // Aktualizacja pozycji obrazu
      pImage->x = x;
      pImage->y = y;

      bool wasEnabled = glIsEnabled( GL_TEXTURE_2D );
      if( ! wasEnabled )
      {
         glEnable( GL_TEXTURE_2D );
      }

      glBindTexture( GL_TEXTURE_2D, pImage->textureID );

      glBegin( GL_QUADS );
         glTexCoord2f( 0.0f, 1.0f ); glVertex2f( ( float )   x, ( float ) ( y + height ) );
         glTexCoord2f( 1.0f, 1.0f ); glVertex2f( ( float ) ( x + width ), ( float ) ( y + height ) );
         glTexCoord2f( 1.0f, 0.0f ); glVertex2f( ( float ) ( x + width ), ( float ) y );
         glTexCoord2f( 0.0f, 0.0f ); glVertex2f( ( float )   x, ( float ) y );
      glEnd();

      glBindTexture( GL_TEXTURE_2D, 0 );

      if( ! wasEnabled )
      {
         glDisable( GL_TEXTURE_2D );
      }
   }
   else
   {
      fprintf( stderr, "Failed to draw image, missing required data (pImage, pHBGL or images) for the operation." );
      return;
   }
}

void FreeImage( Image *pImage )
{
   if( pImage )
   {
      if( pImage->textureID )
      {
         glDeleteTextures( 1, &pImage->textureID );
      }
      free( pImage );
   }
}

void PrintImageStruct( const Image *pImage )
{
   printf( "\033[2J" );
   printf( "\033[H" );

   if( pImage )
   {
      printf( "Image Structure\n" );
      printf( "[\n" );
      printf( "   HBGL     : %p\n", pImage->pHBGL );
      printf( "   TextureID: %d\n", pImage->textureID );
      printf( "   ImageID  : %d\n", pImage->imageID );
      printf( "   X        : %d\n", pImage->x );
      printf( "   Y        : %d\n", pImage->y );
      printf( "   Width    : %d\n", pImage->width );
      printf( "   Height   : %d\n", pImage->height );
      printf( "   Channels : %d\n", pImage->channels );
      printf( "]\n" );
   }
   else
   {
      printf( "Image structure is NULL\n" );
      return;
   }

   fflush( stdout );
}
