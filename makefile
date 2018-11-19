src = $(wildcard src/*.c)
obj = $(src:.c=.o)

LDFLAGS = -g -Wall

$(obj): $(src)
	gcc -c $(LDFLAGS) -o $@ $<

HoboSim: $(obj)
	gcc -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	del ".\src\*.o"
	del "*.exe"
