/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include <vpr/vpr.h>
#include <vpr/IO/Port/SerialPort.h>
#include <vpr/IO/Port/SerialTypes.h>




int main (int argc, char* argv[])
{
   vpr::SerialPort* port;

   port = new vpr::SerialPort(argv[1]);

   port->setOpenReadWrite();
   port->setBlocking(false);
   try
   {
      port->open();
      unsigned char read_buffer[27];
      //int val;

      std::cerr << "Port opened\n";
      //port->setUpdateAction(vpr::SerialTypes::NOW);
      //port->flushQueue(vpr::SerialTypes::IO_QUEUES);
      port->clearAll();
      port->setRead(true);
      port->setMinInputSize(1);
      //port->setCanonicalInput(false);
      //port->setLocalAttach(true);
      //port->setBreakByteIgnore(true);

      port->setOutputBaudRate(115200); // Put me before input to be safe
      port->setInputBaudRate(115200);
      port->setCharacterSize(vpr::SerialTypes::CS_BITS_8);

      //port->setHardwareFlowControl(false);
      //port->setParityGeneration(false);
      //port->setInputParityCheck(false);
      //port->setStartStopInput(false);
      //port->setStopBits(1);
      std::cerr << "reading\n";

      const char *hex = "0123456789abcdef";

      unsigned int how_many = 0;
      //port->flushQueue(vpr::SerialTypes::IO_QUEUES);
      unsigned int min_val[14];
      unsigned int max_val[14];

      for ( int i = 0; i > -1; i++ )
      {
         std::vector<vpr::Uint8> sm_rd;
         unsigned int how_many_start = 0;
         unsigned int how_many_end = 0;
         unsigned int bytes_read;
         bytes_read = port->read(sm_rd, 1);
         vpr::Uint8 switcher;

         std::vector<unsigned int> glove_rec;

         for(unsigned int j = 0; j < bytes_read; ++j)
         {
            //switcher = ((sm_rd[j] >> 4) & 0x0f) | ((sm_rd[j] << 4) & 0xf0);
            switcher = sm_rd[j];
               //std::cout
               //   << hex[((switcher & 0xf0) >> 4) & 0x0f ]
               //   << hex[((switcher & 0x0f) >> 0) & 0x0f ]  << " " << std::endl;
            while(switcher != 0x3c)
            {
               bytes_read = port->read(sm_rd, 1);
               switcher = sm_rd[j];
               //std::cout
               //   << hex[((switcher & 0xf0) >> 4) & 0x0f ]
               //   << hex[((switcher & 0x0f) >> 0) & 0x0f ]  << " ";
            }
#if 1
            if(switcher == 0x3c)
            {
               //std::cout << float(sm_rd) / 255.0 << " ";
               //port->read(&sm_rd, 1);
               how_many_start++;
               //std::cout << "<";
               //std::cout << std::endl;
               how_many = 0;
               //std::cout
               //   << hex[((switcher & 0xf0) >> 4) & 0x0f ]
               //   << hex[((switcher & 0x0f) >> 0) & 0x0f ] ;
               glove_rec.push_back(  ( ((switcher & 0xf0) >> 4) & 0x0f ) );
               glove_rec.push_back(  ( ((switcher & 0x0f) >> 0) & 0x0f ) );

               //std::cout << int( ((switcher & 0xf0) >> 4) & 0x0f )  << " "
               //          << int( ((switcher & 0x0f) >> 0) & 0x0f ) << std::endl;

               bytes_read = 0;
               while( bytes_read < 28 )
               {
                  unsigned int bytes_this_read = 0;
                  bytes_this_read = port->read(sm_rd, 28 - bytes_read);
                  bytes_read+=bytes_this_read;
                  for( unsigned int kk=0; kk < bytes_this_read; ++kk)
                  {
                     //switcher = ((sm_rd[kk] >> 4) & 0x0f) | ((sm_rd[kk] << 4) & 0xf0);
                     switcher = sm_rd[kk]; // >> 4) & 0x0f) | ((sm_rd[kk] << 4) & 0xf0);
                     glove_rec.push_back( (((switcher & 0xf0) >> 4) & 0x0f ));
                     glove_rec.push_back( (((switcher & 0x0f) >> 0) & 0x0f ));

                     //std::cout << " "
                     //   << int(((switcher & 0xf0) >> 4) & 0x0f);
                     //   << hex[((switcher & 0x0f) >> 0) & 0x0f ] ;
                   }
               }
               //std::cout << int(glove_rec[6]) << std::endl;


               unsigned int sens_num = 12;
               unsigned int offset = 7;
               unsigned int reading = 0;

               //( ((glove_rec[sens_num + offset] & 0x0f) >> 4) & 0x0f ) |
               //std::cout << int ( ( glove_rec[sens_num + offset] << 8) & 0x0f00) << " "
               //          << int ( ( glove_rec[sens_num + offset + 1] << 4) & 0x00f0) << " "
               //          << int ( ( glove_rec[sens_num + offset + 2] << 0) & 0x000f) << std::endl;
                          //( ((glove_rec[sens_num + offset + 1] & 0x0f) >> 4) & 0x0f );// |
                          //( ((glove_rec[sens_num + offset + 2] & 0x0f) >> 4) & 0x0f ) ;
               reading =     ( ( ( (glove_rec[sens_num + offset] & 0x000f) << 8) & 0x0f00) |
                               ( ( (glove_rec[sens_num + offset + 1] & 0x000f) << 4) & 0x00f0) |
                               ( ( (glove_rec[sens_num + offset + 2] & 0x000f) << 0) & 0x000f) );

               for(unsigned int foo = 0; foo < glove_rec.size(); foo++)
               {
                  if( /* (foo <= 5 && foo % 2 == 0) || */
                      (foo >  5 && foo % 3 == 0 && foo < 48) /*||
                      (foo >= 54 && foo % 2 == 0)*/
                    )
                     std::cout << "[";

                  if(foo >  5 && foo % 3 == 0 && foo < 48)
                  {
                     reading=( ( ( (glove_rec[foo] & 0x000f) << 8) & 0x0f00) |
                               ( ( (glove_rec[foo + 1] & 0x000f) << 4) & 0x00f0) |
                               ( ( (glove_rec[foo + 2] & 0x000f) << 0) & 0x000f) );
                     unsigned int entry = (foo - 6) / 3;
                     if(i == 0)
                     {
                        min_val[entry] = reading;
                        max_val[entry] = reading;
                     }
                     else
                     {
                        if( reading > max_val[entry] )
                        {
                           max_val[entry] = reading;
                        }
                        if( reading < min_val[entry] )
                        {
                           min_val[entry] = reading;
                        }
                     }
                     float top = (reading - min_val[entry]);
                     float bottom = (max_val[entry] - min_val[entry]);
                     float new_reading = (top / bottom);

                     //std::cout << reading;
                     std::cout << entry << "] " << std::fixed << std::setprecision(2) << new_reading << " ";
                  }
                  else if( foo <= 5 || foo >=54 )
                  {
                     //std::cout << hex[((glove_rec[foo] & 0x000f) >> 0) & 0x000f ] ;
                  }

                  //if( /* (foo <= 5 && foo % 2 == 1) || */
                    //  (foo > 5 && foo % 3 == 2 && foo < 51) /* ||
                     // (foo >= 54 && foo % 2 == 1)*/
                   // )
                   //  std::cout << "] ";

                  /*
                  if( foo == 5 )
                  {
                     std::cout << "   ";
                  }
                  if( foo == 53 )
                  {
                     std::cout << "   ";
                  }
                  */
               }
               std::cout << std::endl;
               //std::cout << "  (" << sens_num << " : " << reading << ")"  << std::endl;
               bytes_read = 0;
               glove_rec.clear();
            }
            //port->flushQueue(vpr::SerialTypes::INPUT_QUEUE);


#endif
#if 0
            how_many++;

            //std::cout << std::setbase(16) << sm_rd[j] << " ";;

            //port->read(&sm_rd,1);
            if(sm_rd[j] == 0x3e)
            {
               //std::cout << float(sm_rd) / 255.0 << " ";
               //port->read(&sm_rd, 1);
               //std::cout << ">";
               how_many_end++;
               std::cout << std::endl;
               how_many = 0;
            }

            if (how_many > 28 )
            {
               std::cout << std::endl;
               how_many = 0;
            }
#endif

         }
         //std::cout << std::endl;
         //std::cout << bytes_read << "  < " << how_many_start << "  >" << how_many_end << std::endl;
         //port->flushQueue(vpr::SerialTypes::INPUT_QUEUE);


#if 0
         std::cout << "... Got it! :" << std::setbase(16) << read_buffer[0] << std::endl;

         memset((void*) &read_buffer, '\0', sizeof(read_buffer));
         port->read(read_buffer, sizeof(read_buffer));

         std::cout << "Type: " << std::setbase(16) << read_buffer[0];
         std::cout << "     Version: " << float(read_buffer[1]) << std::endl;
         for( unsigned int i = 2; i < 25; ++i )
         {
            std::cout << float(read_buffer[i]) << " ";
         }
         std::cout << std::endl;
         std::cout << "Checksum: " << int(read_buffer[25]);
         std::cout << " " << std::setbase(16) << read_buffer[26] << std::endl;

         //val = atoi(read_buffer);
         //val++;
#endif
      }
      //std::cout << std::endl;
   }
   catch(...)
   {
      std::cout << "Serial Port Failed" << std::endl;
   }

   return 0;
}
