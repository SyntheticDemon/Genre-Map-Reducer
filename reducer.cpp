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

int main(int argc ,char * argv[])
{
    int read_fd=atoi(argv[0]);
    int write_fd=atoi(argv[1]);
    char buffer[CHAR_BUFFER_LENGTH];
    cout <<"Reducer received file descritors" << read_fd << write_fd << endl;
    close(write_fd);
    read(read_fd,buffer,CHAR_BUFFER_LENGTH);
    cout << "Reducer Read from pipe "<<buffer << endl;
    return 0;
}