.PHONY : clean

vpath_src=.. ../../fortify ../../randlib/src ../CaNew
vpath %.c    $(vpath_src)
vpath %.cpp  $(vpath_src)
vpath %.hpp  $(vpath_src)
vpath %.h    $(vpath_src)

# The X11 base dir on your system
X11BASE=/usr/X11R6
# Add directories with X11 include files here
X11INCS=-I$(X11BASE)/include
# put X11 required libraries and directories here
X11LIBS=-L$(X11BASE)/lib -lX11

SDLDEFS = -D__XWIN__

I_DIRS=-I../../fortify -I.. -I../../randlib/src -I../CaNew
#P_DEFS=-DGRAPHICS -DPERIODIC_BOUNDARY

#CXXFLAGS = -O3 -Wall -std=gnu++0x $(I_DIRS) $(X11INCS)  $(SDLDEFS) $(P_DEFS)
CXXFLAGS = -g -Wall -std=gnu++0x $(I_DIRS) $(X11INCS)  $(SDLDEFS) $(P_DEFS)

O = main_hk.o hk.o 

L = -lm -ltiff

# Compile in 64 bit or 32 bit 
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
   E = hk64
else
   E = hk
endif

$(E): $(O) 
	g++ -o $(E) $(O) $(L)


clean:
	rm $(E) $(O)


# DEPENDENCIES

hk.o: main_hk.cpp hk.cpp RWFile.h smattpl.h


