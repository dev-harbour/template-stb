/*
 *
 */

#include "hbgl.h"

int main()
{
   HBGL *app = WindowNew( 830, 450, "Test text" );
   Font *font = SystemFontNew( app, "Courier_New", 12 );

   Background( app, 0xF1F1F1 );

   while( ! MainLoop( app ) )
   {
      BEGINDRAWING( app );

         int startX = ( app->width - font->width ) / 2;
         int startY = ( app->height - font->height ) / 2;

         DrawFont( font, startX, startY, "Hello, World!", 0x0 );

         PrintFontStruct( font );

      ENDDRAWING( app );
      WaitEvents();
   }

   CloseAll( app );
}
