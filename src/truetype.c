/*
 *
 */

#include "hbgl.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// static
static float calculate_padding( Font *pFont )
{
   float maxHeight = 0;
   float paddingRatio = 0.1f;

   for( int i = 0; i < 380; ++i )
   {
      float charHeight = pFont->cdata[ i ].y1 - pFont->cdata[ i ].y0;
      maxHeight = fmax( maxHeight, charHeight );
   }
   return maxHeight * paddingRatio;
}

static int decode_utf8( const unsigned char *text, int *outCodepoints, int maxSize )
{
   int i = 0, j = 0;
   while( text[ i ] != '\0' && j < maxSize - 1 ) // -1 to make room for -1 at the end
   {
      int codepoint = 0;
      if( ( text[ i ] & 0x80 ) == 0 )
      {
         codepoint = text[ i ];
         i += 1;
      }
      else if( ( text[ i ] & 0xE0 ) == 0xC0 )
      {
         codepoint = ( ( text[ i ] & 0x1F ) << 6 ) | ( text[ i + 1 ] & 0x3F );
         i += 2;
      }
      else if( ( text[ i ] & 0xF0 ) == 0xE0 )
      {
         codepoint = ( ( text[ i ] & 0x0F ) << 12 ) | ( ( text[ i + 1 ] & 0x3F ) << 6 ) | ( text[ i + 2 ] & 0x3F );
         i += 3;
      }
      else if( ( text[ i ] & 0xF8 ) == 0xF0 )
      {
         codepoint = ( ( text[ i ] & 0x07 ) << 18 ) | ( ( text [ i + 1 ] & 0x3F ) << 12 ) | ( ( text[ i + 2 ] & 0x3F ) << 6 ) | ( text[ i + 3 ] & 0x3F );
         i += 4;
      }
      else
      {
         return -1;
      }
      outCodepoints[ j++ ] = codepoint;
   }
   outCodepoints[ j ] = -1;
   return ( text[ i ] == '\0' ) ? 0 : -1;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
Font *SystemFontNew( HBGL *pHBGL, const char *fontName, int fontSize )
{
   char fontPath[ 1024 ];
   FILE *pFile = NULL;

   #if defined( _WIN32 ) || defined( _WIN64 )
      snprintf( fontPath, sizeof( fontPath ), "C:\\Windows\\Fonts\\%s.ttf", fontName );
      pFile = fopen( fontPath, "rb" );
   #elif defined( __APPLE__ ) || defined( __MACH__ )
      snprintf( fontPath, sizeof( fontPath ), "/Library/Fonts/%s.ttf", fontName );
      pFile = fopen( fontPath, "rb" );
   #elif defined( __linux__ )
      // Lista katalogów do przeszukania
      // find / -name your_font.ttf 2>/dev/null
      const char *directories[] =
      {
         "/usr/share/fonts/truetype/msttcorefonts/",
         "/usr/share/fonts/truetype/",
         "/usr/share/fonts/",
         "/usr/local/share/fonts/",
         NULL // Terminator
      };

      for( int i = 0; directories[ i ] != NULL; ++i )
      {
         snprintf( fontPath, sizeof( fontPath ), "%s%s.ttf", directories[ i ], fontName );
         pFile = fopen( fontPath, "rb" );
         if( pFile != NULL )
         {
            break;
         }
      }
   #else
      fprintf( stderr, "Unsupported platform\n" );
      return NULL;
   #endif

   if( ! pFile )
   {
      fprintf( stderr, "Font file not found in system directories: %s", fontName );
      // Prepare the path to a non-existent file
      snprintf( fontPath, sizeof( fontPath ), "%s.ttf", fontName );
   }
   else
   {
      fclose( pFile );
   }

   return FontNew( pHBGL, fontPath, fontSize );
}

Font *FontNew( HBGL *pHBGL, const char *fontPath, int fontSize )
{
   Font *pFont = malloc( sizeof( Font ) );
   if( ! pFont )
   {
      fprintf( stderr, "Failed to allocate memory for new font." );
      return NULL;
   }

   memset( pFont, 0, sizeof( Font ) );

   pFont->pHBGL = pHBGL;
   pFont->fontSize = fontSize;

   pFont->pHBGL->fontCount++;
   pFont->pHBGL->fonts = realloc( pFont->pHBGL->fonts, pFont->pHBGL->fontCount * sizeof( Font * ) );
   if( ! pFont->pHBGL->fonts )
   {
      fprintf( stderr, "Failed to reallocate memory for font array." );
      free( pFont );
      return NULL;
   }
   pFont->pHBGL->fonts[ pFont->pHBGL->fontCount - 1 ] = pFont;

   pFont->fontID = pFont->pHBGL->fontCount;

   FILE *pFile = fopen( fontPath, "rb" );
   if( ! pFile )
   {
      fprintf( stderr, "Failed to open font file: %s", fontPath );
      // Increment the counter of failed font loads
      pFont->pHBGL->failedFontCount++;

      // Cleanup if image loading failed
      pFont->pHBGL->fontCount--;
      if( pFont->pHBGL->fontCount > 0 )
      {
         // Set the last valid element of the array to NULL
         pFont->pHBGL->fonts[ pFont->pHBGL->fontCount ] = NULL;
      }
      else
      {
         free( pFont->pHBGL->fonts );
         pFont->pHBGL->fonts = NULL;
      }

      // Returning Font with zeroed dimensions to avoid errors with unloaded images.
      memset( pFont, 0, sizeof( Font ) );
      return pFont;
   }

   fseek( pFile, 0, SEEK_END );
   size_t size = ftell( pFile );
   fseek( pFile, 0, SEEK_SET );

   unsigned char *ttfBuffer = malloc( size );
   if( ! ttfBuffer )
   {
      fprintf( stderr, "Failed to allocate memory for font buffer.\n" );
      fclose( pFile );
      return NULL;
   }

   size_t readElements = fread( ttfBuffer, 1, size, pFile );
   if( readElements != size )
   {
      fprintf( stderr, "Failed to read font file: expected %zu elements, got %zu.", size, readElements );
      free( ttfBuffer );
      fclose( pFile );
      return NULL;
   }

   fclose( pFile );

   stbtt_fontinfo  fontInfo;
   if( ! stbtt_InitFont( &fontInfo, ttfBuffer, stbtt_GetFontOffsetForIndex( ttfBuffer, 0 ) ) )
   {
      fprintf( stderr, "Failed to initialize the font." );
      free( ttfBuffer );
      return NULL;
   }

   unsigned char tempBitmap[ FONT_TEXTURE_SIZE * FONT_TEXTURE_SIZE ];
   stbtt_BakeFontBitmap( ttfBuffer, 0, pFont->fontSize, tempBitmap, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, 32, 96, pFont->cdata );

   glGenTextures( 1, &pFont->textureID );
   glBindTexture( GL_TEXTURE_2D, pFont->textureID );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tempBitmap );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

   free( ttfBuffer );
   return pFont;
}

