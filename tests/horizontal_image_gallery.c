/*
 *
 */

#include "hbgl.h"

int main()
{
   HBGL *app = WindowNew( 830, 450, "Horizontal image gallery" );
   Image *img1 = ImageNew( app, "../docs/assets/button/button-1.png" );
   Image *img2 = ImageNew( app, "../docs/assets/button/button-2.png" );
   Image *img3 = ImageNew( app, "../docs/assets/button/button-3.png" );
   Image *img4 = ImageNew( app, "../docs/assets/button/button-4.png" );
   Image *img5 = ImageNew( app, "../docs/assets/button/button-5.png" );

   Background( app, 0x324032 );

   while( ! MainLoop( app ) )
   {
      BEGINDRAWING( app );

         int spaceBetweenImages = 10;
         int totalImagesWidth = 5 * img1->width + 4 * spaceBetweenImages; // dla pięciu obrazów
         int startX = ( app->width - totalImagesWidth ) / 2;              // wyśrodkuj wszystkie obrazy
         int startY = ( app->height - img1->height ) / 2;

         DrawImage( img1, startX, startY, img1->width, img1->height );
         DrawImage( img2, startX + img1->width + spaceBetweenImages, startY, img2->width, img2->height );
         DrawImage( img3, startX + 2 * ( img1->width + spaceBetweenImages ), startY, img3->width, img3->height );
         DrawImage( img4, startX + 3 * ( img1->width + spaceBetweenImages ), startY, img4->width, img4->height );
         DrawImage( img5, startX + 4 * ( img1->width + spaceBetweenImages ), startY, img5->width, img5->height );

      ENDDRAWING( app );
      WaitEvents();
   }

   CloseAll( app );
}
