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

#include <vrj/vrjConfig.h>

#include <vrj/Util/Debug.h>
#include <vrj/Util/FileIO.h>

namespace vrj
{

std::vector<std::string> FileIO::mPaths;

void FileIO::addFilePath( const std::string& filepath )
{
   std::string demangled = replaceEnvVars( filepath );
   mPaths.push_back( demangled );
}

//: true -
bool FileIO::fileExists( const char* const name )
{
   std::string stdstring_name = name;
   std::string demangled_name = demangleFileName( stdstring_name, "" );
    FILE* file = ::fopen( demangled_name.c_str(), "r" );
    if (file == NULL)
    {
        return false;
    }

    else
    {
        ::fclose( file );
        return true;
    }
}

bool FileIO::fileExists( const std::string& name )
{
   return fileExists( name.c_str() );
}

bool FileIO::fileExistsResolvePath( const char* const filename, std::string& realPath )
{
   realPath = resolvePathForName( filename );
   return fileExists( realPath.c_str() );
}

bool FileIO::fileExistsResolvePath( const std::string& filename, std::string& realPath )
{
   return fileExistsResolvePath( filename.c_str(), realPath );
}

std::string FileIO::resolvePathForName( const char* const filename )
{
   std::string stdstring_name = filename;
   std::string demangled_name = demangleFileName( stdstring_name, "" );

   for (unsigned int x = 0; x < FileIO::mPaths.size(); ++x)
   {
      std::string slash = "/";
      std::string temp  = FileIO::mPaths[x] + slash + demangled_name;

      // if this path works, then return it.
      if (fileExists( temp ))
      {
         std::cout<<"Fixed path: "<<temp.c_str()<<"\n"<<std::flush;
         return temp;
      }
   }

   // couldn't find any that matched, so just return the filename.
   //cout<<"Did not need to fix path: "<<demangled_name<<"\n"<<flush;
   return demangled_name;
}

/** filename handling routines **/

//: Returns a copy of s with all environment variable names replaced
//+ with their values.
std::string FileIO::replaceEnvVars( const std::string& s )
{
    unsigned int i, j;
    int lastpos = 0;
    std::string result = "";
    for (i = 0; i < s.length(); i++)
    {
        if (s[i] == '$')
        {
            //process an env var
            result += std::string(s, lastpos, i - lastpos);
            i++; // skip $
            if (s[i] == '{')
            {
               // now search for the closing brace...
                for (j = i; j < s.length(); j++)
                    if (s[j] == '}')
                        break;
                std::string var(s,i+1,j-i-1);
                //cout << "searching for env var '" << var.c_str() << '\'' << endl;
                std::string res = getenv( var.c_str() );
                if (res == "")
                {
                   vprDEBUG( vprDBG_ALL, 0 ) << clrOutNORM(clrYELLOW,"!!! WARNING: ENV VARIABLE NOT FOUND !!!:") << var.c_str() <<" does not exist in your environment, please set it... Your application's behaviour is now undefined!\n" << vprDEBUG_FLUSH;
                   result += var; // don't replace the var (or replace var with var...)
                }
                else
                {
                   result += res; // replace var with the found env variable.
                }
                i = j+1;
                lastpos = i;
            }
            else {
                for (j = i; j < s.length(); j++)
                    if (s[j] == '/' || s[j] == '\\')
                        break;
                std::string var(s,i,j-i);
                //cout << "searching for env var '" << var.c_str() << '\'' << endl;
                std::string res = getenv (var.c_str());
                if (res == "")
                {
                   vprDEBUG( vprDBG_ALL, 0 ) << clrOutNORM(clrYELLOW,"!!! WARNING: ENV VARIABLE NOT FOUND !!!:") << var.c_str() <<" does not exist in your environment, please set it... Your application's behaviour is now undefined!\n" << vprDEBUG_FLUSH;
                   result += var; // don't replace the var (or replace var with var...)
                }
                else
                {
                   result += res; // replace var with the found env variable.
                }
                i = j;
                lastpos = i;
            }
        }
    }
    result += std::string(s, lastpos, s.length() - lastpos);
    return result;
}



//: is n an absolute path name?
bool FileIO::isAbsolutePathName (const std::string& n)
{
#ifdef WIN32
    return ((n.length() > 0) && (n[0] == '\\'))
        || ((n.length() > 2) && (n[1] == ':') && (n[2] == '\\'));
#else
    return (n.length() > 0) && (n[0] == '/');
#endif
}



std::string FileIO::demangleFileName (const std::string& n, std::string parentfile)
{

   std::string fname = replaceEnvVars (n);

   if (!isAbsolutePathName(fname))
   {
      // it's a relative pathname... so we have to add in the path part
      // of parentfile...
      //         cout << "demangling relative pathname '" << fname.c_str() << "' with parent dir '"
      //              << parentfile.c_str() << "'\n" << endl;
      int lastslash = 0;
      for (unsigned int i = 0; i < parentfile.length(); i++)
      {
            if (parentfile[i] == '/')
                lastslash = i;
#ifdef WIN32
            if (parentfile[i] == '\\')
                lastslash = i;
#endif
      }
      if (lastslash)
      {
         std::string s(parentfile, 0, lastslash+1);
         fname = s + n;
      }
   }

   return fname;
}

};
