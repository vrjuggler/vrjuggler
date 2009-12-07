#python

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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
# *************** <auto-copyright.pl END do not edit this line> ***************

import glob
import os, os.path
import re
import shutil
import sys
import time
import traceback
import getopt
pj = os.path.join

EXIT_STATUS_SUCCESS              = 0
EXIT_STATUS_NO_MSVS              = 1
EXIT_STATUS_MISSING_DATA_FILE    = 2
EXIT_STATUS_MSVS_START_ERROR     = 3
EXIT_STATUS_INVALID_PATH         = 4
EXIT_STATUS_MISSING_REQ_VALUE    = 5
EXIT_STATUS_UNSUPPORTED_COMPILER = 6
EXIT_STATUS_INVALID_ARGUMENT     = 7

gJugglerDir      = os.path.dirname(os.path.abspath(sys.argv[0]))
gOptionsFileName = "options.cache"
gBuild64         = False

gJdomJars = [
   'jdom.jar',
   'jaxen-core.jar',
   'jar',
   'jaxen-jdom.jar',
   'xerces.jar',
   'xml-apis.jar',
   'saxpath.jar'
]

gTweekJars = [
   'Tweek.jar',
   'TweekBeanDelivery.jar',
   'TweekBeans.jar',
   'TweekEvents.jar',
   'TweekNet.jar',
   'TweekServices.jar',
   'kunststoff-mod.jar'
]

gTweekExtJars = [
   'ui.jar',
   'wizard.jar'
]

gJcclJars = [
   'jccl_config.jar',
   'jccl_editors.jar'
]

gJcclRtrcJars = [
   'jccl_rtrc.jar'
]

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

def detectVisualStudioVersion(reattempt = False):
   (cl_stdin, cl_stdout, cl_stderr) = os.popen3('cl')
   cl_version_line = cl_stderr.readline()

   cl_ver_match = re.compile(r'Compiler Version ((\d+)\.(\d+)\.(\d+))')
   ver_string_match = cl_ver_match.search(cl_version_line)

   cl_major = 0
   cl_minor = 0

   if ver_string_match is not None:
      cl_major = int(ver_string_match.group(2))
      cl_minor = int(ver_string_match.group(3))

      if cl_major == 13 and cl_minor < 10:
         printStatus("Visual Studio .NET 2002 is not supported")
         sys.exit(EXIT_STATUS_UNSUPPORTED_COMPILER)
      elif cl_major == 13 and cl_minor >= 10:
         vs_ver = '.NET 2003'
      elif cl_major == 14:
         vs_ver = '2005'
      else:
         vs_ver = '2008'

      printStatus("It appears that we will be using Visual Studio %s"%vs_ver)
      #printStatus("   compiler version: %s.%s"%(cl_major,cl_minor))

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
         printStatus("Visual studio not in path, attempting to find...")
         # Common installation directories for Visual Studio.
         vs_dirs = [r'C:\Program Files\Microsoft Visual Studio 9.0', 
                    r'C:\Program Files\Microsoft Visual Studio 8',
                    r'C:\Program Files\Microsoft Visual Studio .NET 2003'
                   ]

         for d in vs_dirs:
            printStatus("Trying path: %s"%d)
            if not os.path.exists(d):
               printStatus("  does not exist.")
            else:
               printStatus("   Using: %s"%d)
               printStatus("      " + d)
               vs_path = [os.path.join(d, r'Common7\IDE'),
                          os.path.join(d, r'VC7\BIN'),
                          os.path.join(d, r'VC\BIN'),
                          os.path.join(d, r'Common7\Tools'),
                          os.path.join(d, r'Common7\Tools\bin\prerelease'),
                          os.path.join(d, r'Common7\Tools\bin')]
               path_add = ';'.join(vs_path)
               os.environ['PATH'] = path_add + os.pathsep + os.getenv('PATH', '')

               # Try again to guess the Visual Studio version.
               return detectVisualStudioVersion(True)

         # If execution reaches this point, our attempts to guess the
         # location of a Visual Studio 7.x installation failed.
         noVisualStudioError()

      # If this is a reattempt to guess the Visual Studio version, then
      # something is wrong with the user's Visual Studio installation.
      else:
         noVisualStudioError()

   return (cl_major, cl_minor)

def chooseVisualStudioDir():
   (cl_ver_major, cl_ver_minor) = detectVisualStudioVersion()

   # For Visual Studio .NET 2003 (version 7.1), we will use the solution in
   # the vc7 subtree.
   if cl_ver_major == 13:
      vc_dir = 'vc7'
   # For Visual Studio 2005, we use the solution in the vc8 subtree.
   elif cl_ver_major == 14:
      vc_dir = 'vc8'
   # Otherwise, we use the solution in the vc9 subtree.
   else:
      vc_dir = 'vc9'

   return (cl_ver_major, cl_ver_minor, vc_dir)

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

