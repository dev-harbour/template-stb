/*
 *
 */

#include "hbgl.h"

int main()
{
   HBGL *app = WindowNew( 830, 830, "Vertically image gallery" );
   Image *img1 = ImageNew( app, "../docs/assets/button/long_button-1.png" );
   Image *img2 = ImageNew( app, "../docs/assets/button/long_button-2.png" );
   Image *img3 = ImageNew( app, "../docs/assets/button/long_button-3.png" );
   Image *img4 = ImageNew( app, "../docs/assets/button/long_button-4.png" );
   Image *img5 = ImageNew( app, "../docs/assets/button/long_button-5.png" );

   Background( app, 0x202D48 );

   while( ! MainLoop( app ) )
   {
      BEGINDRAWING( app );

         int spaceBetweenImages = 10;
         int totalImagesHeight = 5 * img1->height + 4 * spaceBetweenImages; // dla pięciu obrazów pionowo
         int startX = ( app->width - img1->width ) / 2;                     // wyśrodkuj w poziomie
         int startY = ( app->height - totalImagesHeight ) / 2;              // wyśrodkuj w pionie

         DrawImage( img1, startX, startY, img1->width, img1->height );
         DrawImage( img2, startX, startY + img1->height + spaceBetweenImages, img2->width, img2->height );
         DrawImage( img3, startX, startY + 2 * ( img1->height + spaceBetweenImages ), img3->width, img3->height );
         DrawImage( img4, startX, startY + 3 * ( img1->height + spaceBetweenImages ), img4->width, img4->height );
         DrawImage( img5, startX, startY + 4 * ( img1->height + spaceBetweenImages ), img5->width, img5->height );

      ENDDRAWING( app );
      WaitEvents();
   }

   CloseAll( app );
}
