#python

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

import glob
import os
import re
import shutil
import sys
import time
import traceback
import getopt

EXIT_STATUS_SUCCESS           = 0
EXIT_STATUS_NO_MSVS           = 1
EXIT_STATUS_MISSING_DATA_FILE = 2
EXIT_STATUS_MSVS_START_ERROR  = 3
EXIT_STATUS_INVALID_PATH      = 4
EXIT_STATUS_MISSING_REQ_VALUE = 5

gJugglerDir      = os.path.dirname(os.path.abspath(sys.argv[0]))
gOptionsFileName = "options.cache"

gHaveTk = False
try:
   import Tkinter
   import tkMessageBox
   import tkFileDialog
   import threading
   gHaveTk = True
except ImportError, ex:
   print ex

class BuildOption:
   def __init__(self, envVar, desc, defaultValue, isDirectory = True,
                required = True):
      self.envVar      = envVar
      self.desc        = desc
      self.default     = defaultValue
      self.required    = required
      self.isDirectory = isDirectory

def guessBoostToolset(reattempt = False):
   (cl_stdin, cl_stdout, cl_stderr) = os.popen3('cl')
   cl_version_line = cl_stderr.readline()

   cl_ver_match = re.compile(r'Compiler Version ((\d+)\.(\d+)\.(\d+)) for')
   ver_string_match = cl_ver_match.search(cl_version_line)

   if ver_string_match is not None:
      cl_major = int(ver_string_match.group(2))
      cl_minor = int(ver_string_match.group(3))

      if cl_major == 13 and cl_minor < 10:
         vs_ver = '.NET 2002'
         boost_tool_guess = 'vc7'
      else:
         vs_ver = '.NET 2003'
         boost_tool_guess = 'vc71'

      printStatus("It appears that we will be using Visual Studio " + vs_ver)
   else:
      boost_tool_guess = ''

   in_status  = cl_stdin.close()
   out_status = cl_stdout.close()
   err_status = cl_stderr.close()

   # If there was an error closing any of the streams returned by
   # os.popen3(), then the command was not opened correctly.  That means
   # that CL.EXE is not in the user's path.
   if in_status is not None or out_status is not None or err_status is not None:
      # If this is not a reattempt to guess the Visual Studio version, then
      # be nice and extend the user's path to include their Visual Studio
      # installation.
      if not reattempt:
         # Common installation directories for Visual Studio 7.x.
         vs_dirs = [r'C:\Program Files\Microsoft Visual Studio .NET',
                    r'C:\Program Files\Microsoft Visual Studio .NET 2003']

         for d in vs_dirs:
            if os.path.exists(d):
               printStatus("NOTE: Using Visual Studio installation in")
               printStatus("      " + d)
               vs_path = [os.path.join(d, r'Common7\IDE'),
                          os.path.join(d, r'VC7\BIN'),
                          os.path.join(d, r'Common7\Tools'),
                          os.path.join(d, r'Common7\Tools\bin\prerelease'),
                          os.path.join(d, r'Common7\Tools\bin')]
               path_add = ';'.join(vs_path)
               os.environ['PATH'] = path_add + os.pathsep + os.getenv('PATH', '')

               # Try again to guess the Visual Studio version.
               return guessBoostToolset(True)

         # If execution reaches this point, our attempts to guess the
         # location of a Visual Studio 7.x installation failed.
         noVisualStudioError()

      # If this is a reattempt to guess the Visual Studio version, then
      # something is wrong with the user's Visual Studio installation.
      else:
         noVisualStudioError()

   return boost_tool_guess

def printStatus(msg):
   '''
   This is a simple wrapper around the standard Python print function.
   We will use a wrapper function for key status messages so that they
   can be redirected either to the console or to a GUI easily.
   '''
   print msg

def noVisualStudioError():
   print "ERROR: Visual Studio commands are not in your path!"
   print "Run vsvars32.bat in this shell or update the %PATH% environment"
   print "variable on your system."
   sys.exit(EXIT_STATUS_NO_MSVS)

def getCacheFileName():
   return os.path.join(gJugglerDir, gOptionsFileName)

def processInput(optionDict, envVar, inputDesc, required = False):
   default_value = optionDict[envVar]
   print "  %s [%s]: " % (inputDesc, default_value),
   input_str = sys.stdin.readline().strip(" \n")

   if input_str == '':
      if required and (default_value is None or default_value == ''):
         print "ERROR: %s value required" % inputDesc
         sys.exit(EXIT_STATUS_MISSING_REQ_VALUE)
      else:
         value_str = default_value
   else:
      value_str = input_str

   optionDict[envVar] = value_str
   os.environ[envVar] = value_str

   return value_str

def getDefaultVars():
   boost_tool_fallback = guessBoostToolset()

   required = []
   required.append(BuildOption('BOOST_ROOT',
                               'Boost C++ installation directory', ''))
   required.append(BuildOption('BOOST_VERSION', 'Boost C++ version',
                               '1_31', False))
   required.append(BuildOption('BOOST_INCLUDES',
                               'Directory containing the Boost C++ header tree',
                               ''))
   required.append(BuildOption('BOOST_TOOL',
                               'The Boost.Build toolset library name component',
                               boost_tool_fallback, False))
   required.append(BuildOption('NSPR_ROOT', 'NSPR installation directory', ''))
   required.append(BuildOption('CPPDOM_ROOT', 'CppDOM installation directory',
                               ''))
   required.append(BuildOption('GMTL_ROOT', 'GMTL installation directory', ''))

   optional = []
   optional.append(BuildOption('JAVA_HOME', 'Java installation directory',
                               r'C:\java', required = False))
   optional.append(BuildOption('JOGL_HOME', 'Jogl installation directory',
                               os.getenv('JAVA_HOME', ''), required = False))
   optional.append(BuildOption('JAVA3D_HOME', 'Java 3D installation directory',
                               os.getenv('JAVA_HOME', ''), required = False))
   optional.append(BuildOption('OMNIORB_ROOT',
                               'omniORB installation directory', '',
                               required = False))
   optional.append(BuildOption('PFROOT',
                               'OpenGL Performer installation directory',
                               r'C:\Program Files\Silicon Graphics\OpenGL Performer',
                               required = False))
   optional.append(BuildOption('MS_SPEECH_SDK_ROOT',
                               'Microsoft Speech SDK directory', '',
                               required = False))
   optional.append(BuildOption('VRPN_ROOT', 'VRPN installation directory', '',
                               required = False))
   optional.append(BuildOption('AUDIERE_ROOT',
                               'Audiere installation directory', '',
                               required = False))
   optional.append(BuildOption('OPENAL_ROOT',
                               'OpenAL SDK installation directory', '',
                               required = False))
   optional.append(BuildOption('TRACKD_API_ROOT',
                               'TrackdAPI installation directory', '',
                               required = False))

   options = {
      'prefix'      : r'C:\vrjuggler',
      'deps-prefix' : r'C:\vrjuggler-deps'
   }

   for opt in required + optional:
      options[opt.envVar] = os.getenv(opt.envVar, opt.default)

   # If there are cached options, read them in.
   cache_file = getCacheFileName()
   if os.path.exists(cache_file):
      execfile(cache_file)

   return required, optional, options

def setVars():
   required, optional, options = getDefaultVars()

   print "+++ Required Settings"
   processInput(options, 'prefix', 'Installation prefix')

   boost_dir = ''
   boost_ver = ''
   for opt in required:
      result = processInput(options, opt.envVar, opt.desc, opt.required)

      # The following is a little hack to get a reasonable default set for
      # the BOOST_INCLUDES variable before the user has to enter it manually.
      if opt.envVar == 'BOOST_ROOT':
         boost_dir = result
      elif opt.envVar == 'BOOST_VERSION':
         boost_ver = result
         options['BOOST_INCLUDES'] = boost_dir + r'\include\boost-' + boost_ver

   print "+++ Optional Settings"
   processInput(options, 'deps-prefix', 'Dependency installation prefix')

   for opt in optional:
      processInput(options, opt.envVar, opt.desc, opt.required)

   postProcessOptions(options)
   writeCacheFile(options)

   return options

def postProcessOptions(options):
   # Check for Boost 1.32 Visual C++ toolset names.
   match = re.compile(r'vc-(\d)_(\d)').match(options['BOOST_TOOL'])

   if match is not None:
      os.environ['BOOST_TOOL'] = 'vc%s%s' % (match.group(1), match.group(2))

   # If the %JAVA_HOME% setting is a valid directory, add its bin subdirectory
   # to the path.
   if os.environ['JAVA_HOME'] != '' and os.path.exists(os.environ['JAVA_HOME']):
      jdk_path = os.path.join(os.environ['JAVA_HOME'], 'bin')
      os.environ['PATH'] = jdk_path + os.pathsep + os.environ['PATH']
      os.environ['JACORB_PATH'] = os.path.join(gJugglerDir, r'external\JacORB')

   if os.environ['OMNIORB_ROOT'] != '' and os.path.exists(os.environ['OMNIORB_ROOT']):
      omni_bin = os.path.join(os.environ['OMNIORB_ROOT'], 'bin')

      if os.path.exists(os.path.join(omni_bin, 'omniidl.exe')):
         os.environ['OMNIORB_BIN'] = omni_bin
      else:
         os.environ['OMNIORB_BIN'] = os.path.join(omni_bin, 'x86_win32')

      # Extend the path to include omniORB's bin directory.
      os.environ['PATH'] = os.environ['OMNIORB_BIN'] + os.pathsep + os.environ['PATH']

      omni_lib = os.path.join(os.environ['OMNIORB_ROOT'], 'lib')

      if os.getenv('PYTHONPATH', '') != '':
         os.environ['PYTHONPATH'] = os.path.join(omni_lib, 'python') + os.pathsep + os.environ['PYTHONPATH']
      else:
         os.environ['PYTHONPATH'] = os.path.join(omni_lib, 'python')

      if os.path.exists(os.path.join(omni_lib, 'omnithread.lib')):
         os.environ['OMNIORB_LIB'] = omni_lib
      else:
         os.environ['OMNIORB_LIB'] = os.path.join(omni_lib, 'x86_win32')

      omni_glob = os.path.join(os.environ['OMNIORB_LIB'], 'omniORB*_rt.lib')
      libs = glob.glob(omni_glob)
      omni_ver_re = re.compile(r'omniORB(\d\d\d)_rt.lib')

      for l in libs:
         match = omni_ver_re.search(l)
         if match is not None:
            os.environ['OMNIORB_VERSION'] = match.group(1)
            break

