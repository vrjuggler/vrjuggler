
#include <iostream>
//#include <assert.h>

#include <syn/AudioFormat.h>

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace synTest
{

   class convertTest : public CppUnit::TestCase
   {
   public:
      convertTest() 
      {
      }

      convertTest( std::string name ) : CppUnit::TestCase( name )
      {
      }

      virtual ~convertTest()
      {
      }
      
      void testme()
      {
         // make sure that the compiler knows that char is signed...
         CPPUNIT_ASSERT( syn::audio_data_traits<signed char>::min() < 0 && "your compiler is not recognizing signed char as having negative values" );
         CPPUNIT_ASSERT( syn::audio_data_traits<signed short>::min() < 0 && "your compiler is not recognizing signed char as having negative values" );
         CPPUNIT_ASSERT( syn::audio_data_traits<signed char>::min() == -128 );
         CPPUNIT_ASSERT( syn::audio_data_traits<signed short>::min() == -32768 );


         unsigned char a = 255, temp_c;
         unsigned short b = 65535, temp_s;
         signed short sb, temp_ss;
         int x;
         signed char test1 = 127, ct1;
         unsigned short test2 = 65535, ust2;
         float fff;

         // quick extents tests...

         // s8 to us16
         test1 = 127;
         syn::audio_convert<signed char, unsigned short>( test1, ust2 );
         //std::cout<<"signed char ("<<(int)test1<<") to unsigned short ("<<(int)ust2<<")\n"<<std::flush;
         CPPUNIT_ASSERT( ust2 == 65280 );

         test1 = 0;
         syn::audio_convert<signed char, unsigned short>( test1, ust2 );
         //std::cout<<"signed char ("<<(int)test1<<") to unsigned short ("<<(int)ust2<<")\n"<<std::flush;
         CPPUNIT_ASSERT( ust2 == 32768 );

         test1 = -128;
         syn::audio_convert<signed char, unsigned short>( test1, ust2 );
         //std::cout<<"signed char ("<<(int)test1<<") to unsigned short ("<<(int)ust2<<")\n"<<std::flush;
         CPPUNIT_ASSERT( ust2 == 0 );


         // u8 to s16
         a = 255;
         syn::audio_convert<unsigned char, signed short>( a, temp_ss );
         //std::cout<<"unsigned char ("<<(int)a<<") to signed short ("<<(int)temp_ss<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_ss == 32512 );

         a = 128;
         syn::audio_convert<unsigned char, signed short>( a, temp_ss );
         //std::cout<<"unsigned char ("<<(int)a<<") to signed short ("<<(int)temp_ss<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_ss == 0 );

         a = 0;
         syn::audio_convert<unsigned char, signed short>( a, temp_ss );
         //std::cout<<"unsigned char ("<<(int)a<<") to signed short ("<<(int)temp_ss<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_ss == -32768 );

         // u8 to u16
         a = 255;
         syn::audio_convert<unsigned char, unsigned short>( a, temp_s );
         //std::cout<<"unsigned char ("<<(int)a<<") to unsigned short ("<<(int)temp_s<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_s == 65280 );

         a = 128;
         syn::audio_convert<unsigned char, unsigned short>( a, temp_s );
         //std::cout<<"unsigned char ("<<(int)a<<") to unsigned short ("<<(int)temp_s<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_s == 32768 );

         a = 0;
         syn::audio_convert<unsigned char, unsigned short>( a, temp_s );
         //std::cout<<"unsigned char ("<<(int)a<<") to unsigned short ("<<(int)temp_s<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_s == 0 );


         // u16 to s8
         test2 = 65535;
         syn::audio_convert<unsigned short, signed char>( test2, ct1 );
         //std::cout<<"unsigned short ("<<(int)test2<<") to signed char ("<<(int)ct1<<")\n"<<std::flush;
         CPPUNIT_ASSERT( ct1 == 127 );

         test2 = 32768;
         syn::audio_convert<unsigned short, signed char>( test2, ct1 );
         //std::cout<<"unsigned short ("<<(int)test2<<") to signed char ("<<(int)ct1<<")\n"<<std::flush;
         CPPUNIT_ASSERT( ct1 == 0 );

         test2 = 0;
         signed char tc;
         syn::audio_convert<unsigned short, signed char>( test2, tc );
         //std::cout<<"unsigned short ("<<(int)test2<<") to signed char ("<<(int)tc<<")\n"<<std::flush;
         CPPUNIT_ASSERT( tc == -128 );

         // u16 to u8
         b = 65535;
         syn::audio_convert<unsigned short, unsigned char>( b, temp_c );
         //std::cout<<"unsigned short ("<<(int)b<<") to unsigned char ("<<(int)temp_c<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_c == 255 );

         b = 32768;
         syn::audio_convert<unsigned short, unsigned char>( b, temp_c );
         //std::cout<<"unsigned short ("<<(int)b<<") to unsigned char ("<<(int)temp_c<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_c == 128 );

         b = 0;
         syn::audio_convert<unsigned short, unsigned char>( b, temp_c );
         //std::cout<<"unsigned short ("<<(int)b<<") to unsigned char ("<<(int)temp_c<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_c == 0 );


         // s16 to us8
         sb = syn::audio_data_traits<signed short>::max();
         syn::audio_convert<signed short, unsigned char>( sb, temp_c );
         //std::cout<<"signed short ("<<(int)sb<<") to unsigned char ("<<(int)temp_c<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_c == 255 );

         sb = 0;
         syn::audio_convert<signed short, unsigned char>( sb, temp_c );
         //std::cout<<"signed short ("<<(int)sb<<") to unsigned char ("<<(int)temp_c<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_c == 128 );

         sb = syn::audio_data_traits<signed short>::min();
         syn::audio_convert<signed short, unsigned char>( sb, temp_c );
         //std::cout<<"signed short ("<<(int)sb<<") to unsigned char ("<<(int)temp_c<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_c == 0 );


         // f to s16
         float f = 1;
         syn::audio_convert<float, signed short>( f, temp_ss );
         //std::cout<<"float ("<<(float)f<<") to signed short ("<<(int)temp_ss<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_ss == 32767 );

         f = 0;
         syn::audio_convert<float, signed short>( f, temp_ss );
         //std::cout<<"float ("<<(float)f<<") to signed short ("<<(int)temp_ss<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_ss == 0 );

         f = -1;
         syn::audio_convert<float, signed short>( f, temp_ss );
         //std::cout<<"float ("<<(float)f<<") to signed short ("<<(int)temp_ss<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_ss == -32768 );


         // f to s8
         f = 1;
         syn::audio_convert<float, signed char>( f, test1 );
         //std::cout<<"float ("<<(float)f<<") to signed char ("<<(int)test1<<")\n"<<std::flush;
         CPPUNIT_ASSERT( test1 == 127 );

         f = 0;
         syn::audio_convert<float, signed char>( f, test1 );
         //std::cout<<"float ("<<(float)f<<") to signed char ("<<(int)test1<<")\n"<<std::flush;
         CPPUNIT_ASSERT( test1 == 0 );

         f = -1;
         syn::audio_convert<float, signed char>( f, test1 );
         //std::cout<<"float ("<<(float)f<<") to signed char ("<<(int)test1<<")\n"<<std::flush;
         CPPUNIT_ASSERT( test1 == -128 );


         // f to u16
         f = 1;
         syn::audio_convert<float, unsigned short>( f, temp_s );
         //std::cout<<"float ("<<(float)f<<") to unsigned short ("<<(int)temp_s<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_s == 65535 );

         f = 0;
         syn::audio_convert<float, unsigned short>( f, temp_s );
         //std::cout<<"float ("<<(float)f<<") to unsigned short ("<<(int)temp_s<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_s == 32768 );

         f = -1;
         syn::audio_convert<float, unsigned short>( f, temp_s );
         //std::cout<<"float ("<<(float)f<<") to unsigned short ("<<(int)temp_s<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_s == 0 );


         // f to us8
         f = 1;
         syn::audio_convert<float, unsigned char>( f, temp_c );
         //std::cout<<"float ("<<(float)f<<") to unsigned char ("<<(int)temp_c<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_c == 255 );

         f = 0.0f;
         syn::audio_convert<float, unsigned char>( f, temp_c );
         //std::cout<<"float ("<<(float)f<<") to unsigned char ("<<(int)temp_c<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_c == 128 );

         f = -1.0f;
         syn::audio_convert<float, unsigned char>( f, temp_c );
         //std::cout<<"float ("<<(float)f<<") to unsigned char ("<<(int)temp_c<<")\n"<<std::flush;
         CPPUNIT_ASSERT( temp_c == 0 );


         // u8 to f
         temp_c = 255;
         syn::audio_convert<unsigned char, float>( temp_c, f );
         //std::cout<<"unsigned char ("<<(int)temp_c<<") to float ("<<(float)f<<")\n"<<std::flush;
         CPPUNIT_ASSERT( f <= 0.993188f && f >= 0.992187f );

         temp_c = 128;
         syn::audio_convert<unsigned char, float>( temp_c, f );
         //std::cout<<"unsigned char ("<<(int)temp_c<<") to float ("<<(float)f<<")\n"<<std::flush;
         CPPUNIT_ASSERT( f == 0.0f );

         temp_c = 0;
         syn::audio_convert<unsigned char, float>( temp_c, f );
         //std::cout<<"unsigned char ("<<(int)temp_c<<") to float ("<<(float)f<<")\n"<<std::flush;
         CPPUNIT_ASSERT( f == -1.0f );

         // s8 to f
         {
            signed char i = 127;
            float o;
            syn::audio_convert<signed char, float>( i, o );
            //std::cout<<"signed char ("<<(int)i<<") to float ("<<o<<") \n"<<std::flush;
            CPPUNIT_ASSERT( o <= 0.993188f && o >= 0.992187f );


            i = -127;
            syn::audio_convert<signed char, float>( i, o );
            //std::cout<<"signed char ("<<(int)i<<") to float ("<<o<<") \n"<<std::flush;
            CPPUNIT_ASSERT( o <= -0.988f );
         }

         // u16 to f
         {
            unsigned short i = 65535;
            float o;
            syn::audio_convert<unsigned short, float>( i, o );
            //std::cout<<"unsigned short ("<<(int)i<<") to float ("<<o<<") \n"<<std::flush;
            CPPUNIT_ASSERT( o <= 0.999970f && o >= 0.999968f );

            i = 0;
            syn::audio_convert<unsigned short, float>( i, o );
            //std::cout<<"unsigned short ("<<(int)i<<") to float ("<<o<<") \n"<<std::flush;
            CPPUNIT_ASSERT( o <= -0.999989f && o >= -1.000010f );
         }


         // spanning tests...

         // u8 to s16 to u8, u8's should match before and after...
         //std::cout<<"\n"<<std::flush;
         for (x = 0; x <= 255; ++x)
         {
            a = (signed char)x;
            syn::audio_convert<unsigned char, signed short>( a, sb );
            unsigned char aa;
            syn::audio_convert<signed short, unsigned char>( sb, aa );
            //std::cout<<"unsigned char ("<<(int)a<<") to signed short ("<<(int)sb<<") back to unsigned char ("<<(int)aa<<")\n"<<std::flush;
            CPPUNIT_ASSERT( a == aa );
         }

         // s8 to u16 to s8, s8's should match before and after...
         //std::cout<<"\n"<<std::flush;
         for (x = -128; x <= 127; ++x)
         {
            signed char sa = (signed char)x;
            syn::audio_convert<signed char, unsigned short>( sa, b );
            signed char aa;
            syn::audio_convert<unsigned short, signed char>( b, aa );
            //std::cout<<"signed char ("<<(int)sa<<") to unsigned short ("<<(int)b<<") back to signed char ("<<(int)aa<<")\n"<<std::flush;
            CPPUNIT_ASSERT( sa == aa );
         }


         // u8 to u16 to u8, u8's should match before and after...
         //std::cout<<"\n"<<std::flush;
         for (x = 0; x <= 255; ++x)
         {
            a = (signed char)x;
            syn::audio_convert<unsigned char, unsigned short>( a, temp_s );
            unsigned char aa;
            syn::audio_convert<unsigned short, unsigned char>( temp_s, aa );
            //std::cout<<"unsigned char ("<<(int)a<<") to unsigned short ("<<(int)temp_s<<") back to unsigned char ("<<(int)aa<<")\n"<<std::flush;
            CPPUNIT_ASSERT( a == aa );
         }

         // s8 to s16 to s8, s8's should match before and after...
         //std::cout<<"\n"<<std::flush;
         for (x = -128; x <= 127; ++x)
         {
            signed char sa = (signed char)x;
            syn::audio_convert<signed char, signed short>( sa, temp_ss );
            signed char aa;
            syn::audio_convert<signed short, signed char>( temp_ss, aa );
            //std::cout<<"signed char ("<<(int)sa<<") to signed short ("<<(int)temp_ss<<") back to signed char ("<<(int)aa<<")\n"<<std::flush;
            CPPUNIT_ASSERT( sa == aa );
         }

         // s8 to s8 to s8, all s8's should match
         //std::cout<<"\n"<<std::flush;
         for (x = -128; x <= 127; ++x)
         {
            signed char sa = (signed char)x, sc;
            syn::audio_convert<signed char, signed char>( sa, sc );
            signed char aa;
            syn::audio_convert<signed char, signed char>( sc, aa );
            ////std::cout<<"signed char ("<<(int)sa<<") to signed char ("<<(int)sc<<") back to signed char ("<<(int)aa<<")\n"<<std::flush;
            CPPUNIT_ASSERT( sa == aa && sa == sc && "s8 to s8 to s8, all s8's should match" );
         }

         // u16 to s16 to u16, u16's should match before and after...
         //std::cout<<"\n"<<std::flush;
         for (x = 0; x <= 65535; ++x)
         {
            unsigned short sa = (unsigned short)x;
            signed short sc;
            syn::audio_convert<unsigned short, signed short>( sa, sc );
            unsigned short aa;
            syn::audio_convert<signed short, unsigned short>( sc, aa );
            ////std::cout<<"unsigned short ("<<(int)sa<<") to signed short ("<<(int)sc<<") back to unsigned short ("<<(int)aa<<")\n"<<std::flush;
            CPPUNIT_ASSERT( sa == aa && "u16 to s16 to u16, u16's should match before and after" );
         }

         // s16 to u16 to s16, s16's should match before and after...
         //std::cout<<"\n"<<std::flush;
         for (x = -32768; x <= 32767; ++x)
         {
            signed short sa = (signed short)x;
            unsigned short sc;
            syn::audio_convert<signed short, unsigned short>( sa, sc );
            signed short aa;
            syn::audio_convert<unsigned short, signed short>( sc, aa );
            ////std::cout<<"signed short ("<<(int)sa<<") to unsigned short ("<<(int)sc<<") back to signed short ("<<(int)aa<<")\n"<<std::flush;
            CPPUNIT_ASSERT( sa == aa && "s16 to u16 to s16, s16's should match before and after" );
         }

         // s8 to f to s8, s8's should match before and after...
         //std::cout<<"\n"<<std::flush;
         for (x = -128; x <= 127; ++x)
         {
            float bok;
            signed char sa = (signed char)x;
            syn::audio_convert<signed char, float>( sa, bok );
            signed char aa;
            syn::audio_convert<float, signed char>( bok, aa );
            //std::cout<<"signed char ("<<(int)sa<<") to float ("<<bok<<") back to signed char ("<<(int)aa<<")\n"<<std::flush;
            CPPUNIT_ASSERT( sa == aa );
         }

         // f to s8 to f, f's should match before and after...
         //std::cout<<"\n"<<std::flush;
         for (fff = -1.0; fff <= 1.0f; fff += 0.01f)
         {
            unsigned short bok;
            syn::audio_convert<float, unsigned short>( fff, bok );
            float aa;
            syn::audio_convert<unsigned short, float>( bok, aa );
            //std::cout<<"float ("<<(float)fff<<") to unsigned short ("<<bok<<") back to float ("<<(float)aa<<")\n"<<std::flush;
            CPPUNIT_ASSERT( fff >= (aa - 0.0001f) && fff <= (aa + 0.0001f) );
         }

         // f to f to f, all f's should match
         //std::cout<<"\n"<<std::flush;
         for (fff = -1.0; fff <= 1.0f; fff += 0.01f)
         {
            float bok;
            syn::audio_convert<float, float>( fff, bok );
            float aa;
            syn::audio_convert<float, float>( bok, aa );
            ////std::cout<<"float ("<<fff<<") to float ("<<bok<<") back to float ("<<aa<<")\n"<<std::flush;
            CPPUNIT_ASSERT( fff == aa && fff == bok && "f to f to f, all f's should match" );
         }


         {
            // make sure format and channel conversions work...
            std::vector<float> buf1;
            buf1.resize( 160 );
            std::vector<float> buf2;
            buf2.resize( 80 );
            // TODO...
         }
      }

      static CppUnit::Test* suite()
      {
         CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("convert tests");
         test_suite->addTest( new CppUnit::TestCaller<convertTest>("convert_test", &convertTest::testme));

         return test_suite;
      }

   protected:

   };

};
