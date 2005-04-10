@echo off
"%JAVA_HOME%\bin\java" -classpath "%JACORB_PATH%\idl.jar;%JACORB_PATH%\logkit-1.2.jar" org.jacorb.idl.parser %*
