#ifndef ALIASMAPPER
#define ALIASMAPPER

#include <vjConfig.h>

#include <map>
#include <fstream.h>
#include <string>
#include <assert.h>

class AliasMapper
{
public:
    void loadDatabase( const char* const filename )
    {
        // clear the map.
        mAliasMapper.clear();

        char buffer[256], buffer2[256];

        ifstream dataFile;
        dataFile.open( filename, ios::in, filebuf::openprot );

        // check for the data file's header
        // it will be one string that == "SLdatafile"
        std::string temp;
        dataFile>>buffer;
        temp = buffer;
        if (temp != "AliasMapperDatabaseVer1.0.0.0")
            assert( false );

        // read from the file two strings at a time, these will be 
        // associated together in the std::map
        while (!dataFile.eof())
        {
            dataFile>>buffer;

            // check to make sure that we're not at end of file yet
            // only chance of that is if there is an odd number of pairs
            // in the .dat file.
            if (!dataFile.eof())
            {   
                dataFile>>buffer2;
                mAliasMapper[buffer] = buffer2;
            }
        }
        dataFile.close();
    }

    const char* const lookup( const char* const alias )
    {
        return mAliasMapper[alias].data();
    }

    // functor that returns true if first arg is less than second arg
    struct lessThanFunctor
    {
        bool operator()(std::string s1, std::string s2) const
        {
            return s1 < s2;
        }
    };

    std::map<std::string, std::string, lessThanFunctor> mAliasMapper;
};
/*
void main()
{
    AliasMapper am;
    am.loadDatabase( "sound.dat" );

    cout<<am.lookup("testSound")<<"\n"<<flush;
    cout<<am.lookup("kevin")<<"\n"<<flush;
    cout<<am.lookup("a")<<"\n"<<flush;
    cout<<am.lookup("b")<<"\n"<<flush;
    cout<<am.lookup("c")<<"\n"<<flush;
    cout<<am.lookup("d")<<"\n"<<flush;
    cout<<am.lookup("e")<<"\n"<<flush;
    cout<<am.lookup("f")<<"\n"<<flush;
    cout<<am.lookup("g")<<"\n"<<flush;
}
*/

#endif
