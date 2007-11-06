@ECHO OFF
rem ************* <auto-copyright.pl BEGIN do not edit this line> *************
rem
rem VR Juggler is (C) Copyright 1998-2007 by Iowa State University
rem
rem Original Authors:
rem   Allen Bierbaum, Christopher Just,
rem   Patrick Hartling, Kevin Meinert,
rem   Carolina Cruz-Neira, Albert Baker
rem
rem This library is free software; you can redistribute it and/or
rem modify it under the terms of the GNU Library General Public
rem License as published by the Free Software Foundation; either
rem version 2 of the License, or (at your option) any later version.
rem
rem This library is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
rem Library General Public License for more details.
rem
rem You should have received a copy of the GNU Library General Public
rem License along with this library; if not, write to the
rem Free Software Foundation, Inc., 59 Temple Place - Suite 330,
rem Boston, MA 02111-1307, USA.
rem
rem ************** <auto-copyright.pl END do not edit this line> **************

IF NOT "%JAVA_HOME%" == "" GOTO TEST_JAVA_EXIST
IF "%JDK_HOME%" == "" GOTO JAVA_ERR
ECHO [NOTE] Setting JAVA_HOME to "%JDK_HOME%"
set JAVA_HOME=%JDK_HOME%
GOTO TEST_JAVA_EXIST

:JAVA_ERR
ECHO [ERR] Neither JAVA_HOME nor JDK_HOME is set.  One of these environment
ECHO [ERR] variables must be set to a valid Java 1.4 or newer installation
ECHO [ERR] directory to run VRJConfig.
GOTO ERREXIT

:TEST_JAVA_EXIST
IF EXIST "%JAVA_HOME%\bin\java.exe" GOTO TEST_VJ_BASE
ECHO [ERR] %JAVA_HOME%\bin\java.exe does not exist, so VRJConfig cannot be
ECHO [ERR] started.

:TEST_VJ_BASE
IF NOT "%VJ_BASE_DIR%" == "" GOTO TEST_VJ_EXIST
set VJ_BASE_DIR=%~dp0..
ECHO NOTE: VJ_BASE_DIR environment variable unset; defaulting to
ECHO       %VJ_BASE_DIR%
ECHO       If this is incorrect, see INSTALL.html and set the value
ECHO       correctly.

:TEST_VJ_EXIST
IF EXIST "%VJ_BASE_DIR%" GOTO TEST_TWEEK_BASE
ECHO [ERR] The VJ_BASE_DIR %VJ_BASE_DIR% does not appear to exist.  Please
ECHO [ERR] check the path and try again.
GOTO ERREXIT

:TEST_TWEEK_BASE
IF NOT "%TWEEK_BASE_DIR%" == "" GOTO TEST_TWEEK_EXIST
ECHO  NOTE: Setting TWEEK_BASE_DIR to "%VJ_BASE_DIR%"
set TWEEK_BASE_DIR=%VJ_BASE_DIR%

:TEST_TWEEK_EXIST
IF EXIST "%TWEEK_BASE_DIR%" GOTO TEST_JCCL_BASE
ECHO [ERR] The TWEEK_BASE_DIR %TWEEK_BASE_DIR% does not appear to exist.  Please
ECHO [ERR] check the path and try again.
GOTO ERREXIT

:TEST_JCCL_BASE
IF NOT "%JCCL_BASE_DIR%" == "" GOTO TEST_JCCL_EXIST
ECHO  NOTE: Setting JCCL_BASE_DIR to "%VJ_BASE_DIR%"
set JCCL_BASE_DIR=%VJ_BASE_DIR%

:TEST_JCCL_EXIST
IF EXIST "%JCCL_BASE_DIR%" GOTO TEST_JCCL_DEF
ECHO [ERR] The JCCL_BASE_DIR %JCCL_BASE_DIR% does not appear to exist.  Please
ECHO [ERR] check the path and try again.
GOTO ERREXIT

:TEST_JCCL_DEF
IF NOT "%JCCL_DEFINITION_PATH%" == "" GOTO RUNJAVA
set jccl_def_arg=-DJCCL_DEFINITION_PATH="%JCCL_DEFINITION_PATH%"

:RUNJAVA
"%JAVA_HOME%\bin\java" -DTWEEK_BASE_DIR="%TWEEK_BASE_DIR%" -DTWEEK_DATA_DIR="share\tweek" -DJCCL_BASE_DIR="%JCCL_BASE_DIR%" -DJCCL_DATA_DIR="share\jccl" -DVJ_BASE_DIR="%VJ_BASE_DIR%" -DVJ_DATA_DIR="share\vrjuggler" %jccl_def_arg% -Djava.library.path="%TWEEK_BASE_DIR%\share\tweek\java\%PROCESSOR_ARCHITECTURE%" -Djava.security.policy="%TWEEK_BASE_DIR%\share\tweek\java\java.security.policy.txt" -cp "%TWEEK_BASE_DIR%\share\tweek\java\jaxen-core.jar;%TWEEK_BASE_DIR%\share\tweek\java\jaxen-jdom.jar;%TWEEK_BASE_DIR%\share\tweek\java\jdom.jar;%TWEEK_BASE_DIR%\share\tweek\java\saxpath.jar;%TWEEK_BASE_DIR%\share\tweek\java\xalan.jar;%TWEEK_BASE_DIR%\share\tweek\java\xerces.jar;%TWEEK_BASE_DIR%\share\tweek\java\xml-apis.jar;%TWEEK_BASE_DIR%\share\tweek\java\Tweek.jar;%TWEEK_BASE_DIR%\share\tweek\java\TweekBeans.jar;%TWEEK_BASE_DIR%\share\tweek\java\TweekEvents.jar;%TWEEK_BASE_DIR%\share\tweek\java\TweekNet.jar;%TWEEK_BASE_DIR%\share\tweek\java\TweekBeanDelivery.jar;%TWEEK_BASE_DIR%\share\tweek\java\TweekServices.jar;%TWEEK_BASE_DIR%\share\tweek\java\kunststoff-mod.jar;%TWEEK_BASE_DIR%\share\tweek\java\liquidlnf.jar;%TWEEK_BASE_DIR%\share\tweek\java\metouia.jar;%TWEEK_BASE_DIR%\share\tweek\java\looks.jar" org.vrjuggler.tweek.Tweek --beanpath="%JCCL_BASE_DIR%\share\jccl\beans" --beanpath="%VJ_BASE_DIR%\share\vrjuggler\beans" --defaultbean="Configuration Editor" %*
GOTO DONE

:ERREXIT
ECHO [ERR] VRJConfig exiting due to previous errors.
PAUSE

:DONE
