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
#define BUFFER_LENGTH 1000
int main(int argc ,char * argv[])
{
    char buffer[BUFFER_LENGTH];
    cout <<"Genre file descritors" << argv[0] << argv[1] << endl;
    close(atoi(argv[1]));
    // read(atoi(argv[0]),buffer,sizeof(char)*BUFFER_LENGTH);
    cout << buffer;
    return 0;
}