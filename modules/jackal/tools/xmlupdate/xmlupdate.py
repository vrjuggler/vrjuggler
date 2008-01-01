#!/usr/bin/env python

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2008 by Iowa State University
#
# Original Authors:
#   Allen Bierbaum, Christopher Just,
#   Patrick Hartling, Kevin Meinert,
#   Carolina Cruz-Neira, Albert Baker
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# *************** <auto-copyright.pl END do not edit this line> ***************

# =============================================================================
# This script will convert config files that were written using the XML-based
# format between August 2001 and May 2002.  It does not convert chunk
# descriptions.
# -----------------------------------------------------------------------------
# NOTE: This script has been tested with Python 2.2 only!
# =============================================================================

import re
import sys
import xml.dom
from xml.dom.minidom import parse
from xml.sax._exceptions import SAXParseException

def handleProperty(prop_node, new_chunk):
   prop_type = prop_node.nodeName

   for prop_child in prop_node.childNodes:
      # Text child or something...
      if prop_child.nodeType == xml.dom.Node.TEXT_NODE:
         prop_val_re = re.compile('"([^"]*)"')
         prop_match  = prop_val_re.search(prop_child.data)

         # For each of the property values matching prop_val_re, we make a new
         # XML element and append a child that is the value.
         while prop_match:
            prop_val = prop_child.data[prop_match.start(1):prop_match.end(1)]

            # Translate "true" and "false" to 1 and 0 for any spelling.
            if re.search('true', prop_val, re.IGNORECASE):
               prop_val = '1'
            elif re.search('false', prop_val, re.IGNORECASE):
               prop_val = '0'

            new_prop = xml.dom.minidom.Element(prop_type)
            new_prop.appendChild(xml.dom.minidom.Text(prop_val))
            new_chunk.appendChild(new_prop)

            # Get the next property value.
            prop_match = prop_val_re.search(prop_child.data, prop_match.end())

      # Child chunk.  For this one, we create a new XML element and make it
      # the root used for handleChunk().  The chunk will be added as a child
      # of new_prop automatically.
      elif prop_child.nodeType == xml.dom.Node.ELEMENT_NODE:
         new_prop = xml.dom.minidom.Element(prop_type)
         handleChunk(prop_child, new_prop)
         new_chunk.appendChild(new_prop)

def handleChunk(node, new_tree):
   chunk_type = node.nodeName
   chunk_name = node.getAttribute('name')

   new_chunk = xml.dom.minidom.Element(chunk_type)

   name_match = re.search(r'.*->([\w\s]+)$', chunk_name)

   # If chunk_name contains a "->", the new chunk's name will just be the
   # last component of the original name.
   if name_match:
      new_chunk.setAttribute('name',
                             chunk_name[name_match.start(1):name_match.end(1)])
   # Otherwise, just copy the name in.
   else:
      new_chunk.setAttribute('name', chunk_name)

   for child in node.childNodes:
      # We only care about children that are elements nodes--they are
      # properties.
      if child.nodeType == xml.dom.Node.ELEMENT_NODE:
         handleProperty(child, new_chunk)
#      else:
#         print "Unexpected child type", child.nodeType, "for", chunk_type

   new_tree.appendChild(new_chunk)

for file in sys.argv[1:]:
   out_file = file + '.new'
   print file, "-->", out_file

   try:
      cur_dom  = parse(file)
      new_dom  = xml.dom.minidom.Document()
      dom_root = new_dom.createElement("ConfigChunkDB")

      for chunk_db in cur_dom.getElementsByTagName("ConfigChunkDB"):
         for child in chunk_db.childNodes:
            # If child is an element node, then it is a chunk.
            if child.nodeType == xml.dom.Node.ELEMENT_NODE:
               handleChunk(child, dom_root)

      new_dom.appendChild(dom_root)
      new_file = open(out_file, 'w')
      new_dom.writexml(new_file, '', '  ', "\n")
      new_file.close()
   except SAXParseException:
      print "WARNING: Skipping", file, " (failed to parse)"
