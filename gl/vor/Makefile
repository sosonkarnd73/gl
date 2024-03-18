LDLIBS=-lGLEW -lGL -lglut
test : vor.o line.o point.o region.o vor.o vert.o
	g++ -o test vor.o vert.o line.o point.o region.o $(LDLIBS)
vor.o : vor.cpp vor.h 
	g++ -c vor.cpp
line.o : line.cpp vor.h
	g++ -c line.cpp
point.o : point.cpp vor.h
	g++ -c point.cpp
region.o : region.cpp vor.h
	g++ -c region.cpp
vert.o : vert2.cpp vor.h
	g++ -c vert2.cpp -o vert.o
clean :
	rm ./*.o test ./*~
