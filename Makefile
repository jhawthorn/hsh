
CFLAGS += -ggdb -Wall
OBJECTS := hsh.o commands.o
TARGET=hsh

$(TARGET): $(OBJECTS)
	gcc -o $(TARGET) $(OBJECTS)

fmt:
	clang-format -i *.c *.h

clean:
	$(RM) *.o $(TARGET)

.PHONY: fmt clean
