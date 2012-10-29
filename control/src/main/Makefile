OBJS=$(patsubst %.cpp,%.o,$(wildcard *.cpp))

CXXINCLUDE+=-I./ -I../common -I../imageproc -I../gui -I../control
CXXINCLUDE+=-I/usr/local/include
CXXINCLUDE+=-I/opt/ros/fuerte/include/opencv -I/opt/ros/fuerte/include 

all: $(OBJS) 

.cpp.o:
	$(CXX)  $(CXXFLAGS) $(CXXDEFINE) -c  $(CXXINCLUDE) $< 

.c.o:
	$(CXX)  $(FLAGS) $(CXXDEFINE) -c  $(CXXFLAGS) $(CXXINCLUDE) $< 

clean:
	$(RM) $(OBJS) *.moc $(UI_HEAD) $(UI_CPP)
