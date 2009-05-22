
CFLAGS += -ggdb -Wall
OBJECTS := rsi.o commands.o

ssi: $(OBJECTS)
	gcc -o rsi $(OBJECTS)

clean:
	$(RM) *.o rsi

