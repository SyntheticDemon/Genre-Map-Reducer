using namespace std;
#include <vector>
#include <iostream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <vector>
#include <fstream>
#include "utils.h"

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

            cout << inp[i][j] << " ";
        }
        cout << endl;
    }
    return;
}

vector<string> read_and_decode_pipe_message(int fd)
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