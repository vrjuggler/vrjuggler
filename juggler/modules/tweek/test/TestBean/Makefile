TEST_BEAN_CLASSES	= testbean/TestBean.class

OBJDIR			= .
JAVA_PKG		= testbean

vpath %.java testbean
vpath %.class testbean

$(OBJDIR)/$(JAVA_PKG)/%.class: %.java
	javac -classpath ../../classes -d . $<

TestBean.jar: $(TEST_BEAN_CLASSES)
	jar cvfm $@ test.MF $(TEST_BEAN_CLASSES)