def getDefaultVars(clVerMajor, clVerMinor):
   required = []
   required.append(BuildOption('BOOST_ROOT',
                               'Boost C++ installation directory', ''))
   required.append(BuildOption('BOOST_VERSION', 'Boost C++ version',
                               '1_31', False))
   required.append(BuildOption('BOOST_INCLUDES',
                               'Directory containing the Boost C++ header tree',
                               ''))
   required.append(BuildOption('NSPR_ROOT', 'NSPR installation directory', ''))
   required.append(BuildOption('NSPR_INCLUDES', 'Directory containing nspr.h',
                               ''))
   required.append(BuildOption('CPPDOM_ROOT', 'CppDOM installation directory',
                               ''))
   required.append(BuildOption('CPPDOM_INCLUDES',
                               'Directory containing the CppDOM header tree',
                               ''))
   required.append(BuildOption('GMTL_ROOT', 'GMTL installation directory', ''))
   required.append(BuildOption('GMTL_INCLUDES',
                               'Directory containing the GMTL header tree',
                               ''))

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
   optional.append(BuildOption('ALUT_ROOT', 'ALUT SDK installation directory',
                               '', required = False))
   optional.append(BuildOption('TRACKD_API_ROOT',
                               'TrackdAPI installation directory', '',
                               required = False))
   optional.append(BuildOption('FTD2XX_ROOT',
                               'FTD2XX SDK installation directory', '',
                               required = False))
   optional.append(BuildOption('MOTION_NODE_SDK_ROOT',
                               'MotionNode SDK installation directory', '',
                               required = False))
   optional.append(BuildOption('DOOZER_ROOT',
                               'Doozer installation directory', '',
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

def setVars(clVerMajor, clVerMinor):
   required, optional, options = getDefaultVars(clVerMajor, clVerMinor)

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
      elif opt.envVar == 'BOOST_VERSION' and \
           options.get('BOOST_INCLUDES', '') == '':
         boost_ver = result
         options['BOOST_INCLUDES'] = boost_dir + r'\include\boost-' + boost_ver

      # The following is a little hack to get a reasonable default set for
      # the NSPR_INCLUDES variable before the user has to enter it manually.
      if opt.envVar == 'NSPR_ROOT' and \
         options.get('NSPR_INCLUDES', '') == '':
         options['NSPR_INCLUDES'] = os.path.join(result, 'include')

      # The following is a little hack to get a reasonable default set for
      # the CPPDOM_INCLUDES variable before the user has to enter it manually.
      if opt.envVar == 'CPPDOM_ROOT' and \
         options.get('CPPDOM_INCLUDES', '') == '':
         options['CPPDOM_INCLUDES'] = os.path.join(result, 'include')

      # The following is a little hack to get a reasonable default set for
      # the GMTL_INCLUDES variable before the user has to enter it manually.
      if opt.envVar == 'GMTL_ROOT' and \
         options.get('GMTL_INCLUDES', '') == '':
         options['GMTL_INCLUDES'] = os.path.join(result, 'include')

   print "+++ Optional Settings"
   processInput(options, 'deps-prefix', 'Dependency installation prefix')

   for opt in optional:
      processInput(options, opt.envVar, opt.desc, opt.required)

   postProcessOptions(options)
   writeCacheFile(options)

   return options

def postProcessOptions(options):
   os.environ['instprefix'] = options['prefix'].replace('\\', '\\\\')

   # If the %JAVA_HOME% setting is a valid directory, add its bin subdirectory
   # to the path.
   if os.environ['JAVA_HOME'] != '' and os.path.exists(os.environ['JAVA_HOME']):
      jdk_path = os.path.join(os.environ['JAVA_HOME'], 'bin')
      os.environ['PATH'] = jdk_path + os.pathsep + os.environ['PATH']
      os.environ['JACORB_PATH'] = os.path.join(gJugglerDir, r'external\JacORB')

   if os.environ['OMNIORB_ROOT'] != '' and os.path.exists(os.environ['OMNIORB_ROOT']):
      # A 64-bit build of omniORB has to have been compiled against a 64-bit
      # build of Python. Unfortunately, when omniidl.exe acts as the Python
      # interpreter, it doesn't take care of setting PYTHONHOME, and this
      # prevents it from being able to find core modules (such as sys.py or
      # os.py). We'll assume that a 64-bit Python interpreter is being used
      # to run this script and use its installation prefix as PYTHONHOME to
      # help out omniidl.exe.
      if gBuild64 and not os.environ.has_key('PYTHONHOME'):
         os.environ['PYTHONHOME'] = sys.prefix

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

      omnithread_glob = os.path.join(os.environ['OMNIORB_LIB'],
                                     'omnithread*_rt.lib')
      libs = glob.glob(omnithread_glob)
      omnithread_ver_re = re.compile(r'omnithread(\d\d)_rt.lib')

      for l in libs:
         match = omnithread_ver_re.search(l)
         if match is not None:
            os.environ['OMNITHREAD_VERSION'] = match.group(1)
            break

   # Determine if al.h is in the base include directory or in include\AL.
   if os.environ['OPENAL_ROOT'] != '':
      if gBuild64:
         subdir = 'Win64'
      else:
         subdir = 'Win32'

      lib_dirs = [os.path.join(os.environ['OPENAL_ROOT'], 'libs'),
                  os.path.join(os.environ['OPENAL_ROOT'], 'libs', subdir)]

      for l in lib_dirs:
         openal_lib = os.path.join(l, 'OpenAL32.lib')
         if os.path.exists(openal_lib):
            os.environ['OPENAL_LIB_DIR'] = l
            break

      header_file = os.path.join(os.environ['OPENAL_ROOT'], 'include', 'AL',
                                 'al.h')
      # If the file is include\AL\al.h, then set the environment variable
      # %HAVE_AL_AL_H% to the preprocessor symbol 'HAVE_AL_AL_H'.
      # See modules/sonix/plugins/OpenAL/OpenALSoundImplementation.cpp.
      if os.path.exists(header_file):
         os.environ['HAVE_AL_AL_H'] = 'HAVE_AL_AL_H'

   # Determine if alut.h is in the base include directory or in include\AL.
   openal_envs = ['ALUT_ROOT', 'OPENAL_ROOT']
   for env in openal_envs:
      if os.environ[env] != '':
         header_file = os.path.join(os.environ[env], 'include', 'AL', 'alut.h')
         # If the file is include\AL\alut.h, then set the environment variable
         # %HAVE_AL_ALUT_H% to the preprocessor symbol 'HAVE_AL_ALUT_H'.
         # See modules/sonix/plugins/OpenAL/OpenALSoundImplementation.cpp.
         if os.path.exists(header_file):
            os.environ['HAVE_AL_ALUT_H'] = 'HAVE_AL_ALUT_H'

   # If the ALUT installation directory is not set, then assume that it is the
   # same as the OpenAL installation directory.
   if os.environ['ALUT_ROOT'] == '' and os.environ['OPENAL_ROOT'] != '':
      os.environ['ALUT_ROOT'] = os.environ['OPENAL_ROOT']

   if os.environ['TRACKD_API_ROOT'] != '' and os.path.exists(os.environ['TRACKD_API_ROOT']):
      trackdapi_dir = os.environ['TRACKD_API_ROOT']
      trackdapi_incdir = os.path.join(trackdapi_dir, 'include')

      # Figure out which Trackd API header to include.
      headers = ['trackdAPI.h', 'trackdAPI_CC.h']
      for h in headers:
         if os.path.exists(os.path.join(trackdapi_incdir, h)):
            os.environ['GADGET_TRACKD_API_H'] = '<%s>' % h
            break

def writeCacheFile(optionDict):
   cache_file = open(getCacheFileName(), 'w')
   for k, v in optionDict.iteritems():
      output = "options['%s'] = r'%s'\n" % (k, v)
      cache_file.write(output)
   cache_file.close()

def getBoostVersion():
   boost_ver_re = re.compile(r'#define\s+BOOST_LIB_VERSION\s+"(.+)"')

   info = ('0', '0', '0')
   if os.environ.has_key('BOOST_INCLUDES'):
      ver_file = open(os.path.join(os.environ['BOOST_INCLUDES'], 'boost',
                                   'version.hpp'))
      lines = ver_file.readlines()
      ver_file.close()
      for l in lines:
         match = boost_ver_re.search(l)
         if match is not None:
            ver_info = match.group(1).split('_')
            info = tuple(ver_info)
            break

   return info

def buildVersion(inputFile, exps):
   ver_file = open(inputFile)
   lines = ver_file.readlines()
   ver_file.close()

   nums = [0 for e in exps]

   for l in lines:
      i = 0
      while i < len(exps):
         match = exps[i].search(l)
         if match is not None:
            nums[i] = match.group(1)
            break
         i = i + 1

   return tuple(nums)

def getCppDOMVersion():
   exps = [
      re.compile(r'#define\s+CPPDOM_VERSION_MAJOR\s+(\d+)'),
      re.compile(r'#define\s+CPPDOM_VERSION_MINOR\s+(\d+)'),
      re.compile(r'#define\s+CPPDOM_VERSION_PATCH\s+(\d+)')
   ]

   info = (0, 0, 0)
   if os.environ.has_key('CPPDOM_INCLUDES'):
      info = buildVersion(os.path.join(os.environ['CPPDOM_INCLUDES'], 'cppdom',
                                       'version.h'),
                          exps)

   return info

def getGMTLVersion():
   exps = [
      re.compile(r'#define\s+GMTL_VERSION_MAJOR\s+(\d+)'),
      re.compile(r'#define\s+GMTL_VERSION_MINOR\s+(\d+)'),
      re.compile(r'#define\s+GMTL_VERSION_PATCH\s+(\d+)')
   ]

   nums = [0, 0, 0]
   if os.environ.has_key('GMTL_INCLUDES'):
      info = buildVersion(os.path.join(os.environ['GMTL_INCLUDES'], 'gmtl',
                                       'Version.h'),
                          exps)

   return info

def updateVersions(vcDir, options):
   class JugglerModule:
      def __init__(self, srcDir, vcDir, projDir, versionEnvVar, substVars,
                   genFiles = None):
         self.source_dir          = os.path.join(gJugglerDir, srcDir)
         self.version_params      = os.path.join(self.source_dir,
                                                 'Makefile.inc.in')
         self.version_file        = os.path.join(self.source_dir, 'VERSION')
         self.version_env_var     = versionEnvVar
         self.version_env_var_dot = versionEnvVar + '_DOT'
         self.subst_vars          = substVars
         self.param_files         = []
         self.proj_dir            = os.path.join(gJugglerDir, vcDir, projDir)

         if genFiles is not None:
            for f in genFiles:
               output = os.path.join(self.proj_dir, f[0])

               if len(f) == 1 or f[1] is None:
                  template = os.path.join(self.source_dir, f[0] + '.in')
               else:
                  template = f[1]

               self.param_files.append((output, template))

      def updateParamFiles(self):
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

      def getVersion(self, joinStr, versionInfo = None):
         if versionInfo is None:
            (version, major, minor, patch, build) = self.__getVersionInfo()
         else:
            (version, major, minor, patch, build) = versionInfo
         return '%d%s%d%s%d' % (major, joinStr, minor, joinStr, patch)

      def setVersionEnvVar(self):
         vi = self.__getVersionInfo()
         os.environ[self.version_env_var]     = self.getVersion('_', vi)
         os.environ[self.version_env_var_dot] = self.getVersion('.', vi)

      def removeOldVersions(self):
         output_files = []
         for ext in ['lib', 'dll', 'exp', 'ilk', 'pdb']:
            output_files += glob.glob(os.path.join(self.proj_dir, '*', '*',
                                                   '*.' + ext))

         mtime = os.path.getmtime
         for f in output_files:
            if mtime(self.version_file) > mtime(f):
               try:
                  os.remove(f)
               except:
                  print "Failed to remove", f

      version_re      = re.compile(r'((\d+)\.(\d+)\.(\d+)-(\d+))\s')
      branch_re       = re.compile(r'BRANCH\s*=\s*(.+)')
      canon_name_re   = re.compile(r'CANON_NAME\s*=\s*(\S.+)')
      subst_re        = re.compile(r'@(\w+)@')
      zero_strip_re   = re.compile(r'^0*([^0]\d+)')

      def __getVersionInfo(self):
         ver_file = open(self.version_file)
         cur_ver  = ver_file.readline()
         ver_file.close()
         ver_match = self.version_re.match(cur_ver)
         version = ver_match.group(1)
         major   = int(ver_match.group(2))
         minor   = int(ver_match.group(3))
         patch   = int(ver_match.group(4))
         build   = int(ver_match.group(5))

         return (version, major, minor, patch, build)

      def __genParamFile(self, output, template):
         (version, major, minor, patch, build) = self.__getVersionInfo()

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
               branch = match.group(1)
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

         subst_vars = self.subst_vars
         subst_vars['VER_NUMBER']         = version_number
         subst_vars['MAJOR_VER_NUMBER']   = str(major)
         subst_vars['MAJOR_VERSION']      = str(major)
         subst_vars['MINOR_VER_NUMBER']   = str(minor)
         subst_vars['MINOR_VERSION']      = str(minor)
         subst_vars['PATCH_VER_NUMBER']   = str(patch)
         subst_vars['MICRO_VERSION']      = str(patch)
         subst_vars['BUILD_VER_NUMBER']   = str(build)
         subst_vars['VER_STRING']         = version_string
         subst_vars['VERSION_DOT']        = '%d.%d.%d' % (major, minor, patch)
         subst_vars['SUBSYSTEM']          = 'NSPR'
         subst_vars['PLATFORM']           = 'Windows'
         subst_vars['data_subdir']        = 'share'
         subst_vars['USE_GCC']            = 'no'
         subst_vars['includedir']         = r'"${prefix}\include"'
         subst_vars['libdir']             = r'${exec_prefix}\lib'

         try:
            input_file  = open(template, 'r')
            input_lines = input_file.readlines()
            input_file.close()

            for i in xrange(len(input_lines)):
               input_lines[i] = \
                  self.subst_re.sub(lambda m: subst_vars.get(m.group(1), ''),
                                    input_lines[i])

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
   rt_part = ""

   vpr_subst_vars = {}
   vpr_subst_vars['vpr_cxxflags'] = '/DBOOST_ALL_DYN_LINK /DCPPDOM_DYN_LINK /EHsc /GR'
   vpr_subst_vars['vpr_ldflags'] = r'/libpath:"$libdir"'
   vpr_subst_vars['vpr_libs'] = ''
   #vpr_subst_vars['subsystem_libs'] = 'libnspr4.lib libplc4.lib'
   vpr_subst_vars['subsystem_libs'] = ''
   vpr_subst_vars['BOOST_ROOT'] = r'${fp_file_cwd}\..\..'
   vpr_subst_vars['BOOST_VERSION_DOT'] = '.'.join(getBoostVersion())
   vpr_subst_vars['BOOST_INCLUDES'] = r'/I"${prefix}\include"'
   vpr_subst_vars['BOOST_LDFLAGS'] = r'/libpath:"${prefix}\lib"'
   vpr_subst_vars['CPPDOM_VERSION'] = '.'.join(getCppDOMVersion())
   vpr_module = JugglerModule(r'modules\vapor', vcDir, 'VPR', 'VPR_VERSION',
                              vpr_subst_vars,
                              [(r'vpr\vprParam.h',), ('vpr.fpc',),
                               ('boost.fpc',), ('boost_system.fpc',),
                               ('boost_filesystem.fpc',),
                               ('boost_signals.fpc',),
                               (r'vpr\version.rc',
                                os.path.join(gJugglerDir, 'version.rc.in'))])
   mods.append(vpr_module)

   # XXX: These are pretty weak assumptions.
   tweek_have_cxx  = options.get('OMNIORB_ROOT', '') != ''
   tweek_have_java = options.get('JAVA_HOME', '') != ''

   tweek_jars = []
   hack_jars = ['looks.jar', 'liquidlnf.jar', 'metouia.jar']
   for j in gTweekJars + hack_jars + gJdomJars:
      tweek_jars.append(os.path.join('${prefix}', 'share', 'tweek', 'java', j))

   tweek_ext_jars = []
   for j in gTweekExtJars:
      tweek_ext_jars.append(os.path.join('${prefix}', 'share', 'tweek',
                                         'java', j))

   tweek_subst_vars = {}
   tweek_subst_vars['tweek_cxxflags'] = '/EHsc /GR'
   if tweek_have_cxx:
      tweek_subst_vars['tweek_cxxflags'] += ' /DTWEEK_HAVE_CXX /D__WIN32__=1 /D__x86__=1 /D__NT__=1 /D__OSVERSION__=5 /DUSE_core_stub_in_nt_dll /DUSE_core_stub_in_nt_dll_NOT_DEFINED_Subject /I"$prefix\\include\\tweek\\idl"'
      tweek_subst_vars['tweek_extra_libs'] = \
         'omnithread%s_rt.lib omniORB%s_rt.lib omniDynamic%s_rt.lib' % \
            (os.environ['OMNITHREAD_VERSION'], os.environ['OMNIORB_VERSION'],
             os.environ['OMNIORB_VERSION'])

   tweek_subst_vars['tweek_ldflags'] = r'/libpath:"$libdir"'
   tweek_subst_vars['tweek_libs'] = ''
   tweek_subst_vars['tweek_idlflags_java'] = r'-I"$prefix\include"'
   tweek_subst_vars['tweek_idlflags_cxx'] = r'-bcxx -Wbh=.h,s=.cpp -I"$prefix\include"'
   tweek_subst_vars['tweek_idl_inc_flag_java'] = '-I'
   tweek_subst_vars['tweek_idl_inc_flag_cxx'] = '-I'
   tweek_subst_vars['tweek_idl_inc_flag_python'] = '-I'
   tweek_subst_vars['tweek_idlgendir_java'] = '-d '
   tweek_subst_vars['tweek_idlgendir_cxx'] = '-C'
   tweek_subst_vars['tweek_idlgendir_python'] = '-C'
   tweek_subst_vars['tweek_java_api_jars'] = ';'.join(tweek_jars)
   tweek_subst_vars['tweek_ext_jars'] = ';'.join(tweek_ext_jars)
   #tweek_subst_vars['CXX_ORB_DEPS'] = 'omniORB4 omnithread3 omniDynamic4'
   tweek_subst_vars['VPR_VERSION'] = vpr_module.getVersion('.')

   if tweek_have_cxx:
      tweek_subst_vars['BUILD_CXX'] = 'Y'
   else:
      tweek_subst_vars['BUILD_CXX'] = 'N'

   if tweek_have_java:
      tweek_subst_vars['BUILD_JAVA'] = 'Y'
   else:
      tweek_subst_vars['BUILD_JAVA'] = 'N'

   tweek_subst_vars['BUILD_PYTHON_IDL'] = 'N'
   tweek_module = JugglerModule(r'modules\tweek', vcDir, 'Tweek_CXX',
                                'TWEEK_VERSION', tweek_subst_vars,
                                [(r'tweek\tweekParam.h',), ('tweek.fpc',),
                                 ('tweek-java.fpc',), ('tweek-python.fpc',),
                                 (r'tweek\version.rc',
                                  os.path.join(gJugglerDir, 'version.rc.in'))])
   mods.append(tweek_module)

   jccl_jars = []
   for j in gJcclJars + gJcclRtrcJars:
      jccl_jars.append(os.path.join('${prefix}', 'share', 'jccl', 'java', j))

   jccl_subst_vars = {}
   jccl_subst_vars['jccl_cxxflags'] = '/EHsc /GR'
   jccl_subst_vars['jccl_ldflags'] = r'/libpath:"$libdir"'
   jccl_subst_vars['jccl_libs'] = ''
   jccl_subst_vars['BUILD_CXX'] = tweek_subst_vars['BUILD_CXX']
   jccl_subst_vars['BUILD_JAVA'] = tweek_subst_vars['BUILD_JAVA']
   jccl_subst_vars['jccl_java_api_jars'] = ';'.join(jccl_jars)
   jccl_subst_vars['VPR_VERSION'] = vpr_module.getVersion('.')
   jccl_module = JugglerModule(r'modules\jackal', vcDir, 'JCCL', 'JCCL_VERSION',
                               jccl_subst_vars,
                               [(r'jccl\jcclParam.h',
                                 os.path.join(gJugglerDir,
                                              r'modules\jackal\common\jccl\jcclParam.h.in')),
                                ('jccl.fpc',),
                                (r'jccl\version.rc',
                                 os.path.join(gJugglerDir, 'version.rc.in'))
                               ])
   mods.append(jccl_module)

   snx_subst_vars = {}
   snx_subst_vars['snx_cxxflags'] = '/EHsc /GR'
   snx_subst_vars['snx_ldflags'] = r'/libpath:"$libdir"'
   snx_subst_vars['snx_libs'] = ''
   snx_subst_vars['VPR_VERSION'] = vpr_module.getVersion('.')
   snx_subst_vars['MIN_GMTL_VERSION'] = '.'.join(getGMTLVersion())
   snx_module = JugglerModule(r'modules\sonix', vcDir, 'Sonix', 'SNX_VERSION',
                              snx_subst_vars,
                              [(r'snx\snxParam.h',), ('sonix.fpc',),
                               (r'snx\version.rc',
                                os.path.join(gJugglerDir, 'version.rc.in'))])
   mods.append(snx_module)

   gadget_subst_vars = {}
   gadget_subst_vars['gadget_cxxflags'] = '/EHsc /GR'
   gadget_subst_vars['gadget_ldflags'] = r'/libpath:"$libdir"'
   gadget_subst_vars['gadget_libs'] = ''
   gadget_subst_vars['gadget_extra_libs'] = \
      'comctl32.lib ws2_32.lib user32.lib'
   gadget_subst_vars['VPR_VERSION'] = jccl_subst_vars['VPR_VERSION']
   gadget_subst_vars['JCCL_VERSION'] = jccl_module.getVersion('.')
   gadget_subst_vars['MIN_GMTL_VERSION'] = snx_subst_vars['MIN_GMTL_VERSION']
   gadget_module = JugglerModule(r'modules\gadgeteer', vcDir, 'Gadgeteer',
                                 'GADGET_VERSION', gadget_subst_vars,
                                 [(r'gadget\gadgetParam.h',),
                                  ('gadgeteer.fpc',),
                                  (r'gadget\version.rc',
                                   os.path.join(gJugglerDir, 'version.rc.in'))
                                 ])
   mods.append(gadget_module)

   vrj_subst_vars = {}
   vrj_subst_vars['vrj_cxxflags'] = '/EHsc /GR'
   vrj_subst_vars['vrj_ldflags'] = r'/libpath:"$libdir"'
   vrj_subst_vars['vrj_libs'] = ''
   vrj_subst_vars['vrj_d3d_lib'] = ''
   vrj_subst_vars['vrj_ogl_lib'] = ''
   vrj_subst_vars['vrj_pf_lib'] = ''
   vrj_subst_vars['vrj_d3d_extra_libs'] = \
      'd3dxof.lib dxguid.lib d3dx9d.lib d3d9.lib winmm.lib'
   vrj_subst_vars['vrj_ogl_extra_libs'] = 'opengl32.lib glu32.lib'
   vrj_subst_vars['vrj_pf_extra_libs'] = \
      '/libpath:"${PFROOT}\lib" libpf.lib libpfdu-util.lib libpfui.lib opengl32.lib glu32.lib'
   vrj_subst_vars['VPR_VERSION'] = jccl_subst_vars['VPR_VERSION']
   vrj_subst_vars['JCCL_VERSION'] = gadget_subst_vars['JCCL_VERSION']
   vrj_subst_vars['SNX_VERSION'] = snx_module.getVersion('.')
   vrj_subst_vars['GADGET_VERSION'] = gadget_module.getVersion('.')
   vrj_subst_vars['BOOST_ROOT'] = r'${fp_file_cwd}\..\..'
   vrj_subst_vars['BOOST_VERSION_DOT'] = '.'.join(getBoostVersion())
   vrj_subst_vars['BOOST_INCLUDES'] = r'/I"${prefix}\include"'
   vrj_subst_vars['BOOST_LDFLAGS'] = r'/libpath:"${prefix}\lib"'
   vrj_module = JugglerModule(r'modules\vrjuggler', vcDir, 'VRJuggler',
                             'VRJ_VERSION', vrj_subst_vars,
                             [(r'vrj\vrjParam.h',), ('vrjuggler.fpc',),
                              ('vrjuggler-direct3d.fpc',),
                              ('vrjuggler-opengl.fpc',),
                              ('vrjuggler-performer.fpc',),
                              ('boost_program_options.fpc',),
                              (r'vrj\version.rc',
                               os.path.join(gJugglerDir, 'version.rc.in'))])
   mods.append(vrj_module)

   for m in mods:
      m.setVersionEnvVar()
      m.updateParamFiles()
      m.removeOldVersions()

def generateAntBuildFiles(vcDir):
   class AntTarget:
      def __init__(self, srcdir, vcDir, moduleName, outputFile = 'build.xml',
                   topSubDir = None):
         '''
         __init__(srcdir, vcDir, moduleName, outputFile, topSubDir)
         Arguments:
         srcdir     -- The location of the Java source to be compiled.
         vcDir      -- The root of the directory tree containing the Visual
                       C++ build system.
         moduleName -- The name of the Visual C++ project (under the vcDir
                       subdirectory) associated with this Ant build.  When
                       concatenated to vcDir, this is where the the .class
                       file(s) and the .jar file(s) will be created.
         outputFile -- The name of the Ant build file to generate.  If not
                       specified, this defaults to 'build.xml'.
         topSubDir  -- The root of the directory where all the work will be
                       done.  This is needed for Ant builds that depend on
                       previously constructed JAR files that will most likely
                       exist somewhere in the vcDir directory tree.  This
                       string is used as part of the replacment for the
                       string @topdir@ in the source build.xml.in file.  If
                       not specified, this defaults to whatever value is
                       passed in for vcDir.
         '''
         if topSubDir is None:
            topSubDir = vcDir

         self.srcdir      = os.path.join(gJugglerDir, srcdir)
         self.topdir      = os.path.join(gJugglerDir, topSubDir)
         self.module_name = os.path.join(gJugglerDir, vcDir, moduleName)
         self.output_file = os.path.join(self.module_name, outputFile)

         if not os.path.exists(self.module_name):
            os.mkdir(self.module_name)
         elif not os.path.isdir(self.module_name):
            printStatus("ERROR: %s exists, but it is not a directory!" % self.module_name)
            sys.exit(EXIT_STATUS_INVALID_PATH)

         self.tweek_jars = []
         for j in gTweekJars:
            self.tweek_jars.append(os.path.join(gJugglerDir, vcDir,
                                                'Tweek_Java', j))

         self.tweek_ext_jars = []
         for j in gTweekExtJars:
            self.tweek_jars.append(os.path.join(gJugglerDir, vcDir,
                                                'Tweek_Java', j))

         self.jccl_jars = []
         for j in gJcclJars:
            self.jccl_jars.append(os.path.join(gJugglerDir, vcDir,
                                               'JCCL_Java', j))

         self.jccl_rtrc_jars = []
         for j in gJcclRtrcJars:
            self.jccl_jars.append(os.path.join(gJugglerDir, vcDir,
                                               'JCCL_Java',
                                               'RTRC_Plugin_Java', j))

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

      jdom_jars = []
      jdom_root = os.path.join(gJugglerDir, 'external', 'jdom')
      for j in gJdomJars:
         if j == 'jdom.jar':
            jdom_jars.append(os.path.join(jdom_root, 'build', j))
         else:
            jdom_jars.append(os.path.join(jdom_root, 'lib', j))

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
   mods.append(AntTarget(r'modules\tweek\java', vcDir, 'Tweek_Java'))
   mods.append(AntTarget(r'modules\tweek\extensions\java', vcDir,
                         'Tweek_Java', 'build-ext.xml'))
   mods.append(AntTarget(r'modules\jackal\config', vcDir, 'JCCL_Java',
                         'build-config.xml'))
   mods.append(AntTarget(r'modules\jackal\editors', vcDir, 'JCCL_Java',
                         'build-editors.xml'))
   mods.append(AntTarget(r'modules\jackal\plugins\corba_rtrc', vcDir,
                         r'JCCL_Java\RTRC_Plugin_Java', 'build.xml'))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig', vcDir, 'VRJConfig',
                         'build.xml', os.path.join(vcDir, 'VRJConfig')))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\commoneditors',
                         vcDir, r'VRJConfig\commoneditors',
                         'build-commoneditors.xml'))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\cave',
                         vcDir, 'VRJConfig', 'build-cave.xml',
                         os.path.join(vcDir, 'VRJConfig')))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\display_window',
                         vcDir, 'VRJConfig', 'build-display_window.xml',
                         os.path.join(vcDir, 'VRJConfig')))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\flock',
                         vcDir, 'VRJConfig', 'build-flock.xml',
                         os.path.join(vcDir, 'VRJConfig')))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\intersense',
                         vcDir, 'VRJConfig', 'build-intersense.xml',
                         os.path.join(vcDir, 'VRJConfig')))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\motionstar',
                         vcDir, 'VRJConfig', 'build-motionstar.xml',
                         os.path.join(vcDir, 'VRJConfig')))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\pinchglove',
                         vcDir, 'VRJConfig', 'build-pinchglove.xml',
                         os.path.join(vcDir, 'VRJConfig')))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\proxyeditor',
                         vcDir, 'VRJConfig', 'build-proxyeditor.xml',
                         os.path.join(vcDir, 'VRJConfig')))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\customeditors\surfacedisplayeditor',
                         vcDir, 'VRJConfig', 'build-surfacedisplayeditor.xml',
                         os.path.join(vcDir, 'VRJConfig')))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\wizards\cluster',
                         vcDir, 'VRJConfig', 'build-wizard-cluster.xml',
                         os.path.join(vcDir, 'VRJConfig')))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\wizards\newdevice',
                         vcDir, 'VRJConfig', 'build-wizard-newdevice.xml',
                         os.path.join(vcDir, 'VRJConfig')))
   mods.append(AntTarget(r'modules\vrjuggler\vrjconfig\wizards\vrsystem',
                         vcDir, 'VRJConfig', 'build-wizard-vrsystem.xml',
                         os.path.join(vcDir, 'VRJConfig')))
   mods.append(AntTarget(r'modules\vrjuggler\plugins\corba_perf_mon',
                         vcDir, r'VRJugglerPlugins\Perf_Plugin_Java',
                         'build.xml'))

   for m in mods:
      m.generateBuildFile()

