ifneq "$(BUILD)" "debug"
  ifneq "$(BUILD)" "release"
    BUILD=debug
  endif
endif

ifeq "$(BUILD)" "debug"
  D=d
  # note: gcc 3.x profile doesn't work
  #DEBUGFLAGS=-O0 -g -pg -D__WXDEBUG__
  DEBUGFLAGS=-g -D__WXDEBUG__
endif

CC            = gcc
CXX           = g++
WINFLAGS=
CURR_OS=$(findstring Windows,$(OS))
MAKEFLAGS+= -j 4

# -I"./wxWidgets/lib/vc_lib/mswd" 
# -I"./wxWidgets/include" 
INCPATH=-I"./boost" -I"./DB/build_unix" -I"./OpenSSL/include" -I"./src" 

#  -L"./wxWidgets/lib/gcc_lib" 
LIBPATHS=-L"./DB/build_unix" -L"./OpenSSL"

# -l rpcrt4
# -l wxmsw28$(D)_richtext -l wxmsw28$(D)_html -l wxmsw28$(D)_core -l wxbase28$(D) -l wxtiff$(D) -l wxjpeg$(D) -l wxpng$(D) -l wxzlib$(D) -l wxregex$(D) -l wxexpat$(D)
LIBS=-l db_cxx -l crypto 

# -mwindows -Wl,--subsystem,windows
ifeq "$(CURR_OS)" "Windows"
  LIBS+=-l kernel32 -l user32 -l gdi32 
  LIBS+=-l comdlg32 -l winspool -l winmm -l shell32 -l comctl32 
  LIBS+=-l ole32 -l oleaut32 -l uuid  -l advapi32 -l ws2_32
  WINFLAGS=-mconsole
else
  CURR_OS=NO_WIN
endif

WXDEFS=-DWIN32 -D__WXMSW__ -D_WINDOWS -DNOPCH
#-std=c++11 -Wmissing-braces
CFLAGS= -mthreads -O0 -w -Wno-invalid-offsetof 
CFLAGS+= -Wformat $(DEBUGFLAGS) $(WXDEFS) $(INCPATH)
#CXXFLAGS      = -O2 -frtti -fexceptions -mthreads -Wall $(DEFINES)

MKDIR_P = mkdir -p
OUT_DIR = ./bin/test/

BASE_1=bignum
BASE_2=hash
BASE_3=rand
BASE_4=time
BASE_5=string
BASE_6=money
BASE_7=convert
BASE_8=base58
BASE_9=address
BASE_10=sync
BASE_11=key
BASE_12=blockfile
BASE_13=app
BASE_14=log
BASE_15=caddress
BASE_16=cmessageheader
BASE_17=net
BASE_18=cnode
BASE_19=coutpoint
BASE_20=cdisktxpos
BASE_21=ctxindex
BASE_22=script
BASE_23=ctxin
BASE_24=ctxout
BASE_25=cinpoint
BASE_26=ctransaction

BASE_50=domain

OBJS1= \
	 obj/$(BASE_1)-test.o 

OBJS2= \
	 obj/$(BASE_2)-test.o 

OBJS3= \
	 obj/$(BASE_4).o \
	 obj/$(BASE_3).o \
	 obj/$(BASE_3)-test.o 

OBJS4= \
	 obj/$(BASE_4).o \
	 obj/$(BASE_4)-test.o 

OBJS5= \
	 obj/$(BASE_5).o \
	 obj/$(BASE_5)-test.o 

OBJS6= \
	 obj/$(BASE_7).o \
	 obj/$(BASE_6).o \
	 obj/$(BASE_5).o \
	 obj/$(BASE_6)-test.o 

OBJS7= \
	 obj/$(BASE_7).o \
	 obj/$(BASE_7)-test.o 

OBJS8= \
	 obj/$(BASE_8).o \
	 obj/$(BASE_8)-test.o 

OBJS9= \
	 obj/$(BASE_9).o \
	 obj/$(BASE_9)-test.o 

OBJS10= \
	 obj/$(BASE_10).o \
	 obj/$(BASE_10)-test.o 

OBJS11= \
	 obj/$(BASE_5).o \
	 obj/$(BASE_11)-test.o 

