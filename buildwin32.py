#python

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2003 by Iowa State University
#
# Original Authors:
#   Allen Bierbaum, Christopher Just,
#   Patrick Hartling, Kevin Meinert,
#   Carolina Cruz-Neira, Albert Baker
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

import os
import re
import sys
import time

def setVars():
   def processInput(optionDict, envVar, inputDesc, required = False):
      default_value = optionDict[envVar]
      print "    Enter %s [%s]: " % (inputDesc, default_value),
      input_str = sys.stdin.readline().strip(" \n")

      if input_str == '':
         if required and (default_value is None or default_value == ''):
            print "ERROR: %s value required" % inputDesc
            os.exit(1)
         else:
            value_str = default_value
      else:
         value_str = input_str

      optionDict[envVar] = value_str
      os.putenv(envVar, value_str)

      return value_str

   cl_ver_match = re.compile(r'Compiler Version ((\d+)\.(\d+)\.(\d+)) for') 
   (cl_stdin, cl_stdout, cl_stderr) = os.popen3('cl')
   cl_version_line = cl_stderr.readline()

   ver_string_match = cl_ver_match.search(cl_version_line)
   if ver_string_match is not None:
      cl_major = int(ver_string_match.group(2))
      cl_minor = int(ver_string_match.group(3))

      if cl_major == 13 and cl_minor < 10:
         print "It appears that we will be using Visual Studio .NET 2002"
         boost_tool_fallback = 'vc7'
      else:
         print "It appears that we will be using Visual Studio .NET 2003"
         boost_tool_fallback = 'vc71'
   else:
      boost_tool_fallback = ''

   cl_stdin.close()
   cl_stdout.close()
   cl_stderr.close()

   options = {
      # Default values for required settings.
      'BOOST_ROOT'     : os.getenv('BOOST_ROOT', ''),
      'BOOST_VERSION'  : os.getenv('BOOST_VERSION', '1_31'),
      'BOOST_INCLUDES' : os.getenv('BOOST_INCLUDES', ''),
      'BOOST_TOOL'     : os.getenv('BOOST_TOOL', boost_tool_fallback),
      'NSPR_ROOT'      : os.getenv('NSPR_ROOT', ''),
      'CPPDOM_ROOT'    : os.getenv('CPPDOM_ROOT', ''),

      # Default values for optional settings.
      'OMNIORB_ROOT'    : os.getenv('OMNIORB_ROOT', ''),
      'PF_ROOT'         : os.getenv('PF_ROOT',
                                    r'C:\Program Files\Silicon Graphics\OpenGL Performer'),
      'VRPN_ROOT'       : os.getenv('VRPN_ROOT', ''),
      'AUDIERE_ROOT'    : os.getenv('AUDIERE_ROOT', ''),
      'TRACKD_API_ROOT' : os.getenv('TRACKD_API_ROOT', '')
   }

   # If there are cached options, read them in.
   cache_file = os.path.join(juggler_dir, 'options.cache')
   if os.path.exists(cache_file):
      execfile(cache_file)

   print "+++ Required Settings"
   boost_ver = processInput(options, 'BOOST_VERSION', 'Boost C++ version')
   boost_dir = processInput(options, 'BOOST_ROOT',
                            'Boost C++ installation directory')

   if options['BOOST_INCLUDES'] == '':
      options['BOOST_INCLUDES'] = r'%s\include\boost-%s' % \
                                     (boost_dir, boost_ver)

   processInput(options, 'BOOST_INCLUDES',
                'directory containing the Boost C++ header tree')

   processInput(options, 'BOOST_TOOL',
                'the Boost.Build toolset used to compile Boost C++')

   processInput(options, 'NSPR_ROOT', 'NSPR installation directory')
   processInput(options, 'CPPDOM_ROOT', 'CppDOM installation directory')

   print "+++ Optional Settings"
   processInput(options, 'OMNIORB_ROOT', 'omniORB installation directory',
                False)
   processInput(options, 'PF_ROOT', 'OpenGL Performer installation directory',
                False)
   processInput(options, 'VRPN_ROOT', 'VRPN installation directory', False)
   processInput(options, 'AUDIERE_ROOT', 'Audiere installation directory',
                False)
   processInput(options, 'TRACKD_API_ROOT', 'TrackdAPI installation directory',
                False)

   cache_file = open(cache_file, 'w')
   for k, v in options.iteritems():
      output = "options['%s'] = r'%s'\n" % (k, v)
      cache_file.write(output)
   cache_file.close()

