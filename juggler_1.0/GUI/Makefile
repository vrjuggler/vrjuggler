
VjCfg.jar: classfiles
	jar cvf VjCfg.jar VjGUI/*.class VjConfig/*.class

classfiles:
	make -f VjConfig/Makefile
	make -f VjGUI/Makefile
	@echo "Made all classfiles."

clean:
	cd VjConfig; make clean
	cd VjGUI; make clean

clobber: clean
	rm VjCfg.jar



