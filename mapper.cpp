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
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "utils.h"
#define BUFFER_LENGTH 10000
#define READ 0
#define WRITE 1
void dump_genres_to_respective_files(string file_no, vector<vector<string>> file_contents)
{
    string processed = "processed";
    for (int i = 0; i < file_contents.size(); i++)
    {
        string temp_file = processed;
        temp_file.append(file_no);
        string book_name = file_contents[i][0];
        for (int j = 1; j < file_contents[i].size(); j++)
        {
            string genre_name = file_contents[i][j];
            string full_file_name = temp_file.append(genre_name);
            cout << "Opening fifo" << endl;
            mkfifo(full_file_name.c_str(), 0666);
            cout << "Opening file" << endl;
            int fifo_fd = open(full_file_name.c_str(), O_APPEND | O_NONBLOCK);
            int shit = fork();
            if (shit == 0)
            {   execl("./a.out","a.out",(char *)0);
                exit(0);
            }
            else
            {
                write(fifo_fd, file_contents[i][0].c_str(), file_contents[i][j].size());
            }

            cout << "Write comeleted" << endl;
        }
    }
}

vector<string> read_and_decode_message(int fd)
{
    char buffer[BUFFER_LENGTH] = {0};
    vector<string> decoded_messages;
    char ch;
    read(fd, buffer, BUFFER_LENGTH);
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
    vector<vector<string>> file_contents;
    char ch;
    cout << "Mapper received file descritors " << read_fd << " " << write_fd << endl;
    vector<string> results = read_and_decode_message(read_fd);
    string where_to_read = results[results.size() - 1];
    string file_no = results[results.size() - 2];
    cout << "Reading file no : " << file_no << " Where to read :" << where_to_read << endl;
    ifstream input_file;

    input_file.open(where_to_read);
    stringstream string_stream;
    string_stream << input_file.rdbuf();
    string contents_str = string_stream.str();
    read_csv(&contents_str[0], file_contents);
    dump_genres_to_respective_files(file_no, file_contents);
    close(write_fd);

    return 0;
}