# ************** <auto-copyright.pl BEGIN do not edit this line> **************
# Doozer
#
# Original Authors:
#   Patrick Hartling, Kevin Meinert
# -----------------------------------------------------------------------------
# VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
# File:          dzr.flt2pfb.mk,v
# Date modified: 2002/02/19 22:01:37
# Version:       1.1
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

#
# This converts .flt models to performer binary format.
# 
####################################################
# Run this with gmake -j for a parallel conversion!
#
####################################################
# Here are the params you should set...
#
# MODELS = model1.flt model2.flt model3.flt model4.flt model5.flt
# FILE_PATHS = -F../images/ -F../images/hud/
# OPTIONS = -M pfb,1,1
# OUTPUT_FILE = $@
# INPUT_FILE = $<
#
####################################################

OPTIMIZED_MODELS=	$(MODELS:.flt=.pfb)

%.pfb: %.flt
	pfconv $(FILE_PATHS) $(OPTIONS) $(INPUT_FILE) $(OUTPUT_FILE)

models: $(OPTIMIZED_MODELS)
	@echo
	@echo ===============================
	@echo    Done...
	@echo ===============================
	@echo

clean:
	@echo
	@echo ===================================================
	@echo Removing: 
	@echo    $(OPTIMIZED_MODELS)
	@echo ===================================================
	@rm $(OPTIMIZED_MODELS)
	@echo
