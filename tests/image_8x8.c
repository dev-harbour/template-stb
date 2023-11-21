/*
 *
 */

#include "hbgl.h"

int main()
{
   HBGL *app = WindowNew( 830, 450, "Test Image 8x8" );
   Image *img = ImageNew( app, "../docs/assets/img/8x8.png" );

   Background( app, 0x323232 );

   while( ! MainLoop( app ) )
   {
      BEGINDRAWING( app );

         for( int i = 0; i < app->width; i += 8 )
         {
            for( int j = 0; j < app->height ; j += 8 )
            {
               DrawImage( img, i, j, 8, 8 );
            }
         }

      ENDDRAWING( app );
      WaitEvents( app );
   }

   CloseAll( app );
   return 0;
}