OBJS12= \
	 obj/$(BASE_5).o \
	 obj/$(BASE_14).o \
	 obj/$(BASE_13).o \
	 obj/$(BASE_12).o \
	 obj/$(BASE_12)-test.o 

OBJS13= \
	 obj/$(BASE_13).o 

OBJS14= \
	 obj/$(BASE_14).o 

OBJS15= \
	 obj/$(BASE_4).o \
	 obj/$(BASE_5).o \
	 obj/$(BASE_15).o \
	 obj/$(BASE_15)-test.o 

OBJS16= \
	 obj/$(BASE_16).o \
	 obj/$(BASE_16)-test.o 

OBJS17= \
	 obj/$(BASE_3).o \
	 obj/$(BASE_4).o \
	 obj/$(BASE_5).o \
	 obj/$(BASE_18).o \
	 obj/$(BASE_17).o \
	 obj/$(BASE_17)-test.o 

OBJS19= \
	 obj/$(BASE_19).o 

OBJS50= \
	 obj/$(BASE_5).o \
	 obj/$(BASE_12).o \
	 obj/$(BASE_13).o \
	 obj/$(BASE_14).o \
	 obj/$(BASE_19).o \
	 obj/$(BASE_20).o \
	 obj/$(BASE_21).o \
	 obj/$(BASE_22).o \
	 obj/$(BASE_23).o \
	 obj/$(BASE_24).o \
	 obj/$(BASE_25).o \
	 obj/$(BASE_26).o \
	 obj/$(BASE_50)-test.o 


all:
	echo $(LIBPATHS) >> out.txt

all: directories \
  bin/test/$(BASE_50)-test.exe \
  bin/test/$(BASE_17)-test.exe \
  bin/test/$(BASE_16)-test.exe \
  bin/test/$(BASE_15)-test.exe \
  bin/test/$(BASE_12)-test.exe \
  bin/test/$(BASE_11)-test.exe \
  bin/test/$(BASE_10)-test.exe \
  bin/test/$(BASE_9)-test.exe \
  bin/test/$(BASE_8)-test.exe \
  bin/test/$(BASE_7)-test.exe \
  bin/test/$(BASE_6)-test.exe \
  bin/test/$(BASE_5)-test.exe \
  bin/test/$(BASE_4)-test.exe \
  bin/test/$(BASE_3)-test.exe \
  bin/test/$(BASE_2)-test.exe \
  bin/test/$(BASE_1)-test.exe

#obj/bignum.o: src/math/bignum.h
#	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_1)-test.o: test/math/$(BASE_1).cpp src/math/$(BASE_1).h 
	@echo $(BASE_1)
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_2)-test.o: test/crypto/$(BASE_2).cpp
	@echo $(BASE_2)
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_3).o: src/crypto/$(BASE_3).cpp 
	@echo $(BASE_3) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_3)-test.o: test/crypto/$(BASE_3).cpp
	@echo $(BASE_3) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_4).o: src/utils/$(BASE_4).cpp 
	@echo $(BASE_4) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_4)-test.o: test/utils/$(BASE_4).cpp 
	@echo $(BASE_4) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_5).o: src/utils/$(BASE_5).cpp 
	@echo $(BASE_5) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_5)-test.o: test/utils/$(BASE_5).cpp 
	@echo $(BASE_5) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_6).o: src/utils/$(BASE_6).cpp 
	@echo $(BASE_6) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_6)-test.o: test/utils/$(BASE_6).cpp 
	@echo $(BASE_6) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_7).o: src/utils/$(BASE_7).cpp 
	@echo $(BASE_7) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_7)-test.o: test/utils/$(BASE_7).cpp 
	@echo $(BASE_7) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_8).o: src/encode/$(BASE_8).cpp 
	@echo $(BASE_8) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_8)-test.o: test/encode/$(BASE_8).cpp 
	@echo $(BASE_8) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_9).o: src/utils/$(BASE_9).cpp 
	@echo $(BASE_9) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_9)-test.o: test/utils/$(BASE_9).cpp 
	@echo $(BASE_9) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_10).o: src/thread/$(BASE_10).cpp 
	@echo $(BASE_10) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_10)-test.o: test/thread/$(BASE_10).cpp 
	@echo $(BASE_10) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_11)-test.o: test/crypto/$(BASE_11).cpp 
	@echo $(BASE_11) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_12).o: src/utils/$(BASE_12).cpp 
	@echo $(BASE_12) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_12)-test.o: test/utils/$(BASE_12).cpp 
	@echo $(BASE_12) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_13).o: src/utils/$(BASE_13).cpp 
	@echo $(BASE_13) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_14).o: src/utils/$(BASE_14).cpp 
	@echo $(BASE_14) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_15).o: src/net/$(BASE_15).cpp 
	@echo $(BASE_15) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_15)-test.o: test/net/$(BASE_15).cpp 
	@echo $(BASE_15) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_16).o: src/net/$(BASE_16).cpp 
	@echo $(BASE_16) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_16)-test.o: test/net/$(BASE_16).cpp 
	@echo $(BASE_16) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_17).o: src/net/$(BASE_17).cpp 
	@echo $(BASE_17) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_17)-test.o: test/net/$(BASE_17).cpp 
	@echo $(BASE_17) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_18).o: src/net/$(BASE_18).cpp 
	@echo $(BASE_18) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_19).o: src/domain/$(BASE_19).cpp 
	@echo $(BASE_19) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_20).o: src/domain/$(BASE_20).cpp 
	@echo $(BASE_20) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_21).o: src/domain/$(BASE_21).cpp 
	@echo $(BASE_21) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_22).o: src/script/$(BASE_22).cpp 
	@echo $(BASE_22) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_23).o: src/domain/$(BASE_23).cpp 
	@echo $(BASE_23) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_24).o: src/domain/$(BASE_24).cpp 
	@echo $(BASE_24) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_25).o: src/domain/$(BASE_25).cpp 
	@echo $(BASE_25) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_26).o: src/domain/$(BASE_26).cpp 
	@echo $(BASE_26) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/$(BASE_50)-test.o: test/domain/$(BASE_50).cpp 
	@echo $(BASE_50) $?
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

