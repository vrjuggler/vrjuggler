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

# Remove the docs from the system
clean:
	-@echo ""
	-@echo "-------------------------------------------"
	-@echo "Removing generated documentation ..."
	-@echo "-------------------------------------------"
	rm -rf html latex man

clobber: clean

