/*
 *
 */

#include "hbgl.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
void openURL( const char *url )
{
   const int commandBufferSize = 256;
   const int commandPrefixMaxSize = 10; // for "xdg-open ", "start ", "open "

   if( strlen( url ) > ( commandBufferSize - commandPrefixMaxSize - 1 ) )
   {
      fprintf(stderr, "URL is too long\n");
      return;
   }

   char command[ commandBufferSize ]; // Create a buffer on command

#if defined( _WIN32 ) || defined( _WIN64 )
   snprintf( command, sizeof( command ), "start %s", url );
#elif defined( __APPLE__ ) || defined( __MACH__ )
   snprintf( command, sizeof( command ), "open %s", url );
#elif defined( __linux__ )
   snprintf( command, sizeof( command ), "xdg-open %s", url );
#else
   fprintf( stderr, "Unsupported platform\n" );
   return;
#endif

   // Execute the command and check the result
   int result = system( command );
   if( result != 0 )
   {
      fprintf( stderr, "Failed to open URL\n" );
   }
}
