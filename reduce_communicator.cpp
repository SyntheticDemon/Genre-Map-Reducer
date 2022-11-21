
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
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    int read_fd = atoi(argv[0]);
    int write_fd = atoi(argv[1]);
    int file_count = atoi(argv[2]);
    char buffer[CHAR_BUFFER_LENGTH];
    string file_path(argv[3]);
    // cout << "Reducer Communicator received file descritors "
    //  << read_fd << " " << write_fd << " " << file_path << " " << file_count << " " << endl;
    file_path.append("processed");
    mkfifo(file_path.c_str(), 0666);
    int received_messages_count = 0;
    vector<int> received_counts;
    int count_sum = 0;
    fd = open(file_path.c_str(), O_RDONLY);

    char arr1[80];
    received_messages_count++;

    read(fd, arr1, sizeof(arr1));
    cout << "Result : " << arr1 << endl;

    // for (int i = 0; i < received_counts.size(); i++)
    // {
        // count_sum = count_sum + received_counts[i];
    // }

    cout << "Genre : " << file_path << " Count of books with this genre in the directory " << count_sum << endl;
    return 0;
}
