
//////////////////////////////////////////////////////////////////////////////
//
//                         -=     endian     =-
//
// Description: "functions to detect and deal with endian-ness."
// This code contributed from software located:
// http://www.bigfoot.com/~KevinMeinert/html/software/index.html
// -- Kevin Meinert, kevinmeinert@bigfoot.com
//////////////////////////////////////////////////////////////////////////////
#ifndef ENDIAN_FUNCS
#define ENDIAN_FUNCS

// confused about namespaces?
// to call these funcs, use kev::byteReverse() for example
namespace kev
{

   //: Swap the bytes in any data type.
   // Motorola and Intel store their bytes in reversed formats <BR>
   // ex: An SGI image is native to the SGI system,            <BR>
   // to be read on an intel machine, it's bytes need to be reversed <BR>
   // NOTE: chars aren't affected by this since it only        <BR>
   // affects the order of bytes, not bits.
   template< class Type >
   inline void  byteReverse(Type& bytes)
   {    	
	    const int size = sizeof(Type);
	    Type buf = 0;
	    int x, y;

	    //we want to index the bytes in the buffer
	    unsigned char* buffer = (unsigned char*) &buf;

	    for ( x = 0, y = size-1; 
                  x < size;
                  ++x, --y )
	    {
		   buffer[x] |= ((unsigned char*)&bytes)[y];
	    }
	    bytes = buf;
   }

   //: check the system for endianess
   inline bool littleEndian() 
   {
	   union 
	   {
		   short   val;
		   char    ch[sizeof(short)];
	   } un;

	   // initialize the memory that we'll probe
	   un.val = 256;       // 0x10

	   // If the 1 byte is in the low-order position (un.ch[1]), this is a
	   // little-endian system.  Otherwise, it is a big-endian system.
	   return un.ch[1] == 1;
   }

   //: check the system for endianess
   inline bool bigEndian()
   {
      return !littleEndian();
   }   
};
#endif