void DrawFont( Font *pFont, int x, int y, const char *text, unsigned int foreground )
{
   if( pFont && pFont->pHBGL && pFont->pHBGL->fonts )
   {
      int i;

      // We start from the position where the text is to be displayed
      float currentX;
      float currentY;

      // Initialization of variables for calculating dimensions
      float minX = x;
      float minY = y;
      float maxX = x;
      float maxY = y;

      bool wasEnabled = glIsEnabled( GL_TEXTURE_2D );
      if( ! wasEnabled )
      {
         glEnable( GL_TEXTURE_2D );
      }

      // Decoding UTF-8
      int codepoints[ 256 ];
      if( decode_utf8( ( const unsigned char * ) text, codepoints, 256 ) < 0 )
      {
         fprintf( stderr, "Incorrect sequence UTF-8." );
         return;
      }

      set_color_from_hex( foreground );
      glBindTexture( GL_TEXTURE_2D, pFont->textureID );
      glBegin( GL_QUADS );
         currentX = x;
         currentY = y;
         i = 0;
         while( codepoints[ i ] != -1 )
         {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad( pFont->cdata, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, codepoints[ i ] - 32, &currentX, &currentY, &q, 1 );

            glTexCoord2f( q.s0, q.t0 ); glVertex2f( q.x0, q.y0 );
            glTexCoord2f( q.s1, q.t0 ); glVertex2f( q.x1, q.y0 );
            glTexCoord2f( q.s1, q.t1 ); glVertex2f( q.x1, q.y1 );
            glTexCoord2f( q.s0, q.t1 ); glVertex2f( q.x0, q.y1 );

            minX = fmin( minX, q.x0 );
            minY = fmin( minY, q.y0 );
            maxX = fmax( maxX, q.x1 );
            maxY = fmax( maxY, q.y1 );

            i++;
         }
      glEnd();
      glBindTexture( GL_TEXTURE_2D, 0 );

      // Position update
      pFont->x = x;
      pFont->y = y;
      pFont->width  = maxX - minX;
      pFont->height = maxY - minY;

      if( ! wasEnabled )
      {
         glDisable( GL_TEXTURE_2D );
      }
   }
   else
   {
      fprintf( stderr, "Failed to draw font, missing required data (pFont, pHBGL or fonts) for the operation." );
      return;
   }
}

