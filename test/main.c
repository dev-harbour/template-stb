/*
 *
 */

#include "hbgl.h"

int main()
{
   HBGL *app = WindowNew( 830, 450, "Test window" );

   Background( app, 0xEAEAEA );

   while( ! MainLoop( app ) )
   {
      BEGINDRAWING( app );

         // code ...
         PrintHBGLStruct( app );

      ENDDRAWING( app );
      WaitEvents();
   }

   CloseAll( app );
}
