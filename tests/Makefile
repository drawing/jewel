
source = $(shell ls *.cpp)

all : a.out
	
a.out : $(source)
	g++ $(source) -lgtest  -lpthread

clean:
	rm a.out
