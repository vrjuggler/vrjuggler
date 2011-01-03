<?xml version="1.0" encoding="UTF-8" ?>

<!-- *********** <auto-copyright.pl BEGIN do not edit this line> *********** -->
<!--
 VR Juggler is (C) Copyright 1998-2011 by Iowa State University

 Original Authors:
   Allen Bierbaum, Christopher Just,
   Patrick Hartling, Kevin Meinert,
   Carolina Cruz-Neira, Albert Baker

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public
 License along with this library; if not, write to the
 Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
 -->
<!-- ************ <auto-copyright.pl END do not edit this line> ************ -->

<!--
  This stylesheet provides an upgrade path for JCCL chunk description
  files using version 2.3 of the file format.  Such files can be updated to
  version 3.0 using an XSLT processor, as follows:

     xsltproc -o junk desc_2.3-3.0.xsl input.desc

  or

     xalan -in input.desc -xsl desc_2.3-3.0.xsl -out junk

  New files with the extension .jdef will be created in a subdirectory of
  the current working directory named 'definitions'.
 -->

<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xmlns:exsl="http://exslt.org/common"
                extension-element-prefixes="exsl">
   <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" />

   <!--
      Define an optional parameter to specify the output directory for the
      resulting multitude of configuration definition files.
   -->
   <xsl:param name="outputdir" select="'definitions'" />


   <!-- The namespace for JCCL config definitions. -->
   <xsl:variable name="definition_namespace">
      <xsl:text>http://www.vrjuggler.org/jccl/xsd/3.0/definition</xsl:text>
   </xsl:variable>

   <!-- The schema for JCCL config definitions. -->
   <xsl:variable name="definition_schema">
      <xsl:text>http://www.vrjuggler.org/jccl/xsd/3.0/definition.xsd</xsl:text>
   </xsl:variable>

   <!-- Define a handy way to insert newlines when necessary. -->
   <xsl:variable name="newline">
