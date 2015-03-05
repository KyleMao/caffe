// Split CIFAR-10 binary version into 10 binary files, each class in one file.
// Zexi Mao, Feb. 2015

#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

const int kFile = 5;
const int kClass = 10;
const int kSampleByte = 3073;
const streampos kBatchByte = 30730000;
const streampos kClassByte = 15365000;

int main(int argc, const char **argv)
{
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " data_dir out_dir" << endl;
        exit(-1);
    }

    string data_dir(argv[1]);
    string out_dir(argv[2]);

    vector<char*> memblocks;
    vector<int> block_pos(kClass, 0);
    for (int i = 0; i < kClass; ++i) {
        char *this_block = new char[kClassByte];
        memblocks.push_back(this_block);
    }

    for (int i = 1; i <= kFile; ++i) {
        char *this_block = new char[kBatchByte];
        char in_name[30];
        sprintf(in_name, "data_batch_%d.bin", i);
        string in_path = data_dir + in_name;
        cout << in_path << endl;
        ifstream bin_file(in_path.c_str(), ios::in|ios::binary);
        if (bin_file.is_open()) {
            bin_file.read(this_block, kBatchByte);
            bin_file.close();
            for (int j = 0; j < kBatchByte; j += kSampleByte) {
                int class_no = (int)((unsigned char)this_block[j]);
                for (int k = j; k < j + kSampleByte; ++k) {
                    memblocks[class_no][block_pos[class_no]] = this_block[k];
                    block_pos[class_no] = block_pos[class_no] + 1;
                }
            }
        } else {
            cerr << "Binary file open error!" << endl;
            exit(-1);
        }
    }

    for (int i = 0; i < kClass; ++i) {
        char out_name[30];
        sprintf(out_name, "class_batch_%d.bin", i);
        string out_path = out_dir + out_name;
        ofstream bout_file(out_path.c_str(), ios::out|ios::binary);
        if (bout_file.is_open()) {
            bout_file.write(memblocks[i], kClassByte);
            bout_file.close();
            cout << "Batch " << i << " finished!" << endl;
        } else {
            cerr << "Binary file creation error!" << endl;
            exit(-1);
        }
    }
    
    return 0;
}

