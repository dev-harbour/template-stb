/*
 *
 */

#include "hbgl.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
void SetCompileDateAndTime( UserMetadata *metadata )
{
   if( metadata )
   {
      int monthDay, year;
      char monthStr[ 4 ];
      sscanf( __DATE__, "%3s %2d %4d", monthStr, &monthDay, &year );
      snprintf( metadata->compileDate, sizeof( metadata->compileDate ), "%04d %s %02d", year, monthStr, monthDay );

      int hour, minute;
      sscanf( __TIME__, "%2d:%2d", &hour, &minute );
      snprintf( metadata->compileTime, sizeof( metadata->compileTime ), "%02d:%02d", hour, minute );
   }
}

void SetUserMetadata( UserMetadata *metadata, int major, int minor, int release, const char *status, const char *description, const char *copyright, const char *license )
{
   if( metadata )
   {
      metadata->major = major;
      metadata->minor = minor;
      metadata->release = release;
      metadata->status = status;
      metadata->description = description;
      metadata->copyright = copyright;
      metadata->license = license;

      SetCompileDateAndTime( metadata );
   }
}

bool PrintUserMetadata( const UserMetadata *metadata )
{
   if( metadata->major != -1 && metadata->minor != -1 && metadata->release != -1 && metadata->status && metadata->description && metadata->copyright && metadata->license )
   {
      printf( "\033[2J" );
      printf( "\033[H" );

      printf( "User Metadata\n" );
      printf( "[\n" );
      printf( "   Major      : %d\n", metadata->major );
      printf( "   Minor      : %d\n", metadata->minor );
      printf( "   Release    : %d\n", metadata->release );
      printf( "   Status     : %s\n", metadata->status );
      printf( "   Build ver. : %s %s\n", metadata->compileDate, metadata->compileTime );
      printf( "   Description: %s\n", metadata->description );
      printf( "   Copyright  : %s\n", metadata->copyright );
      printf( "   License    : %s\n", metadata->license );
      printf( "]\n" );
      return T;
   }
   else
   {
      printf( "Error: Not all required data was provided in Metadata.\n" );
      return F;
   }
}
