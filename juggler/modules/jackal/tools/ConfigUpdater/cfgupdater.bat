@echo off
rem
rem ************* <auto-copyright.pl BEGIN do not edit this line> *************
rem
rem VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
rem -----------------------------------------------------------------
rem File:          $RCSfile$
rem Date modified: $Date$
rem Version:       $Revision$
rem -----------------------------------------------------------------
rem
rem ************** <auto-copyright.pl END do not edit this line> **************
@echo on

java -DVJ_BASE_DIR=%VJ_BASE_DIR% -DJCCL_BASE_DIR=%JCCL_BASE_DIR% -jar %JCCL_BASE_DIR%/bin/ConfigUpdater.jar %1 %2 %3 %4 %5 %6 %7 %8 %9
