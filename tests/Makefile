
source = $(shell ls *.cpp)
proto=test_node.pb.cc

all : a.out
	
a.out : $(proto) $(source)
	g++ $(source) $(proto) -lgtest -lpthread -lprotobuf

$(proto) : test_node.proto
	protoc --cpp_out=. test_node.proto
clean:
	rm a.out test_node.pb.cc test_node.pb.h
