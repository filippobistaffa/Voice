.PHONY: all

ifndef OUT
OUT=./voice
endif

CMP=g++
WARN=-Wall -Wno-unused-result -Wno-deprecated-declarations -Wno-sign-compare -Wno-maybe-uninitialized
OPTIM=-Ofast -march=native -funroll-loops -funsafe-loop-optimizations -falign-functions=16 -falign-loops=16 -fopenmp
NOOPTIM=-O0 -march=native -fopenmp
DBG=-g ${NOOPTIM}

DEFS=
INC=${DEFS}
LDIR=
LINK=

COBJSUBDIR=cobj
DEPSUBDIR=dep

ECHOCC=>&2 echo "[\033[01;33m CC \033[0m]"
ECHOLD=>&2 echo "[\033[01;36m LD \033[0m]"
ECHOJC=>&2 echo "[\033[01;35m JC \033[0m]"

OPT=${OPTIM} # Put desired optimisation level here

define compilec
${ECHOCC} $(notdir $<) ;\
mkdir -p ${DEPSUBDIR} ;\
tmp=`mktemp` ;\
${CMP} ${DEFS} ${INC} -MM ${OPT} $< >> $$tmp ;\
if [ $$? -eq 0 ] ;\
then echo -n "${COBJSUBDIR}/" > ${DEPSUBDIR}/$(notdir $<).d ;\
cat $$tmp >> ${DEPSUBDIR}/$(notdir $<).d ;\
rm $$tmp ;\
mkdir -p ${COBJSUBDIR} ;\
cd ${COBJSUBDIR} ;\
${CMP} ${DEFS} -c ${INC} ${OPT} ${WARN} ../$< ;\
else \
ret=$$? ;\
rm $$tmp ;\
exit $$ret ;\
fi
endef

all: voice
	@true

-include ${DEPSUBDIR}/*.d

voice: ${COBJSUBDIR}/voice.o
	@${ECHOLD} voice
	@${CMP} ${OPT} ${LDIR} $^ ${LINK} -o ${OUT}

order:
	@${ECHOJC} Order
	@javac -cp .:* Order.java

clean:
	@echo "Removing subdirectories..."
	@rm -rf ${COBJSUBDIR} ${DEPSUBDIR}