void DrawFontBg( Font *pFont, int x, int y, const char *text, unsigned int foreground, unsigned int background )
{
   if( pFont && pFont->pHBGL && pFont->pHBGL->fonts )
   {
      int i;

      // We start from the position where the text is to be displayed
      float currentX;
      float currentY;

      // Initialization of variables for calculating dimensions
      float minX = x;
      float minY = y;
      float maxX = x;
      float maxY = y;

      bool wasEnabled = glIsEnabled( GL_TEXTURE_2D );
      if( ! wasEnabled )
      {
         glEnable( GL_TEXTURE_2D );
      }

      // Decoding UTF-8
      int codepoints[ 256 ];
      if( decode_utf8( ( const unsigned char * ) text, codepoints, 256 ) < 0 )
      {
         fprintf( stderr, "Incorrect sequence UTF-8." );
         return;
      }

      currentX = x;
      currentY = y;
      i = 0;
      while( codepoints[ i ] != -1 )
      {
         stbtt_aligned_quad q;
         stbtt_GetBakedQuad( pFont->cdata, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, codepoints[ i ] - 32, &currentX, &currentY, &q, 1 );

         // Calculating dimensions (bounding box)
         minX = fmin( minX, q.x0 );
         minY = fmin( minY, q.y0 );
         maxX = fmax( maxX, q.x1 );
         maxY = fmax( maxY, q.y1 );

         i++;
      }

      float padding = calculate_padding( pFont );

      set_color_from_hex( background );
      // Drawing a background with a margin
      glBegin( GL_QUADS );
         glVertex2f( minX - padding, minY - padding ); // Dolny lewy róg
         glVertex2f( maxX + padding, minY - padding ); // Dolny prawy róg
         glVertex2f( maxX + padding, maxY + padding ); // Górny prawy róg
         glVertex2f( minX - padding, maxY + padding ); // Górny lewy róg
      glEnd();

      // Commented alternative
      /*
      float backgroundWidth = ( maxX - minX ) + ( 2 * padding );
      float backgroundHeight = ( maxY - minY ) + ( 2 * padding );
      FillRect( minX - padding, minY - padding, backgroundWidth, backgroundHeight, background );
      */

      set_color_from_hex( foreground );
      glBindTexture( GL_TEXTURE_2D, pFont->textureID );
      glBegin( GL_QUADS );
         currentX = x;
         currentY = y;
         i = 0;
         while( codepoints[ i ] != -1 )
         {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad( pFont->cdata, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, codepoints[ i ] - 32, &currentX, &currentY, &q, 1 );

            glTexCoord2f( q.s0, q.t0 ); glVertex2f( q.x0, q.y0 );
            glTexCoord2f( q.s1, q.t0 ); glVertex2f( q.x1, q.y0 );
            glTexCoord2f( q.s1, q.t1 ); glVertex2f( q.x1, q.y1 );
            glTexCoord2f( q.s0, q.t1 ); glVertex2f( q.x0, q.y1 );

            i++;
         }
      glEnd();
      glBindTexture( GL_TEXTURE_2D, 0 );

      // Position update
      pFont->x = x;
      pFont->y = y;
      pFont->width = maxX - minX;
      pFont->height = maxY - minY;

      if( ! wasEnabled )
      {
         glDisable( GL_TEXTURE_2D );
      }
   }
   else
   {
      fprintf( stderr, "Failed to draw font, missing required data (pFont, pHBGL or fonts) for the operation.");
      return;
   }
}

void FreeFont( Font *pFont )
{
   if( pFont )
   {
      if( pFont->textureID )
      {
         glDeleteTextures( 1, &pFont->textureID );
      }
      free( pFont );
   }
}

void PrintFontStruct( const Font *pFont )
{
   printf( "\033[2J" );
   printf( "\033[H" );

   if( pFont )
   {
      printf( "Image Structure\n" );
      printf( "[\n" );
      printf( "   HBGL     : %p\n", pFont->pHBGL );
      printf( "   TextureID: %d\n", pFont->textureID );
      printf( "   FontID   : %d\n", pFont->fontID );
      printf( "   X        : %d\n", pFont->x );
      printf( "   Y        : %d\n", pFont->y );
      printf( "   Width    : %d\n", pFont->width );
      printf( "   Height   : %d\n", pFont->height );
      printf( "   Font size: %d\n", pFont->fontSize );
      printf( "]\n" );
   }
   else
   {
      printf( "Font structure is NULL\n" );
      return;
   }

   fflush( stdout );
}