def writeCacheFile(optionDict):
   cache_file = open(getCacheFileName(), 'w')
   for k, v in optionDict.iteritems():
      output = "options['%s'] = r'%s'\n" % (k, v)
      cache_file.write(output)
   cache_file.close()

def generateVersionHeaders():
   class JugglerModule:
      def __init__(self, srcDir, projDir, genFiles = None):
         self.source_dir     = os.path.join(gJugglerDir, srcDir)
         self.version_params = os.path.join(self.source_dir, 'Makefile.inc.in')
         self.version_file   = os.path.join(self.source_dir, 'VERSION')
         self.param_files    = []

         if genFiles is not None:
            for f in genFiles:
               output = os.path.join(gJugglerDir, 'vc7', projDir, f[0])

               if len(f) == 1 or f[1] is None:
                  template = os.path.join(self.source_dir, f[0] + '.in')
               else:
                  template = f[1]

               self.param_files.append((output, template))

      def generateParamFiles(self):
         for (output, template) in self.param_files:
            if os.path.exists(output):
               mtime = os.path.getmtime
               # This test to determine if the module's param header needs to
               # be regenerated is equivalent to that used by the UNIX build
               # system.
               if mtime(self.version_file) > mtime(output) or \
                  mtime(template) > mtime(output):
                  self.__genParamFile(output, template)
            else:
               self.__genParamFile(output, template)

      version_re      = re.compile(r'((\d+)\.(\d+)\.(\d+)-(\d+))\s')
      branch_re       = re.compile(r'BRANCH\s*=\s*(\w+)')
      canon_name_re   = re.compile(r'CANON_NAME\s*=\s*(\S.+)')
      vernum_re       = re.compile(r'@VER_NUMBER@')
      major_vernum_re = re.compile(r'@MAJOR_VER_NUMBER@')
      minor_vernum_re = re.compile(r'@MINOR_VER_NUMBER@')
      patch_vernum_re = re.compile(r'@PATCH_VER_NUMBER@')
      verstr_re       = re.compile(r'@VER_STRING@')
      zero_strip_re   = re.compile(r'^0*([^0]\d+)')

      def __genParamFile(self, output, template):
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
            input_file  = open(template, 'r')
            input_lines = input_file.readlines()
            input_file.close()

            for i in xrange(len(input_lines)):
               line = input_lines[i]
               if self.vernum_re.search(line):
                  input_lines[i] = self.vernum_re.sub(version_number, line)
               elif self.major_vernum_re.search(line):
                  input_lines[i] = self.major_vernum_re.sub(str(major), line)
               elif self.minor_vernum_re.search(line):
                  input_lines[i] = self.minor_vernum_re.sub(str(minor), line)
               elif self.patch_vernum_re.search(line):
                  input_lines[i] = self.patch_vernum_re.sub(str(patch), line)
               elif self.verstr_re.search(line):
                  input_lines[i] = self.verstr_re.sub(version_string, line)

            printStatus("Generating updated " + output)
            param_header = open(output, 'w')
            param_header.writelines(input_lines)
            param_header.close()
         except IOError, ex:
            printStatus("ERROR: Could not read from %s" % template)
            printStatus(ex)
            printStatus("Cannot continue; exiting with error status.")
            sys.exit(EXIT_STATUS_MISSING_DATA_FILE)

   mods = []
   mods.append(JugglerModule(r'modules\vapor', 'VPR',
                             [(r'vpr\vprParam.h',), (r'vpr\vprParam.cpp',)]))
   mods.append(JugglerModule(r'modules\tweek', 'Tweek_CXX',
                             [(r'tweek\tweekParam.h',),
                              (r'tweek\tweekParam.cpp',)]))
   mods.append(JugglerModule(r'modules\jackal', 'JCCL',
                             [(r'jccl\jcclParam.h',
                               os.path.join(gJugglerDir,
                                            r'modules\jackal\common\jccl\jcclParam.h.in')),
                              (r'jccl\jcclParam.cpp',
                               os.path.join(gJugglerDir,
                                            r'modules\jackal\common\jccl\jcclParam.cpp.in'))
                             ]))
   mods.append(JugglerModule(r'modules\sonix', 'Sonix',
                             [(r'snx\snxParam.h',), (r'snx\snxParam.cpp',)]))
   mods.append(JugglerModule(r'modules\gadgeteer', 'Gadgeteer',
                             [(r'gadget\gadgetParam.h',),
                              (r'gadget\gadgetParam.cpp',)]))
   mods.append(JugglerModule(r'modules\vrjuggler', 'VRJuggler',
                             [(r'vrj\vrjParam.h',), (r'vrj\vrjParam.cpp',)]))

   for m in mods:
      m.generateParamFiles()