def doInstall(prefix, buildDir):
   makeTree(prefix)
   installExternal(prefix, buildDir)
   installVPR(prefix, buildDir)
   installTweek(prefix, buildDir)
   installTweekJava(prefix, buildDir)
   installJCCL(prefix, buildDir)
   installJCCLJava(prefix, buildDir)
   installJCCLPlugins(prefix, buildDir)
   installJCCLPluginsJava(prefix, buildDir)
   installSonix(prefix, buildDir)
   installSonixPlugins(prefix, buildDir)
   installGadgeteer(prefix, buildDir)
   installGadgeteerDrivers(prefix, buildDir)
   installGadgeteerPlugins(prefix, buildDir)
   installVRJuggler(prefix, buildDir)
   installVRJConfig(prefix, buildDir)
   installVRJugglerPlugins(prefix, buildDir)
   installVRJugglerPluginsJava(prefix, buildDir)
   installMsvcRT(prefix)

def mkinstalldirs(dir):
#   print "Checking for", dir
   if not os.path.exists(dir):
      (head, tail) = os.path.split(dir)
      if len(head) > 0:
         mkinstalldirs(head)
      os.mkdir(dir)

def makeTree(prefix):
   mkinstalldirs(os.path.join(prefix, 'bin'))
   mkinstalldirs(os.path.join(prefix, 'include'))
   mkinstalldirs(os.path.join(prefix, 'lib'))
   mkinstalldirs(os.path.join(prefix, 'lib', 'flagpoll'))
   mkinstalldirs(os.path.join(prefix, 'share'))