<xsl:text>
</xsl:text>
   </xsl:variable>

   <xsl:template match="/">
      <xsl:variable name="version">
         <xsl:value-of select="processing-instruction('org-vrjuggler-jccl-settings')" />
      </xsl:variable>

      <!--
         This stylesheet is only for use with version 2.3 of the chunk
         description file format.
      -->
      <xsl:if test="not(contains($version, 'desc.db.version=&quot;2.3&quot;'))">
         <xsl:message terminate="yes">
            <xsl:value-of select="$newline" />
            <xsl:text>Cannot determine version number or description </xsl:text>
            <xsl:text>version is not 2.3.</xsl:text>
            <xsl:value-of select="$newline" />
            <xsl:text>Version = '</xsl:text>
            <xsl:value-of select="$version" />
            <xsl:text>'</xsl:text>
            <xsl:value-of select="$newline" />
         </xsl:message>
      </xsl:if>

      <!--
         Process each chunk description and output the appropriate configuration
         definition file for each chunk description.
      -->
      <xsl:for-each select="ChunkDescDB/ChunkDesc">
         <xsl:apply-templates select="." />
      </xsl:for-each>

      <xsl:message terminate="yes">
         <xsl:text>New definition files (.jdef files) are in the </xsl:text>
         <xsl:text>'definitions' subdirectory.</xsl:text>
      </xsl:message>
   </xsl:template>

   <!-- Match chunk descriptions. -->
   <xsl:template match="ChunkDesc">
      <xsl:variable name="name"><xsl:value-of select="@name" /></xsl:variable>
      <xsl:variable name="token"><xsl:value-of select="@token" /></xsl:variable>
      <xsl:variable name="outputfile"><xsl:value-of select="concat($outputdir, '/', $token, '.jdef')" /></xsl:variable>

      <!--
         Convert the version decimal to an integer. What we do here is assume
         that the user only incremented the decimal part of the version (which
         is zero-based). So we take the decimal and add one so that the version
         is one-based.
      -->
      <xsl:variable name="version">
         <xsl:choose>
            <!--
               Default to version 1 if no version attribute exists.
               Techincally, this shouldn't be needed because version 2.3 of
               the file format requires that a version attribute exist.
               However, we'll include this check just to be safe.
            -->
            <xsl:when test="string-length(@version) = 0">
               <xsl:text>1</xsl:text>
            </xsl:when>
            <xsl:otherwise>
               <xsl:variable name="orig_version"><xsl:value-of select="number(@version)" /></xsl:variable>
               <xsl:variable name="decimal"><xsl:value-of select="$orig_version - floor($orig_version)" /></xsl:variable>
               <xsl:variable name="remainder">
                  <xsl:value-of select="substring(format-number($decimal, '#.0'), 2)" />
               </xsl:variable>
               <xsl:value-of select="number($remainder) + 1" />
            </xsl:otherwise>
         </xsl:choose>
      </xsl:variable>

      <!-- Start a new document. -->
      <exsl:document href="{$outputfile}" method="xml" encoding="UTF-8" indent="yes">
         <xsl:message terminate="no">
            <xsl:text>Creating </xsl:text><xsl:value-of select="$outputfile"/>
         </xsl:message>

         <!-- Add the new version information. -->
         <xsl:processing-instruction name="org-vrjuggler-jccl-settings">definition.version="3.0"</xsl:processing-instruction>
         <xsl:value-of select="$newline" />

         <!-- Create the new XML tree. -->
         <xsl:element name="definition" namespace="http://www.vrjuggler.org/jccl/xsd/3.0/definition">
            <xsl:attribute name="xmlns:xsi">
               <xsl:text>http://www.w3.org/2001/XMLSchema-instance</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="xsi:schemaLocation">
               <xsl:value-of select="$definition_namespace" />
               <xsl:text> </xsl:text>
               <xsl:value-of select="$definition_schema" />
            </xsl:attribute>
            <xsl:attribute name="name"><xsl:value-of select="$token" /></xsl:attribute>
            <xsl:value-of select="$newline" />

            <!-- Create version of this definition loosely based off its current version. -->
            <xsl:element name="definition_version">
               <xsl:attribute name="version"><xsl:value-of select="$version" /></xsl:attribute>
               <xsl:attribute name="label"><xsl:value-of select="$name" /></xsl:attribute>
               <xsl:value-of select="$newline" />

               <!-- Copy the help across. -->
               <xsl:element name="help">
                  <xsl:value-of select="help" />
               </xsl:element>
               <xsl:value-of select="$newline" />

               <!-- Make an empty parent node. -->
               <xsl:element name="parent" />
               <xsl:value-of select="$newline" />

               <!-- Copy the categories, but prefix the categories with a '/' -->
               <xsl:for-each select="category">
                  <xsl:element name="category">
                     <xsl:text>/</xsl:text>
                     <xsl:value-of select="." />
                  </xsl:element>
                  <xsl:value-of select="$newline" />
               </xsl:for-each>

               <!-- Process the property descriptions. -->
               <xsl:apply-templates select="PropertyDesc" />

               <!-- Add an empty upgrade_transform node. -->
               <xsl:element name="upgrade_transform" />
               <xsl:value-of select="$newline" />
            </xsl:element>
            <xsl:value-of select="$newline" />

         </xsl:element>
      </exsl:document>
   </xsl:template>

   <xsl:template match="PropertyDesc">
      <!-- Figure out the type. -->
      <xsl:variable name="valuetype">
         <xsl:choose>
            <xsl:when test="@type = 'bool'"><xsl:text>boolean</xsl:text></xsl:when>
            <xsl:when test="@type = 'int'"><xsl:text>integer</xsl:text></xsl:when>
            <xsl:when test="@type = 'float'"><xsl:text>float</xsl:text></xsl:when>
            <xsl:when test="@type = 'string'"><xsl:text>string</xsl:text></xsl:when>
            <xsl:when test="@type = 'chunk'"><xsl:text>configelementpointer</xsl:text></xsl:when>
            <xsl:when test="@type = 'embeddedchunk'"><xsl:text>configelement</xsl:text></xsl:when>
         </xsl:choose>
      </xsl:variable>

      <!-- Output a property node. -->
      <xsl:element name="property">
         <xsl:attribute name="valuetype">
            <xsl:value-of select="$valuetype" />
         </xsl:attribute>
         <xsl:attribute name="variable">
            <xsl:choose>
               <xsl:when test="@variable = '0' or @variable = 'false'"><xsl:text>false</xsl:text></xsl:when>
               <xsl:when test="@variable = '1' or @variable = 'true'"><xsl:text>true</xsl:text></xsl:when>
               <xsl:otherwise><xsl:text>false</xsl:text></xsl:otherwise>
            </xsl:choose>
         </xsl:attribute>
         <xsl:attribute name="name">
            <xsl:value-of select="@token" />
         </xsl:attribute>
         <xsl:value-of select="$newline" />

         <!-- Property help. -->
         <xsl:element name="help">
            <xsl:value-of select="help" />
         </xsl:element>
         <xsl:value-of select="$newline" />

         <!-- Convert items to values. -->
         <xsl:for-each select="item">
            <xsl:element name="value">
               <xsl:attribute name="label"><xsl:value-of select="@label" /></xsl:attribute>
               <!--
                  A default value attribute is only applicable when the type is
                  not 'configelement' or 'configelementpointer'.
               -->
               <xsl:if test="not($valuetype = 'configelement' or $valuetype = 'configelementpointer')">
                  <xsl:attribute name="defaultvalue">
                     <!-- Make sure booleans get converted to strings if necessary. -->
                     <xsl:choose>
                        <xsl:when test="$valuetype = 'boolean'">
                           <xsl:choose>
                              <!-- 'false' and '0' are false. -->
                              <xsl:when test="@defaultvalue = 'false' or @defaultvalue = '0'">
                                 <xsl:text>false</xsl:text>
                              </xsl:when>
                              <!-- Everything else is true. -->
                              <xsl:otherwise><xsl:text>true</xsl:text></xsl:otherwise>
                           </xsl:choose>
                        </xsl:when>
                        <!-- If not boolean, straight conversion. -->
                        <xsl:otherwise>
                           <xsl:value-of select="@defaultvalue" />
                        </xsl:otherwise>
                     </xsl:choose>
                  </xsl:attribute>
               </xsl:if>
            </xsl:element>
            <xsl:value-of select="$newline" />
         </xsl:for-each>

         <!-- Copy over the allowed types. -->
         <xsl:for-each select="allowedType">
            <xsl:element name="allowed_type">
               <xsl:value-of select="." />
            </xsl:element>
            <xsl:value-of select="$newline" />
         </xsl:for-each>

         <!-- Copy over enumerations. -->
         <xsl:for-each select="enumeration">
            <!-- Enumerations are not valid for embedded elements or element pointers. -->
            <xsl:if test="not($valuetype = 'configelement' or $valuetype = 'configelementpointer')">
               <xsl:element name="enum">
                  <xsl:attribute name="label"><xsl:value-of select="@name" /></xsl:attribute>
                  <xsl:attribute name="value"><xsl:value-of select="@value" /></xsl:attribute>
               </xsl:element>
               <xsl:value-of select="$newline" />
            </xsl:if>
         </xsl:for-each>

      </xsl:element>
      <xsl:value-of select="$newline" />
   </xsl:template>

</xsl:stylesheet>
