@ECHO OFF
rem ************* <auto-copyright.pl BEGIN do not edit this line> *************
rem
rem VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

IF NOT "%TWEEK_BASE_DIR%"=="" GOTO ELSE3
ECHO [ERR] TWEEK_BASE_DIR unset; please set the environment variable 
ECHO [ERR] TWEEK_BASE_DIR to point to your Tweek installation.  For more 
ECHO [ERR] information, please see INSTALL.html
GOTO ERREXIT
:ELSE3
IF EXIST "%TWEEK_BASE_DIR%" GOTO RUNJAVA
ECHO [ERR] The TWEEK_BASE_DIR %TWEEK_BASE_DIR% does not appear to exist.
ECHO [ERR] Please check the path and try again.
GOTO ERREXIT
:RUNJAVA
set TWEEK_CLASSPATH=%TWEEK_BASE_DIR%\share\tweek\java\jaxen-core.jar;%TWEEK_BASE_DIR%\share\tweek\java\jaxen-jdom.jar;%TWEEK_BASE_DIR%\share\tweek\java\jdom.jar;%TWEEK_BASE_DIR%\share\tweek\java\saxpath.jar;%TWEEK_BASE_DIR%\share\tweek\java\xalan.jar;%TWEEK_BASE_DIR%\share\tweek\java\xerces.jar;%TWEEK_BASE_DIR%\share\tweek\java\xml-apis.jar;%TWEEK_BASE_DIR%\share\tweek\java\Tweek.jar;%TWEEK_BASE_DIR%\share\tweek\java\TweekBeans.jar;%TWEEK_BASE_DIR%\share\tweek\java\TweekEvents.jar;%TWEEK_BASE_DIR%\share\tweek\java\TweekNet.jar;%TWEEK_BASE_DIR%\share\tweek\java\TweekBeanDelivery.jar;%TWEEK_BASE_DIR%\share\tweek\java\TweekServices.jar;%TWEEK_BASE_DIR%\share\tweek\java\kunststoff-mod.jar;%TWEEK_BASE_DIR%\share\tweek\java\liquidlnf.jar;%TWEEK_BASE_DIR%\share\tweek\java\metouia.jar;%TWEEK_BASE_DIR%\share\tweek\java\looks.jar

java -Dsun.java2d.opengl=true -DTWEEK_BASE_DIR="%TWEEK_BASE_DIR%" -Djava.library.path="%TWEEK_BASE_DIR%\share\tweek\java\%PROCESSOR_ARCHITECTURE%" -Djava.security.policy="%TWEEK_BASE_DIR%\share\tweek\java\java.security.policy.txt" -cp "%TWEEK_CLASSPATH%" org.vrjuggler.tweek.Tweek %*
GOTO DONE
:ERREXIT
ECHO [ERR] Tweek exiting due to previous errors.
:DONE
