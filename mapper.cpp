
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
                counts.insert(make_pair(file_contents[i][j], 0));
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
    std::map<std::string, int> genre_to_fd;
    map<string, int> counts = return_counts(file_contents);
    cout << counts.find("History")->second << "History" << endl;
    execl("./test", "./test", (char *)0);
    for (int i = 0; i < file_contents.size(); i++)
    {
        string book_name = file_contents[i][0];
        for (int j = 1; j < file_contents[i].size(); j++)
        {

            // string temp_file = processed;
            // string genre_name = file_contents[i][j];
            // temp_file.append(genre_name);
            // auto it = genre_to_fd.find(genre_name);
            // int genre_fd = it->second;
            // cout << "Genre fd " << genre_fd << endl;
            // if (it == genre_to_fd.end())
            // {
            //     cout << "Opening fifo " << temp_file << endl;
            //     int fifo_result = mkfifo(temp_file.c_str(), 0666);
            //     std::cout << "errno " << errno << std::endl;
            //     std::cout << "errno str::" << strerror(errno) << std::endl;
            //     cout << "Opening fifo result : " << fifo_result << endl;
            //     int fifo_fd = open(temp_file.c_str(), O_CREAT | O_APPEND | O_NONBLOCK);
            //     if (fifo_fd > 0)
            //     {

            //         genre_to_fd.insert(make_pair(genre_name, fifo_fd));
            //         cout << "Inserted received fd" << genre_to_fd.find(genre_name)->second << endl;
            //     }
            //     else
            //     {
            //         cout << "Could not open fifo and return it's fd" << endl;
            //     }
            // }
            // else
            // {
            //     cout << "Fifo had already existed" << endl;
            // }

            // int shit = fork();
            // int a =write(genre_fd, book_name.c_str(), book_name.size());
            // cout << "Write comeleted"<< a << endl;
            // temp_file.clear();
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