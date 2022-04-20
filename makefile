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
CFLAGS=  -mthreads -O0 -w -Wno-invalid-offsetof 
CFLAGS+= -Wformat $(DEBUGFLAGS) $(WXDEFS) $(INCPATH)
#CXXFLAGS      = -O2 -frtti -fexceptions -mthreads -Wall $(DEFINES)

MKDIR_P = mkdir -p
OUT_DIR = ./bin/test/

all:
	echo $(LIBPATHS) >> out.txt

all: directories bin/test/bignum-test.exe

#obj/bignum.o: src/math/bignum.h
#	$(CXX) -c $(CFLAGS) -O3 -o $@ $<

obj/bignum-test.o: test/math/bignum.cpp src/math/bignum.h
	$(CXX) -c $(CFLAGS) -O3 -o $@ $<
  
OBJS= \
	 obj/bignum-test.o 

directories: 
	${MKDIR_P} ${OUT_DIR}

bin/test/bignum-test.exe: $(OBJS)
	$(CXX) $(CFLAGS) $(WINFLAGS) -o $@ $(LIBPATHS) $(OBJS) $(LIBS)

clean:
	echo $(OS)
	rm -f obj/*
	rm -f bin/test/*.exe