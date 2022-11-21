
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
#include <string.h>
#include <errno.h>
#include "utils.h"
#include <iterator>
#include <map>

#define BUFFER_LENGTH 10000
#define READ 0
#define WRITE 1
std::map<string, int> return_counts(vector<vector<string>> file_contents)
{
    map<string, int> counts;
    for (int i = 0; i < file_contents.size(); i++)
    {
        for (int j = 1; j < file_contents[i].size(); j++)
        {
            auto it = counts.find(file_contents[i][j]);
            if (it == counts.end())
            {
                counts.insert(make_pair(file_contents[i][j], 1));
            }
            else
            {
                it->second++;
            }
        }
    }
    return counts;
}

void dump_genres_to_respective_files(string file_no, vector<vector<string>> file_contents)
{
    string processed = "processed";
    map<string, int> counts = return_counts(file_contents);
    int status;
    int pipe_util[2];
    int ret_value = pipe(pipe_util);
    for (auto iter = counts.begin(); iter != counts.end(); ++iter)
    {
        string genre = iter->first;
        int count = iter->second;
        int fork_return = fork();
        if (fork_return == 0)
        {
            execl("./map_communicator", to_string(count).c_str(), genre.c_str(), (char *)0);
            exit(0);
        }
        // else
        // {
        //     cout << "Key : " << iter->first << " Value " << iter->second << " File number " << file_no << endl;
        //     cout << "File number : " << file_no << " Count : " << count << " Genre " << genre << endl;
        // }
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
    print_2d_vector(file_contents);
    // cout << "Mapper received file descritors " << read_fd << " " << write_fd << endl;
    vector<string> results = read_and_decode_message(read_fd);
    string where_to_read = results[results.size() - 1];
    string file_no = results[results.size() - 2];
    // cout << "Reading file no : " << file_no << " Where to read :" << where_to_read << endl;
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