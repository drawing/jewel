
source = $(shell ls *.cpp *.cc)

all : a.out
	
a.out : $(source)
	g++ $(source) -lgtest -lpthread -lprotobuf

clean:
	rm a.out
