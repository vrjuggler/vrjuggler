<?xml version="1.0" encoding="UTF-8"?>

<!-- *********** <auto-copyright.pl BEGIN do not edit this line> *********** -->
<!--
 VR Juggler is (C) Copyright 1998-2010 by Iowa State University

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
  This stylesheet provides an upgrade path for JCCL configuration files
  using version 2.1 of the file format.  Such files can be updated to version
  3.0 using an XSLT processor, as follows:

     xsltproc -o new-file.jconf cfg_2.1-3.0.xsl old-file.config

  or

     xalan -in old-file.config -xsl cfg_2.1-3.0.xsl -out new-file.jconf

 -->

<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
   <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" />

   <!-- The schema for JCCL configurations. -->
   <xsl:variable name="configuration_schema">
      <xsl:text>http://www.vrjuggler.org/jccl/xsd/3.0/configuration</xsl:text>
   </xsl:variable>

   <!-- Define a handy way to insert newlines when necessary. -->
   <xsl:variable name="newline">
<xsl:text>
</xsl:text>
   </xsl:variable>

   <xsl:template match="/">
      <xsl:variable name="version">
         <xsl:call-template name="file-version" />
      </xsl:variable>

      <!--
         This stylesheet is only for use with version 2.1 of the configuration
         file format.
      -->
      <xsl:if test="not($version = '2.1')">
         <xsl:message terminate="yes">
            <xsl:value-of select="$newline" />
            <xsl:text>Cannot determine version number or format </xsl:text>
            <xsl:text>version is not 2.1</xsl:text>
            <xsl:value-of select="$newline" />
            <xsl:text>Version = '</xsl:text>
            <xsl:value-of select="$version" />
            <xsl:text>'</xsl:text>
            <xsl:value-of select="$newline" />
         </xsl:message>
      </xsl:if>

      <!-- Add the new version information. -->
      <xsl:processing-instruction name="org-vrjuggler-jccl-settings">configuration.version="3.0"</xsl:processing-instruction>
      <xsl:value-of select="$newline" />

      <!-- Create the new XML tree. -->
      <xsl:element name="configuration" namespace="http://www.vrjuggler.org/jccl/xsd/3.0/configuration">
         <xsl:attribute name="schemaLocation" namespace="http://www.w3.org/2001/XMLSchema-instance">
            <xsl:value-of select="$configuration_schema" />
            <xsl:text> </xsl:text>
            <xsl:value-of select="$configuration_schema" />
            <xsl:text>.xsd</xsl:text>
         </xsl:attribute>
         <xsl:attribute name="name"><xsl:text>A configuration</xsl:text></xsl:attribute>
         <xsl:value-of select="$newline" />
         
         <!-- Add an include child for each included configuration. -->
         <xsl:for-each select="/processing-instruction('org-vrjuggler-jccl-include')">
            <xsl:element name="include">
               <xsl:call-template name="processing-instruction-attribute">
                  <xsl:with-param name="pi"><xsl:value-of select="." /></xsl:with-param>
                  <xsl:with-param name="name">file</xsl:with-param>
               </xsl:call-template>
<!--               <xsl:value-of select="@file" /> -->
            </xsl:element>
            <xsl:value-of select="$newline" />
         </xsl:for-each>

         <!-- Create the definition path -->
         <xsl:element name="definition_path">
            <xsl:value-of select="$newline" />
            <!--
               Add the directory for each included chunk desc DB. Note that this
               could possibly add the same directory in multiple times but since
               that won't cause any problems, we don't care about it.
            -->
            <xsl:for-each select="/processing-instruction('org-vrjuggler-jccl-includedesc')">
               <xsl:element name="dir">
                  <xsl:variable name="file">
                     <xsl:call-template name="processing-instruction-attribute">
                        <xsl:with-param name="name">file</xsl:with-param>
                     </xsl:call-template>
                  </xsl:variable>
                  <xsl:variable name="dirindex">
                     <xsl:call-template name="string-rfind">
                        <xsl:with-param name="str"><xsl:value-of select="$file"/></xsl:with-param>
                        <xsl:with-param name="char">/</xsl:with-param>
                     </xsl:call-template>
                  </xsl:variable>
                  <xsl:choose>
                     <xsl:when test="$dirindex != 0">
                        <xsl:value-of select="substring($file, 0, $dirindex)" />
                     </xsl:when>
                     <xsl:otherwise>
                        <xsl:text>.</xsl:text>
                     </xsl:otherwise>
                  </xsl:choose>
