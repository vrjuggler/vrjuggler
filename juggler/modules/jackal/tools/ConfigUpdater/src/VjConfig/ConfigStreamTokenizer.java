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



package VjConfig;

import java.io.IOException;
import java.io.Reader;
import java.io.StreamTokenizer;


/* The ConfigStreamTokenizer configures itself as follows:
 *
 * - quote marks are used as string delimiters
 * - '#'-style shell comments are recognized & ignored
 * - c-style comments are recognized & ignored
 * - c++ style comments (//) are ignored.
 *
 * The main difference with a regular StreamTokenizer is that 
 * nextToken now raises an IOException when an eof token is 
 * encountered - we don't ever expect to see one of those.
 * moreover, since numbers are not recognized & eol's aren't 
 * considered significant, that means we can always assume 
 * that if nextToken returns
 * without throwing an exception, then we read a string token.
 */

public class ConfigStreamTokenizer extends StreamTokenizer {


    public ConfigStreamTokenizer(Reader r) {
	super(r);

	resetSyntax();
	eolIsSignificant(false);
	whitespaceChars (' ',' ');
	whitespaceChars ('\t','\t');
	whitespaceChars ('\n','\n');
	wordChars('*','@'); // includes numbers
	wordChars('a','z');
	wordChars('A','Z');
	wordChars('/','/');
	wordChars('_','_');
	ordinaryChar('/');
	ordinaryChar('*');
	commentChar('#');
	quoteChar('"');
	slashSlashComments(true);
	slashStarComments(true);
	
    }



    public int nextToken() throws IOException {

	int retval;
	/* There seems to be a bug in Windows JDK causing us
	 * to get spurious carriage returns. This causes us
	 * to ignore them.
	 */
	do {
	    retval = super.nextToken();
	} while (retval == 13);


	/*
	  System.err.println ("ConfigStreamTokenizer - read token:");
	  switch (retval) {
	  case TT_WORD:
	  case '"':
	  System.err.println ("  Word: '" + sval + "'");
	  break;
	  case TT_NUMBER:
	  System.err.println ("  Number");
	  break;
	  case TT_EOL:
	  System.err.println ("  EOL");
	  break;
	  case TT_EOF:
	  System.err.println ("  EOF");
	  break;
	  default:
	  System.err.println ("  Hex " + Integer.toString(retval,16));
	  }
	*/
	
	if (retval == TT_EOF)
	    throw (new IOException());
	else
	    return retval;
    }
    
}
