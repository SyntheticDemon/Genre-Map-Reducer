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

bool does_file_exist(const char *file_name)
{
    std::ifstream infile(file_name);
    return infile.good();
}

bool is_fifo(const char *path)
{
    std::error_code ec;
    bool res = filesystem::is_fifo(path, ec);
    if (ec.value() != 0)
        std::cerr << ec.message() << std::endl;
    return res;
}

int main(int argc, char *argv[])
{
    int read_fd = atoi(argv[0]);
    int write_fd = atoi(argv[1]);
    char buffer[CHAR_BUFFER_LENGTH];
    // cout << "Reducer received file descritors " << read_fd << " " << write_fd << endl;
    vector<string> decoded_response = read_and_decode_pipe_message(read_fd);
    string genre = decoded_response[decoded_response.size() - 2];
    int file_count = stoi(decoded_response[decoded_response.size() - 1]);
    string pipe_name = "/tmp/processed";
    int status;
    vector<int> received_count;
    for (int i = 1; i < file_count; i++)
    {
        int status;
        int pipe_util[2];
        int ret_value = pipe(pipe_util);
        int pid = fork();
        if (pid == 0)
        {
            string file_dir = pipe_name;
            string comm;
            file_dir.append(to_string(i));
            file_dir.append(genre);
            execl("./reduce_communicator", to_string(pipe_util[READ]).c_str(), to_string(pipe_util[WRITE]).c_str(),
                  file_dir.c_str(),
                  (char *)0);
            exit(0);
        }
        else
        {
            wait(&status);
            char buffer[80];
            read(pipe_util[READ], buffer, sizeof(buffer));
            // cout << "Received count from reduce communicator " << buffer << endl;
            received_count.push_back(atoi(buffer));
            close(write_fd);
        }
    }
    
    int zero = 0;
    for (int i = 0; i < received_count.size(); i++)
    {
        zero = zero + received_count[i];
    }
    cout << "Genre reducer final result " << genre << " " << zero << endl;

    return 0;
}