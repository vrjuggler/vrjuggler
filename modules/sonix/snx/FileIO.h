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

#ifndef _SNX_FILE_IO_H_
#define _SNX_FILE_IO_H_

#include <snx/snxConfig.h>

#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>

#include <snx/Endian.h>


namespace snx
{

namespace FileIO
{
   SNX_API(bool) fileExists(const char* name);

   SNX_API(int) fileSize(const char* filename);

   SNX_API(void) fileLoad(const char* filename,
                          std::vector<unsigned char>& data);

   enum Endianness
   {
      LITTLE, BIG
   };

   /**
    * Read.  This function is designed to work with little endian files, such
    * as TGA and BMP.  This will return data that is correctly translated for
    * your system architecture.  In other words, reading a little endian BMP
    * on a big endian mips system returns (translated) bigendian data.  For
    * example, reading a little endian TGA on a little endian intel system,
    * returns (untranslated) littleendian data.
    */
   template<class typeT>
   inline int ReadData(Endianness fileByteOrdering, FILE* fp, typeT& data)
   {
      int size = ::fread( &data, sizeof(typeT), 1, fp );

      // if we're not on a little endian machine (intel is little endian) then
      // reverse the bytes.
      if (fileByteOrdering == snx::FileIO::LITTLE && vpr::System::isBigEndian() ||
         fileByteOrdering == snx::FileIO::BIG && vpr::System::isLittleEndian())
      {
         snx::Endian::byteReverse( data );
      }

      return size;
   }

   /**
    * Write.  This function is designed to work with little endian files,
    * such as TGA and BMP.
    */
   template<class typeT>
   inline int WriteData(Endianness fileByteOrdering, FILE* fp,
                        const typeT& data)
   {
      typeT tempData = data;

      // if we're not on a little endian machine (i.e. intel is little endian,
      // mips is big), then reverse the bytes.
      if ( fileByteOrdering == LITTLE && vpr::System::isBigEndian() ||
           fileByteOrdering == BIG && vpr::System::isLittleEndian() )
      {
         snx::Endian::byteReverse( tempData );
      }

      int size = ::fwrite( &tempData, 1, sizeof(typeT), fp );

      return size;
   }

   /** Reads one byte of data from the file stream. */
   int ReadByte(FILE *fp, unsigned char& value);

   /**
    * Reads one short of data from the file stream.
    * @note These enhance readability and type checking; try to use them over
    *       the generic ReadData function.
    */
   int ReadShort(Endianness fileByteOrdering, FILE* fp, unsigned short& value);

   /**
    * Reads one long of data from the file stream.
    * @note These enhance readability and type checking; try to use them over
    *       the generic ReadData function.
    */
   int ReadLong(Endianness fileByteOrdering, FILE *fp, unsigned long& value);

   /** Writes one byte of data to the file stream. */
   int WriteByte(FILE *fp, const unsigned char& value);

   /**
    * Writes one short of data to the file stream.
    * @note These enhance readability and type checking; try to use them over
    *       the generic WriteData function.
    */
   int WriteShort(Endianness fileByteOrdering, FILE* fp,
                  const unsigned short& value);

   /**
    * Writes one long of data to the file stream.
    * @note These enhance readability and type checking; try to use them over
    *       the generic WriteData function.
    */
   int WriteLong(Endianness fileByteOrdering, FILE *fp,
                 const unsigned long& value);

   void getLine( std::ifstream& f, std::string& text  );

   void getAll( std::ifstream& f, std::string& buffer );

} // namespace FileIO

} // namespce snx

#endif
