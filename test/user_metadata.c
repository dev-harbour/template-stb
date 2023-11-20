/*
 *
 */

#define VERSION_MAJOR   0
#define VERSION_MINOR   0
#define VERSION_RELEASE 0
#define VERSION_STATUS  "dev"
#define DESCRIPTION     "Your project's name"
#define COPYRIGHT       "Copyright (c) 2023 Your name or organization name"
#define LICENSE         "MIT License"

#include "hbgl.h"

int main()
{
   HBGL *app = WindowNew( 830, 450, "Test metadata" );

   Background( app, 0xF1F1F1 );

   UserMetadata metadata;
   SetUserMetadata( &metadata, VERSION_MAJOR, VERSION_MINOR, VERSION_RELEASE, VERSION_STATUS, DESCRIPTION, COPYRIGHT, LICENSE );

   while( ! MainLoop( app ) )
   {
      BEGINDRAWING( app );

         // code ...
         PrintUserMetadata( &metadata );

      ENDDRAWING( app );
      WaitEvents();
   }

   CloseAll( app );
}