def generateVersionHeaders():
   class JugglerModule:
      def __init__(self, srcDir, header, projDir, templateFile = None):
         self.source_dir     = os.path.join(juggler_dir, srcDir)
         self.header         = os.path.join(juggler_dir, 'vc7', projDir,
                                            header)
         self.version_params = os.path.join(self.source_dir, 'Makefile.inc.in')
         self.version_file   = os.path.join(self.source_dir, 'VERSION')

         if templateFile is not None:
            self.header_template = templateFile
         else:
            self.header_template = os.path.join(self.source_dir, header + '.in')

      def generateParamHeader(self):
         if os.path.exists(self.header):
            mtime = os.path.getmtime
            # This test to determine if the module's param header needs to be
            # regenerated is equivalent to that used by the UNIX build system.
            if mtime(self.version_file) > mtime(self.header) or \
               mtime(self.header_template) > mtime(self.header):
               self.__genHeader()
         else:
            self.__genHeader()

      version_re    = re.compile(r'((\d+)\.(\d+)\.(\d+)-(\d+))\s')
      branch_re     = re.compile(r'BRANCH\s*=\s*(\w+)')
      canon_name_re = re.compile(r'CANON_NAME\s*=\s*(\S.+)')
      vernum_re     = re.compile(r'@VER_NUMBER@')
      verstr_re     = re.compile(r'@VER_STRING@')
      zero_strip_re = re.compile(r'^0*([^0]\d+)')

      def __genHeader(self):
         ver_file = open(self.version_file)
         cur_ver  = ver_file.readline()
         ver_file.close()
         ver_match = self.version_re.match(cur_ver)
         version = ver_match.group(1)
         major   = int(ver_match.group(2))
         minor   = int(ver_match.group(3))
         patch   = int(ver_match.group(4))

         # NOTE: This will not always be identical to the UNIX version because
         # Python does not have %e as a time formatting directive.
         date       = time.strftime('%b %d, %Y %H:%M:%S')
         canon_name = ''
         branch     = ''

         param_file = open(self.version_params, 'r')
         params     = param_file.readlines()
         param_file.close()

         # This is basically a poor man's grep.  Can this be done better?
         for line in params:
            match = self.branch_re.match(line)
            if match is not None:
               branch_name = match.group(1)
               continue

            match = self.canon_name_re.match(line)
            if match is not None:
               canon_name = match.group(1)
               continue

         version_number = '%03d%03d%03d' % (major, minor, patch)
         version_string = "\"v%s '%s' (NSPR) %s %s\"" % \
                             (version, canon_name, branch, date)

         # Strip leading zeroes from version_number.  Is there an easier way
         # to do this?
         version_number = self.zero_strip_re.match(version_number).group(1)

         try:
            input_file  = open(self.header_template, 'r')
            input_lines = input_file.readlines()
            input_file.close()

            for i in xrange(len(input_lines)):
               line = input_lines[i]
               if self.vernum_re.search(line):
                  input_lines[i] = self.vernum_re.sub(version_number, line)
               elif self.verstr_re.search(line):
                  input_lines[i] = self.verstr_re.sub(version_string, line)

            print "Generating updated", self.header
            param_header = open(self.header, 'w')
            param_header.writelines(input_lines)
            param_header.close()
         except IOError, ex:
            print "ERROR: Could not read from %s" % self.header_template
            print ex
            print "Cannot continue; exiting with error status"
            sys.exit(2)

   mods = []
   mods.append(JugglerModule(r'modules\vapor', r'vpr\vprParam.h', 'VPR'))
   mods.append(JugglerModule(r'modules\tweek', r'tweek\tweekParam.h',
                             'Tweek_CXX'))
   mods.append(JugglerModule(r'modules\jackal', r'jccl\jcclParam.h', 'JCCL',
                             os.path.join(juggler_dir,
                                          r'modules\jackal\common\jccl\jcclParam.h.in')))
   mods.append(JugglerModule(r'modules\sonix', r'snx\snxParam.h', 'Sonix'))
   mods.append(JugglerModule(r'modules\gadgeteer', r'gadget\gadgetParam.h',
                             'Gadgeteer'))
   mods.append(JugglerModule(r'modules\vrjuggler', r'vrj\vrjParam.h',
                             'VRJuggler'))

   for m in mods:
      m.generateParamHeader()

def doInstall():
   pass

def main():
   setVars()
   generateVersionHeaders()

   # The environment variable %VSINSTALLDIR% is set by vsvars32.bat.
   # XXX: Is there a better way to get the absolute path to devenv.exe,
   # possibly from the user's path?
   devenv_cmd = r'%s' % os.path.join(os.getenv('VSINSTALLDIR', ''),
                                     'devenv.exe')
   solution_file = r'%s' % os.path.join(juggler_dir, 'vc7', 'Juggler.sln')
   status = os.spawnl(os.P_WAIT, devenv_cmd, 'devenv', solution_file)

   if status == 0:
      print "Proceed with VR Juggler installation? (y/n) ",
      proceed = sys.stdin.readline().strip(" \n")
      if proceed.lower().startswith('y'):
         doInstall()


juggler_dir = os.path.dirname(os.path.abspath(sys.argv[0]))
if __name__ == '__main__':
   main()
