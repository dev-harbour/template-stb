/*
 *
 */

#include "hbgl.h"

void myOnClickFunction()
{
   printf( "Image button was clicked!\n" );
   openURL( "https://github.com/dev-harbour/hbgl" );
}

int main()
{
   HBGL *app = WindowNew( 830, 450, "Test image button" );
   ImageButton *imgBtn = ImageButtonNew( app, "../docs/assets/img/harbour_logo.png", myOnClickFunction );

   Background( app, 0xF1F1F1 );

   while( ! MainLoop( app ) )
   {
      BEGINDRAWING( app );

         int startX = ( app->width  - imgBtn->width ) / 2;
         int startY = ( app->height - imgBtn->height ) / 2;

         DrawImageButton( imgBtn, startX, startY, imgBtn->width, imgBtn->height );

      ENDDRAWING( app );
      WaitEvents();
   }

   CloseAll( app );
}