def generateAntBuildFiles():
   class AntTarget:
      def __init__(self, srcdir, moduleName, outputFile = 'build.xml',
                   topSubDir = 'vc7'):
         '''
         __init__(srcdir, moduleName, outputFile, topSubDir)
         Arguments:
         srcdir     -- The location of the Java source to be compiled.
         moduleName -- The name of the Visual C++ project (under the 'vc7'
                       subdirectory) associated with this Ant build.  When
                       concatenated to 'vc7', this is where the the .class
                       file(s) and the .jar file(s) will be created.
         outputFile -- The name of the Ant build file to generate.  If not
                       specified, this defaults to 'build.xml'.
         topSubDir  -- The root of the directory where all the work will be
                       done.  This is needed for Ant builds that depend on
                       previously constructed JAR files that will most likely
                       exist somewhere in the 'vc7' directory tree.  This
                       string is used as part of the replacment for the
                       string @topdir@ in the source build.xml.in file.  If
                       not specified, this defaults to 'vc7'.
         '''
         self.srcdir      = os.path.join(gJugglerDir, srcdir)
         self.topdir      = os.path.join(gJugglerDir, topSubDir)
         self.module_name = os.path.join(gJugglerDir, 'vc7', moduleName)
         self.output_file = os.path.join(self.module_name, outputFile)

         if not os.path.exists(self.module_name):
            os.mkdir(self.module_name)
         elif not os.path.isdir(self.module_name):
            printStatus("ERROR: %s exists, but it is not a directory!" % self.module_name)
            sys.exit(EXIT_STATUS_INVALID_PATH)

      # This form of regular expressions appears to be necessary because
      # the sub() method does not handle backslashes in the replacement string
      # the way I would like.
      srcdir_re         = re.compile(r'^(.*)@srcdir@(.*)$')
      topdir_re         = re.compile(r'^(.*)@topdir@(.*)$')
      juggler_root_re   = re.compile(r'^(.*)@JUGGLERROOT_ABS@(.*)$')
      jdom_jar_re       = re.compile(r'^(.*)@JDOM_JAR@(.*)$')
      tweek_jars_re     = re.compile(r'^(.*)@TWEEK_JARS@(.*)$')
      tweek_ext_jars_re = re.compile(r'^(.*)@TWEEK_EXT_JARS@(.*)$')
      jccl_jars_re      = re.compile(r'^(.*)@JCCL_JARS@(.*)$')
      java_orb_jar_re   = re.compile(r'^(.*)@JAVA_ORB_JAR@(.*)$')
      jogl_jars_re      = re.compile(r'^(.*)@JOGL_JARS@(.*)$')
      java3d_jars_re    = re.compile(r'^(.*)@JAVA3D_JAR@(.*)$')

      jdom_jars = [
         os.path.join(gJugglerDir, r'external\jdom\build\jdom.jar'),
         os.path.join(gJugglerDir, r'external\jdom\lib\jaxen-core.jar'),
         os.path.join(gJugglerDir, r'external\jdom\lib\xalan.jar'),
         os.path.join(gJugglerDir, r'external\jdom\lib\jaxen-jdom.jar'),
         os.path.join(gJugglerDir, r'external\jdom\lib\xerces.jar'),
         os.path.join(gJugglerDir, r'external\jdom\lib\xml-apis.jar'),
         os.path.join(gJugglerDir, r'external\jdom\lib\saxpath.jar')
      ]

      tweek_jars = [
         os.path.join(gJugglerDir, r'vc7\Tweek_Java', 'Tweek.jar'),
         os.path.join(gJugglerDir, r'vc7\Tweek_Java', 'TweekBeans.jar'),
         os.path.join(gJugglerDir, r'vc7\Tweek_Java', 'TweekEvents.jar'),
         os.path.join(gJugglerDir, r'vc7\Tweek_Java', 'TweekNet.jar'),
         os.path.join(gJugglerDir, r'vc7\Tweek_Java', 'TweekBeanDelivery.jar'),
         os.path.join(gJugglerDir, r'vc7\Tweek_Java', 'TweekServices.jar'),
         os.path.join(gJugglerDir, r'vc7\Tweek_Java', 'Viewers.jar'),
         os.path.join(gJugglerDir, r'vc7\Tweek_Java', 'kunststoff-mod.jar')
      ]

      tweek_ext_jars = [
         os.path.join(gJugglerDir, r'vc7\Tweek_Java', 'ui.jar'),
         os.path.join(gJugglerDir, r'vc7\Tweek_Java', 'wizard.jar')
      ]

      jccl_jars = [
         os.path.join(gJugglerDir, r'vc7\JCCL_Java', 'jccl_config.jar'),
         os.path.join(gJugglerDir, r'vc7\JCCL_Java', 'jccl_editors.jar')
      ]

      jccl_rtrc_jars = [
         os.path.join(gJugglerDir, r'vc7\JCCL_Java\RTRC_Plugin_Java',
                      'jccl_rtrc.jar')
      ]

      jogl_jars = [
         os.path.join(os.environ['JOGL_HOME'], 'jogl.jar'),
         os.path.join(os.environ['JOGL_HOME'], 'jogl-demos-util.jar')
      ]

      java3d_jars = [
         os.path.join(os.environ['JAVA3D_HOME'], r'jre\lib\ext\j3daudio.jar'),
         os.path.join(os.environ['JAVA3D_HOME'], r'jre\lib\ext\j3dcore.jar'),
         os.path.join(os.environ['JAVA3D_HOME'], r'jre\lib\ext\j3dutils.jar'),
         os.path.join(os.environ['JAVA3D_HOME'], r'jre\lib\ext\vecmath.jar')
      ]

      def generateBuildFile(self):
         input_file = open(os.path.join(self.srcdir, 'build.xml.in'), 'r')
         input = input_file.readlines()
         input_file.close()

         for i in xrange(len(input)):
            line = input[i]

            if self.srcdir_re.search(line):
               match = self.srcdir_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], self.srcdir,
                                        match.groups()[1])
            elif self.topdir_re.search(line):
               match = self.topdir_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], self.topdir,
                                        match.groups()[1])
            elif self.juggler_root_re.search(line):
               match = self.juggler_root_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], gJugglerDir,
                                        match.groups()[1])
            elif self.java_orb_jar_re.search(line):
               match = self.java_orb_jar_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], "",
                                        match.groups()[1])
            elif self.jdom_jar_re.search(line):
               jars = os.pathsep.join(self.jdom_jars)
               match = self.jdom_jar_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], jars,
                                        match.groups()[1])
            elif self.tweek_jars_re.search(line):
               jars = os.pathsep.join(self.tweek_jars + self.jdom_jars)
               match = self.tweek_jars_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], jars,
                                        match.groups()[1])
            elif self.tweek_ext_jars_re.search(line):
               jars = os.pathsep.join(self.tweek_ext_jars)
               match = self.tweek_ext_jars_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], jars,
                                        match.groups()[1])
            elif self.jccl_jars_re.search(line):
               jars = os.pathsep.join(self.jccl_jars + self.jccl_rtrc_jars)
               match = self.jccl_jars_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], jars,
                                        match.groups()[1])
            elif self.jogl_jars_re.search(line):
               jars = os.pathsep.join(self.jogl_jars)
               match = self.jogl_jars_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], jars,
                                        match.groups()[1])
            elif self.java3d_jars_re.search(line):
               jars = os.pathsep.join(self.java3d_jars)
               match = self.java3d_jars_re.search(line)
               input[i] = '%s%s%s\n' % (match.groups()[0], jars,
                                        match.groups()[1])

         build_file = open(self.output_file, 'w')
         build_file.writelines(input)
         build_file.close()

   mods = []
   mods.append(AntTarget(r'modules\tweek\java', 'Tweek_Java'))
   mods.append(AntTarget(r'modules\tweek\extensions\java', 'Tweek_Java',
                         'build-ext.xml'))
   mods.append(AntTarget(r'modules\jackal\config', 'JCCL_Java',
                         'build-config.xml'))
   mods.append(AntTarget(r'modules\jackal\editors', 'JCCL_Java',
                         'build-editors.xml'))
   mods.append(AntTarget(r'modules\jackal\plugins\corba_rtrc',
                         r'JCCL_Java\RTRC_Plugin_Java', 'build.xml'))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig', 'VRJConfig',
                         'build.xml'))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\commoneditors',
                         r'VRJConfig\commoneditors',
                         'build-commoneditors.xml'))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\display_window',
                         'VRJConfig', 'build-display_window.xml',
                         r'vc7\VRJConfig'))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\flock',
                         'VRJConfig', 'build-flock.xml', r'vc7\VRJConfig'))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\intersense',
                         'VRJConfig', 'build-intersense.xml',
                         r'vc7\VRJConfig'))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\motionstar',
                         'VRJConfig', 'build-motionstar.xml',
                         r'vc7\VRJConfig'))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\pinchglove',
                         'VRJConfig', 'build-pinchglove.xml',
                         r'vc7\VRJConfig'))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\proxyeditor',
                         'VRJConfig', 'build-proxyeditor.xml',
                         r'vc7\VRJConfig'))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\surfacedisplayeditor',
                         'VRJConfig', 'build-surfacedisplayeditor.xml',
                         r'vc7\VRJConfig'))
   mods.append(AntTarget(r'modules\vrjuggler\plugins\corba_perf_mon',
                         r'VRJugglerPlugins\Perf_Plugin_Java', 'build.xml'))

   for m in mods:
      m.generateBuildFile()

def doInstall(prefix):
   makeTree(prefix)
   installExternal(prefix)
   installVPR(prefix)
   installTweek(prefix)
   installTweekJava(prefix)
   installJCCL(prefix)
   installJCCLJava(prefix)
   installJCCLPlugins(prefix)
   installJCCLPluginsJava(prefix)
   installSonix(prefix)
   installSonixPlugins(prefix)
   installGadgeteer(prefix)
   installGadgeteerDrivers(prefix)
   installGadgeteerPlugins(prefix)
   installVRJuggler(prefix)
   installVRJConfig(prefix)
   installVRJugglerPlugins(prefix)
   installVRJugglerPluginsJava(prefix)
   installMsvcRT(prefix)

def mkinstalldirs(dir):
#   print "Checking for", dir
   if not os.path.exists(dir):
      (head, tail) = os.path.split(dir)
      mkinstalldirs(head)
      os.mkdir(dir)

def makeTree(prefix):
   mkinstalldirs(os.path.join(prefix, 'bin'))
   mkinstalldirs(os.path.join(prefix, 'include'))
   mkinstalldirs(os.path.join(prefix, 'lib'))
   mkinstalldirs(os.path.join(prefix, 'share'))

def installDir(startDir, destDir, allowedExts = None, disallowedExts = None,
               disallowedFiles = None):
   cwd = os.getcwd()
   mkinstalldirs(destDir)

   os.chdir(startDir)
   contents = os.listdir(startDir)

   if disallowedExts is None:
      disallowedExts = []

   if disallowedFiles is None:
      disallowedFiles = []

   # Add some extensions that should always be disallowed.  This relieves the
   # caller from having to add these repeatedly.
   disallowedExts.append('.ilk')
   disallowedExts.append('.ncb')
   disallowedExts.append('.pdb')
   disallowedExts.append('.suo')

   skip_dirs = ['CVS', 'autom4te.cache']
   for f in contents:
      if os.path.isdir(f):
         if f in skip_dirs:
            continue

         start_dir = os.path.join(startDir, f)
         dest_dir  = os.path.join(destDir, f)
         installDir(start_dir, dest_dir, allowedExts, disallowedExts)
      else:
         (root, f_ext) = os.path.splitext(f)
         if allowedExts is None:
            if f_ext not in disallowedExts:
               shutil.copy2(f, destDir)
         elif f_ext in allowedExts:
            (head, tail) = os.path.split(f)
            if f not in disallowedFiles:
               shutil.copy2(f, destDir)

   os.chdir(cwd)

def installLibs(srcRoot, destdir,
                buildTypes = ['ReleaseDLL', 'DebugDLL', 'Release', 'Debug'],
                extensions = ['.dll', '.lib']):
   for t in buildTypes:
      srcdir = os.path.join(srcRoot, t)
      if os.path.exists(srcdir):
         installDir(srcdir, destdir, extensions)

def installExternal(prefix):
   # Install Doozer (even though it probably won't be used).
   printStatus("Installing Doozer ...")
   destdir = os.path.join(prefix, 'share', 'Doozer')
   srcdir  = os.path.join(gJugglerDir, 'external', 'Doozer')
   installDir(srcdir, destdir, ['.mk'])

def installVPR(prefix):
   printStatus("Installing VPR headers and libraries ...")

   destdir = os.path.join(prefix, 'include', 'vpr')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'vapor', 'vpr')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(gJugglerDir, 'vc7', 'VPR', 'vpr')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(gJugglerDir, 'vc7', 'VPR')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'share', 'vpr', 'test')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'vapor', 'test')
   installDir(srcdir, destdir, None, ['.in'])

   # Install additional files into <prefix>\share\vpr
   destdir = os.path.join(prefix, 'share', 'vpr')
   srcroot = os.path.join(gJugglerDir, 'modules', 'vapor')

   shutil.copy2(os.path.join(gJugglerDir, 'COPYING.txt'), destdir)
   extra_files = ['ChangeLog', 'README.txt', 'RELEASE_NOTES.txt']
   for f in extra_files:
      shutil.copy2(os.path.join(srcroot, f), destdir)

