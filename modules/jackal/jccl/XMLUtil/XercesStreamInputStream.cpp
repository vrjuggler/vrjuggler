/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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


#include <Utils/vjXercesStreamInputStream.h>

vjXercesStreamInputStream::vjXercesStreamInputStream (std::istream& _in, const char* _terminator): terminator (_terminator) {
    //terminator = strdup (_terminator);
    termlength = strlen (terminator);
    search_state = 0;
    finished = false;
    in = &_in;
}

/*virtual*/ vjXercesStreamInputStream::~vjXercesStreamInputStream() {
    ;
}

/*virtual*/ unsigned int vjXercesStreamInputStream::curPos() const {
    return 0;
}

/*virtual*/ unsigned int vjXercesStreamInputStream::readBytes (XMLByte* const buf, const unsigned int _buflen) {
    // we shorten buflen here so that we always know we'll have space
    // to tack on text if we were following a false lead.
    // the problem is that, even if we're real close to the end of the
    // buffer, if we see something that might be the terminator it's eas
    // ier to pursue it right here.
    unsigned int buflen = _buflen - termlength;
    unsigned int i;
    char ch;
    
    if (finished)
        return 0;
    
    for (i = 0; i < buflen;) {
        if (!in->get(ch))
            break; // it was eof
        if (ch == terminator[search_state]) {
            search_state++;
            if (search_state == termlength) {
                finished = true;
                break;
            }
        }
        else {
            if (search_state > 0) {
                // stick the false lead in the buffer
                strncpy ((char*)&buf[i], terminator, search_state);
                i += search_state;
                search_state = 0;
            }
            buf[i++] = ch;
        }
    }
    
    return i;
}
