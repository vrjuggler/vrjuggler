/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * sonix
 *
 * Original Authors:
 *   Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <SNX heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <snx/snxConfig.h>

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <boost/concept_check.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/version.hpp>

#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <snx/FileIO.h>

namespace snx
{

namespace FileIO
{

bool fileExists(const char* name)
{
   if (name == NULL)
   {
      return false;
   }

   try
   {
      boost::filesystem::path file_path(name, boost::filesystem::native);
      return boost::filesystem::exists(file_path);
   }
   catch (boost::filesystem::filesystem_error& ex)
   {
      boost::ignore_unused_variable_warning(ex);
      return false;
   }
}
   
int fileSize(const char* filename)
{
   if (filename == NULL) return 0;
   
   if (!fileExists( filename ))
   {
      return 0;
   }

   // Boost.Filesystem in Boost 1.32.0 has a function that returns the
   // size of a file.  Prefer its use over the other mess.
#if BOOST_VERSION >= 103200
   try
   {
      boost::filesystem::path file_path(filename, boost::filesystem::native);
      return boost::filesystem::file_size(file_path);
   }
   catch (boost::filesystem::filesystem_error& ex)
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR") << ": Could not get size of file '"
         << filename << "': " << ex.what() << std::endl << vprDEBUG_FLUSH;
      return 0;
   }
#else /* BOOST_VERSION < 103200 */
#if defined(_MSC_VER) && _MSC_VER >= 14
   FILE* fh = new FILE;
   ::fopen_s(&fh, filename, "rb");
   vprASSERT(fh != NULL); 

   int size(0);

   if ( fseek(fh, 0, SEEK_END) == 0 )
   {
      return ftell(fh);
   }

   delete fh;
   return size;
#else /* ! _MSC_VER || _MSC_VER < 14 */
   FILE* fh = fopen( filename, "rb" );
   vprASSERT(fh != NULL); 

   if ( fseek(fh, 0, SEEK_END) == 0 )
   {
      return ftell(fh);
   }
   else
   {
      return 0;
   }
#endif
#endif  /* BOOST_VERSION >= 103200 */
}

void fileLoad(const char* filename, std::vector<unsigned char>& data)
{
   if (filename == NULL) return;
   
   if (!fileExists( filename ))
   {
      return;
   }

   int size = fileSize( filename );
   data.resize( size );

#if defined(_MSC_VER) && _MSC_VER >= 14
   FILE* fh = new FILE;
   ::fopen_s(&fh, filename, "rb");
#else
   FILE* fh = fopen( filename, "rb" );
#endif

   unsigned int file_length = fread( &data[0], 1, data.size(), fh );
   vprASSERT(file_length == data.size());
   fclose( fh );

#if defined(_MSC_VER) && _MSC_VER >= 14
   delete fh;
#endif
}

// Read one byte of data from the file stream
int ReadByte( FILE *fp, unsigned char& value )
{
   return ::fread( &value, 1, sizeof(unsigned char), fp );
}

// Read one short of data from the file stream
// NOTE: these enhance readability and type checking, try to use them over the generic ReadData function
int ReadShort( Endianness fileByteOrdering, FILE* fp, unsigned short& value )
{
   return ReadData( fileByteOrdering, fp, value );
}

// Read one long of data from the file stream
// NOTE: these enhance readability and type checking, try to use them over the generic ReadData function
int ReadLong( Endianness fileByteOrdering, FILE *fp, unsigned long& value )
{
   return ReadData( fileByteOrdering, fp, value );
}

// Write one byte of data to the file stream
int WriteByte( FILE *fp, const unsigned char& value )
{
   return ::fwrite( &value, 1, sizeof(unsigned char), fp );
}

// Write one short of data to the file stream
// NOTE: these enhance readability and type checking, try to use them over the generic WriteData function
int WriteShort( Endianness fileByteOrdering, FILE* fp, const unsigned short& value )
{
   return WriteData( fileByteOrdering, fp, value );
}

// Write one long of data to the file stream
// NOTE: these enhance readability and type checking, try to use them over the generic WriteData function
int WriteLong( Endianness fileByteOrdering, FILE *fp, const unsigned long& value )
{
   return WriteData( fileByteOrdering, fp, value );
}

void getLine( std::ifstream& f, std::string& text  )
{
   char buffer[2049];
   f.getline( buffer, 2048, '\n' );
   buffer[2048] = '\0';
   if (f.gcount() < 2048)
   {
      buffer[f.gcount()] = '\0';
   }
   text = buffer;
}

void getAll( std::ifstream& f, std::string& buffer )
{
   //cout<<"Reading:["<<flush;
   while ((f.eof() == false) && (f.fail() == 0) )
   //while (f.gcount() >= 2048)
   {
      //cout<<"."<<flush;
      char buf[2049];
      f.read( buf, 2048 );
      buf[2048] = '\0';
      if (f.gcount() < 2048)
      {
         buf[f.gcount()] = '\0';
      }
      buffer += buf;
   }
   //cout<<"]\n"<<flush;
   //cout << "Gcount == " << f.gcount() << "\n"<<flush;
}

} // End of FileIO namespace

} // End of snx namespace
