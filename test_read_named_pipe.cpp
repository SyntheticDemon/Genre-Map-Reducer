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
int main(int argc ,char * argv[])
{
    char buffer[100];
    string name = "processed1Fiction";
    int fd = open(name.c_str(), O_RDONLY|O_NONBLOCK);
    cout << fd ;
    read(fd,buffer,100);
    cout << "Parallel buffer"<< buffer << "Parallel Buffer"<< endl;
    return 0;
}