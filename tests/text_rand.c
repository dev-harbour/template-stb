/*
 *
 */

#include "hbgl.h"

int main()
{
   HBGL *app = WindowNew( 830, 450, "Test text" );
   Font *font = SystemFontNew( app, "Courier_New", 36 );

   Background( app, 0xF1F1F1 );

   while( ! MainLoop( app ) )
   {
      BEGINDRAWING( app );

      for( int i = 0; i < 20; i++ )
      {
         int x = 1 + rand() / ( ( RAND_MAX + 1u ) / app->width );
         int y = 1 + rand() / ( ( RAND_MAX + 1u ) / app->height );

         unsigned int color = rand() & 0xFFFFFF;

         DrawFont( font, x, y, "Hello, World!", color );
      }

   ENDDRAWING( app );
   WaitEvents();
   }

   CloseAll( app );
}
