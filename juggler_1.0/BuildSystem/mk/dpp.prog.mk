# =============================================================================
# dpp.prog.mk,v 1.1.1.1 2000/12/08 06:16:36 patrick Exp
#
# This file <dpp.prog.mk> is intended to be used by makefiles that need to
# build programs.  It provides some handy targets and includes other .mk
# files that are typically necessary for this process.
# -----------------------------------------------------------------------------
# The targets defined here all invoke the 'all' target in the includning
# makefile.  They are as follows:
#
# static        - Build a statically linked binary with debugging symbols.
# dynamic       - Build a dynamically linked binary with debugging symbols.
# static_optim  - Build a statically linked optimized binary.
# dynamic_optim - Build a dynamically linked optimized binary.
#
# These targts pass values for the following variables:
#
# LDFLAGS_BEGIN - The linker flags needed prior to listing the libraries to
#                 link.
# LDFLAGS_END   - The linker flags needed after listing the libraries to
#                 link.  This is typically empty.
# LIBS          - The libraries to link.
# OPTIMIZER     - The optimizer flags for the compiler.  This will either be
#                 debugging options or optimization options.
# =============================================================================

static:
	@$(MAKE) LDFLAGS_BEGIN="$(LDFLAGS) $(LDFLAGS_STATIC)"		\
          LDFLAGS_END="$(LDFLAGS_DYNAMIC)" OPTIMIZER="$(DBG_FLAGS)"	\
          LIBS="$(LIBS) $(STATIC_LIBS)" all

dynamic:
	@$(MAKE) LDFLAGS_BEGIN="$(LDFLAGS)" OPTIMIZER="$(DBG_FLAGS)" all

static_optim:
	@$(MAKE) LDFLAGS_BEGIN="$(LDFLAGS) $(LDFLAGS_STATIC)"		\
          LDFLAGS_END="$(LDFLAGS_DYNAMIC)"  OPTIMIZER="$(OPT_FLAGS)"	\
          LIBS="$(LIBS) $(STATIC_LIBS)" all

dynamic_optim:
	@$(MAKE) LDFLAGS_BEGIN="$(LDFLAGS)" OPTIMIZER="$(OPT_FLAGS)" all

include $(MKPATH)/dpp.compile.mk
include $(MKPATH)/dpp.dep.mk
include $(MKPATH)/dpp.idl.mk
include $(MKPATH)/dpp.clean.mk
