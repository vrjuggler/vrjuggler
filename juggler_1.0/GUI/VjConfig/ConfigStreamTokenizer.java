
package VjConfig;

import java.io.IOException;
import java.io.Reader;
import java.io.StreamTokenizer;

/* I'm gonna be making a lot of these, so I might as well wrap a class
 * around it.
 */

/* The ConfigStreamTokenizer configures itself as follows:
 *
 * - quote marks are used as string delimiters
 * - '#'-style shell comments are recognized & ignored
 * - c-style comments are recognized & ignored
 * - c++ style comments (//) are ignored.
 *
 * The main difference with a regular StreamTokenizer is that nextToken
 * now raises an IOException when an eof token is encountered - we don't
 * ever expect to see one of those.
 * moreover, since numbers are not recognized & eol's aren't considered
 * significant, that means we can always assume that if nextToken returns
 * without throwing an exception, then we read a string token.
 */

public class ConfigStreamTokenizer extends StreamTokenizer {


  public ConfigStreamTokenizer(Reader r) {
    super(r);

//     resetSyntax();
//     //ordinaryChar('/');
//     eolIsSignificant(false);
//     whitespaceChars (' ',' ');
//     whitespaceChars ('\t','\t');
//     whitespaceChars ('\n','\n');
//     wordChars('*','@'); // includes numbers
//     wordChars('a','z');
//     wordChars('A','Z');
//     wordChars('/','/');
//     wordChars('_','_');
//     commentChar('#');
//     quoteChar('"');
//     slashSlashComments(true);
//     slashStarComments(true);


    resetSyntax();
    //ordinaryChar('/');
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
    int retval = super.nextToken();
if (retval == 13) {
	//System.err.println ("nexttoken recursing");
return nextToken();
}

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
