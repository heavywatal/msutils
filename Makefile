MSDIR := msdir
DSFMTDIR := dSFMT-src-2.2.3
DESTDIR := ${HOME}/local/bin
CFLAGS := -O3
CPPFLAGS := -fno-strict-aliasing -iquote ${DSFMTDIR}
LDFLAGS  := -lm
TARGET_ARCH := -m64 -msse -msse2 -msse3 -mfpmath=sse
INSTALL := install

.DEFAULT_GOAL := all
.PHONY: all clean install

all: ms ms_rand1 ms_rand2 sample_stats
	@:

clean:
	${RM} ms sample_stats ms_rand*

install:
	${INSTALL} -d ${DESTDIR}
	${INSTALL} ms ${DESTDIR}
	${INSTALL} sample_stats ${DESTDIR}

${MSDIR}/ms.c:
	curl -o- https://webshare.uchicago.edu/users/rhudson1/Public/ms.folder/ms.tar.gz | tar xz

${DSFMTDIR}/dSFMT.c:
	curl -o- http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/${DSFMTDIR}.tar.gz | tar xz

ms: ${MSDIR}/ms.c ${MSDIR}/streec.c randdSFMT.c ${DSFMTDIR}/dSFMT.c
	${LINK.c} -DDSFMT_MEXP=19937 -Wno-return-type $^ ${OUTPUT_OPTION}

sample_stats: ${MSDIR}/sample_stats.c ${MSDIR}/tajd.c
	${LINK.c} $^ ${OUTPUT_OPTION}

ms_rand1: ${MSDIR}/ms.c ${MSDIR}/streec.c ${MSDIR}/rand1.c
	${LINK.c} $^ ${OUTPUT_OPTION}

ms_rand1t: ${MSDIR}/ms.c ${MSDIR}/streec.c ${MSDIR}/rand1t.c
	${LINK.c} $^ ${OUTPUT_OPTION}

ms_rand2: ${MSDIR}/ms.c ${MSDIR}/streec.c ${MSDIR}/rand2.c
	${LINK.c} $^ ${OUTPUT_OPTION}

ms_rand2t: ${MSDIR}/ms.c ${MSDIR}/streec.c ${MSDIR}/rand2t.c
	${LINK.c} $^ ${OUTPUT_OPTION}
