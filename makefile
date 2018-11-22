src  = $(wildcard src/*.c)
obj  = $(src:.c=.o)
head = $(wildcard src/*.h)
CC   = gcc


LDFLAGS = -g -Wall -Isrc

game: $(obj)
	$(CC) -o HoboSim.exe $^ $(LDFLAGS)


.PHONY: clean

clean:
	del ".\src\*.o"
	del "*.exe"