def smartCopy(srcfile, dst):
   """ Only copy file if it has changed, and delete it first.
       Drop in replacement for shutil.copy2.
       srcfile - Full path to source file to copy.
       dst - Destination filename or directory.
   """
   if os.path.isdir(dst):
      dst = os.path.join(dst, os.path.basename(srcfile))

   # Verify we need to copy and make sure to delete if needed
   if os.path.isfile(dst):
      stat_src = os.stat(srcfile)
      stat_dst = os.stat(dst)
      if (stat_src.st_size == stat_dst.st_size) and \
         (stat_src.st_mtime == stat_dst.st_mtime):
         #print "skipping: ", dst
         return   # File doesn't need to be copied
      #print "removing: ", dst
      os.remove(dst)

   # Copy it
   shutil.copy2(srcfile, dst)

def installDir(startDir, destDir, allowedExts = None, disallowedExts = None,
               disallowedFiles = None):
   #print "   %s ==> %s"%(startDir, destDir)
   cwd = os.getcwd()

   # Make sure that destDir specifies an absolute path.
   if not os.path.isabs(destDir):
      destDir = os.path.abspath(destDir)

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

   skip_dirs = ['.svn', 'CVS', 'autom4te.cache']
   for f in contents:
      if os.path.isdir(f):
         if f in skip_dirs:
            continue

         start_dir = os.path.join(startDir, f)
         dest_dir  = os.path.join(destDir, f)
         installDir(start_dir, dest_dir, allowedExts, disallowedExts,
                    disallowedFiles)
      else:
         try:
            (root, f_ext) = os.path.splitext(f)
            if allowedExts is None:
               if f_ext not in disallowedExts:
                  smartCopy(f, pj(destDir,f))
            elif f_ext in allowedExts:
               if f not in disallowedFiles:
                  smartCopy(f, pj(destDir,f))
         except (IOError, os.error), why:
            print "Can't copy %s to %s: %s" % (f, destDir, str(why))

   os.chdir(cwd)

