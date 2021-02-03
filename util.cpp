#include "util.hpp"
#include "crossplatform.hpp"

#include <time.h>

#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

vector<string> SplitString(string str, char delim){

    vector<string> ret;
    
    auto cstr = str.c_str();
    size_t l = 0, r = 0;

    for(; ; ++r){
        char c = cstr[r];
        if (c == 0){
            if (l == r) { break; }
            ret.push_back(str.substr(l, r-1));
        } else if (c == delim){
            if (l == r){    
            }
        }
    }

    return ret;
}

void WriteToLog(string line){
    time_t tval = time(NULL);
    auto lt = localtime(&tval);
    ostringstream nline;
    ostringstream fileName;
    nline << lt->tm_hour<<":"<<lt->tm_min<<":"<<lt->tm_sec<<"\t\t"<<line;
    fileName << lt->tm_mon << "_" << lt->tm_mday << ".log";
    string file = getExecDir() + "logs/" + fileName.str();
    ofstream out(file, ios::out | fstream::app);
    out << line << endl;
}


