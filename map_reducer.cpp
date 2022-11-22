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
#include <dirent.h>
#include "string"
#include "utils.h"

string vec_to_csv(vector<vector<string>> v)
{
    string result_string;
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[0].size(); j++)
        {
            result_string.append(v[i][j]);
            result_string.append(",");
        }
    }
    return result_string;
}

string form_mapping_message(vector<vector<string>> genres, int i, string file_directory)
{
    string message = vec_to_csv(genres);
    message.append(to_string(i));
    message.append(",");
    string temp_file_dir = file_directory;
    temp_file_dir.append("/part");
    temp_file_dir.append(to_string(i));
    message.append(temp_file_dir);
    message.append(".csv");
    message.append(",");
    return message;
}
string form_reducing_message(string genre, int file_count)
{
    string message;
    message.append(genre);
    message.append(",");
    message.append(to_string(file_count));
    message.append(",");
    return message;
}
int count_part_files_in_dir(string file_dir)
{
    DIR *dp;
    int i = 0;
    struct dirent *ep;
    dp = opendir(file_dir.c_str());

    if (dp != NULL)
    {
        while (ep = readdir(dp))
            i++;

        (void)closedir(dp);
    }
    return i;
}

int main(int argsc, char *argv[])
{

    int pid = -1;
    int file_count;
    int status;
    string file_directory, temp, genres_loc;
    vector<vector<string>> genres;
    if (pid != 0)
    {
        // file_directory = argv[1];
        file_directory = "library";
        temp = file_directory;
        genres_loc = temp.append("/genres.csv");
        ifstream inFile;
        inFile.open(genres_loc);
        stringstream string_stream;
        string_stream << inFile.rdbuf();
        string genres_str = string_stream.str();
        read_csv(&genres_str[0], genres);
        file_count = count_part_files_in_dir(file_directory) - 3;
        status = 0;
    }
    for (auto i = 1; i <= file_count; i++)
    {
        int pipe_util[2];
        int ret_value = pipe(pipe_util);
        if (ret_value == 0)
        {
            pid = fork();
            if (pid == 0)
            {
                execl("./mapper", to_string(pipe_util[READ]).c_str(), to_string(pipe_util[WRITE]).c_str(), (char *)0);
                exit(0);
            }
            else
            {
                string message = form_mapping_message(genres, i, file_directory);
                write(pipe_util[WRITE], message.c_str(), message.size());
                close(pipe_util[READ]);
                wait(&status);
            }
        }
    }
    for (auto i = 0; i < genres[0].size(); i++)
    {
        int pipe_util[2];
        int ret_value = pipe(pipe_util);
        if (ret_value == 0)
        {
            pid = fork();
            if (pid == 0)
            {
                execl("./reducer", to_string(pipe_util[READ]).c_str(), to_string(pipe_util[WRITE]).c_str(), (char *)0);
                exit(0);
            }
            else
            {
                string message = form_reducing_message(genres[0][i], file_count);
                write(pipe_util[WRITE], message.c_str(), message.size());
                close(pipe_util[READ]);
                wait(&status);
            }
        }
    }

    return 1;
}
