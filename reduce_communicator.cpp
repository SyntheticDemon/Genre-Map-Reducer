
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
    int read_fd = atoi(argv[0]);
    int write_fd = atoi(argv[1]);
    string file_path(argv[2]);
    // cout << "Reducer Communicator received file descritors "
        //  << read_fd << " " << write_fd << " " << file_path << " " << endl;
    mkfifo(file_path.c_str(), 0666);
    while (1)
    {
        char buffer[80] = {0};
        fd = open(file_path.c_str(), O_RDONLY);
        read(fd, buffer, sizeof(buffer));
        // cout << "Received message buffer" << endl;
        write(write_fd, buffer ,sizeof(buffer));
        close(fd);
        break;
    }
    close(read_fd);
    return 0;
}
