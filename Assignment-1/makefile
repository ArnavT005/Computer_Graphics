CC = g++
CFLAGS = `pkg-config --cflags SDL2 SDL2_image`
LIBS = `pkg-config --libs SDL2 SDL2_image`
CPP = main.cpp softwareRasterizer.cpp
OBJ = $(CPP:.cpp=.o)
EXE = rasterizer.out
IMG = out.png

.PHONY: run
run: $(EXE)
	./$<

$(EXE): $(OBJ)
	$(CC) $(LIBS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -c

.PHONY: clean
clean:
	rm $(OBJ)

.PHONY: remove
remove:
	rm $(OBJ) $(EXE)

.PHONY: destroy
destroy:
	rm $(OBJ) $(EXE) $(IMG)