/*
 *
 */

#include "hbgl.h"

int main()
{
   HBGL *app = WindowNew( 830, 450, "Test shapes" );

   Background( app, 0xEAEAEA );

   while( ! MainLoop( app ) )
   {
      BEGINDRAWING( app );

         int xMax = app->width;
         int yMax = app->height;

         for( int i = 0; i < 1000; i++ )
         {
            int x = 1 + rand() / ( ( RAND_MAX + 1u ) / xMax );
            int y = 1 + rand() / ( ( RAND_MAX + 1u ) / yMax );
            unsigned int color = rand() & 0xFFFFFF;
            Point( x, y, color );
         }

         for( int i = 0; i < 10; i++ )
         {
            int x = 1 + rand() / ( ( RAND_MAX + 1u ) / xMax );
            int y = 1 + rand() / ( ( RAND_MAX + 1u ) / yMax );
            int size = rand() % 10 + 1;
            unsigned int color = rand() & 0xFFFFFF;
            PointSize( x, y, size, color );
         }

         for( int i = 0; i < 5; i++ )
         {
            int x = 1 + rand() / ( ( RAND_MAX + 1u ) / xMax );
            int y = 1 + rand() / ( ( RAND_MAX + 1u ) / yMax );
            int width  = 1 + rand() / ( ( RAND_MAX + 1u ) / xMax );
            int height = 1 + rand() / ( ( RAND_MAX + 1u ) / yMax );
            unsigned int color = rand() & 0xFFFFFF;
            Line( x, y, width, height, color );
         }

         for( int i = 0; i < 5; i++ )
         {
            int x = 1 + rand() / ( ( RAND_MAX + 1u ) / xMax );
            int y = 1 + rand() / ( ( RAND_MAX + 1u ) / yMax );
            int width  = 1 + rand() / ( ( RAND_MAX + 1u ) / xMax );
            int height = 1 + rand() / ( ( RAND_MAX + 1u ) / yMax );
            int size = rand() % 5 + 1;
            unsigned int color = rand() & 0xFFFFFF;
            LineWidth( x, y, width, height, size, color );
         }

      ENDDRAWING( app );
      WaitEvents();
   }

   CloseAll( app );
}
