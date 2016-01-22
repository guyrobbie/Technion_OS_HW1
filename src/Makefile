# Makefile for the smash program
CC = gcc
CFLAGS = -g -Wall
CCLINK = $(CC) $(CFLAGS)
OBJS = smash.o list.o vars.o commands.o signals.o
RM = rm -f
# Creating the  executable
smash: $(OBJS)
	$(CCLINK) -o smash $(OBJS)
    
# Creating the object files
commands.o: commands.c commands.h list.h vars.h

list.o: list.c list.h

smash.o: smash.c commands.h list.h vars.h 

variables.o: vars.c vars.h list.h

signals.o: signals.c signals.h

# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