def installTweek(prefix):
   printStatus("Installing Tweek C++ headers, libraries, and data files ...")

   destdir = os.path.join(prefix, 'include', 'tweek')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'tweek', 'tweek')
   installDir(srcdir, destdir, ['.h', '.idl'])

   srcdir  = os.path.join(gJugglerDir, 'vc7', 'Tweek_CXX', 'tweek')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(gJugglerDir, 'vc7', 'Tweek_CXX')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'share', 'tweek', 'test')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'tweek', 'test')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'tweek', 'data')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'tweek', 'data')
   installDir(srcdir, destdir)

   # Install additional files into <prefix>\share\tweek
   destdir = os.path.join(prefix, 'share', 'tweek')
   srcroot = os.path.join(gJugglerDir, 'modules', 'tweek')

   shutil.copy2(os.path.join(gJugglerDir, 'COPYING.txt'), destdir)
   extra_files = ['ChangeLog', 'RELEASE_NOTES.txt']
   for f in extra_files:
      shutil.copy2(os.path.join(srcroot, f), destdir)

def installTweekJava(prefix):
   srcdir = os.path.join(gJugglerDir, 'vc7', 'Tweek_Java')

   if os.path.exists(os.path.join(srcdir, 'Tweek.jar')):
      printStatus("Installing Tweek Java libraries and data files ...")

      jars = [
         'Tweek.jar',
         'TweekBeanDelivery.jar',
         'TweekBeans.jar',
         'TweekEvents.jar',
         'TweekNet.jar',
         'TweekServices.jar',
         'kunststoff-mod.jar',
         'ui.jar',
         'wizard.jar'
      ]

      beans     = ['Viewers']
      ext_beans = []

      destdir = os.path.join(prefix, 'share', 'tweek', 'java')
      mkinstalldirs(destdir)

      # Install the base JAR files that make up the Tweek Java API.
      for j in jars:
         shutil.copy2(os.path.join(srcdir, j), destdir)

      # Install the tweek_jni DLL.
      dll = os.path.join(srcdir, 'tweek_jni', 'ReleaseDLL', 'tweek_jni.dll')
      arch = os.environ['PROCESSOR_ARCHITECTURE']
      destdir = os.path.join(destdir, arch)
      mkinstalldirs(destdir)
      shutil.copy2(dll, destdir)

      destdir = os.path.join(prefix, 'share', 'tweek', 'beans')
      mkinstalldirs(destdir)

      bean_srcdir = srcdir
      xml_srcdir  = os.path.join(gJugglerDir, 'modules', 'tweek', 'java')

      # Install the standard Tweek Beans.
      for b in beans:
         jar = b + '.jar'
         xml = b + '.xml'
         shutil.copy2(os.path.join(bean_srcdir, jar), destdir)
         shutil.copy2(os.path.join(xml_srcdir, xml), destdir)

      xml_srcdir  = os.path.join(gJugglerDir, 'modules', 'tweek', 'extensions',
                                 'java')

      # Install the extension Tweek Beans.
      for b in ext_beans:
         jar = b + '.jar'
         xml = b + '.xml'
         shutil.copy2(os.path.join(bean_srcdir, jar), destdir)
         shutil.copy2(os.path.join(xml_srcdir, xml), destdir)

      # Install tweek.bat.
      srcdir = os.path.join(gJugglerDir, 'modules', 'tweek', 'java')
      destdir = os.path.join(prefix, 'bin')
      shutil.copy2(os.path.join(srcdir, 'tweek.bat'), destdir)

      # Install JacORB IDL compiler.
      srcdir = os.path.join(gJugglerDir, 'external', 'JacORB')
      installDir(srcdir, destdir, ['.jar'])
      shutil.copy2(os.path.join(srcdir, 'idl.bat'), destdir)

      # Destination for all remaining .jar files.
      destdir = os.path.join(prefix, 'share', 'tweek', 'java')

      # Install JDOM.
      srcdir = os.path.join(gJugglerDir, 'external', 'jdom', 'lib')
      installDir(srcdir, destdir, ['.jar'])
      srcdir = os.path.join(gJugglerDir, 'external', 'jdom', 'build')
      installDir(srcdir, destdir, ['.jar'])

      # Install various look and feel implementations.
      laf_jars = [
         r'jgoodies-looks\looks.jar',
         r'liquid\liquidlnf.jar',
         r'metouia\metouia.jar'
      ]

      srcroot = os.path.join(gJugglerDir, 'external', 'swing-laf')
      for j in laf_jars:
         shutil.copy2(os.path.join(srcroot, j), destdir)
   else:
      printStatus("Tweek Java API not built.  Skipping.")

def installJCCL(prefix):
   printStatus("Installing JCCL C++ headers, libraries, and tools ...")

   destdir = os.path.join(prefix, 'include', 'jccl')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'jackal', 'common', 'jccl')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(gJugglerDir, 'modules', 'jackal', 'config', 'jccl')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(gJugglerDir, 'modules', 'jackal', 'rtrc', 'jccl')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(gJugglerDir, 'vc7', 'JCCL', 'jccl')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(gJugglerDir, 'vc7', 'JCCL')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'share', 'jccl', 'test')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'jackal', 'test')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'jccl', 'tools')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'jackal', 'tools')
   installDir(srcdir, destdir)

   schema_root = os.path.join(prefix, 'share', 'jccl', 'data', 'schema')

   srcdir  = os.path.join(gJugglerDir, 'modules', 'jackal', 'data')
   destdir = os.path.join(schema_root, 'www.vrjuggler.org', 'jccl', 'xsd',
                          '3.0')
   mkinstalldirs(destdir)
   shutil.copy2(os.path.join(srcdir, 'configuration.xsd'), destdir)

   destdir = os.path.join(schema_root, 'www.vrjuggler.org', 'jccl', 'xsd',
                          '3.1')
   mkinstalldirs(destdir)
   shutil.copy2(os.path.join(srcdir, 'definition.xsd'), destdir)

   destdir = schema_root
   srcdir  = os.path.join(gJugglerDir, 'modules', 'jackal', 'data',
                          'stdschemas')
   installDir(srcdir, destdir)

   # Install additional files into <prefix>\share\jccl
   destdir = os.path.join(prefix, 'share', 'jccl')
   srcroot = os.path.join(gJugglerDir, 'modules', 'jackal')

   shutil.copy2(os.path.join(gJugglerDir, 'COPYING.txt'), destdir)
   extra_files = ['ChangeLog', 'RELEASE_NOTES.txt']
   for f in extra_files:
      shutil.copy2(os.path.join(srcroot, f), destdir)

def installJCCLPlugins(prefix):
   printStatus("Installing JCCL C++ plug-ins ...")

   destdir = os.path.join(prefix, 'lib', 'jccl', 'plugins')
   srcroot = os.path.join(gJugglerDir, 'vc7', 'JCCL', 'RTRC_Plugin_CXX')
   installLibs(srcroot, destdir, extensions = ['.dll'])

def installJCCLJava(prefix):
   srcdir = os.path.join(gJugglerDir, 'vc7', 'JCCL_Java')

   if os.path.exists(os.path.join(srcdir, 'jccl_config.jar')):
      printStatus("Installing JCCL Java libraries and data files ...")

      destdir = os.path.join(prefix, 'share', 'jccl', 'beans')
      mkinstalldirs(destdir)

      jars = [
         'jccl_config.jar',
         'jccl_editors.jar'
      ]

      for j in jars:
         shutil.copy2(os.path.join(srcdir, j), destdir)

      srcdir = os.path.join(gJugglerDir, 'modules', 'jackal', 'config')
      shutil.copy2(os.path.join(srcdir, 'jccl_config.xml'), destdir)

      # Install dependencies.
      dep_jars = [
         r'TableLayout\TableLayout.jar'
      ]

      destdir = os.path.join(prefix, 'share', 'jccl', 'java')
      srcroot = os.path.join(gJugglerDir, 'external')

      mkinstalldirs(destdir)
      for j in dep_jars:
         shutil.copy2(os.path.join(srcroot, j), destdir)
   else:
      printStatus("JCCL Java API not built.  Skipping.")

def installJCCLPluginsJava(prefix):
   srcdir = os.path.join(gJugglerDir, 'vc7', 'JCCL_Java', 'RTRC_Plugin_Java')

   if os.path.exists(os.path.join(srcdir, 'jccl_rtrc.jar')):
      printStatus("Installing JCCL Java plug-ins ...")

      destdir = os.path.join(prefix, 'share', 'jccl', 'beans')
      shutil.copy2(os.path.join(srcdir, 'jccl_rtrc.jar'), destdir)

      srcdir = os.path.join(gJugglerDir, 'modules', 'jackal', 'plugins',
                            'corba_rtrc')
      shutil.copy2(os.path.join(srcdir, 'jccl_rtrc.xml'), destdir)
   else:
      printStatus("JCCL Java plug-ins not built.  Skipping.")

def installSonix(prefix):
   printStatus("Installing Sonix headers, libraries, and samples ...")

   destdir = os.path.join(prefix, 'include', 'snx')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'sonix', 'snx')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(gJugglerDir, 'vc7', 'Sonix', 'snx')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(gJugglerDir, 'vc7', 'Sonix')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'share', 'sonix', 'samples')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'sonix', 'samples')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'sonix', 'data')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'sonix', 'data')
   installDir(srcdir, destdir)

   # Install additional files into <prefix>\share\sonix
   destdir = os.path.join(prefix, 'share', 'sonix')
   srcroot = os.path.join(gJugglerDir, 'modules', 'sonix')

   shutil.copy2(os.path.join(gJugglerDir, 'COPYING.txt'), destdir)
   extra_files = ['ChangeLog', 'README.txt']
   for f in extra_files:
      shutil.copy2(os.path.join(srcroot, f), destdir)

