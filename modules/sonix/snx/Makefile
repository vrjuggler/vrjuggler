CXX = CC 
CXXFLAGS = -LANG:std -g -n32 -mips3 
vpath %.cpp . avatars

VPR_LIBS	= -L/home/users/kevn/software/VaPoR/lib32 -B static -lvpr -B dynamic -L/home/users/allenb/Irix/nspr_pthread/lib -lnspr4 -lpthread

INCLUDEPATHS = -I/home/users/allenb/Irix/nspr_pthread/include -I${VPR_BASE_DIR}/include
LIBS = ${VPR_LIBS} -B dynamic -lm


default:
	CC -LANG:std -g ${INCLUDEPATHS} main.cpp ${LIBS}
#	g++ -g -I/home/users/allenb/Irix/nspr_pthread/include -I${VPR_BASE_DIR}/include main.cpp
