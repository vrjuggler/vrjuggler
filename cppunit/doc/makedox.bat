SET VERSION=1.9.8
doxygen CppUnit-win.dox
xcopy FAQ html /Y

@echo Warnings:
@type CppUnit-dox.log