def installSonixPlugins(prefix):
   printStatus("Installing Sonix plug-ins ...")

   destdir_dbg = os.path.join(prefix, 'lib', 'snx', 'plugins', 'dbg')
   destdir_opt = os.path.join(prefix, 'lib', 'snx', 'plugins', 'opt')

   srcroot = os.path.join(gJugglerDir, 'vc7', 'Sonix', 'OpenAL')
   installLibs(srcroot, destdir_dbg, ['DebugDLL'], ['.dll'])
   installLibs(srcroot, destdir_opt, ['ReleaseDLL'], ['.dll'])

   srcroot = os.path.join(gJugglerDir, 'vc7', 'Sonix', 'Audiere')
   installLibs(srcroot, destdir_dbg, ['DebugDLL'], ['.dll'])
   installLibs(srcroot, destdir_opt, ['ReleaseDLL'], ['.dll'])

def installGadgeteer(prefix):
   printStatus("Installing Gadgeteer headers, libraries, and samples ...")

   destdir = os.path.join(prefix, 'include', 'gadget')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'gadgeteer', 'gadget')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(gJugglerDir, 'vc7', 'Gadgeteer', 'gadget')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'include', 'cluster')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'gadgeteer', 'cluster')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(gJugglerDir, 'vc7', 'Gadgeteer')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'share', 'gadgeteer', 'data')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'gadgeteer', 'data')
   installDir(srcdir, destdir)

   destdir = os.path.join(prefix, 'share', 'gadgeteer', 'samples')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'gadgeteer', 'samples')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'gadgeteer', 'test')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'gadgeteer', 'test')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'gadgeteer', 'tools')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'gadgeteer', 'tools')
   installDir(srcdir, destdir, None, ['.in'])

   # Install additional files into <prefix>\share\gadgeteer
   destdir = os.path.join(prefix, 'share', 'gadgeteer')
   srcroot = os.path.join(gJugglerDir, 'modules', 'gadgeteer')

   shutil.copy2(os.path.join(gJugglerDir, 'COPYING.txt'), destdir)
   extra_files = ['ChangeLog', 'RELEASE_NOTES.txt']
   for f in extra_files:
      shutil.copy2(os.path.join(srcroot, f), destdir)

def installGadgeteerDrivers(prefix):
   printStatus("Installing Gadgeteer device drivers ...")

   destdir = os.path.join(prefix, 'lib', 'gadgeteer', 'drivers')
   srcroot = os.path.join(gJugglerDir, 'vc7', 'Gadgeteer')

   drivers = ['DataGlove', 'DirectXJoystick', 'Fastrak', 'Flock', 'IBox',
              'IntersenseAPI', 'IS900', 'MotionStar', 'MSFTSpeechRecognition',
              'PinchGlove', 'SerialEncoder', 'SpaceBall', 'TrackdAPI', 'VRPN']

   for d in drivers:
      srcdir = os.path.join(srcroot, d)
      installLibs(srcdir, destdir, extensions = ['.dll'])

def installGadgeteerPlugins(prefix):
   printStatus("Installing Gadgeteer cluster plug-ins ...")

   destdir = os.path.join(prefix, 'include', 'plugins',
                          'ApplicationDataManager')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'gadgeteer', 'plugins',
                          'ApplicationDataManager')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib', 'gadgeteer', 'plugins')
   srcroot = os.path.join(gJugglerDir, 'vc7', 'Gadgeteer')

   plugins = ['ApplicationDataManager', 'RemoteInputManager',
              'StartBarrierPlugin', 'SwapLockTCPPlugin']

   for p in plugins:
      srcdir = os.path.join(srcroot, p)
      installLibs(srcdir, destdir, extensions = ['.dll'])

def installVRJuggler(prefix):
   printStatus("Installing VR Juggler headers, libraries, and samples ...")

   destdir = os.path.join(prefix, 'include', 'vrj')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'vrjuggler', 'vrj')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(gJugglerDir, 'vc7', 'VRJuggler', 'vrj')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(gJugglerDir, 'vc7', 'VRJuggler')
   installLibs(srcroot, destdir)

   srcroot = os.path.join(gJugglerDir, 'vc7', 'VRJuggler',
                          'OpenGL_Draw_Manager')
   installLibs(srcroot, destdir)

   srcroot = os.path.join(gJugglerDir, 'vc7', 'VRJuggler',
                          'Performer_Draw_Manager')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'share', 'vrjuggler', 'data')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'vrjuggler', 'data')
   installDir(srcdir, destdir)

   destdir = os.path.join(prefix, 'share', 'vrjuggler', 'samples')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'vrjuggler', 'samples')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'vrjuggler', 'test')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'vrjuggler', 'test')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'vrjuggler', 'tools')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'vrjuggler', 'tools')
   installDir(srcdir, destdir, None, ['.in'])

   # Install additional files into <prefix>\share\vrjuggler
   destdir = os.path.join(prefix, 'share', 'vrjuggler')
   srcroot = os.path.join(gJugglerDir, 'modules', 'vrjuggler')

   shutil.copy2(os.path.join(gJugglerDir, 'COPYING.txt'), destdir)
   extra_files = ['ChangeLog', 'RELEASE_NOTES.txt']
   for f in extra_files:
      shutil.copy2(os.path.join(srcroot, f), destdir)

def installVRJConfig(prefix):
   jardir = os.path.join(gJugglerDir, 'vc7', 'VRJConfig')

   if os.path.exists(os.path.join(jardir, 'VRJConfig.jar')):
      printStatus("Installing VRJConfig ...")

      # XXX: Enumerating these JAR files is really a pain.  These lists should
      # be constructed using a glob.
      lib_jars = [
         'ClusterWizard.jar',
         'Devices.jar',
         'NewDevice.jar',
         'Simulator.jar'
      ]

      vrjconfig_src = os.path.join(gJugglerDir, 'modules', 'vrjuggler',
                                   'vrjconfig')
      bean_jars = [
         'VRJConfig.jar'
      ]

      common_editors = glob.glob(os.path.join(jardir, 'commoneditors', '*.jar'))

      custom_editor_src = os.path.join(vrjconfig_src, 'customeditors')
      custom_editors = [
         ('display_window', 'DisplayWindowEditor'),
         ('flock', 'FlockEditor'),
         ('intersense', 'IntersenseEditor'),
         ('motionstar', 'MotionStarEditor'),
         ('pinchglove', 'PinchGloveEditor'),
         ('proxyeditor', 'ProxyEditor'),
         ('surfacedisplayeditor', 'SurfaceDisplayEditor')
      ]

      # Install JAR files that act as reusable Java class libraries.
      destdir = os.path.join(prefix, 'share', 'vrjuggler', 'java')
      mkinstalldirs(destdir)
      for j in lib_jars + common_editors:
         jar_file = os.path.join(jardir, j)
         if os.path.exists(jar_file):
            shutil.copy2(jar_file, destdir)

      # Install the base set of VRJConfig JavaBeans.
      destdir = os.path.join(prefix, 'share', 'vrjuggler', 'beans')
      mkinstalldirs(destdir)
      for j in bean_jars:
         jar_file = os.path.join(jardir, j)
         if os.path.exists(jar_file):
            shutil.copy2(jar_file, destdir)

      shutil.copy2(os.path.join(vrjconfig_src, 'VRJConfig.xml'), destdir)

      # Install any custom editors that were compiled.
      destdir = os.path.join(prefix, 'share', 'vrjuggler', 'beans',
                             'customeditors')
      mkinstalldirs(destdir)
      for e in custom_editors:
         jar_file = os.path.join(jardir, e[1] + '.jar')
         xml_file = os.path.join(custom_editor_src, e[0], e[1] + '.xml')
         if os.path.exists(jar_file):
            shutil.copy2(xml_file, destdir)
            shutil.copy2(jar_file, destdir)

      # Install vrjconfig.bat.
      destdir = os.path.join(prefix, 'bin')
      shutil.copy2(os.path.join(vrjconfig_src, 'vrjconfig.bat'), destdir)

      # Install dependencies.
      dep_jars = [
         r'jgraph\lib\jgraph.jar',
         r'jgraph\lib\jgraphaddons.jar',
      ]

      srcroot = os.path.join(gJugglerDir, 'external')
      destdir = os.path.join(prefix, 'share', 'vrjuggler', 'java')
      for j in dep_jars:
         shutil.copy2(os.path.join(srcroot, j), destdir)
   else:
      printStatus("VRJConfig not built.  Skipping.")

def installVRJugglerPlugins(prefix):
   printStatus("Installing VR Juggler C++ plug-ins ...")

   destdir = os.path.join(prefix, 'lib', 'vrjuggler', 'plugins')
   srcroot = os.path.join(gJugglerDir, 'vc7', 'VRJugglerPlugins',
                          'Perf_Plugin_CXX')
   installLibs(srcroot, destdir, extensions = ['.dll'])

def installVRJugglerPluginsJava(prefix):
   srcdir = os.path.join(gJugglerDir, 'vc7', 'VRJugglerPlugins',
                         'Perf_Plugin_Java')

   plugins = [('PerformanceMonitor', 'corba_perf_mon')]

   for p in plugins:
      name = p[0]
      dir  = p[1]
      if os.path.exists(os.path.join(srcdir, name + '.jar')):
         printStatus("Installing VR Juggler Java plug-ins ...")

         destdir = os.path.join(prefix, 'share', 'vrjuggler', 'beans')
         mkinstalldirs(destdir)
         shutil.copy2(os.path.join(srcdir, name + '.jar'), destdir)

         srcdir = os.path.join(gJugglerDir, 'modules', 'vrjuggler', 'plugins',
                               dir)
         shutil.copy2(os.path.join(srcdir, name + '.xml'), destdir)
      else:
         printStatus("VR Juggler %s Java plug-ins not built.  Skipping." % name)

   # Install JFreeChart.
   destdir = os.path.join(prefix, 'share', 'vrjuggler', 'java')
   srcdir = os.path.join(gJugglerDir, 'external', 'jfreechart')
   installDir(srcdir, destdir, ['.jar'])

