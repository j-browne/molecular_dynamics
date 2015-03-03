CC      = g++
#CFLAGS  = -Wall -Wextra -O3 -march=native 
CFLAGS += -pedantic -g
#FFLAGS += $(shell pkg-config --cflags plplotd-f95)
#LDFLAGS = $(shell pkg-config --libs plplotd-f95)
LIBS    =

COMPILE = $(CC) $(CFLAGS)
LINK = $(CC) $(LDFLAGS)

TARGET = md.exe       # Name of final executable to produce
OBJS = md.o vec3.o particle.o rand_tools.o # List of object dependencies

$(TARGET): $(OBJS)
	$(LINK) -o $@ $^ $(LIBS)

%.o:%.cpp
	$(COMPILE) -c $<

clean: 
	rm $(OBJS)
