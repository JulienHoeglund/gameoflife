CC=g++
CFLAGS=-Wall
OBJ=GameOfLife.o
LIBS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)
GameOfLife: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
clean:
	$(RM) *.o
	$(RM) GameOfLife
test:
	$ ./GameOfLife