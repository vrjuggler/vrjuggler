
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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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





//////////////////////////////////////////////////////////////////////////////
//
//                         -=     CFileIO class     =-
//
// Description: "stuff useful for file i/o most handle endian-ness issues.
//
///////////////// <auto-copyright BEGIN do not edit this line> /////////////////
//
//    $Date$
//    $Revision$
//    Copyright (C) 1998, 1999, 2000  Kevin Meinert, KevinMeinert@bigfoot.com
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Library General Public
//    License as published by the Free Software Foundation; either
//    version 2 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Library General Public License for more details.
//
//    You should have received a copy of the GNU Library General Public
//    License along with this library; if not, write to the Free
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
///////////////// <auto-copyright END do not edit this line> ///////////////////

//////////////////////////////////////////////////////////////////////////////
#ifndef CFILEIO_INCLUDED
#define CFILEIO_INCLUDED

#include <string>
#include <vector>
#include <fstream.h>
#include <stdio.h> // for FILE

#include "snx/Endian.h" //needed for snxEndian::isBig, snxEndian::isLittle funcs

namespace snxFileIO
{
	//: true - 
	inline bool fileExists( const char* const name )
	{
      if (name == NULL) return false;
      
		FILE* file = ::fopen( name, "r" );
		if (file == NULL)
		{
			return false;
		}
		
		else
		{
			::fclose( file );
			return true;
		}
	}
	
   inline int fileSize( const char* const filename )
   {
      if (filename == NULL) return 0;
      
      if (!fileExists( filename ))
         return 0;

      FILE* fh = fopen( filename, "rb" );
	   
      const int one_kilobyte = 1024;
      const int arbitraliy_chosen_size = one_kilobyte * 1000;
      
      char data[arbitraliy_chosen_size];
      int file_length( arbitraliy_chosen_size ); // makes the first while condition
      int total_size = 0;
      
      while (file_length == arbitraliy_chosen_size)
      {
         file_length = fread( &data, 1, arbitraliy_chosen_size, fh );
         total_size += file_length;
      }
	   
      fclose( fh );

      return total_size;
   }
   
   inline void fileLoad( const char* const filename, std::vector<unsigned char>& data )
   {
      if (filename == NULL) return;
      
      if (!fileExists( filename ))
         return;

      int size = fileSize( filename );
      data.resize( size );
      
      FILE* fh = fopen( filename, "rb" );
	   int file_length = fread( &data[0], 1, data.size(), fh );
      assert( file_length == data.size() );
      fclose( fh );
   }


   enum Endianness
   {
      LITTLE, BIG
   };

	//: Read
	// this function is designed to work with little endian files, such as TGA and BMP.
   // this will return data that is correctly translated for your system architecture.
   // i.e. reading a little endian BMP on a big endian mips system, 
   //      returns (translated) bigendian data
	// i.e. reading a little endian TGA on a little endian intel system, 
   //      returns (untranslated) littleendian data
	template<class typeT>
	inline static int ReadData( Endianness fileByteOrdering, FILE* fp, typeT& data )
	{
		int size = ::fread( &data, sizeof(typeT), 1, fp );

		// if we're not on a little endian machine (intel is little endian) then reverse the bytes.
		if (fileByteOrdering == snxFileIO::LITTLE && snxEndian::isBig() ||
         fileByteOrdering == snxFileIO::BIG && snxEndian::isLittle())
      {
         snxEndian::byteReverse( data );
		}

		return size;
	}

	//: Write
	// this function is designed to work with little endian files, such as TGA and BMP.
	template<class typeT>
	inline static int WriteData( Endianness fileByteOrdering, FILE* fp, const typeT& data )
	{
		register typeT tempData = data;

		// if we're not on a little endian machine (i.e. intel is little endian, mips is big) 
      // then reverse the bytes.
		if (fileByteOrdering == LITTLE && snxEndian::isBig() ||
         fileByteOrdering == BIG && snxEndian::isLittle())
      {
			snxEndian::byteReverse( tempData );
		}
			
		int size = ::fwrite( &tempData, 1, sizeof(typeT), fp );

		return size;
	}



	// Read one byte of data from the file stream
	inline static int ReadByte( FILE *fp, unsigned char& value )
	{
		return ::fread( &value, 1, sizeof(unsigned char), fp );
	}

	// Read one short of data from the file stream
	// NOTE: these enhance readability and type checking, try to use them over the generic ReadData function
	inline static int ReadShort( Endianness fileByteOrdering, FILE* fp, unsigned short& value )
	{
		return ReadData( fileByteOrdering, fp, value );
	}

	// Read one long of data from the file stream
	// NOTE: these enhance readability and type checking, try to use them over the generic ReadData function
	inline static int ReadLong( Endianness fileByteOrdering, FILE *fp, unsigned long& value )
	{
		return ReadData( fileByteOrdering, fp, value );
	}

	// Write one byte of data to the file stream
	inline static int WriteByte( FILE *fp, const unsigned char& value )
	{
		return ::fwrite( &value, 1, sizeof(unsigned char), fp );
	}

	// Write one short of data to the file stream
	// NOTE: these enhance readability and type checking, try to use them over the generic WriteData function
	inline static int WriteShort( Endianness fileByteOrdering, FILE* fp, const unsigned short& value )
	{
		return WriteData( fileByteOrdering, fp, value );
	}

	// Write one long of data to the file stream
	// NOTE: these enhance readability and type checking, try to use them over the generic WriteData function
	inline static int WriteLong( Endianness fileByteOrdering, FILE *fp, const unsigned long& value )
	{
		return WriteData( fileByteOrdering, fp, value );
	}

   inline static void getLine( ifstream& f, std::string& text  )
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

   inline static void getAll( ifstream& f, std::string& buffer )
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
   
}; //end namespace.

#endif
