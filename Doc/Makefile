PERCEPS_DIR = .
PERCEPS= $(PERCEPS_DIR)/bin/perceps
FILTERS = ./filters
TEMPLATES_INT = ./templates/internal
TEMPLATES_PUB = ./templates/public
SRCDIR = ..
SRC = $(SRCDIR)/Sync $(SRCDIR)/Config $(SRCDIR)/Input $(SRCDIR)/Kernel	\
      $(SRCDIR)/SharedMem $(SRCDIR)/Threads $(SRCDIR)/Environment	\
      $(SRCDIR)/Math $(SRCDIR)/Performance
SRC_EX = ExampleClass.h ExampleClass.C
OUT_INT = ./internal_doc
OUT_PUB = ./public_doc
OUT_EX = ./ex_html
JAVA_CLASS = $(PERCEPS_DIR)/classes

docs: doc-internal doc-public

doc-internal:
	$(PERCEPS) -a -b -c -h -f -d $(OUT_INT) -F $(FILTERS)		\
          -t $(TEMPLATES_INT) $(SRC)
	cp $(JAVA_CLASS)/*.class $(OUT_INT)

doc-public:
	$(PERCEPS) -a -b -c -h -f -d $(OUT_PUB) -F $(FILTERS)		\
          -t $(TEMPLATES_PUB) $(SRC)
	cp $(JAVA_CLASS)/*.class $(OUT_PUB)

example:
	$(PERCEPS) -a -b -c -h -d $(OUT_EX) -t $(TEMPLATES) $(SRC_EX)
	cp $(JAVA_CLASS)/*.class $(OUT_EX)

clean:
	@ if [ -f $(OUT_PUB)/* ]; then rm $(OUT_PUB)/*; fi
	@ if [ -f $(OUT_INT)/* ]; then rm $(OUT_INT)/*; fi
