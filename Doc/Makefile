PERCEPS_DIR = .
PERCEPS= $(PERCEPS_DIR)/bin/perceps
TEMPLATES= ./templates
SRCDIR = ..
SRC = $(SRCDIR)/Sync $(SRCDIR)/Config $(SRCDIR)/Input $(SRCDIR)/Kernel	\
      $(SRCDIR)/SharedMem $(SRCDIR)/Threads $(SRCDIR)/Environment $(SRCDIR)/Math
SRC_EX = ExampleClass.h ExampleClass.C
OUT= ./src_html
OUT_EX = ./ex_html
JAVA_CLASS = $(PERCEPS_DIR)/classes

doc :
	$(PERCEPS) -a -b -c -h -f -d $(OUT) -t $(TEMPLATES) $(SRC)
	cp $(JAVA_CLASS)/*.class $(OUT)

example:
	$(PERCEPS) -a -b -c -h -d $(OUT_EX) -t $(TEMPLATES) $(SRC_EX)
	cp $(JAVA_CLASS)/*.class $(OUT_EX)

clean :
	@ if [ -f $(OUT)/* ]; then rm $(OUT)/*; fi
