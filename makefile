all: map_reducer mapper reducer reduce test
map_reducer :  
	g++ map_reducer.cpp utils.cpp -o map_reducer
mapper : reducer.cpp utils.cpp
	g++ mapper.cpp  utils.cpp -o mapper
reducer : mapper.cpp utils.cpp 
	g++ reducer.cpp utils.cpp -o reducer
reduce : reduce.cpp utils.cpp
	g++ reduce.cpp utils.cpp -o reduce
test : test.cpp utils.cpp
	g++ test.cpp utils.cpp -o test
.PHONY: clean
clean:
	rm mapper processed* reducer map_reducer