def installLibs(srcRoot, destdir,
                buildPlatforms = ['Win32', 'x64'],
                buildTypes = [('ReleaseDLL',), ('DebugDLL', 'debug'),
                              ('DebugRtDLL',)],
                extensions = ['.dll', '.lib', '.exp']):
   for p in buildPlatforms:
      for t in buildTypes:
         build_dir = t[0]
         if len(t) == 2:
            cur_destdir = os.path.join(destdir, t[1])
         else:
            cur_destdir = destdir

         srcdir = os.path.join(srcRoot, p, build_dir)
         if os.path.exists(srcdir):
            installDir(srcdir, cur_destdir, extensions)

def installExternal(prefix, buildDir):
   pass

def installVPR(prefix, buildDir):
   printStatus("Installing VPR headers and libraries ...")

   destdir = os.path.join(prefix, 'include', 'vpr')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'vapor', 'vpr')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(buildDir, 'VPR', 'vpr')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(buildDir, 'VPR')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'lib', 'flagpoll')
   fpc_file = os.path.join(buildDir, 'VPR', 'vpr.fpc')
   smartCopy(fpc_file, destdir)

   destdir = os.path.join(prefix, 'share', 'vpr', 'test')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'vapor', 'test')
   installDir(srcdir, destdir, None, ['.in'])

   # Install additional files into <prefix>\share\vpr
   destdir = os.path.join(prefix, 'share', 'vpr')
   srcroot = os.path.join(gJugglerDir, 'modules', 'vapor')

   smartCopy(os.path.join(gJugglerDir, 'COPYING.txt'), destdir)
   extra_files = ['ChangeLog', 'README.txt', 'RELEASE_NOTES.txt']
   for f in extra_files:
      smartCopy(os.path.join(srcroot, f), destdir)

def installTweek(prefix, buildDir):
   printStatus("Installing Tweek C++ headers, libraries, and data files ...")

   destdir = os.path.join(prefix, 'include', 'tweek')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'tweek', 'tweek')
   installDir(srcdir, destdir, ['.h', '.idl'])

   srcdir  = os.path.join(buildDir, 'Tweek_CXX', 'tweek')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(buildDir, 'Tweek_CXX')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'lib', 'flagpoll')
   smartCopy(os.path.join(buildDir, 'Tweek_CXX', 'tweek.fpc'), destdir)
   smartCopy(os.path.join(buildDir, 'Tweek_CXX', 'tweek-java.fpc'), destdir)
   smartCopy(os.path.join(buildDir, 'Tweek_CXX', 'tweek-python.fpc'), destdir)

   destdir = os.path.join(prefix, 'share', 'tweek', 'test')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'tweek', 'test')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'tweek', 'data')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'tweek', 'data')
   installDir(srcdir, destdir)

   # Install additional files into <prefix>\share\tweek
   destdir = os.path.join(prefix, 'share', 'tweek')
   srcroot = os.path.join(gJugglerDir, 'modules', 'tweek')

   smartCopy(os.path.join(gJugglerDir, 'COPYING.txt'), destdir)
   extra_files = ['ChangeLog', 'RELEASE_NOTES.txt']
   for f in extra_files:
      smartCopy(os.path.join(srcroot, f), destdir)

def installTweekJava(prefix, buildDir):
   srcdir = os.path.join(buildDir, 'Tweek_Java')

   if os.path.exists(os.path.join(srcdir, gTweekJars[0])):
      printStatus("Installing Tweek Java libraries and data files ...")

      beans     = ['Viewers']
      ext_beans = []

      destdir = os.path.join(prefix, 'share', 'tweek', 'java')
      mkinstalldirs(destdir)

      # Install the base JAR files that make up the Tweek Java API.
      for j in gTweekJars + gTweekExtJars:
         smartCopy(os.path.join(srcdir, j), destdir)

      # Install the tweek_jni DLL.
      for p in ['Win32', 'x64']:
         dll = os.path.join(srcdir, 'tweek_jni', p, 'ReleaseDLL',
                            'tweek_jni.dll')
         if os.path.exists(dll):
            arch = os.environ['PROCESSOR_ARCHITECTURE']
            destdir = os.path.join(destdir, arch)
            mkinstalldirs(destdir)
            smartCopy(dll, destdir)

      destdir = os.path.join(prefix, 'share', 'tweek', 'beans')
      mkinstalldirs(destdir)

      bean_srcdir = srcdir
      xml_srcdir  = os.path.join(gJugglerDir, 'modules', 'tweek', 'java')

      # Install the standard Tweek Beans.
      for b in beans:
         jar = b + '.jar'
         xml = b + '.xml'
         smartCopy(os.path.join(bean_srcdir, jar), destdir)
         smartCopy(os.path.join(xml_srcdir, xml), destdir)

      xml_srcdir  = os.path.join(gJugglerDir, 'modules', 'tweek', 'extensions',
                                 'java')

      # Install the extension Tweek Beans.
      for b in ext_beans:
         jar = b + '.jar'
         xml = b + '.xml'
         smartCopy(os.path.join(bean_srcdir, jar), destdir)
         smartCopy(os.path.join(xml_srcdir, xml), destdir)

      # Install tweek.bat.
      srcdir = os.path.join(gJugglerDir, 'modules', 'tweek', 'java')
      destdir = os.path.join(prefix, 'bin')
      smartCopy(os.path.join(srcdir, 'tweek.bat'), destdir)

      # Install JacORB IDL compiler.
      srcdir = os.path.join(gJugglerDir, 'external', 'JacORB')
      installDir(srcdir, destdir, ['.jar'])
      smartCopy(os.path.join(srcdir, 'idl.bat'), destdir)

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
         smartCopy(os.path.join(srcroot, j), destdir)
   else:
      printStatus("Tweek Java API not built.  Skipping.")

