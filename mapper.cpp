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
#define BUFFER_LENGTH 10000
#define READ 0
#define WRITE 1
vector<string> read_and_decode_message(int fd){
    char buffer[BUFFER_LENGTH]={0};
    vector<string> decoded_messages;
    char ch;
    read(fd,buffer,BUFFER_LENGTH);
    std::string s=buffer;
    
    auto start = 0U;
    string delim=",";
    auto end = s.find(delim);
    while (end != std::string::npos)
    {
        decoded_messages.push_back(s.substr(start, end - start));
        start = end + delim.length();
        end = s.find(delim, start);
    }
    return decoded_messages;
}
int main(int argc, char *argv[])
{
    int read_fd = atoi(argv[0]);
    int write_fd = atoi(argv[1]);
    char buffer[BUFFER_LENGTH];
    char ch;
    cout << "Mapper received file descritors " << read_fd<<" "<< write_fd << endl;
    vector<string> results=read_and_decode_message(read_fd);
    cout << results[results.size()-1];
    
    close(write_fd);
    return 0;
}