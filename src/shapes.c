/*
 *
 */

#include "hbgl.h"

//* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
void Point( int x, int y, unsigned int color )
{
   set_color_from_hex( color );
   glPointSize( 1.0 );
   glBegin( GL_POINTS );
      glVertex2i( x, y + 1 );
   glEnd();
}

void PointSize( int x, int y, int pointSize, unsigned int color )
{
   if( pointSize < 1 )
   {
      fprintf( stderr, "Point size should be greater than 0.\n" );
      pointSize = 1;
   }

   set_color_from_hex( color );
   glPointSize( pointSize );
   glBegin( GL_POINTS );
      glVertex2i( x, y + 1 );
   glEnd();
}

void Line( int x, int y, int width, int height, unsigned int color )
{
   set_color_from_hex( color );
   glBegin( GL_LINES );
      glVertex2i( x, y );
      glVertex2i( x + width, y + height );
   glEnd();
}

void LineWidth( int x, int y, int width, int height, int lineWidth, unsigned int color )
{
   if( lineWidth < 1 )
   {
      fprintf( stderr, "Line width should be greater than 0.\n" );
      lineWidth = 1;
   }

   set_color_from_hex( color );
   glLineWidth( lineWidth );
   glBegin( GL_LINES );
      glVertex2i( x, y );
      glVertex2i( x + width, y + height );
   glEnd();
}

void Rect( int x, int y, int width, int height, unsigned int color )
{
   set_color_from_hex( color );
   glBegin( GL_LINE_LOOP );
      glVertex2i( x, y );
      glVertex2i( x + width, y );
      glVertex2i( x + width, y + height );
      glVertex2i( x, y + height + 1 );
   glEnd();
}

void RectWidthToInside( int x, int y, int width, int height, int numberLines, unsigned int color )
{
   if( numberLines < 1 )
   {
      fprintf( stderr, "Number of lines should be greater than 0.\n" );
      numberLines = 1;
   }

   set_color_from_hex( color );
   for( int i = 0; i < numberLines; i++ )
   {
      // Top
      Line( x, y + i, width, 0, color );
      // Right
      Line( x + width - i, y + i, 0, height - i, color );
      // Bottom
      Line( x, y + height - i, width, 0, color );
      //Lef
      Line( x + i, y + i, 0, height - i, color );
   }
}

void RectWidthToCenter( int x, int y, int width, int height, int numberLines, unsigned int color )
{
   if( numberLines < 1 )
   {
      fprintf( stderr, "Number of lines should be greater than 0.\n" );
      numberLines = 1;
   }

   int halfLines = numberLines / 2;

   set_color_from_hex( color );
   for( int i = 0; i < halfLines; i++ )
   {
      // Rysowanie do środka
      // Top
      Line( x, y + i, width, 0, color );
      // Right
      Line( x + width - i, y + i, 0, height - i, color );
      // Bottom
      Line( x, y + height - i, width, 0, color );
      //Lef
      Line( x + i, y + i, 0, height - i, color );
   }

   for( int i = 0; i < numberLines - halfLines; i++ )
   {
      // Rysowanie na zewnątrz
      // Top
      Line( x - i, y - i, width + i * 2, 0, color );
      // Right
      Line( x + width + i, y - i, 0, height + i * 2, color );
      // Bottom
      Line( x - 1 - i, y + height + i, width + 1 + i * 2, 0, color );
      // Left
      Line( x - i, y - i, 0, height + i * 2, color );
   }
}

void RectWidthToOutside( int x, int y, int width, int height, int numberLines, unsigned int color )
{
   if( numberLines < 1 )
   {
      fprintf( stderr, "Number of lines should be greater than 0.\n" );
      numberLines = 1;
   }

   set_color_from_hex( color );
   for( int i = 0; i < numberLines; i++ )
   {
      // Top
      Line( x - i, y - i, width + i * 2, 0, color );
      // Right
      Line( x + width + i, y - i, 0, height + i * 2, color );
      // Bottom
      Line( x - 1 - i, y + height + i, width + 1 + i * 2, 0, color );
      // Left
      Line( x - i, y - i, 0, height + i * 2, color );
   }
}

void FillRect( int x, int y, int width, int height, unsigned int color )
{
   set_color_from_hex( color );
   glBegin( GL_QUADS );
   glVertex2f( x, y );
   glVertex2f( x + width, y );
   glVertex2f( x + width, y + height );
   glVertex2f( x, y + height );
   glEnd();
}
