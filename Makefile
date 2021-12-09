MSDIR := msdir
DSFMT_VERSION := 2.2.5
DSFMT_SRC := dSFMT-${DSFMT_VERSION}
DESTDIR := ${HOME}/local/bin
CFLAGS := -O3 -Wall -Wno-implicit-function-declaration
CPPFLAGS := -DNDEBUG -fno-strict-aliasing
CXXFLAGS := -O3 -std=c++11 -Wall
LIBSEQUENCE := $(shell brew --prefix 2>/dev/null)
LDLIBS := -lm
TARGET_ARCH := -m64 -msse -msse2 -msse3 -mfpmath=sse
INSTALL := install

.DEFAULT_GOAL := all
.PHONY: all clean install

all: ms ms_rand1 ms_rand2 sample_stats sample_stats++ summstats
	@:

clean:
	${RM} ms sample_stats ms_rand*

install:
	${INSTALL} -d ${DESTDIR}
	${INSTALL} ms ${DESTDIR}
	${INSTALL} sample_stats ${DESTDIR}

${MSDIR}/ms.c:
	@echo "Download ms.tar.gz from http://home.uchicago.edu/rhudson1/source/mksamples.html"
	@echo "Expand it here: tar xzf ~/Downloads/ms.tar.gz"
	@exit 1

${DSFMT_SRC}/dSFMT.c:
	curl -L https://github.com/MersenneTwister-Lab/dSFMT/archive/refs/tags/v${DSFMT_VERSION}.tar.gz | tar xzf -

ms: ${MSDIR}/ms.o ${MSDIR}/streec.o randdSFMT.c ${DSFMT_SRC}/dSFMT.c
	${LINK.c} -I${DSFMT_SRC} -DDSFMT_MEXP=19937 -DSFMT_DO_NOT_USE_OLD_NAMES -Wno-return-type $^ ${LDLIBS} ${OUTPUT_OPTION}

sample_stats: ${MSDIR}/sample_stats.o ${MSDIR}/tajd.o
	${LINK.c} $^ ${LDLIBS} ${OUTPUT_OPTION}

ms_rand1: ${MSDIR}/ms.o ${MSDIR}/streec.o ${MSDIR}/rand1.o
	${LINK.c} $^ ${LDLIBS} ${OUTPUT_OPTION}

ms_rand1t: ${MSDIR}/ms.o ${MSDIR}/streec.o ${MSDIR}/rand1t.o
	${LINK.c} $^ ${LDLIBS} ${OUTPUT_OPTION}

ms_rand2: ${MSDIR}/ms.o ${MSDIR}/streec.o ${MSDIR}/rand2.o
	${LINK.c} $^ ${LDLIBS} ${OUTPUT_OPTION}

ms_rand2t: ${MSDIR}/ms.o ${MSDIR}/streec.o ${MSDIR}/rand2t.o
	${LINK.c} $^ ${LDLIBS} ${OUTPUT_OPTION}

sample_stats++: sample_stats.cpp fst.hpp
	${LINK.cpp} $< -I${LIBSEQUENCE}/include -L${LIBSEQUENCE}/lib -lsequence ${LDLIBS} ${OUTPUT_OPTION}

summstats: summstats.cpp
	${LINK.cpp} $< -I${LIBSEQUENCE}/include -L${LIBSEQUENCE}/lib -lsequence ${LDLIBS} ${OUTPUT_OPTION}
