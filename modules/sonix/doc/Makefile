# Default targets
default: all
all: docs

# Builds the documentation
docs:
	-@echo ""
	-@echo "-------------------------------------------"
	-@echo "Generating Doxygen docs ..."
	-@echo "-------------------------------------------"
	doxygen sonix.doxy
	-@echo ""
	-@echo "-------------------------------------------"
	-@echo "Generating docbook docs ..."
	-@echo "-------------------------------------------"
	cd programmer.guide; gmake html html-chunk pdf

install:
	-@echo ""
	-@echo "-------------------------------------------"
	-@echo "Installing Doxygen docs ..."
	-@echo "-------------------------------------------"
	-@echo ""
	-@echo "-------------------------------------------"
	-@echo "Installing docbook docs ..."
	-@echo "-------------------------------------------"
	cd programmer.guide; gmake install

# Remove the docs from the system
clean:
	-@echo ""
	-@echo "-------------------------------------------"
	-@echo "Removing generated documentation ..."
	-@echo "-------------------------------------------"
	rm -rf html latex man

clobber: clean

