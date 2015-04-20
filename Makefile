
# File Makefile

files := $(shell ls)
srcs  := $(filter %.cpp, $(files))
objs  := $(patsubst %.cpp, %.o, $(srcs))

CXX := g++
CXXFLAGS := -Wall -O

all : main
.PHONY : all

main : $(objs)
	$(CXX)  $^ -o $@ $(LDFLAGS)

%.o : %.cpp %.h

.PHONY : log_clean clean print
print :
	echo $(objs)
 
clean:
	-rm main *.o *~

