@ECHO OFF
IF NOT "%VJ_BASE_DIR%" == "" GOTO ELSE1
ECHO [ERR] VJ_BASE_DIR unset; please set the environment variable VJ_BASE_DIR to
ECHO [ERR] point to your VR Juggler installation directory.  For more information, 
ECHO [ERR] please see INSTALL.html
GOTO ERREXIT
:ELSE1
IF EXIST "%VJ_BASE_DIR%" GOTO ELSE2
ECHO [ERR] The VJ_BASE_DIR %VJ_BASE_DIR% does not appear to exist.  Please 
ECHO [ERR] check the path and try again.
GOTO ERREXIT
:ELSE2
IF NOT "%TWEEK_BASE_DIR%"=="" GOTO ELSE3
ECHO  WARNING: Setting TWEEK_BASE_DIR to "%VJ_BASE_DIR%"
set TWEEK_BASE_DIR=%VJ_BASE_DIR%
:ELSE3
IF EXIST "%TWEEK_BASE_DIR%" GOTO RUNJAVA
ECHO [ERR] The TWEEK_BASE_DIR %TWEEK_BASE_DIR does not appear to exist.  Please 
ECHO [ERR] check the path and try again.
GOTO ERREXIT
:RUNJAVA
java -DTWEEK_BASE_DIR=%TWEEK_BASE_DIR% -DVJ_BASE_DIR=%VJ_BASE_DIR% -Djava.security.policy=%TWEEK_BASE_DIR%/bin/java.security.policy.txt -cp %TWEEK_BASE_DIR%/bin/jdom.jar;%TWEEK_BASE_DIR%/bin/xerces.jar;%TWEEK_BASE_DIR%/bin/Tweek.jar;%TWEEK_BASE_DIR%/bin/TweekBeans.jar;%TWEEK_BASE_DIR%/bin/TweekEvents.jar;%TWEEK_BASE_DIR%/bin/TweekNet.jar;%TWEEK_BASE_DIR%/bin/TweekBeanDelivery.jar;%TWEEK_BASE_DIR%/bin/TweekServices.jar;%TWEEK_BASE_DIR%/bin/kunststoff-mod.jar;%TWEEK_BASE_DIR%/bin/metouia.jar org.vrjuggler.tweek.Tweek --defaultbean=VRJConfig %1 %2 %3 %4 %5 %6 %7 %8 %9
EXIT
:ERREXIT
ECHO [ERR] VRJConfig exiting due to previous errors.
