FLAGS = -Wall -g -lm -lpthread -lX11
OFILES = image_ppm.o mosaique_Grey.o DataBase_Grey.o mosaique_Color.o DataBase_Color.o
PATH_SRC = ./src

run : build
	./main
	
build : $(OFILES) main.o 
	g++ -o main *.o $(FLAGS) 

image_ppm.o : $(PATH_SRC)/image_ppm.cpp $(PATH_SRC)/image_ppm.h 
	g++ -c $< $(FLAGS) 
	
mosaique_Grey.o : $(PATH_SRC)/mosaique_Grey.cpp $(PATH_SRC)/mosaique_Grey.h 
	g++ -c $< $(FLAGS) 

DataBase_Grey.o : $(PATH_SRC)/DataBase_Grey.cpp $(PATH_SRC)/DataBase_Grey.h 
	g++ -c $< $(FLAGS) 

mosaique_Color.o : $(PATH_SRC)/mosaique_Color.cpp $(PATH_SRC)/mosaique_Color.h 
	g++ -c $< $(FLAGS) 

DataBase_Color.o : $(PATH_SRC)/DataBase_Color.cpp $(PATH_SRC)/DataBase_Color.h 
	g++ -c $< $(FLAGS) 

main.o : main.cpp
	g++ -c $< $(FLAGS)

# valgrind : build 
# 	valgrind --track-origins=yes ./main

clean: 
	rm -f *.o main