def installMsvcRT(prefix):
   printStatus("Installing MSVC runtime DLLs")

   try:
      srcroot = os.environ['SystemRoot']
      destdir = os.path.join(prefix, 'lib')

      # Get *every* MSVC runtime DLL.  This list could be shortened at some
      # point if anyone cares to try.
      dlls = glob.glob(os.path.join(srcroot, 'System32', 'msvc*.dll'))

      for d in dlls:
         shutil.copy2(d, destdir)
   except KeyError, ex:
      printStatus("WARNING: Could not install MSVC runtime DLLs")
      print ex

def doDependencyInstall(prefix):
   makeTree(prefix)
   installNSPR(prefix)
   installCppDOM(prefix)
   installBoost(prefix)
   installGMTL(prefix)
   installAudiere(prefix)
   installOpenAL(prefix)
   installOmniORB(prefix)

def simpleInstall(name, root, prefix, optional = False):
   if optional and root == '':
      return

   printStatus("Installing " + name)

   # Install all header files.
   srcdir = os.path.join(root, 'include')

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'include')
      installDir(srcdir, destdir, ['.h', '.hpp', '.ipp'])

   # Install all libraries.
   srcdir = os.path.join(root, 'lib')

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'lib')
      installDir(srcdir, destdir)

   # Install all executables.
   srcdir = os.path.join(root, 'bin')

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'bin')
      installDir(srcdir, destdir)

def installNSPR(prefix):
   simpleInstall('NSPR headers and libraries', os.environ['NSPR_ROOT'],
                 prefix)

def installCppDOM(prefix):
   simpleInstall('CppDOM headers and libraries', os.environ['CPPDOM_ROOT'],
                 prefix)

def installBoost(prefix):
   printStatus("Installing Boost headers and libraries")

   srcroot = os.environ['BOOST_ROOT']

   srcdir = os.environ['BOOST_INCLUDES']
   destdir = os.path.join(prefix, 'include')
   installDir(srcdir, destdir)

   srcdir = os.path.join(srcroot, 'lib')
   destdir = os.path.join(prefix, 'lib')
   mkinstalldirs(destdir)

   lib_list = glob.glob(os.path.join(srcdir, '*filesystem*')) + \
              glob.glob(os.path.join(srcdir, '*python*'))

   for f in lib_list:
      shutil.copy2(f, destdir)

def installGMTL(prefix):
   simpleInstall('GMTL headers', os.environ['GMTL_ROOT'], prefix)

def installAudiere(prefix):
   simpleInstall('Audiere headers, libraries, and executables',
                 os.getenv('AUDIERE_ROOT', ''), prefix, True)

def installOpenAL(prefix):
   printStatus("Installing OpenAL DLL")
   srcdir  = os.environ['OPENAL_ROOT']
   if srcdir != "":
       destdir = os.path.join(prefix, 'bin')
       shutil.copy2(os.path.join(srcdir, 'dll', 'OpenAL32.dll'), destdir)

def installOmniORB(prefix):
   root = os.getenv('OMNIORB_ROOT', '')
   if root == '':
      return

   printStatus('Installing omniORB headers, libraries, and executables')

   # Install all header files.
   srcdir = os.path.join(root, 'include')

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'include')
      installDir(srcdir, destdir, ['.h', '.hh'])

   # Install all libraries.
   # NOTE: When we install the omniORB .lib files, we get rid of the x86_win32
   # subdirectory.
   srcdir = os.environ['OMNIORB_LIB']

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'lib')
      installDir(srcdir, destdir)

   srcdir = os.path.join(root, 'lib', 'python')

   if os.path.exists(srcdir):
      # Install the omniidl Python bits into the bin directory so that users
      # do not have to set %PYTHONPATH%.
      destdir = os.path.join(prefix, 'bin')
      installDir(srcdir, destdir)

   # If omnipython is installed along with omniORB, we need to install it, too.
   srcdir = os.path.join(root, 'lib', 'python1.5')

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'lib', 'python1.5')
      installDir(srcdir, destdir)

   # Install all executables and DLLs.
   # NOTE: When we install the omniORB .dll files, we get rid of the x86_win32
   # subdirectory.
   srcdir = os.environ['OMNIORB_BIN']

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'bin')
      installDir(srcdir, destdir)

   srcdir = os.path.join(root, 'bin', 'scripts')

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'bin', 'scripts')
      installDir(srcdir, destdir)

