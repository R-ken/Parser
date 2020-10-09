APPS = project

OBJECTS = parserClasses.o project.o
HEADERS = parserClasses.h

CXXFLAGS += -g -Wall -std=c++11

all: $(APPS)

$(OBJECTS): $(HEADERS)
$(APPS): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

clean:
	$(RM) *.o $(APPS)

tar:
	tar -czf Final_Project.tar.gz *.cpp *.h


