CC=g++
CFLAGS=-Wall -O2 -I.
DEPS = ConcurrentQueue.h ConcurrentBlockingQueue.h
OBJ = Testouille.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

demo: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

check: demo
	./demo

clean:
	rm *.o demo