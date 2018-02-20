
CFLAGS += -ggdb -Wall
OBJECTS := rsi.o commands.o
TARGET=rsi

$(TARGET): $(OBJECTS)
	gcc -o $(TARGET) $(OBJECTS)

fmt:
	clang-format -i *.c *.h

clean:
	$(RM) *.o $(TARGET)

.PHONY: fmt clean
