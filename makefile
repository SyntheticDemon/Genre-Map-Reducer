all: map_reducer mapper reducer 
map_reducer :  
	g++ map_reducer.cpp utils.cpp -o map_reducer
mapper : reducer.cpp
	g++ mapper.cpp -o mapper
reducer : mapper.cpp utils.cpp
	g++ reducer.cpp utils.cpp -o reducer
.PHONY: clean

clean:
	rm mapper reducer map_reducer