include subdirs.mk

top_srcdir = ..

PERCEPS_DIR = .
PERCEPS= perl $(PERCEPS_DIR)/bin/perceps
FILTERS = ./filters
TEMPLATES_INT = ./templates/internal
TEMPLATES_PUB = ./templates/public

SRC_EX = ExampleClass.h ExampleClass.C
OUT_INT = ./internal_doc
OUT_PUB = ./public_doc
OUT_EX = ./ex_html
JAVA_CLASS = $(PERCEPS_DIR)/classes

docs: doc-internal doc-public

doc-internal:
	$(PERCEPS) -a -b -c -h -f -d $(OUT_INT) -F $(FILTERS)		\
          -t $(TEMPLATES_INT) $(SUBDIRS)
	cp $(JAVA_CLASS)/*.class $(OUT_INT)

doc-public:
	$(MAKE) BASE_OUT="$(OUT_PUB)" TEMPLATES="$(TEMPLATES_PUB)"	\
          all-doc $(GROUPS)

all-doc:
	$(PERCEPS) -a -b -c -h -f -d $(BASE_OUT) -F $(FILTERS)		\
          -t $(TEMPLATES) $(SUBDIRS)
	cp $(JAVA_CLASS)/*.class $(BASE_OUT)

Config:
	$(PERCEPS) -a -b -c -h -f -d $(BASE_OUT)/$@ -F $(FILTERS)	\
          -t $(TEMPLATES)_xml $(CONFIG_GROUP_DIRS)

Input:
	$(PERCEPS) -a -b -c -h -f -d $(BASE_OUT)/$@ -F $(FILTERS)	\
          -t $(TEMPLATES)_xml $(INPUT_GROUP_DIRS)

Kernel:
	$(PERCEPS) -a -b -c -h -f -d $(BASE_OUT)/$@ -F $(FILTERS)	\
          -t $(TEMPLATES)_xml $(KERNEL_GROUP_DIRS)

Math:
	$(PERCEPS) -a -b -c -h -f -d $(BASE_OUT)/$@ -F $(FILTERS)	\
          -t $(TEMPLATES)_xml $(MATH_GROUP_DIRS)

Primitives:
	$(PERCEPS) -a -b -c -h -f -d $(BASE_OUT)/$@ -F $(FILTERS)	\
          -t $(TEMPLATES)_xml $(PRIMITIVES_GROUP_DIRS)

Sound:
	$(PERCEPS) -a -b -c -h -f -d $(BASE_OUT)/$@ -F $(FILTERS)	\
          -t $(TEMPLATES)_xml $(SOUND_GROUP_DIRS)

Utils:
	$(PERCEPS) -a -b -c -h -f -d $(BASE_OUT)/$@ -F $(FILTERS)	\
          -t $(TEMPLATES)_xml $(UTILS_GROUP_DIRS)

example:
	$(PERCEPS) -a -b -c -h -d $(OUT_EX) -t $(TEMPLATES) $(SRC_EX)
	cp $(JAVA_CLASS)/*.class $(OUT_EX)

clean:
	@ if [ -f $(OUT_PUB)/* ]; then rm $(OUT_PUB)/*; fi
	@ if [ -f $(OUT_INT)/* ]; then rm $(OUT_INT)/*; fi