class GuiFrontEnd:
   def __init__(self, master):
      self.mRoot = master
      self.mRoot.title("VR Juggler Win32 Build")
      self.mRoot.protocol("WM_DELETE_WINDOW", self.cleanup)
      self.mRoot.bind("<Destroy>", lambda e: self.cleanup)

      self.createUI()

      # Replace the console version of printStatus() with our own version.
      global printStatus
      printStatus = self.printMessage

      required, optional, options = getDefaultVars()
      self.mOptions   = options
      self.mTkOptions = {}

      # Make a StringVar dictionary.
      for k in options:
         self.mTkOptions[k] = self.__str2TkinterStrVar(options[k])

      self.makeOptionsInterface(required, optional)
      self.update()

   def __str2TkinterStrVar(self, inputStr):
      temp = Tkinter.StringVar()
      temp.set(inputStr)
      return temp

   def __writeCacheFile(self):
      cache_file = open(getCacheFileName(), 'w')
      for k, v in self.mTkOptions.iteritems():
         output = "options['%s'] = r'%s'\n" % (k, v.get())
         cache_file.write(output)
      cache_file.close()

   def printMessage(self, msg):
      self.mRoot.OutputFrame.MessageText['state'] = 'normal'
      self.mRoot.OutputFrame.MessageText.insert(Tkinter.END, msg + "\n", "a")
      self.mRoot.OutputFrame.MessageText['state'] = 'disabled'

   def cleanup(self):
      self.__writeCacheFile()
      self.mRoot.destroy()

   def createUI(self):
      # Set up the frames.
      pad_amount = 10

      # Settings.
      self.Juggler = "#0EAE06"
      self.JugglerYellow = "#EECE26"
      self.JugglerPurple = "#8E76AA"
      self.HeaderFont = (16)

      self.mRoot.HeaderFrame = Tkinter.Frame(self.mRoot, borderwidth = 1,
                                             relief = "sunken",
                                             bg = self.JugglerYellow)
      self.mRoot.HeaderFrame.grid(row = 0, column = 0,
                                  sticky = Tkinter.N + Tkinter.E + Tkinter.S + Tkinter.W,
                                  ipadx = pad_amount, ipady = pad_amount)

      self.mRoot.SettingsFrame = Tkinter.Frame(self.mRoot, borderwidth = 1,
                                               relief = "sunken",
                                               bg = self.JugglerYellow)
      self.mRoot.SettingsFrame.grid(row = 0, column = 1,
                                    sticky = Tkinter.N + Tkinter.E + Tkinter.S + Tkinter.W,
                                    ipadx = pad_amount, ipady = pad_amount)

      self.mRoot.CommandFrame = Tkinter.Frame(self.mRoot, borderwidth = 1,
                                              relief = "sunken",
                                              bg = self.JugglerYellow)
      self.mRoot.CommandFrame.grid(row = 1, column = 0,
                                   sticky = Tkinter.N + Tkinter.E + Tkinter.S + Tkinter.W,
                                   ipadx = pad_amount, ipady = pad_amount)

      self.mRoot.OutputFrame = Tkinter.Frame(self.mRoot, borderwidth = 1,
                                             relief = "sunken",
                                             bg = self.JugglerYellow)
      self.mRoot.OutputFrame.grid(row = 1, column = 1,
                                  sticky = Tkinter.N + Tkinter.E + Tkinter.S + Tkinter.W,
                                  ipadx = pad_amount, ipady = pad_amount)

      self.mRoot.StatusFrame = Tkinter.Frame(self.mRoot, borderwidth = 1,
                                             relief = "sunken",
                                             bg = self.JugglerYellow)
      self.mRoot.StatusFrame.grid(row = 2, column = 0, columnspan = 2,
                                  sticky = Tkinter.W + Tkinter.E)

      # HeaderFrame Innards
      self.mRoot.HeaderFrame.vjImage = \
         Tkinter.PhotoImage(file = r"vc7\juggler-logo.gif", format = "gif")
      self.mRoot.HeaderFrame.Image = Tkinter.Label(self.mRoot.HeaderFrame,
                                                   image = self.mRoot.HeaderFrame.vjImage,
                                                   bg = self.JugglerYellow)
      self.mRoot.HeaderFrame.Image.grid(row = 0, column = 0, sticky = Tkinter.N)
      self.mRoot.HeaderFrame.ImageLabel = \
         Tkinter.Label(self.mRoot.HeaderFrame,
                       text = "VR Juggler Win32 Build Configuration",
                       bg = self.JugglerYellow, font = self.HeaderFont)
      self.mRoot.HeaderFrame.ImageLabel.grid(row = 1, column = 0,
                                             sticky = Tkinter.N)
      self.mRoot.HeaderFrame.rowconfigure(1, weight = 1)

      # SettingsFrame Innards
      self.mRoot.SettingsFrame.RequiredSettingsFrame = \
         Tkinter.Frame(self.mRoot.SettingsFrame, borderwidth = 1,
                       relief = "sunken", bg = self.JugglerYellow)
      self.mRoot.SettingsFrame.RequiredSettingsFrame.grid(row = 1, column = 0,
                                                          sticky = Tkinter.N + Tkinter.E + Tkinter.S + Tkinter.W)
      self.mRoot.SettingsFrame.RequiredSettingsFrame.SettingsRows = {}
      self.mRoot.SettingsFrame.OptionalSettingsFrame = \
         Tkinter.Frame(self.mRoot.SettingsFrame, borderwidth = 1,
                       relief = "sunken", bg = self.JugglerYellow)
      self.mRoot.SettingsFrame.OptionalSettingsFrame.grid(row = 2, column = 0,
                                                          sticky = Tkinter.N + Tkinter.E + Tkinter.S + Tkinter.W)
      self.mRoot.SettingsFrame.OptionalSettingsFrame.SettingsRows = {}

      #OutputFrame Innards
      self.mRoot.OutputFrame.MessageText = Tkinter.Text(self.mRoot.OutputFrame,
                                                        height = 20,
                                                        width = 100,
                                                        state = 'disabled')
      self.mRoot.OutputFrame.MessageText.grid(row = 0, column = 0)
      self.mRoot.OutputFrame.MessageText.tag_config("a", foreground = "blue")

      self.mRoot.OutputFrame.MessageText.ScrollBar = \
         Tkinter.Scrollbar(self.mRoot.OutputFrame)
      self.mRoot.OutputFrame.MessageText.ScrollBar.grid(row = 0, column = 1,
                                                        sticky = Tkinter.W + Tkinter.N + Tkinter.S)

      self.mRoot.OutputFrame.MessageText.config(yscrollcommand = self.mRoot.OutputFrame.MessageText.ScrollBar.set)
      self.mRoot.OutputFrame.MessageText.ScrollBar.config(command = self.mRoot.OutputFrame.MessageText.yview)

      #StatusFrame Innards
      self.mRoot.StatusFrame.Label = Tkinter.Label(self.mRoot.StatusFrame,
                                                   text = "Status: ")
      self.mRoot.StatusFrame.Label.grid(row=0, column=0, rowspan=2)#, sticky="W")
      self.mRoot.StatusFrame.StatusLabel = Tkinter.Label(self.mRoot.StatusFrame,
                                                         text = "Test",
                                                         anchor = Tkinter.W)
      self.mRoot.StatusFrame.StatusLabel.grid(row = 0, column = 1,
                                              sticky = Tkinter.EW)
      self.mRoot.StatusFrame.columnconfigure(1, weight = 1)

   def makeOptionsInterface(self, required, optional):
      # RequiredSettingsFrame
      next_row = 0
      self.mRoot.SettingsFrame.RequiredSettingsFrame.Label = \
         Tkinter.Label(self.mRoot.SettingsFrame.RequiredSettingsFrame,
                       font = self.HeaderFont, text = "Required Settings",
                       bg = self.JugglerPurple)
      self.mRoot.SettingsFrame.RequiredSettingsFrame.Label.grid(row = next_row,
                                                                column = 0,
                                                                columnspan = 3,
                                                                sticky = Tkinter.EW)
      self.mRoot.SettingsFrame.RequiredSettingsFrame.columnconfigure(0, weight = 1)
      next_row = next_row + 1
      self.makeEntryRow(self.mRoot.SettingsFrame.RequiredSettingsFrame,
                        "Installation Prefix:", 'prefix', next_row)
      next_row = next_row + 1

      for opt in required:
         self.makeEntryRow(self.mRoot.SettingsFrame.RequiredSettingsFrame,
                           opt.desc, opt.envVar, next_row, opt.required,
                           opt.isDirectory)
         next_row += 1

      # OptionalSettingsFrame
      next_row = 0
      self.mRoot.SettingsFrame.OptionalSettingsFrame.Label = \
         Tkinter.Label(self.mRoot.SettingsFrame.OptionalSettingsFrame,
                       font = self.HeaderFont, text = "Optional Settings",
                       bg = self.JugglerPurple)
      self.mRoot.SettingsFrame.OptionalSettingsFrame.Label.grid(row = next_row,
                                                                column = 0,
                                                                columnspan = 3,
                                                                sticky = Tkinter.EW)
      self.mRoot.SettingsFrame.OptionalSettingsFrame.columnconfigure(0, weight = 1)
      next_row = next_row + 1

      self.makeEntryRow(self.mRoot.SettingsFrame.OptionalSettingsFrame,
                        "Dependency installation prefix:", 'deps-prefix',
                        next_row, False)
      next_row = next_row + 1

      for opt in optional:
         self.makeEntryRow(self.mRoot.SettingsFrame.OptionalSettingsFrame,
                           opt.desc, opt.envVar, next_row, opt.required,
                           opt.isDirectory)
         next_row += 1

      # CommandFrame Innards.
      next_row = 0
      self.mRoot.CommandFrame.OpenVSCheck = \
         Tkinter.Checkbutton(self.mRoot.CommandFrame,
                             text="Open Visual Studio IDE",
                             bg = self.JugglerYellow,
                             activebackground = self.JugglerYellow,
                             onvalue = "Yes", offvalue = "No")
      self.mRoot.CommandFrame.OpenVSCheck.Variable = Tkinter.StringVar()
      self.mRoot.CommandFrame.OpenVSCheck.Variable.set("No")
      self.mRoot.CommandFrame.OpenVSCheck["variable"] = \
         self.mRoot.CommandFrame.OpenVSCheck.Variable
      self.mRoot.CommandFrame.OpenVSCheck.grid(row = next_row, column = 0,
                                               sticky = Tkinter.EW, pady = 4)
      next_row = next_row + 1
      self.mRoot.CommandFrame.InstallJugglerCheck = \
         Tkinter.Checkbutton(self.mRoot.CommandFrame, text = "Install Juggler",
                             bg = self.JugglerYellow,
                             activebackground = self.JugglerYellow,
                             onvalue ="Yes", offvalue = "No")
      self.mRoot.CommandFrame.InstallJugglerCheck.Variable = Tkinter.StringVar()
      self.mRoot.CommandFrame.InstallJugglerCheck.Variable.set("Yes")
      self.mRoot.CommandFrame.InstallJugglerCheck["variable"] = \
         self.mRoot.CommandFrame.InstallJugglerCheck.Variable
      self.mRoot.CommandFrame.InstallJugglerCheck.grid(row = next_row,
                                                       column = 0,
                                                       sticky = Tkinter.EW,
                                                       pady = 4)
      next_row = next_row + 1
      self.mRoot.CommandFrame.InstallJugglerDepsCheck = \
         Tkinter.Checkbutton(self.mRoot.CommandFrame,
                             text = "Install Juggler Dependencies",
                             bg = self.JugglerYellow,
                             activebackground = self.JugglerYellow,
                             onvalue = "Yes", offvalue = "No")
                             #,command=self.installDeps, state="disabled")
      self.mRoot.CommandFrame.InstallJugglerDepsCheck.Variable = Tkinter.StringVar()
      self.mRoot.CommandFrame.InstallJugglerDepsCheck.Variable.set("No")
      self.mRoot.CommandFrame.InstallJugglerDepsCheck["variable"] = \
         self.mRoot.CommandFrame.InstallJugglerDepsCheck.Variable
      self.mRoot.CommandFrame.InstallJugglerDepsCheck.grid(row = next_row,
                                                           column = 0,
                                                           sticky = Tkinter.EW,
                                                           pady = 4)
      next_row = next_row + 1

      self.mRoot.CommandFrame.BuildInstallButton = \
         Tkinter.Button(self.mRoot.CommandFrame, text = "Build and Install",
                        command = self.doBuild, state = "disabled")
      self.mRoot.CommandFrame.BuildInstallButton.grid(row = next_row,
                                                      column = 0,
                                                      sticky = Tkinter.EW,
                                                      pady = 4)
      next_row = next_row + 1

   def update(self):
         self.updateRequiredOptions()
         self.updateOptionsValidation()
         self.updateCommandFrame()

   def entryValidation(self, entry, update = False):
      if os.path.isdir(entry.get()):
         entry.config(fg = "blue")
      else:
         entry.config(fg = "red")

      if update:
         self.updateRequiredOptions()
         self.updateCommandFrame()

   def updateOptionValidation(self, setting):
      if setting["isDirectory"]:
         self.entryValidation(setting["Entry"])

   def updateOptionsValidation(self):
      for k in self.mRoot.SettingsFrame.RequiredSettingsFrame.SettingsRows:
         self.updateOptionValidation(self.mRoot.SettingsFrame.RequiredSettingsFrame.SettingsRows[k])

      for k in self.mRoot.SettingsFrame.OptionalSettingsFrame.SettingsRows:
         self.updateOptionValidation(self.mRoot.SettingsFrame.OptionalSettingsFrame.SettingsRows[k])

   def updateRequiredOptions(self):
      self.mReqSettingsSet = True
      for k in self.mRoot.SettingsFrame.RequiredSettingsFrame.SettingsRows:
         if self.mTkOptions[k].get() == "":
            self.mReqSettingsSet = False

   def updateCommandFrame(self):
      if self.mReqSettingsSet:
         #self.mRoot.CommandFrame.BuildButton.config(state = "normal")
         self.mRoot.CommandFrame.BuildInstallButton.config(state = "normal")
      else:
         #self.mRoot.CommandFrame.BuildButton.config(state = "disabled")
         self.mRoot.CommandFrame.BuildInstallButton.config(state = "disabled")

   def validateOptions(self):
      status = True

      # Make sure that all options that are directories reference valid
      # directories.
      inv_dir_list = []
      for k in self.mTkOptions:
         if self.mOptionWidgetsDict[k][4] and \
            not os.path.isdir(self.mTkOptions[k].get() and \
            self.mTkOptions[k].get() != ""):
            status = False
            inv_dir_list.append(self.mOptionWidgetsDict[k][0]['text'])

      if not status:
         tkMessageBox.showwarning(
            "Invalid Directory",
            "The following settings reference non-existant directories and must be set before proceeding.\n%s" % (inv_dir_list) )
         return status

      return status

   def doBuild(self):
      self.mRoot.CommandFrame.BuildInstallButton.config(state = "disabled")

      # Set the environment vars.
      for k in self.mTkOptions.iterkeys():
         os.environ[k] = self.mTkOptions[k].get()

      if True:#self.validateOptions():
         postProcessOptions(self.mOptions)
         self.__writeCacheFile()

         self.BuildThread = threading.Thread(None, self.runVisualStudio,
                                             "BuildThread")
         self.BuildThread.start()

   def installJuggler(self):
      doInstall(self.mTkOptions['prefix'].get())

   def installDeps(self):
      doDependencyInstall(self.mTkOptions['deps-prefix'].get())

   def getFile(self, optionIndex, initialDir, toEntry):
      def clearAndGet(self, optionIndex, initialDir):
         result_dir = tkFileDialog.askdirectory(title = optionIndex,
                                                initialdir = initialDir)
         if result_dir != '':
            self.mTkOptions[optionIndex].set(result_dir)

      return lambda: clearAndGet(self, optionIndex, initialDir)

   def makeEntryRow(self, master, label, optionIndex, row, required = True,
                    isDirectory = True):
      # Label.
      label_ref = Tkinter.Label(master, text = label)
      label_ref.grid(row = row, column = 0,
                     sticky = Tkinter.N + Tkinter.E + Tkinter.S + Tkinter.W,
                     pady = 2)

      # Entry.
      entry_ref = Tkinter.Entry(master, width = 75,
                                textvariable = self.mTkOptions[optionIndex])
      entry_ref.grid(row = row, column = 1,
                     sticky = Tkinter.N + Tkinter.E + Tkinter.S + Tkinter.W,
                     pady = 2)

      if isDirectory:
         if required:
            self.mTkOptions[optionIndex].trace_variable('w',
               lambda n, i, m: self.entryValidation(entry_ref, True))
         else:
            self.mTkOptions[optionIndex].trace_variable('w',
               lambda n, i, m: self.entryValidation(entry_ref, False))


      # Button, if there is one.
      button_ref = None
      if isDirectory:
         button_ref = Tkinter.Button(master, text = 'Browse',
                                     command = self.getFile(optionIndex,
                                                            self.mTkOptions[optionIndex].get(),
                                                            entry_ref))
         button_ref.grid(row = row, column = 2,
                         sticky = Tkinter.N + Tkinter.E + Tkinter.S + Tkinter.W,
                         pady = 2)

      master.SettingsRows[optionIndex] = {
         "Label" : label_ref,
         "Entry" : entry_ref,
         "Button" : button_ref,
         "isRequired" : required,
         "isDirectory" : isDirectory
         }

   def buildFinished(self):
      print self.mRoot.CommandFrame.InstallJugglerCheck.Variable.get()
      print self.mRoot.CommandFrame.InstallJugglerDepsCheck['state']

      if self.mRoot.CommandFrame.InstallJugglerCheck.Variable.get() == "Yes":
         self.printMessage("Installing Juggler...")
         doInstall(self.mTkOptions['prefix'].get())

      if self.mRoot.CommandFrame.InstallJugglerDepsCheck.Variable.get() == "Yes":
         self.printMessage("Installing Juggler Dependencies...")
         doDependencyInstall(self.mTkOptions['deps-prefix'].get())

      self.printMessage("Build and Installation Finished.")
      self.updateCommandFrame()

   def runVisualStudio(self):
      #print "generateVersionHeaders()"
      self.printMessage("Generating Version Headers.")
      generateVersionHeaders()
      self.printMessage("Generating Ant Build Files.")
      generateAntBuildFiles()

      devenv_cmd = getVSCmd()
      (devenv_cmd_no_exe, ext) = os.path.splitext(devenv_cmd)
      devenv_cmd_no_exe = '"%s"' % (devenv_cmd_no_exe)

      #solution_file = r'%s' % os.path.join(gJugglerDir, 'vc7', 'Juggler.sln')
      solution_file = r'%s' % os.path.join('vc7', 'Juggler.sln')
      build_args = r'/build Debug'

      if self.mRoot.CommandFrame.OpenVSCheck.Variable.get() == "No":
         cmd = devenv_cmd_no_exe + ' ' + solution_file + ' ' + build_args

         print cmd
         try:
            stdin = os.popen(cmd)

            while True:
               line = stdin.readline()
               if not line:
                  break
               self.mRoot.OutputFrame.MessageText['state'] = 'normal'
               self.mRoot.OutputFrame.MessageText.insert(Tkinter.END, line)
               self.mRoot.OutputFrame.MessageText.yview("moveto", 1.0)
               self.mRoot.OutputFrame.MessageText['state'] = 'disabled'
         except OSError, osEx:
            print "Could not execute %s: %s" % (cmd, osEx)
            sys.exit(3)
      else:
         cmd = devenv_cmd_no_exe + ' ' + solution_file
         try:
            self.printMessage("Visual Studio has been opened.  Build the Solution and then exit Visual Studio to continue the Installation.")
            status = os.spawnl(os.P_WAIT, devenv_cmd, 'devenv', solution_file)
         except OSError, osEx:
            print "Could not execute %s: %s" % (cmd, osEx)
            sys.exit(EXIT_STATUS_MSVS_START_ERROR)
      self.buildFinished()

