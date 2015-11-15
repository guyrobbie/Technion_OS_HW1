# Makefile for the smash program
CC = gcc
CFLAGS = -g -Wall
CCLINK = $(CC)
OBJS = infi.o 
RM = rm -f
# Creating the  executable
infi: $(OBJS)
	$(CCLINK) -o infi $(OBJS)
# Creating the object files
infi.o: infi.c
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

