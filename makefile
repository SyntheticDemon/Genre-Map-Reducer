all: map_reducer mapper reducer
map_reducer :  
	g++ map_reducer.cpp -o map_reducer
mapper : reducer.cpp
	g++ mapper.cpp -o mapper
reducer : mapper.cpp
	g++ reducer.cpp -o reducer
.PHONY: clean

clean:
	rm mapper reducer map_reducer