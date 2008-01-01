<?xml version="1.0" encoding="UTF-8"?>

<!-- *********** <auto-copyright.pl BEGIN do not edit this line> *********** -->
<!--
 VR Juggler is (C) Copyright 1998-2008 by Iowa State University

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
 Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 Boston, MA 02111-1307, USA.
 -->
<!-- ************ <auto-copyright.pl END do not edit this line> ************ -->

<!--
  This stylesheet provides an upgrade path for JCCL config definition files
  using version 3.0 of the file format.  Such files can be updated to
  version 3.1 using an XSLT processor, as follows:

     xsltproc -o output.jdef jdef_3.0-3.1.xsl input.jdef

  or

     xalan -in input.jdef -xsl jdef_3.0-3.1.xsl -out output.jdef
 -->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xmlns:jdef30="http://www.vrjuggler.org/jccl/xsd/3.0/definition"
                xmlns:jdef31="http://www.vrjuggler.org/jccl/xsd/3.1/definition">
   <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

   <xsl:variable name="jdef30">http://www.vrjuggler.org/jccl/xsd/3.0/definition</xsl:variable>
   <xsl:variable name="jdef31">http://www.vrjuggler.org/jccl/xsd/3.1/definition</xsl:variable>

   <xsl:variable name="jdef_schema">
      <xsl:text>http://www.vrjuggler.org/jccl/xsd/3.1/definition</xsl:text>
   </xsl:variable>

   <xsl:variable name="jdef_schema_file">
      <xsl:text>http://www.vrjuggler.org/jccl/xsd/3.1/definition.xsd</xsl:text>
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
         This stylesheet is only for use with version 3.0 of the config
         definition file format.
      -->
      <xsl:if test="not($version = '3.0')">
         <xsl:message terminate="yes">
            <xsl:value-of select="$newline" />
            <xsl:text>Cannot determine version number or description </xsl:text>
            <xsl:text>version is not 3.0.</xsl:text>
            <xsl:value-of select="$newline" />
            <xsl:text>Version = '</xsl:text>
            <xsl:value-of select="$version" />
            <xsl:text>'</xsl:text>
            <xsl:value-of select="$newline" />
         </xsl:message>
      </xsl:if>

      <!-- Add the new version information. -->
      <xsl:processing-instruction name="org-vrjuggler-jccl-settings">definition.version="3.1"</xsl:processing-instruction>
      <xsl:value-of select="$newline" />

      <xsl:apply-templates />
   </xsl:template>

   <xsl:template match="jdef30:definition">
      <xsl:element name="definition">
         <xsl:attribute name="xmlns">
            <xsl:value-of select="$jdef31"/>
         </xsl:attribute>

         <xsl:attribute name="name">
            <xsl:value-of select="@name" />
         </xsl:attribute>
         <xsl:attribute name="icon_path">
            <xsl:value-of select="@icon_path" />
         </xsl:attribute>
         <xsl:attribute name="xsi:schemaLocation"
                        namespace="http://www.w3.org/2001/XMLSchema-instance">
            <xsl:value-of select="$jdef_schema" />
            <xsl:text> </xsl:text>
            <xsl:value-of select="$jdef_schema_file" />
         </xsl:attribute>

         <xsl:apply-templates />
      </xsl:element>
   </xsl:template>

   <xsl:template match="jdef30:definition_version">
      <xsl:element name="definition_version">
         <xsl:attribute name="version">
            <xsl:value-of select="@version" />
         </xsl:attribute>
         <xsl:attribute name="label">
            <xsl:value-of select="@label" />
         </xsl:attribute>
         <xsl:element name="abstract">
            <xsl:text>false</xsl:text>
         </xsl:element>
         <xsl:apply-templates />
      </xsl:element>
   </xsl:template>

   <xsl:template match="jdef30:help">
      <xsl:element name="help">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <xsl:template match="jdef30:parent">
      <xsl:element name="parent">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <xsl:template match="jdef30:category">
      <xsl:element name="category">
         <xsl:value-of select="." />
      </xsl:element>
   </xsl:template>

   <xsl:template match="jdef30:upgrade_transform">
      <xsl:element name="upgrade_transform">
         <xsl:value-of select="$newline" />
         <xsl:copy-of select="./*" />
         <xsl:value-of select="$newline" />
      </xsl:element>
   </xsl:template>

   <xsl:template match="jdef30:property">
      <xsl:element name="property">
         <xsl:attribute name="valuetype">
            <xsl:value-of select="@valuetype" />
         </xsl:attribute>
         <xsl:attribute name="variable">
            <xsl:value-of select="@variable" />
         </xsl:attribute>
         <xsl:attribute name="name">
            <xsl:value-of select="@name" />
         </xsl:attribute>
         <xsl:value-of select="$newline"/>

         <xsl:element name="help">
            <xsl:value-of select="./jdef30:help" />
         </xsl:element>
         <xsl:value-of select="$newline"/>

         <xsl:for-each select="./jdef30:value">
            <xsl:element name="value">
               <xsl:attribute name="label">
                  <xsl:value-of select="@label" />
               </xsl:attribute>

               <xsl:if test="count(@defaultvalue) = 1">
                  <xsl:attribute name="defaultvalue">
                     <xsl:value-of select="@defaultvalue" />
                  </xsl:attribute>
               </xsl:if>
            </xsl:element>
            <xsl:value-of select="$newline"/>
         </xsl:for-each>

         <xsl:if test="count(./jdef30:enum) &gt; 0">
            <xsl:element name="enumeration">
               <xsl:attribute name="editable">
                  <xsl:text>false</xsl:text>
               </xsl:attribute>
               <xsl:value-of select="$newline"/>
               <xsl:for-each select="./jdef30:enum">
                  <xsl:element name="enum">
                     <xsl:attribute name="label">
                        <xsl:value-of select="@label" />
                     </xsl:attribute>
                     <xsl:attribute name="value">
                        <xsl:value-of select="@value" />
                     </xsl:attribute>
                  </xsl:element>
                  <xsl:value-of select="$newline"/>
               </xsl:for-each>
            </xsl:element>
            <xsl:value-of select="$newline"/>
         </xsl:if>

         <xsl:for-each select="./jdef30:allowed_type">
            <xsl:element name="allowed_type">
               <xsl:value-of select="." />
            </xsl:element>
            <xsl:value-of select="$newline"/>
         </xsl:for-each>
      </xsl:element>
   </xsl:template>

   <!-- Get the definition file format version. -->
   <xsl:template name="file-version">
      <xsl:call-template name="processing-instruction-attribute">
         <xsl:with-param name="pi">
            <xsl:value-of select="processing-instruction('org-vrjuggler-jccl-settings')" />
         </xsl:with-param>
         <xsl:with-param name="name">definition.version</xsl:with-param>
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
</xsl:stylesheet>

