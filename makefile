CC     = gcc
CLFAGS = -Wall -g
BUILD_DIR = .\src\obj

src = $(wildcard src/*.c)
obj = $(patsubst src/%.c,src/obj/%.o,$(src))

.PHONY: all game checkdirs clean

all: checkdirs game

game: $(obj)
	$(CC) -o HoboSim.exe $^ $(CFLAGS) 

src/obj/%.o: src/%.c
	$(CC) -c $< -o $@ $(CLFAGS)

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir $@

clean:
	del /q $(BUILD_DIR)
	del HoboSim.exe