<!--                  <xsl:value-of select="substring-before($file, '/')" /> -->
               </xsl:element>
               <xsl:value-of select="$newline" />
            </xsl:for-each>
         </xsl:element>
         <xsl:value-of select="$newline" />

         <!-- Put all the configuration elements as children of an elements node. -->
         <xsl:element name="elements">
            <xsl:value-of select="$newline" />
            <xsl:for-each select="ConfigChunkDB/*">
               <xsl:call-template name="update-element"/>
            </xsl:for-each>
         </xsl:element>
         <xsl:value-of select="$newline" />
      </xsl:element>

      <xsl:message>
         <xsl:text>Processing is complete. You should now use VRJConfig</xsl:text>
         <xsl:value-of select="$newline" />
         <xsl:text>to verify the results and make any necessary changes.</xsl:text>
         <xsl:value-of select="$newline" />
      </xsl:message>
   </xsl:template>

   <!--
      Update individual configuration elements, leaving most of their original
      contents intact.  The most important change is the addition of version
      information.
   -->
   <xsl:template name="update-element">
      <xsl:param name="elt" select="."/>
      <xsl:element name="{name($elt)}">
         <xsl:attribute name="name">
            <xsl:value-of select="$elt/@name"/>
         </xsl:attribute>
         <xsl:attribute name="version">
            <xsl:text>1</xsl:text>
         </xsl:attribute>
         <xsl:value-of select="$newline" />
         <!-- Iterate over all the properties of $elt. -->
         <xsl:for-each select="$elt/*">
            <xsl:choose>
               <!--
                  If we have an XML element with nested XML, that is a property
                  whose value is a nested config element.  In that case, we
                  need to call this template recursively.
               -->
               <xsl:when test="count(./*/*) > 0">
                  <xsl:element name="{name(.)}">
                     <xsl:value-of select="$newline" />
                     <xsl:call-template name="update-element">
                        <xsl:with-param name="elt" select="./*"/>
                     </xsl:call-template>
                  </xsl:element>
                  <xsl:value-of select="$newline" />
               </xsl:when>
               <!--
                  If the current XML element does not have XML children, then
                  we just copy it.
               -->
               <xsl:otherwise>
                  <xsl:copy-of select="."/>
                  <xsl:value-of select="$newline" />
               </xsl:otherwise>
            </xsl:choose>
         </xsl:for-each>
      </xsl:element>
      <xsl:value-of select="$newline" />
   </xsl:template>

   <!-- Get the configuration file format version. -->
   <xsl:template name="file-version">
      <xsl:call-template name="processing-instruction-attribute">
         <xsl:with-param name="pi">
            <xsl:value-of select="processing-instruction('org-vrjuggler-jccl-settings')" />
         </xsl:with-param>
         <xsl:with-param name="name">config.db.version</xsl:with-param>
      </xsl:call-template>
   </xsl:template>

   <!-- Gets the value of an attribute in a processing instruction. -->
   <xsl:template name="processing-instruction-attribute">
      <xsl:param name="pi" select="." />
      <xsl:param name="name" />

      <!-- Put the contents of the PI into a variable. -->
      <xsl:variable name="text">
         <xsl:value-of select="$pi"/>
      </xsl:variable>

      <xsl:if test="contains($text, concat($name, '=&quot;'))">
         <xsl:value-of select="substring-before(substring-after($text, concat($name, '=&quot;')), '&quot;')" />
      </xsl:if>
   </xsl:template>

   <!-- Finds the first index of the search character within the search string. -->
   <xsl:template name="string-find">
      <xsl:param name="str" />
      <xsl:param name="char" />
      <xsl:param name="index" select="'1'" />

      <xsl:choose>
         <!-- Empty string. Terminate. -->
         <xsl:when test="string-length($str) = 0">
            <xsl:text>0</xsl:text>
         </xsl:when>
         <!-- Found the char. Return the index. -->
         <xsl:when test="substring($str, 1, 1) = $char">
            <xsl:value-of select="$index" />
         </xsl:when>
         <!-- Current char is not a match. Recurse on string remainder. -->
         <xsl:otherwise>
            <xsl:call-template name="string-find">
               <xsl:with-param name="str"><xsl:value-of select="substring($str, 2)" /></xsl:with-param>
               <xsl:with-param name="char"><xsl:value-of select="$char" /></xsl:with-param>
               <xsl:with-param name="index"><xsl:value-of select="$index + 1" /></xsl:with-param>
            </xsl:call-template>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!-- Finds the index of the last search character within the search string. -->
   <xsl:template name="string-rfind">
      <xsl:param name="str" />
      <xsl:param name="char" />
      <xsl:param name="index" select="string-length($str)" />

      <xsl:choose>
         <!-- Empty string. Terminate. -->
         <xsl:when test="string-length($str) = 0">
            <xsl:text>0</xsl:text>
         </xsl:when>
         <!-- Found the char. Return the index. -->
         <xsl:when test="substring($str, string-length($str), 1) = $char">
            <xsl:value-of select="$index" />
         </xsl:when>
         <!-- Current char is not a match. Recurse on string remainder. -->
         <xsl:otherwise>
            <xsl:call-template name="string-rfind">
               <xsl:with-param name="str"><xsl:value-of select="substring($str, 0, string-length($str))" /></xsl:with-param>
               <xsl:with-param name="char"><xsl:value-of select="$char" /></xsl:with-param>
               <xsl:with-param name="index"><xsl:value-of select="$index - 1" /></xsl:with-param>
            </xsl:call-template>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

</xsl:stylesheet>
