/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _JCCL_CONFIGURATION_H_
#define _JCCL_CONFIGURATION_H_

#include <jccl/jcclConfig.h>
#include <cppdom/cppdom.h>
#include <vector>
#include <jccl/Config/ConfigElementPtr.h>


namespace jccl
{

/** \class Configuration Configuration.h jccl/Config/Configuration.h
 *
 * List of jccl::ConfigElement objects.
 */
class JCCL_API Configuration
{
public:

   /** Constructor.  Creates an empty configuration. */
   Configuration();

   /** Copy Constructor.
    *  This performs a shallow copy - this & cfg contain the same
    *  instances of ConfigElement.
    */
   Configuration(const Configuration& cfg);

   /** Destructor.
    *  Note that since we use ConfigElementPtrs for memory management,
    *  the elements contained in self are only deleted if there are no
    *  other references to them.
    */
   ~Configuration();

   /** Returns the filename that this configuration was last loaded from.
    *  @return A string filename, or the empty string if load() was
    *          never called.
    */
   const std::string& getFileName() const;

   /** Sets the filename associated with this configuration to fname. */
   void setFileName(const std::string& fname);

   /**
    * Finds an element with a given name.
    *
    * @param name The name of an instance of jccl::ConfigElement.
    *
    * @return A jccl::ConfigElementPtr reference to a jccl::ConfigElement in
    *         self whose name matches the argument, or a
    *         jccl::ConfigElementPtr referring to NULL if no such exists.
    */
   ConfigElementPtr get(const std::string& name) const;

   /** Returns all jccl::ConfigElement objects of a given type.
    *  @param typeName The token of a config definition.
    *  @param elements Vector that will return with only the elements found of
    *                  the given type.
    */
   void getByType(const std::string& typeName,
                  std::vector<ConfigElementPtr>& elements) const;

   /** Removes an element with the given name.
    *  @param name Name of an instance of jccl::ConfigElement.
    *  @return True if a matching jccl::ConfigElement was found and removed,
    *          false otherwise.
    */
   bool remove(const std::string& name);

   /**
    * Adds the given element to our list of elements.
    *
    * @param new_element ConfigElement to add.
    * @note If we already have a ConfigElement with the same name, replace it.
    *
    * @since 1.3.1
    */
   void add(ConfigElementPtr newElement);

   /* IO functions: */

   /** Write contents of self to out. */
   friend JCCL_API std::ostream& operator<<(std::ostream& out,
                                            const Configuration& self);

   /** Reads contents of self from in.
    *  Note that the previous contents of self are not removed (although
    *  a jccl::ConfigElement in self can be replaced by a newly-read element
    *  with the same name).
    *  @param in An istream.
    *  @param self A Configuration.
    *  @return in.
    */
   friend JCCL_API std::istream& operator>>(std::istream& in,
                                            Configuration& self);

   /**
    * Loads ConfigElements from the given file.
    *
    * @param fname      Name of the file to load.
    * @param parentfile Name of the "parent" file.  This is used for
    *                   loading included.  If filename is a relative path,
    *                   the path part of parentfile is prepended to it.
    *
    * @return true if the file was found and no fatal errors occurred;
    *         false if the file was not found or there was a fatal
    *         error while parsing.
    */
   bool load(const std::string& fname, const std::string& parentfile = "");

   /**
    * Writes ConfigElements to the given file.
    * This uses ConfigIO to handle writing, and uses the "default"
    * ConfigFile output format (currently XML).
    *
    * @param fname The name of file to create and write.
    *
    * @return true if succesful, false if unable to create the file.
    */
   bool save(const std::string& fname) const;

   /**
    * Returns a reference to the internal collection of element pointers.
    * Use with care!
    */
   std::vector<jccl::ConfigElementPtr>& vec();

public:
   void setDefinitionPath(cppdom::NodePtr definitionPathNode);

   void setConfigurationNode(cppdom::NodePtr cfgNode);

   void extendDefinitionPath(cppdom::NodePtr definitionPathNode);

   bool loadFromElementNode(cppdom::NodePtr elementsNode,
                            const std::string& currentFile = "<unknown>");

   void createConfigurationNode(cppdom::NodePtr& cfgNode) const;

protected:
   /**
    * The name of the file that this list of elements was loaded from.  Used
    * for locating included files.
    */
   std::string mFileName;

   /** The definitions path associatted with this configuration. */
   std::vector<std::string> mDefsPath;

   /** All the configuration elements contained in this configuration. */
   std::vector<ConfigElementPtr> mElements;
private:
   cppdom::NodePtr mConfigurationNode;
};

} // End of jccl namespace

#endif
