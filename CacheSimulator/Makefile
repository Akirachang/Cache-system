CacheSimulator: main.o CacheSimulator.o
	g++ -o CacheSimulator main.o CacheSimulator.o
main.o: main.cpp CacheSimulator.h
	g++ -o main.o -c main.cpp
CacheSimulator.o: CacheSimulator.cpp CacheSimulator.h
	g++ -o CacheSimulator.o -c CacheSimulator.cpp
run:
	make
	./CacheSimulator
clean:
	rm *.o CacheSimulator