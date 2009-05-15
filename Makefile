
CFLAGS += -ggdb -Wall
OBJECTS := ssi.o commands.o dir.o

ssi: $(OBJECTS)
	gcc -o ssi $(OBJECTS)

clean:
	$(RM) *.o

