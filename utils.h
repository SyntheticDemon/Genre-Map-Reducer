#ifndef UTILS
#define UTILS
#define WRITE 1
#define CHAR_BUFFER_LENGTH 1000
#define READ 0
#define WRITE 1
void read_csv(const string &csvSource, vector<vector<string>> &lines);
void print_2d_vector(vector<vector<string>> &inp);
vector<string> read_and_decode_pipe_message(int fd);
#endif
