/*
 *
 */

#include "hbgl.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
bool openURL( const char *url )
{
   const int commandBufferSize = 256;
   const int commandPrefixMaxSize = 10; // for "xdg-open ", "start ", "open "

   if( strlen( url ) > ( commandBufferSize - commandPrefixMaxSize - 1 ) )
   {
      fprintf( stderr, "URL is too long\n" );
      return F;
   }

   char command[ commandBufferSize ]; // Create a buffer for command

#if defined( _WIN32 ) || defined( _WIN64 )
   snprintf( command, sizeof( command ), "start %s", url );
#elif defined( __APPLE__ ) || defined( __MACH__ )
   snprintf( command, sizeof( command ), "open %s", url );
#elif defined( __linux__ )
   snprintf( command, sizeof( command ), "xdg-open %s", url );
#else
   fprintf( stderr, "Unsupported platform\n" );
   return F;
#endif

   // Execute the command and check the result
   int result = system( command );
   if( result != 0 )
   {
      fprintf( stderr, "Failed to open URL\n" );
      return F;
   }

   return T;
}

bool openEmailClient( const char *emailAddress )
{
   const int commandBufferSize = 256;
   const int commandPrefixMaxSize = 10; // for "xdg-open ", "start ", "open "

   if( strlen( emailAddress ) > ( commandBufferSize - commandPrefixMaxSize - 1 ) )
   {
      fprintf( stderr, "Email address is too long\n" );
      return F;
   }

   char command[ commandBufferSize ];

#if defined( _WIN32 ) || defined( _WIN64 )
   snprintf( command, sizeof( command ), "start mailto:%s", emailAddress );
#elif defined( __APPLE__ ) || defined( __MACH__ )
   snprintf( command, sizeof( command ), "open mailto:%s", emailAddress );
#elif defined( __linux__ )
   snprintf( command, sizeof( command ), "xdg-email %s", emailAddress );
#else
   fprintf( stderr, "Unsupported platform\n" );
   return F;
#endif

   int result = system( command );
   if( result != 0 )
   {
      fprintf( stderr, "Failed to open email client\n" );
      return F;
   }

   return T;
}
