// C program to implement one side of FIFO
// This side reads first, then reads

using namespace std;
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <vector>
#include <fstream>
#include <sstream>
#include "utils.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    int fd;
    string file_path(argv[1]);
    string count(argv[0]);
    char buffer[CHAR_BUFFER_LENGTH];
    // cout << "Mapper communicator received arguments : " << count << " " << file_path << endl;

    file_path.append("processed");
    int fd1;
    mkfifo(file_path.c_str(), 0666);
    fd1 = open(file_path.c_str(), O_WRONLY  );
    count.append(",");
    int write_result = write(fd1, count.c_str(), count.size());
    cout <<" Write successful? " << write_result << endl;
    close(fd1);
    
    return 0;
}