def installJCCL(prefix, buildDir):
   printStatus("Installing JCCL C++ headers, libraries, and tools ...")

   destdir = os.path.join(prefix, 'include', 'jccl')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'jackal', 'common', 'jccl')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(gJugglerDir, 'modules', 'jackal', 'config', 'jccl')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(gJugglerDir, 'modules', 'jackal', 'rtrc', 'jccl')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(buildDir, 'JCCL', 'jccl')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(buildDir, 'JCCL')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'lib', 'flagpoll')
   smartCopy(os.path.join(buildDir, 'JCCL', 'jccl.fpc'), destdir)

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
   smartCopy(os.path.join(srcdir, 'configuration.xsd'), destdir)

   destdir = os.path.join(schema_root, 'www.vrjuggler.org', 'jccl', 'xsd',
                          '3.1')
   mkinstalldirs(destdir)
   smartCopy(os.path.join(srcdir, 'definition.xsd'), destdir)

   destdir = schema_root
   srcdir  = os.path.join(gJugglerDir, 'modules', 'jackal', 'data',
                          'stdschemas')
   installDir(srcdir, destdir)

   # Install additional files into <prefix>\share\jccl
   destdir = os.path.join(prefix, 'share', 'jccl')
   srcroot = os.path.join(gJugglerDir, 'modules', 'jackal')

   smartCopy(os.path.join(gJugglerDir, 'COPYING.txt'), destdir)
   extra_files = ['ChangeLog', 'RELEASE_NOTES.txt']
   for f in extra_files:
      smartCopy(os.path.join(srcroot, f), destdir)

def installJCCLPlugins(prefix, buildDir):
   printStatus("Installing JCCL C++ plug-ins ...")

   destdir = os.path.join(prefix, 'lib', 'jccl', 'plugins')
   srcroot = os.path.join(buildDir, 'JCCL', 'RTRC_Plugin_CXX')
   installLibs(srcroot, destdir, extensions = ['.dll', '.exp'])

def installJCCLJava(prefix, buildDir):
   srcdir = os.path.join(buildDir, 'JCCL_Java')

   if os.path.exists(os.path.join(srcdir, 'jccl_config.jar')):
      printStatus("Installing JCCL Java libraries and data files ...")

      destdir = os.path.join(prefix, 'share', 'jccl', 'beans')
      mkinstalldirs(destdir)

      jars = [
         'jccl_config.jar',
         'jccl_editors.jar'
      ]

      for j in jars:
         smartCopy(os.path.join(srcdir, j), destdir)

      srcdir = os.path.join(gJugglerDir, 'modules', 'jackal', 'config')
      smartCopy(os.path.join(srcdir, 'jccl_config.xml'), destdir)

      # Install dependencies.
      dep_jars = [
         r'TableLayout\TableLayout.jar'
      ]

      destdir = os.path.join(prefix, 'share', 'jccl', 'java')
      srcroot = os.path.join(gJugglerDir, 'external')

      mkinstalldirs(destdir)
      for j in dep_jars:
         smartCopy(os.path.join(srcroot, j), destdir)
   else:
      printStatus("JCCL Java API not built.  Skipping.")

def installJCCLPluginsJava(prefix, buildDir):
   srcdir = os.path.join(buildDir, 'JCCL_Java', 'RTRC_Plugin_Java')

   if os.path.exists(os.path.join(srcdir, gJcclRtrcJars[0])):
      printStatus("Installing JCCL Java plug-ins ...")

      destdir = os.path.join(prefix, 'share', 'jccl', 'beans')
      for j in gJcclRtrcJars:
         smartCopy(os.path.join(srcdir, j), destdir)

      srcdir = os.path.join(gJugglerDir, 'modules', 'jackal', 'plugins',
                            'corba_rtrc')
      smartCopy(os.path.join(srcdir, 'jccl_rtrc.xml'), destdir)
   else:
      printStatus("JCCL Java plug-ins not built.  Skipping.")

def installSonix(prefix, buildDir):
   printStatus("Installing Sonix headers, libraries, and samples ...")

   destdir = os.path.join(prefix, 'include', 'snx')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'sonix', 'snx')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(buildDir, 'Sonix', 'snx')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(buildDir, 'Sonix')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'lib', 'flagpoll')
   smartCopy(os.path.join(buildDir, 'Sonix', 'sonix.fpc'), destdir)

   destdir = os.path.join(prefix, 'share', 'sonix', 'samples')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'sonix', 'samples')
   installDir(srcdir, destdir, None, ['.in'])

   destdir = os.path.join(prefix, 'share', 'sonix', 'data')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'sonix', 'data')
   installDir(srcdir, destdir)

   # Install additional files into <prefix>\share\sonix
   destdir = os.path.join(prefix, 'share', 'sonix')
   srcroot = os.path.join(gJugglerDir, 'modules', 'sonix')

   smartCopy(os.path.join(gJugglerDir, 'COPYING.txt'), destdir)
   extra_files = ['ChangeLog', 'README.txt']
   for f in extra_files:
      smartCopy(os.path.join(srcroot, f), destdir)

def installSonixPlugins(prefix, buildDir):
   printStatus("Installing Sonix plug-ins ...")

   destdir = os.path.join(prefix, 'lib', 'sonix', 'plugins')
   plugins = ['OpenAL', 'Audiere']
   for p in plugins:
      installLibs(os.path.join(buildDir, 'Sonix', p), destdir,
                  buildTypes = [('ReleaseDLL', 'opt'), ('DebugDLL', 'dbg'),
                                ('DebugRtDll', 'dbgrt')],
                  extensions = ['.dll', '.exp'])

def installGadgeteer(prefix, buildDir):
   printStatus("Installing Gadgeteer headers, libraries, and samples ...")

   destdir = os.path.join(prefix, 'include', 'gadget')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'gadgeteer', 'gadget')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(buildDir, 'Gadgeteer', 'gadget')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'include', 'cluster')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'gadgeteer', 'cluster')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(buildDir, 'Gadgeteer')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'lib', 'flagpoll')
   smartCopy(os.path.join(buildDir, 'Gadgeteer', 'gadgeteer.fpc'), destdir)

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

   smartCopy(os.path.join(gJugglerDir, 'COPYING.txt'), destdir)
   extra_files = ['ChangeLog', 'RELEASE_NOTES.txt']
   for f in extra_files:
      smartCopy(os.path.join(srcroot, f), destdir)

def installGadgeteerDrivers(prefix, buildDir):
   printStatus("Installing Gadgeteer device drivers ...")

   destdir = os.path.join(prefix, 'lib', 'gadgeteer', 'drivers')
   srcroot = os.path.join(buildDir, 'Gadgeteer')

   drivers = ['DTrack', 'DataGlove', 'DirectXJoystick', 'Ether24', 'Fastrak',
              'Flock', 'IBox', 'IntersenseAPI', 'IS900', 'MotionNode',
              'MotionStar', 'MSFTSpeechRecognition', 'PinchGlove',
              'SerialEncoder', 'SpaceBall', 'TrackdAPI', 'VRPN', 'Wanda',
              'X-IST']

   for d in drivers:
      srcdir = os.path.join(srcroot, d)
      installLibs(srcdir, destdir, extensions = ['.dll', '.exp'])

   if os.environ['FTD2XX_ROOT'] != "":
      if gBuild64:
         srcdir = os.path.join(os.environ['FTD2XX_ROOT'], 'amd64')
      else:
         srcdir = os.path.join(os.environ['FTD2XX_ROOT'], 'i386')

      printStatus("Installing FTD2XX DLLs")
      destdir = os.path.join(prefix, 'bin')

      ftd2xx_dlls = glob.glob(os.path.join(srcdir, '*.dll'))
      for d in ftd2xx_dlls:
         smartCopy(d, destdir)

def installGadgeteerPlugins(prefix, buildDir):
   printStatus("Installing Gadgeteer cluster plug-ins ...")

   destdir = os.path.join(prefix, 'include', 'plugins',
                          'ApplicationDataManager')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'gadgeteer', 'plugins',
                          'ApplicationDataManager')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib', 'gadgeteer', 'plugins')
   srcroot = os.path.join(buildDir, 'Gadgeteer')

   plugins = ['ApplicationDataManager', 'RemoteInputManager']

   for p in plugins:
      srcdir = os.path.join(srcroot, p)
      installLibs(srcdir, destdir, extensions = ['.dll', '.exp'])