directories: 
	${MKDIR_P} ${OUT_DIR}

bin/test/$(BASE_1)-test.exe: $(OBJS1)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS1) $(LIBS)

bin/test/$(BASE_2)-test.exe: $(OBJS2)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS2) $(LIBS)

bin/test/$(BASE_3)-test.exe: $(OBJS3)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS3) $(LIBS)

bin/test/$(BASE_4)-test.exe: $(OBJS4)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS4) $(LIBS)

bin/test/$(BASE_5)-test.exe: $(OBJS5)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS5) $(LIBS)

bin/test/$(BASE_6)-test.exe: $(OBJS6)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS6) $(LIBS)

bin/test/$(BASE_7)-test.exe: $(OBJS7)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS7) $(LIBS)

bin/test/$(BASE_8)-test.exe: $(OBJS8)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS8) $(LIBS)

bin/test/$(BASE_9)-test.exe: $(OBJS9)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS9) $(LIBS)

bin/test/$(BASE_10)-test.exe: $(OBJS10)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS10) $(LIBS)

bin/test/$(BASE_11)-test.exe: $(OBJS11)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS11) $(LIBS)

bin/test/$(BASE_12)-test.exe: $(OBJS12)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS12) $(LIBS)

bin/test/$(BASE_15)-test.exe: $(OBJS15)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS15) $(LIBS)

bin/test/$(BASE_16)-test.exe: $(OBJS16)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS16) $(LIBS)

bin/test/$(BASE_17)-test.exe: $(OBJS17)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS17) $(LIBS)

bin/test/$(BASE_50)-test.exe: $(OBJS50)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS50) $(LIBS)

run-test:
	bin/test/$(BASE_1)-test 
	bin/test/$(BASE_2)-test 
	bin/test/$(BASE_3)-test 
	bin/test/$(BASE_4)-test 
	bin/test/$(BASE_5)-test 
	bin/test/$(BASE_6)-test 
	bin/test/$(BASE_7)-test 
	bin/test/$(BASE_8)-test 
	bin/test/$(BASE_9)-test 
	bin/test/$(BASE_10)-test
	bin/test/$(BASE_11)-test
	bin/test/$(BASE_12)-test
	bin/test/$(BASE_15)-test
	bin/test/$(BASE_16)-test
	bin/test/$(BASE_17)-test
	bin/test/$(BASE_50)-test

clean:
	echo $(OS)
	rm -f obj/*
	rm -f bin/test/*.exe