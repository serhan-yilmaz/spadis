#MATLABDIR=/usr/local/MATLAB/R2017a/bin/
MATLABDIR=

CXXFLAGS=-Wall -O3 -g -Wno-deprecated -Wno-sign-compare -fopenmp -fPIC -std=c++11
LDFLAGS=-lboost_program_options -lgomp
VER=1.00
SOURCEDIR=src/cpp/
OBJECTDIR=src/obj/
BINARYDIR=binaries/
BIN=spadis ${MEXOBJ}

.mkdir: 
	mkdir -p ${OBJECTDIR} && touch .mkdir

.PHONY: matlab

matlab: .mkdir .mlab

tgz: clean
	mkdir -p spadis-${VER}/${SOURCEDIR} &&\
	cp -r Makefile ${BINARYDIR} spadis-${VER} &&\
	cp -r ${SOURCEDIR}*.?pp ${SOURCEDIR}*.h spadis-${VER}/${SOURCEDIR} &&\
	tar -czf spadis-${VER}.tgz spadis-${VER}&&\
	rm -rf spadis-${VER}
	
clean:
	rm -f $(BIN) ${OBJECTDIR}*.o .mlab .mkdir *.tgz

${OBJECTDIR}node.o: ${SOURCEDIR}Node.cpp ${SOURCEDIR}Node.h
	g++ ${CXXFLAGS} -c ${SOURCEDIR}Node.cpp -o ${OBJECTDIR}node.o
${OBJECTDIR}graph.o: ${SOURCEDIR}Graph.cpp ${SOURCEDIR}Graph.h
	g++ ${CXXFLAGS} -c ${SOURCEDIR}Graph.cpp -o ${OBJECTDIR}graph.o
${OBJECTDIR}options.o: ${SOURCEDIR}Options.cpp ${SOURCEDIR}Options.h
	g++ ${CXXFLAGS} -c ${SOURCEDIR}Options.cpp -o ${OBJECTDIR}options.o
${OBJECTDIR}optimizer.o: ${SOURCEDIR}Optimizer.cpp ${SOURCEDIR}Optimizer.h
	g++ ${CXXFLAGS} -c ${SOURCEDIR}Optimizer.cpp -o ${OBJECTDIR}optimizer.o

.mlab: ${SOURCEDIR}matlab.cpp ${OBJECTDIR}options.o ${OBJECTDIR}node.o ${OBJECTDIR}graph.o ${OBJECTDIR}optimizer.o  
#${MATLABDIR}/bin/mex
	${MATLABDIR}mex ${SOURCEDIR}matlab.cpp -output ${BINARYDIR}/spadis -v -g -O -largeArrayDims -lut "CXXFLAGS=\$$CXXFLAGS ${CXXFLAGS}" "LDFLAGS=\$$LDFLAGS ${LDFLAGS} ${OBJECTDIR}optimizer.o ${OBJECTDIR}options.o ${OBJECTDIR}graph.o ${OBJECTDIR}node.o -lgomp" \
	&& touch .mlab