def installVRJuggler(prefix, buildDir):
   printStatus("Installing VR Juggler headers, libraries, and samples ...")

   destdir = os.path.join(prefix, 'include', 'vrj')
   srcdir  = os.path.join(gJugglerDir, 'modules', 'vrjuggler', 'vrj')
   installDir(srcdir, destdir, ['.h'])

   srcdir  = os.path.join(buildDir, 'VRJuggler', 'vrj')
   installDir(srcdir, destdir, ['.h'])

   destdir = os.path.join(prefix, 'lib')
   srcroot = os.path.join(buildDir, 'VRJuggler')
   installLibs(srcroot, destdir)

   srcroot = os.path.join(buildDir, 'VRJuggler', 'OpenGL_Draw_Manager')
   installLibs(srcroot, destdir)

   srcroot = os.path.join(buildDir, 'VRJuggler', 'Performer_Draw_Manager')
   installLibs(srcroot, destdir)

   srcroot = os.path.join(buildDir, 'VRJuggler', 'Direct3D_Draw_Manager')
   installLibs(srcroot, destdir)

   destdir = os.path.join(prefix, 'lib', 'flagpoll')
   smartCopy(os.path.join(buildDir, 'VRJuggler', 'vrjuggler.fpc'), destdir)
   smartCopy(os.path.join(buildDir, 'VRJuggler', 'vrjuggler-direct3d.fpc'),
             destdir)
   smartCopy(os.path.join(buildDir, 'VRJuggler', 'vrjuggler-opengl.fpc'),
             destdir)
   if os.getenv('PFROOT', '') != '':
      smartCopy(os.path.join(buildDir, 'VRJuggler', 'vrjuggler-performer.fpc'),
                destdir)

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

   smartCopy(os.path.join(gJugglerDir, 'COPYING.txt'), destdir)
   extra_files = ['ChangeLog', 'RELEASE_NOTES.txt']
   for f in extra_files:
      smartCopy(os.path.join(srcroot, f), destdir)

def installVRJConfig(prefix, buildDir):
   jardir = os.path.join(buildDir, 'VRJConfig')

   if os.path.exists(os.path.join(jardir, 'VRJConfig.jar')):
      printStatus("Installing VRJConfig ...")

      vrjconfig_src = os.path.join(gJugglerDir, 'modules', 'vrjuggler',
                                   'vrjconfig')
      bean_jars = [
         'VRJConfig.jar'
      ]

      common_editors = glob.glob(os.path.join(jardir, 'commoneditors', '*.jar'))

      custom_editor_src = os.path.join(vrjconfig_src, 'customeditors')
      custom_editors = [
         ('cave', 'CaveEditor'),
         ('display_window', 'DisplayWindowEditor'),
         ('flock', 'FlockEditor'),
         ('intersense', 'IntersenseEditor'),
         ('motionstar', 'MotionStarEditor'),
         ('pinchglove', 'PinchGloveEditor'),
         ('proxyeditor', 'ProxyEditor'),
         ('surfacedisplayeditor', 'SurfaceDisplayEditor')
      ]

      wizard_src = os.path.join(vrjconfig_src, 'wizards')
      wizards = [
         ('cluster', 'ClusterWizard'),
         ('newdevice', 'NewDeviceWizard'),
         ('vrsystem', 'VRSystemWizard'),
      ]

      # Install JAR files that act as reusable Java class libraries.
      destdir = os.path.join(prefix, 'share', 'vrjuggler', 'java')
      mkinstalldirs(destdir)
      for j in common_editors:
         jar_file = os.path.join(jardir, j)
         if os.path.exists(jar_file):
            smartCopy(jar_file, destdir)

      # Install the base set of VRJConfig JavaBeans.
      destdir = os.path.join(prefix, 'share', 'vrjuggler', 'beans')
      mkinstalldirs(destdir)
      for j in bean_jars:
         jar_file = os.path.join(jardir, j)
         if os.path.exists(jar_file):
            smartCopy(jar_file, destdir)

      smartCopy(os.path.join(vrjconfig_src, 'VRJConfig.xml'), destdir)

      # Install any custom editors that were compiled.
      destdir = os.path.join(prefix, 'share', 'vrjuggler', 'beans',
                             'customeditors')
      mkinstalldirs(destdir)
      for e in custom_editors:
         jar_file = os.path.join(jardir, e[1] + '.jar')
         xml_file = os.path.join(custom_editor_src, e[0], e[1] + '.xml')
         if os.path.exists(jar_file):
            smartCopy(xml_file, destdir)
            smartCopy(jar_file, destdir)

      # Install any wizards that were compiled.
      destdir = os.path.join(prefix, 'share', 'vrjuggler', 'beans', 'wizards')
      mkinstalldirs(destdir)
      for e in wizards:
         jar_file = os.path.join(jardir, e[1] + '.jar')
         if os.path.exists(jar_file):
            smartCopy(jar_file, destdir)

      # Install vrjconfig.bat.
      destdir = os.path.join(prefix, 'bin')
      smartCopy(os.path.join(vrjconfig_src, 'vrjconfig.bat'), destdir)

      # Install dependencies.
      dep_jars = [
         r'jgraph\lib\jgraph.jar',
         r'jgraph\lib\jgraphaddons.jar',
      ]

      srcroot = os.path.join(gJugglerDir, 'external')
      destdir = os.path.join(prefix, 'share', 'vrjuggler', 'java')
      for j in dep_jars:
         smartCopy(os.path.join(srcroot, j), destdir)
   else:
      printStatus("VRJConfig not built.  Skipping.")

def installVRJugglerPlugins(prefix, buildDir):
   printStatus("Installing VR Juggler C++ plug-ins ...")

   destdir = os.path.join(prefix, 'lib', 'vrjuggler', 'plugins')
   srcroot = os.path.join(buildDir, 'VRJugglerPlugins', 'Perf_Plugin_CXX')
   installLibs(srcroot, destdir, extensions = ['.dll', '.exp'])

def installVRJugglerPluginsJava(prefix, buildDir):
   srcdir = os.path.join(buildDir, 'VRJugglerPlugins', 'Perf_Plugin_Java')

   plugins = [('PerformanceMonitor', 'corba_perf_mon')]

   for p in plugins:
      name = p[0]
      dir  = p[1]
      if os.path.exists(os.path.join(srcdir, name + '.jar')):
         printStatus("Installing VR Juggler Java plug-ins ...")

         destdir = os.path.join(prefix, 'share', 'vrjuggler', 'beans')
         mkinstalldirs(destdir)
         smartCopy(os.path.join(srcdir, name + '.jar'), destdir)

         srcdir = os.path.join(gJugglerDir, 'modules', 'vrjuggler', 'plugins',
                               dir)
         smartCopy(os.path.join(srcdir, name + '.xml'), destdir)
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
      sys_dir = os.path.join(srcroot, 'System32')
      dlls = glob.glob(os.path.join(sys_dir, 'msvc*.dll'))

      for d in dlls:
         smartCopy(d, pj(destdir, d))

      #smartCopy(d, pj(destdir,d))
      smartCopy(os.path.join(sys_dir, 'dbghelp.dll'), destdir)
   except KeyError, ex:
      printStatus("WARNING: Could not install MSVC runtime DLLs")
      print ex

def doDependencyInstall(prefix, buildDir):
   makeTree(prefix)
   installNSPR(prefix)
   installCppDOM(prefix)
   installBoost(prefix, buildDir)
   installGMTL(prefix)
   installAudiere(prefix)
   installOpenAL(prefix)
   installOmniORB(prefix)
   installDoozer(prefix)

def simpleInstall(name, root, prefix, includeDir = None, libDir = 'lib',
                  optional = False):
   if optional and root == '':
      return

   printStatus("Installing " + name)

   # Install all header files.
   if includeDir is None:
      includeDir = os.path.join(root, 'include')

   srcdir = includeDir

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'include')
      installDir(srcdir, destdir, ['.h', '.hpp', '.ipp'])

   # Install all libraries.
   srcdir = os.path.join(root, libDir)

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'lib')
      installDir(srcdir, destdir)

   # Install all executables.
   srcdir = os.path.join(root, 'bin')

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'bin')
      installDir(srcdir, destdir)

   # Install all data files.
   srcdir = os.path.join(root, 'share')

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'share')
      installDir(srcdir, destdir)

def installNSPR(prefix):
   simpleInstall('NSPR headers and libraries', os.environ['NSPR_ROOT'],
                 prefix, os.environ['NSPR_INCLUDES'])

def installCppDOM(prefix):
   if gBuild64:
      libdir = 'lib64'
   else:
      libdir = 'lib'

   simpleInstall('CppDOM headers and libraries', os.environ['CPPDOM_ROOT'],
                 prefix, os.environ['CPPDOM_INCLUDES'], libdir)

def installDoozer(prefix):
   simpleInstall('Installing Doozer makefile bits',
                 os.getenv('DOOZER_ROOT', ''), prefix, optional = True)

