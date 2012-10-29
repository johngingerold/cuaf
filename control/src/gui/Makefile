OBJS=$(patsubst %.cpp,%.o,$(wildcard *.cpp))

CXXINCLUDE+=-I./  -I../control -I../camera -I../map -I/usr/local/include
CXXFLAGS+=-Wall

all: $(OBJS)

.cpp.o:
	$(CXX)  $(CXXFLAGS) $(CXXDEFINE) -c  $(CXXINCLUDE) $< 

.c.o:
	$(CXX)  $(FLAGS) $(CXXDEFINE) -c  $(CXXFLAGS) $(CXXINCLUDE) $< 

clean:
	$(RM) $(OBJS)
