#SOURCES = main.cpp player.cpp 
OBJECTS = main.o screenSaver.o
EXECUTABLE = screenSaver

run : $(EXECUTABLE) 
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	g++ -o $(EXECUTABLE) $(OBJECTS) -lsfml-graphics -lsfml-window -lsfml-system

%.o : %.cpp %.h
	g++ -c $<

clean:
	rm *.o $(EXECUTABLE)
