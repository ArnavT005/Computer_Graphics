CC = g++
CFLAGS = `pkg-config --cflags SDL2 SDL2_image`
LIBS = `pkg-config --libs SDL2 SDL2_image`
CPP = main.cpp softwareRasterizer.cpp
EXE = $(CPP:.cpp=.o)

.PHONY: run
run: rasterizer
	./rasterizer

rasterizer: $(EXE)
	$(CC) $(LIBS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -c

.PHONY: clean
clean:
	rm *.o

.PHONY: remove
remove:
	rm rasterizer