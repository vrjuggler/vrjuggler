/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _JCCL_CHUNKDESCDB_H_
#define _JCCL_CHUNKDESCDB_H_

#include <jccl/jcclConfig.h>

#include <string>
#include <cppdom/cppdom.h>
/*
#include <vpr/vpr.h>
#include VPR_HASH_MAP_INCLUDE
*/
#include <map>

#include <jccl/Config/ChunkDescPtr.h>

namespace jccl
{

/** List of chunk deskcs
 *  All ChunkDescs contained in a DB are guaranteed to have a
 *  unique token.  If a ChunkDesc with an already used token is
 *  added to a ChunkDescDB, it replaces the previous ChunkDesc.
 *
 * Based off a map to guarantee unique indexing and to increase
 * lookup speed.  Also, allows more general methods for merging
 * and iterating through the db.
 *
 *  XML rep:
 *  <ChunkDescDB>
 *    <ChunkDesc ...>...</ChunkDesc> (one for each chunk desc)
 *  </ChunkDescDB>
 */
class JCCL_CLASS_API ChunkDescDB
{
public:

   /** Constructor.  Creates an empty ChunkDescDB. */
   ChunkDescDB ()
   {;}

   /** Destructor.  Doesn't really do anything. */
   ~ChunkDescDB ()
   {;}

   /** Get the chunk desc for the given token
   * @returns NULL ptr if not found, else a shared ptr to the desc
   */
   ChunkDescPtr get(const std::string descToken);

   /** Writes self to out
   * @post out is appended with the xml rep of ChunkDescDB
   */
   friend std::ostream& operator << (std::ostream& out, const ChunkDescDB& self);

   /** Reads self in from out
   * @pre  in must be at the start of the CheckDescDB xml element token
   * @post Clears db and fills with the contents of in
   */
   friend std::istream& operator >> (std::istream& in, ChunkDescDB& self);

   /** Loads a chunkdesc file
   * @param fname - name of file to load
   * @post File is opened and ChunkDescs are read and inserted
   *      into self (using >>).
   * @returns true - if file was opened succesfully
   * @returns false - otherwise
   * @returns Return value only deals with opening the file, and
   *      true doesn't neccessarily mean no parsing errors
   *      occurred.
   */
   bool load (const std::string& filename, const std::string& parentfile = "");

   /** Saves a chunkdesc file
   * ARGS: fname - name of file to load
   * POST: File is opened and ChunkDescs are written to it
   *      using << operator.
   * @returns true - if file was opened succesfully
   * @returns false - otherwise
   */
   bool save (const std::string& filename);

   /**
    * Returns a reference to the internal collection of chunk descriptions.
    * Use with care!
    */
   std::map<std::string, ChunkDescPtr>& map()
   {
      return mDescs;
   }

protected:
   /** Load the chunks from a given "ChunkDescDB" element into the db
   * @post All ChunkDesc's that are children of chunkDescDBNode are loaded
   */
   bool loadFromChunkDescDBNode(cppdom::NodePtr chunkDescDBNode);

   /** Creates a chunkDescDB element node and puts all the contained descs under it
   * @param chunkDescDBNode - returns with the new node that contains all the children desc
   * This function is used to create the root descdb node for writing out dbs
   */
   void createChunkDescDBNode(cppdom::NodePtr& chunkDescDBNode) const;

   std::map<std::string, ChunkDescPtr> mDescs;
};

} // End of jccl namespace

#endif
