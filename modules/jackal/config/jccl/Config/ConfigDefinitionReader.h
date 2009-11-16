/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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
#ifndef _JCCL_CONFIG_DEFINITION_READER_H_
#define _JCCL_CONFIG_DEFINITION_READER_H_

#include <jccl/jcclConfig.h>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <cppdom/cppdom.h>
#include <jccl/Config/ConfigDefinitionPtr.h>

namespace jccl
{
   /** \class ConfigDefinitionReader ConfigDefinitionReader.h jccl/Config/ConfigDefinitionReader.h
    *
    * This object knows how to deserialize all versions of a configuration
    * definition from a stream.
    */
   class JCCL_CLASS_API ConfigDefinitionReader
   {
   public:
      /**
       * Creates a new config definition reader that will read from the given
       * file.
       */
      ConfigDefinitionReader();

      /** Destroys this config definition reader. */
      ~ConfigDefinitionReader();

   public:
      /**
       * Reads all versions of a configuration definition from the given input
       * stream.
       *
       * @param in the input stream from which to read definitions.
       *
       * @return  a list of all the definitions found in the stream.
       *
       * @throws std::runtime_error
       *             if there was a problem while reading the stream.
       */
      std::vector<ConfigDefinitionPtr> read(std::istream& in)
         throw(std::runtime_error);

      /**
       * Reads all versions of a configuration definition from the given file.
       *
       * @param filename   the file from which to read definitions.
       *
       * @return  a list of all the definitions found in the stream.
       *
       * @throws std::runtime_error
       *             if there was a problem while reading from the file.
       */
      std::vector<ConfigDefinitionPtr> read(const std::string& filename)
         throw(std::runtime_error);

   private:
      /**
       * Determines the version of the definition file format used in the
       * given document.
       *
       * @throws std::runtime_error
       *             if the version cannot be determined.
       */
      std::string getFileFormatVersion(cppdom::DocumentPtr doc)
         throw(std::runtime_error);
   };
} // End of jccl namespace


#endif
