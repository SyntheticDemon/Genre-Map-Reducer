all: map_reducer mapper reducer reduce_communicator map_communicator
map_reducer :  
	g++ map_reducer.cpp utils.cpp -o map_reducer
mapper : mapper.cpp utils.cpp
	g++ mapper.cpp  utils.cpp -o mapper
map_communicator : map_communicator.cpp utils.cpp
	g++ map_communicator.cpp  utils.cpp -o map_communicator
reducer : mapper.cpp utils.cpp 
	g++ reducer.cpp utils.cpp -o reducer
reduce_communicator : reduce_communicator.cpp utils.cpp
	g++ reduce_communicator.cpp utils.cpp -o reduce_communicator
.PHONY: clean
clean:
	rm mapper *processed* reducer map_reducer map_communicator reduce_communicator