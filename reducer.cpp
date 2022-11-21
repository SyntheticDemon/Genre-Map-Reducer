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
#include <filesystem>

bool is_fifo(const char *path)
{
    std::error_code ec;
    bool res = filesystem::is_fifo(path, ec);
    if (ec.value() != 0)
        std::cerr << ec.message() << std::endl;
    return res;
}

vector<string> read_and_decode_message(int fd)
{
    char buffer[CHAR_BUFFER_LENGTH] = {0};
    vector<string> decoded_messages;
    char ch;
    read(fd, buffer, CHAR_BUFFER_LENGTH);
    std::string s = buffer;
    auto start = 0U;
    string delim = ",";
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
    char buffer[CHAR_BUFFER_LENGTH];
    // cout << "Reducer received file descritors " << read_fd << " " << write_fd << endl;
    vector<string> decoded_response = read_and_decode_message(read_fd);
    string genre = decoded_response[decoded_response.size() - 2];
    int file_count = stoi(decoded_response[decoded_response.size() - 1]);
    string pipe_name = "processed";
    int status;
    int pipe_util[2];
    int ret_value = pipe(pipe_util);
    if (ret_value == 0)
    {
        int pid = fork();
        if (pid == 0)
        {
            close(write_fd);
            execl("./reduce_communicator", to_string(pipe_util[READ]).c_str(), to_string(pipe_util[WRITE]).c_str(),
            to_string(file_count).c_str(), genre.c_str(), (char *)0);
            exit(0);
        }
        else
        {
            close(pipe_util[WRITE]);
            // cout << "Waiting" << endl;
        }
    }

    return 0;
}