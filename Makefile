# Build mode
# 0: development (max safety, no optimisation)
# 1: release (min safety, optimisation)

BUILD_MODE?=0

# Path to PBMake

PATH_PBMAKE=../../GitHub/PBMake

# Compiler arguments depending on BUILD_MODE

ifeq ($(BUILD_MODE), 0)
	BUILD_ARG=-I$(PATH_PBMAKE)/Include -Wall -Wextra -Og -ggdb -g3 -DPBERRALL='1' \
	  -DBUILDMODE=$(BUILD_MODE) 
	LINK_ARG=-L$(PATH_PBMAKE)/Lib -lpbdevgtk -lm -rdynamic
else 
  ifeq ($(BUILD_MODE), 1)
	  BUILD_ARG=-I$(PATH_PBMAKE)/Include -Wall -Wextra -Werror -Wfatal-errors -O3 \
		  -DPBERRSAFEMALLOC='1' -DPBERRSAFEIO='1' -DBUILDMODE=$(BUILD_MODE)
	  LINK_ARG=-L$(PATH_PBMAKE)/Lib -lpbreleasegtk -lm -rdynamic
	endif
endif

# Compiler argument  for GTK

GTKPARAM=`pkg-config --cflags gtk+-3.0` -DBUILDWITHGRAPHICLIB=1
GTKLINK=`pkg-config --libs gtk+-3.0`
CAIROPARAM=`pkg-config --cflags cairo`
CAIROLINK=`pkg-config --libs cairo`
GTK_BUILD_ARG:=$(GTKPARAM) $(CAIROPARAM)
GTK_LINK_ARG:=-lm $(GTKLINK) $(CAIROLINK)

# Compiler

COMPILER=gcc-7

# Rules for the executable

all: clean main

main: main.o gui.o Makefile
	$(COMPILER) main.o gui.o $(LINK_ARG) $(GTK_LINK_ARG) -o main 

main.o: main.c gui.h Makefile
	$(COMPILER) $(BUILD_ARG) $(GTK_BUILD_ARG) -c main.c 

clean:
	rm -f *.o main

gui.o: gui.c drawing.c gui-callbacks.c gui.h Makefile
	$(COMPILER) $(BUILD_ARG) $(GTK_BUILD_ARG) -c gui.c

run:
	main -root ./ -conf ./appConf.json
