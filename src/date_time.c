/*
 *
 */

#include "hbgl.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
int MonthNameToNumber( const char* monthName )
{
   const char *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

   for( int i = 0; i < 12; i++ )
   {
      if( strncmp( months[ i ], monthName, 3 ) == 0 )
      {
         return i + 1;
      }
   }
   return -1;
}