def installBoost(prefix, buildDir):
   printStatus("Installing Boost headers and libraries")

   destdir = os.path.join(prefix, 'lib', 'flagpoll')
   fpc_files = glob.glob(os.path.join(buildDir, 'VPR', 'boost*.fpc'))
   fpc_files += glob.glob(os.path.join(buildDir, 'VRJuggler', 'boost*.fpc'))
   for f in fpc_files:
      smartCopy(f, destdir)

   srcroot = os.environ['BOOST_ROOT']

   srcdir = os.environ['BOOST_INCLUDES']
   destdir = os.path.join(prefix, 'include')
   installDir(srcdir, destdir)

   srcdir = os.path.join(srcroot, 'lib')
   destdir = os.path.join(prefix, 'lib')
   mkinstalldirs(destdir)

   lib_list = glob.glob(os.path.join(srcdir, '*boost_*'))

   for f in lib_list:
      #print "  ==> ", f
      smartCopy(f, destdir)

def installGMTL(prefix):
   simpleInstall('GMTL headers', os.environ['GMTL_ROOT'], prefix,
                 os.environ['GMTL_INCLUDES'])

   # Install all libraries.
   srcdir = os.path.join(os.environ['GMTL_ROOT'], 'share', 'flagpoll')

   if os.path.exists(srcdir):
      destdir = os.path.join(prefix, 'lib', 'flagpoll')
      installDir(srcdir, destdir)

def installAudiere(prefix):
   simpleInstall('Audiere headers, libraries, and executables',
                 os.getenv('AUDIERE_ROOT', ''), prefix, optional = True)

def installOpenAL(prefix):
   srcdir = os.environ['OPENAL_ROOT']

   if srcdir != "":
      printStatus("Installing OpenAL DLL")
      destdir = os.path.join(prefix, 'bin')

      # OpenAL 1.0 and 1.1 put the redistributable DLL in different places.
      dll_dirs = [os.path.join(srcdir, 'dll')]

      sysroot = os.environ['SystemRoot']

      # For a 64-bit build, we know that we only have to to look in one place
      # for the DLL.
      if gBuild64:
         dll_dirs.append(os.path.join(sysroot, 'system32'))
      else:
         dll_dirs += [os.path.join(sysroot, 'SysWOW64'),
                      os.path.join(sysroot, 'system32')]

      for d in dll_dirs:
         dll = os.path.join(d, 'OpenAL32.dll')
         if os.path.exists(dll):
            smartCopy(dll, destdir)
            break

   srcdir = os.environ['ALUT_ROOT']
   if srcdir != "":
      printStatus("Installing ALUT DLL")
      destdir = os.path.join(prefix, 'bin')

      alut_dll = os.path.join(srcdir, 'lib', 'alut.dll')
      if os.path.exists(alut_dll):
         smartCopy(alut_dll, destdir)

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

      (cl_ver_major, cl_ver_minor, vc_dir) = chooseVisualStudioDir()
      required, optional, options = getDefaultVars(cl_ver_major, cl_ver_minor)
      self.mOptions   = options
      self.mTkOptions = {}
      self.mVcDir     = vc_dir

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
      self.mRoot.CommandFrame.Build64Check = \
         Tkinter.Checkbutton(self.mRoot.CommandFrame,
                             text="64-bit Build",
                             bg = self.JugglerYellow,
                             activebackground = self.JugglerYellow,
                             onvalue = "Yes", offvalue = "No")
      self.mRoot.CommandFrame.Build64Check.Variable = Tkinter.StringVar()
      build64 = "No"
      if gBuild64:
         build64 = "Yes"
      self.mRoot.CommandFrame.Build64Check.Variable.set(build64)
      self.mRoot.CommandFrame.Build64Check["variable"] = \
         self.mRoot.CommandFrame.Build64Check.Variable
      self.mRoot.CommandFrame.Build64Check.grid(row = next_row, column = 0,
                                                sticky = Tkinter.EW, pady = 4)
      next_row = next_row + 1
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
            "The following settings reference non-existent directories and must be set before proceeding.\n%s" % inv_dir_list)
         return status

      return status

   def doBuild(self):
      self.mRoot.CommandFrame.BuildInstallButton.config(state = "disabled")

      # Set the environment vars.
      for k in self.mTkOptions.iterkeys():
         os.environ[k] = self.mTkOptions[k].get()

      # This has to be done before calling postProcessOptions().
      if self.mRoot.CommandFrame.Build64Check.Variable.get() == "Yes":
         global gBuild64
         gBuild64 = True

      if True:#self.validateOptions():
         postProcessOptions(self.mOptions)
         self.__writeCacheFile()

         self.BuildThread = threading.Thread(None, self.runVisualStudio,
                                             "BuildThread")
         self.BuildThread.start()

   def installJuggler(self):
      doInstall(self.mTkOptions['prefix'].get(),
                os.path.join(gJugglerDir, self.mVcDir))

   def installDeps(self):
      doDependencyInstall(self.mTkOptions['deps-prefix'].get(),
                          os.path.join(gJugglerDir, self.mVcDir))

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
         doInstall(self.mTkOptions['prefix'].get(),
                   os.path.join(gJugglerDir, self.mVcDir))

      if self.mRoot.CommandFrame.InstallJugglerDepsCheck.Variable.get() == "Yes":
         self.printMessage("Installing Juggler Dependencies...")
         doDependencyInstall(self.mTkOptions['deps-prefix'].get(),
                             os.path.join(gJugglerDir, self.mVcDir))

      self.printMessage("Build and Installation Finished.")
      self.updateCommandFrame()

   def runVisualStudio(self):
      #print "updateVersions()"
      self.printMessage("Generating Version Headers.")
      updateVersions(self.mVcDir, self.mOptions)
      self.printMessage("Generating Ant Build Files.")
      generateAntBuildFiles(self.mVcDir)

      devenv_cmd = getVSCmd()
      (devenv_cmd_no_exe, ext) = os.path.splitext(devenv_cmd)
      devenv_cmd_no_exe = '"%s"' % (devenv_cmd_no_exe)

      solution_file = r'"%s"' % os.path.join(gJugglerDir, self.mVcDir,
                                             'Juggler.sln')
      build_args = r'/build DebugDLL'

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
            sys.exit(EXIT_STATUS_MSVS_START_ERROR)
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
   # devenv is used by the full version of Visual Studio. VCExpress is the
   # launch command used by Visual C++ Express Edition.
   cmds = ['devenv.exe', 'VCExpress.exe']

   for p in str.split(os.getenv('PATH', ''), os.pathsep):
#      print "Searching in", p
      for c in cmds:
         cmd = os.path.join(p, c)
         if os.path.exists(cmd):
            devenv_cmd = cmd
            break

      if devenv_cmd is not None:
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
      cmd_opts, cmd_args = getopt.getopt(sys.argv[1:], "cano:h",
                                         ["64", "nogui", "nobuild", "auto",
                                          "options-file=", "help"])
   except getopt.GetoptError:
      usage()
      sys.exit(EXIT_STATUS_INVALID_ARGUMENT)

   skip_vs = False

   global gOptionsFileName
   global gBuild64
   for o, a in cmd_opts:
      if o in ("-c","--nogui"):
         disable_tk = True
      elif o == "--64":
         gBuild64 = True
      elif o in ("-o", "--options-file="):
         gOptionsFileName = a

         # Make sure file exists.
         if not os.path.isfile(gOptionsFileName):
            print "No file %s exists.  Will use default options." % \
                  gOptionsFileName
      elif o in ("-n", "--nobuild"):
         skip_vs = True
      elif o in ("-h", "--help"):
         usage()
         sys.exit(0)

   # If Tkinter is not available or the user disabled the Tk frontend, use
   # the text-based interface.
   if not gHaveTk or disable_tk:
      (cl_ver_major, cl_ver_minor, vc_dir) = chooseVisualStudioDir()
      options = setVars(cl_ver_major, cl_ver_minor)
      updateVersions(vc_dir, options)
      generateAntBuildFiles(vc_dir)

      try:
         status = 0

         if not skip_vs:
            devenv_cmd    = getVSCmd()
            solution_file = r'%s' % os.path.join(gJugglerDir, vc_dir,
                                                 'Juggler.sln')

            status = os.spawnl(os.P_WAIT, devenv_cmd, 'devenv', solution_file)

         if status == 0:
            print "Proceed with VR Juggler installation [y]: ",
            proceed = sys.stdin.readline().strip(" \n")
            if proceed == '' or proceed.lower().startswith('y'):
               doInstall(options['prefix'], os.path.join(gJugglerDir, vc_dir))

               print "Proceed with VR Juggler dependency installation [y]: ",
               proceed = sys.stdin.readline().strip(" \n")
               if proceed == '' or proceed.lower().startswith('y'):
                  doDependencyInstall(options['deps-prefix'], os.path.join(gJugglerDir, vc_dir))
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
   print "Python script for building VR Juggler on Windows.\n"
   print "-c, --nogui              Disable the Tkinter GUI front end"
   print "                         (i.e., Run in command line mode)."
   print "--64                     Indicate that a 64-bit build will"
   print "                         be made."
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
      elif exitEx.code == EXIT_STATUS_UNSUPPORTED_COMPILER:
         status = 'unsupported compiler'
      elif exitEx.code == EXIT_STATUS_INVALID_ARGUMENT:
         status = 'invalid command line argument'
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
