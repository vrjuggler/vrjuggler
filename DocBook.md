# DocBook #

  * [DocBook website](http://www.docbook.org/)

## Guides ##

  * [DocBook Guide](http://www.docbook.org/tdg/en/)
  * [Guide to DocBook](http://www-106.ibm.com/developerworks/library/l-docbk.html)
  * [Frequently asked questions](DocBookFaq.md) that we have collected the answers to.
  * [DocBook XSL Documentation](http://docbook.sourceforge.net/release/xsl/current/doc/)
  * [DocBook XSL reference](http://docbook.sourceforge.net/release/xsl/current/doc/reference.html)

## Tools to Edit DocBook ##

  * [LyX](http://www.lyx.org/)
  * [Conglomerate](http://www.conglomerate.org/)
  * [Abiword](http://www.gnome.org/gnome-office/abiword.shtml)
  * [XML editors that support !DocBook in some way](XmlEditorWithDocBookSupport.md)
  * The DocBook XSL stylesheets must be processed by an [XSLT processor](XsltProcessors.md) to convert to HTML, XML-FO, etc.]
  * Once DocBook is converted to XML-FO, you still have to use an [XML-FO engine](XmlFoEngines.md) to generate the final view.
  * [Tips and links for customizing XSL-FO stylesheets](CustomizingXslFo.md)

### Open issues with !LyX/DocBook ###

  * [Shortcomings with DocBook support in LyX](LyxDocBookSupport.md)
  * [Five minute review](http://www.teledyn.com/help/XML/lyx2db/t1.html) by someone else about the same thing

## Tools to Generate PDF ##

  * XEP: Generates gorgeous PDFs, costs $5000
  * [Apache FOP](http://xmlgraphics.apache.org/fop/): Generates good PDFs, free
  * PassiveTeX: Requires [TeXLive](http://www.tug.org/texlive.html), does not work reliably
  * Jade with JadeTeX: Uses DSSSL (deprecated) but does a good job

## Tools to Edit XSL, XSTL, XSL:FO ##

  * [List at xmlsoftware.com](http://www.xmlsoftware.com/xsleditors/)
  * [IBM's XSL Editor](http://www.alphaworks.ibm.com/tech/xsleditor): Written in Java, but apparently only works inside a Windows-only app.  ???
  * [IBM's XSL Formatting Objects Composer](http://www.alphaworks.ibm.com/tech/xfc?open&l=766,t=grx,p=xfc): Free, Java
  * Tool for tracing XSL conversions: http://www.alphaworks.ibm.com/aw.nsf/reqs/xsltrace

## Converters to DocBook ##

  * [Logictran](http://www.logictran.com/) will convert RTF to XML DocBook and other formats (HTML, XHTML, etc.).  doc2net will convert directly from DOC to XML on the command line.
  * [html2db](http://freshmeat.net/projects/html2db/) will convert HTML files to DocBook SGML
    * Read about my harrowing experience of [using html2db](UsingHtml2DB.md).