def getVSCmd():
   devenv_cmd = None
   for p in str.split(os.getenv('PATH', ''), os.pathsep):
#      print "Searching in", p
      if os.path.exists(os.path.join(p, 'devenv.exe')):
         devenv_cmd = os.path.join(p, 'devenv.exe')
         break

   if devenv_cmd is None:
      # The environment variable %VSINSTALLDIR% is set by vsvars32.bat.
      print "WARNING: Falling back on the use of %VSINSTALLDIR%"
      devenv_cmd = r'%s' % os.path.join(os.getenv('VSINSTALLDIR', ''),
                                        'devenv.exe')

   return devenv_cmd

def main():
   disable_tk = False

   try:
      cmd_opts, cmd_args = getopt.getopt(sys.argv[1:], "cao:h",
                                         ["nogui", "auto", "options-file=",
                                          "help"])
   except getopt.GetoptError:
      usage()
      sys.exit(4)

   global gOptionsFileName
   for o, a in cmd_opts:
      if o in ("-c","--nogui"):
         disable_tk = True
      elif o in ("-o", "--options-file="):
         gOptionsFileName = a

         # Make sure file exists.
         if not os.path.isfile(gOptionsFileName):
            print "No file %s exists.  Will use default options." % \
                  gOptionsFileName
      elif o in ("-h", "--help"):
         usage()
         sys.exit(0)

   # If Tkinter is not available or the user disabled the Tk frontend, use
   # the text-based interface.
   if not gHaveTk or disable_tk:
      options = setVars()
      generateVersionHeaders()
      generateAntBuildFiles()

      devenv_cmd    = getVSCmd()
      solution_file = r'%s' % os.path.join(gJugglerDir, 'vc7', 'Juggler.sln')

      try:
         status = os.spawnl(os.P_WAIT, devenv_cmd, 'devenv', solution_file)

         if status == 0:
            print "Proceed with VR Juggler installation [y]: ",
            proceed = sys.stdin.readline().strip(" \n")
            if proceed == '' or proceed.lower().startswith('y'):
               doInstall(options['prefix'])

               print "Proceed with VR Juggler dependency installation [y]: ",
               proceed = sys.stdin.readline().strip(" \n")
               if proceed == '' or proceed.lower().startswith('y'):
                  doDependencyInstall(options['deps-prefix'])
      except OSError, osEx:
         print "Could not execute %s: %s" % (devenv_cmd, osEx)
         sys.exit(EXIT_STATUS_MSVS_START_ERROR)

      sys.exit(EXIT_STATUS_SUCCESS)
   else:
      root = Tkinter.Tk()
      my_app = GuiFrontEnd(root)
      root.mainloop()

def usage():
   print "Usage: %s [OPTIONS]" % (sys.argv[0])
   print "Python script for building VRJuggler on Windows.\n"
   print "-n, --nogui              Disable the Tkinter GUI FrontEnd, e.g. Run in command line mode."
   #print "-a, --auto               Does not interactively ask for values of any options.  Uses the Default values, 'options.cache' if it exists, or the file given by the -o option.  Only used in command line mode."
   print "-o, --options-file=FILE  Uses FILE to Load/Save Options."
   print "-h, --help               Print this usage text and quit."

if __name__ == '__main__':
   try:
      main()
   except SystemExit, exitEx:
      if exitEx.code == EXIT_STATUS_SUCCESS:
         status = 'successful completion'
      elif exitEx.code == EXIT_STATUS_NO_MSVS:
         status = 'no Visual Studio installation found'
      elif exitEx.code == EXIT_STATUS_MISSING_DATA_FILE:
         status = 'could not read data file required for compiling'
      elif exitEx.code == EXIT_STATUS_MSVS_START_ERROR:
         status = 'could not start Visual Studio'
      elif exitEx.code == EXIT_STATUS_INVALID_PATH:
         status = 'invalid directory structure'
      elif exitEx.code == EXIT_STATUS_MISSING_REQ_VALUE:
         status = 'required value not given'
      else:
         status = 'error encountered'

      print "Exiting with status %d (%s)" % (exitEx.code, status)
      print "Press <ENTER> to quit ..."
      sys.stdin.readline()

      # Exit for real without throwing another SystemExit exception.
      os._exit(exitEx.code)
   except:
      info = sys.exc_info()
      traceback.print_exception(info[0], info[1], info[2])
      print "An exception was caught.  Press <ENTER> to quit ..."
      sys.stdin.readline()
