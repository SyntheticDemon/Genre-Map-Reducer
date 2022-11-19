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
#define CHAR_BUFFER_LENGTH 1000
void read_csv(const string &csvSource, vector<vector<string>> &lines)
{
    bool inQuote(false);
    bool newLine(false);
    string field;
    lines.clear();
    vector<string> line;

    string::const_iterator aChar = csvSource.begin();
    while (aChar != csvSource.end())
    {
        switch (*aChar)
        {
        case '"':
            newLine = false;
            inQuote = !inQuote;
            break;

        case ',':
            newLine = false;
            if (inQuote == true)
            {
                field += *aChar;
            }
            else
            {
                line.push_back(field);
                field.clear();
            }
            break;

        case '\n':
        case '\r':
            if (inQuote == true)
            {
                field += *aChar;
            }
            else
            {
                if (newLine == false)
                {
                    line.push_back(field);
                    lines.push_back(line);
                    field.clear();
                    line.clear();
                    newLine = true;
                }
            }
            break;

        default:
            newLine = false;
            field.push_back(*aChar);
            break;
        }

        aChar++;
    }

    if (field.size())
        line.push_back(field);

    if (line.size())
        lines.push_back(line);
}
void print_2d_vector(vector<vector<string>> &inp)
{
    for (int i = 0; i < inp.size(); i++)
    {
        for (int j = 0; j < inp[i].size(); j++)
        {

            cout << inp[i][j];
        }
        cout << endl;
    }
    return;
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
string vec_to_csv(vector<vector<string>> v){
    string result_string;
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[0].size() ; j++)
        {
            result_string.append(v[i][j]);
        }
        result_string.append("\n");
    }
    return result_string; 
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
        file_directory = argv[1];
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
        int unnanmed_pipe_fds[2];
        int ret_value = pipe(unnanmed_pipe_fds);
        if (ret_value == 0)
        {
            pid = fork();
            if (pid == 0)
            {
                sleep(1);

                execl("./mapper", to_string(unnanmed_pipe_fds[0]).c_str(),
                      to_string(unnanmed_pipe_fds[1]).c_str(), (char *)0);

                // string temp = "parts";
                // temp.append(to_string(i));
                // cout << temp;
                exit(0);
            }
            else
            {
                close(unnanmed_pipe_fds[0]);
                write(unnanmed_pipe_fds[1],vec_to_csv(genres).c_str(),CHAR_BUFFER_LENGTH);
                cout << "Waiting"<<endl;
                wait(&status);
            }
        }
        else
        {
        }
    }
    for (auto i = 1; i <= genres[0].size(); i++)
    {

        int unnamed_pipe[2];
        int ret_value = pipe(unnamed_pipe);
        if (ret_value == 0)
        {
            pid = fork();
            if (pid == 0)
            {
                sleep(2);
                execl("./reducer", to_string(unnamed_pipe[0]).c_str(), to_string(unnamed_pipe[1]).c_str(), (char *)0);
                // string temp = "parts";
                // temp.append(to_string(i));
                // cout << temp;
                exit(0);
            }
            else
            {
                close(unnamed_pipe[0]);
                write(unnamed_pipe[1], vec_to_csv(genres).c_str(), CHAR_BUFFER_LENGTH);
                cout <<"Waiting \n";
                wait(&status);
            }
        }
        else
        {
            cout << "Big Failure";
        }
    }
    return 1;
}
