#include <iostream>
#include <io.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <istream>
#include <typeinfo>
#include "FromCSVtoData.h"

using namespace std;

vector<string> csv_read_row(istream& file, char delimiter);
vector<vector<unsigned char>> data_v;


int main()
{
    string path = "C:\\Users\\youngbin\\Desktop\\data\\*.*"; //로컬(임시) 경로
    string path_buf = "C:\\Users\\youngbin\\Desktop\\data\\";

    _finddata_t fd;
    intptr_t handle;

    if ((handle = _findfirst(path.c_str(), &fd)) == -1L)
        cout << "No file in the directory" << '\n';

    ifstream file(path_buf); //로컬(임시) 파일 경로


    do
    {
        cout << fd.name << endl;
        
        string file_path = { path_buf + (string)fd.name};
        ifstream file(file_path);

        
        if (file.fail())
        {
            cout << "file name: " << fd.name << " is No exists" << endl && 0;
        }
        
        while (file.good())
        {
            vector<string> row = csv_read_row(file, ',');
            uint8_t* d;
            
            if (!row[0].find("#"))
                continue;
            else
            {
                int len = row.size() - 2;
                for (int i = 0; i < len; i++)
                {
                    d = new uint8_t[row[i].size()];
                    for (int j = 0; j < row[i].size(); j++) {
                        d[j] = (uint8_t)row[i].at(j);
                    }
                    
                    //add(d, timestamp); //
                }
            }
            //cout << endl;
        }
        
    } while (_findnext(handle, &fd) == 0); //다음 파일로 이동

    file.close();
    _findclose(handle);

    return 0;

}

vector<string> csv_read_row(istream& file, char div)
{
    stringstream ss;
    bool flag = false;
    vector<string> row;
    
    while (file.good())
    {
        char c = file.get();

        if (!flag && c == '"')
            flag = true;

        else if (flag && c == '"')
            if (file.peek() == '"')
                ss << (char)file.get();

            else
                flag = false;

        else if (!flag && c == div)
        {
            row.push_back(ss.str());
            ss.str("");
        }

        else if (!flag && (c == '\r' || c == '\n'))
        {
            if (file.peek() == '\n') 
                file.get();

            row.push_back(ss.str());

            return row;
        }

        else
            ss << c;
    }
}

