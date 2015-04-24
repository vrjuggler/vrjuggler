# Writing Documentation Using DocBook #

The Juggler Project "guides" are books written using DocBook. DocBook is a standard method for organizing documents based on content using SGML/XML tags.  From this document source, many final versions of the document can be generated. See the [DocBook site](http://www.docbook.org) and [The DocBook Guide](http://www.docbook.org/tdg/en/) for details. They also have a valudable [mailing list archive](http://lists.oasis-open.org/archives/docbook-apps/).

Tools for rendering DocBook documents: [tar.bz2](http://www.infiscape.com/~patrick/docbook.tar.bz2) [7Z](http://www.infiscape.com/~patrick/docbook.7z) [ZIP](http://www.infiscape.com/~patrick/docbook.zip)

## Tag Use ##

As we convert our documentation to DocBook, we need to be sure to use tags consistently so that the generated output will share a common look and feel.  (This is not to imply that we want to try to do formatting in the XML files but rather that we want to ensure that transformations performed on various XML files will give results with a coherent style.)

### Top Ten Tags ###

Ideally, people who can use the following ten tags can do everything we need for writing DocBook documents.  At the very least, these should be enough to get started.

  1. [book](http://www.docbook.org/tdg/en/html/book.html): At present, most of our documents are going to be books
  1. [chapter](http://www.docbook.org/tdg/en/html/chapter.html): Books must have chapters
  1. [section](http://www.docbook.org/tdg/en/html/section.html): Chapters must have sections (do not use sect1, sect2, etc.)
  1. [para](http://www.docbook.org/tdg/en/html/para.html): Chapters, sections, and lists must have paragraphs
  1. [classname](http://www.docbook.org/tdg/en/html/classname.html): Denote class names
  1. [methodname](http://www.docbook.org/tdg/en/html/methodname.html): Denote method names
  1. [itemizedlist](http://www.docbook.org/tdg/en/html/itemizedlist.html): Bulleted lists
  1. [programlisting](http://www.docbook.org/tdg/en/html/programlisting.html): Show example code
  1. [figure](http://www.docbook.org/tdg/en/html/figure.html): Encapsulate figures
  1. [mediaobject](http://www.docbook.org/tdg/en/html/mediaobject.html): Figure images go in media objects

### Detailed Tag List ###

  * Chapters: `chapter`
  * Sections: `section`
    * Do not use `sect1`, `sect2`, etc.
  * Tables: `table`
    * For the time being, do not use `informaltable`.  FOP cannot handle them.
  * Figures: `figure`
    * Based on my experience with TeX and LaTeX, I recommend enabling the `float` attribute on figures.  By default, floating figures are disabled (set to 0).
  * Images in figures: `mediaobject`
  * Code examples: `programlisting` or `programlistingco`
    * Do not enclose these within `<para>...</para>` blocks.
    * Enabling line numbering for long program listings is probably a good idea.
    * For each `<area>` element, the specification of a value for the `label` attribute is questionable.  On the one hand, it can be used as an alternate value if the graphic is not found.  On the other hand, I am not sure that the `alt` element of `<fo:external-graphic>` is part of XSL:FO 1.0.  In particular, XEP does not like it.
    * Xalan 2 must be used for callouts to be placed into generated HTML or FO files.
    * Place the text of the program listing within the enclosing tags.  Do not use CDATA to specify the code example.  Early experiments with this proved to be a little awkward when editing the XML with Morphon.
  * Class names/object types: `classname`
  * Variable names: `varname`
  * Types: `type`
    * So far, I have been using this for non-class types such as `int`
  * Method/member function specifications: `methodsynopsis`
    * In this case, I have been using the following (using `vjPfApp::preForkInit()` as an example):

```
<methodsynopsis>
  <modifier>public</modifier>
  <type>void</type>
  <methodname>preForkInit</methodname>
  <void/>
</methodsynopsis>
```

  * This example is a little simplistic since it takes no arguments and has a `void` return type.  For multiple arguments, use `<methodparam><funcparams>...</funcparams></methodparam>` (where "..." would be the literal argument list, commas included).  For a single argument, use `<methodparam><parameter>...</parameter></methodparam>` (where "..." would be the single argument).  The generator is responsible for filling in the rest.
  * Method names: `methodname`
    * This would be used to include the namespace and containing class since the whole string makes up the method name.
  * Function names: `function`
    * This is for non-member functions.  Include the parentheses and possible man page information.  For example:

```
<function>fork(2)</function>
```

  * File/directory names: `filename`
  * Equations: `equation`, `informalequation`, and `inlineequation`
    * Equations are a little tricky.  The recommended method is to use media objects (`<mediaobject>` or `<inlinemediaobject>` appropriately).  Within these, there probably has to be an image object (until MathML is better supported?).  After that, there can be a text object (`<textobject>`) containing a phrase (`<phrase>`) with a text version of the equation.  There may be multiple text objects, and it is possible to use !TeX code in these text objects.  The following shows a possible informal equation:

```
<informalequation>
  <mediaobject>
    <imageobject>
      <imagedata fileref="equation.gif" format="GIF"/>
    </imageobject>
    <textobject>
      <phrase>V' = M * V</phrase>
    </textobject>
    <textobject role="tex">
      <phrase>V^{\prime }=M*V</phrase>
    </textobject>
  </mediaobject>
</informalequation>
```

  * Trademarked names: `trademark`. For example:

```
<trademark>CAVElib</trademark>
```

  * First use of a new term: `firstterm`
  * Notes giving further information about a subject: `note`
    * I have found that putting notes inside the `<para></para>` to which they refer gives pretty good results when generating rendered text.  Other placements are possible that may give better or similar results.
    * Do not start the note with the text "NOTE:".  The renderers fill that in appropriately.
  * Links to outside information via a URL: `ulink`
  * Preprocessor directives: `function`
    * Not sure about this one...
  * Preprocessor-defined symbols: `symbol`
    * Not sure about this one either...
  * Comments (not XML-style comments): `remark`
    * These can be used for notations in the text.  By default, the DocBook XSL stylesheets will render this, but this can be disabled.
  * Bibliographies/references: `bibliography` containing `biblioentry` elements that in turn contain `biblioset` elements
    * Using `<biblioentry><biblioset>...</biblioset></biblioentry>` (where they may be one or more `<biblioset>` blocks affords the most flexibility when rendering the bibliography. A `<biblioentry>` is a "database" of raw reference information. Not all elements from it may be used in the final rendering depending on the restrictions and needs of the style. A `<biblioset>` specifies a raw collection of related information within a bibliography entry.
    * The `<biblioset>` elements need to have their `relation` attribute set.  This is to provide the stylesheet with information about what the set contains. Known values are `journal`, `article`, and `book`. There is no fixed enumeration in the DTD, so the stylesheets probably dictate what is support and what is not.

**NOTE**: The current DocBook XSL stylesheets do not do anything fancy with bibliography entries.  The order of rendering is the same as the order in the original XML.  No elements appear to be left out when rendering regardless of the stylesheet used. This apparent failing may be corrected in future releases of DocBook's XSL files.

## General Tips ##

  * Do not use underscores in tag labels (e.g., to identify figures, tables, or sections).  SGML IDREFs do not allow that character for some reason, so it's best just to avoid using them.
  * It seems that the DocBook XSL stylesheets version 1.49 cannot handle a bibliography contained in a book `<part>...</para>`.  There may be more to this than I currently understand, however.