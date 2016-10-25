#ifndef RLUTIL_H
#define RLUTIL_H

#include <sys/types.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <chrono>
#include <unistd.h>

using namespace std;

class Util {
public:

    template<class T>
    static string toString(vector<T> ts) {
        stringstream ss;
        ss << "[";
        bool flag = false;
        for(auto t :ts) {
            ss << (flag? ", ": "") << t;
            flag = true;
        }
        ss << "]";
        return ss.str();
    }

    static long getSystemTimestamp(long offset = 0) {
        using namespace std::chrono;
        milliseconds ms = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
        );
        return ms.count() + offset;
    }

    template<class T>
    static void clear(vector<T>& v) {
        for(unsigned int i = 0; i < v.size(); i++) {
            try{
                delete v[i];
            } catch (...) {}
        }
        v.clear();
        v.shrink_to_fit();
    }

    template<class T>
    static void clear(list<T>& l) {
        while(l.size() > 0) {
            try{
                delete l.front();
                l.pop_front();
            } catch (...) {}
        }
        l.clear();
    }

    static unsigned int getProcessorsCount() {
        return sysconf(_SC_NPROCESSORS_ONLN) - 2;
    }

    static string humanizeDataAmount(long long amount) {
        long long byte = 1;
        long long kByte = byte * 1024;
        long long mByte = kByte * 1024;
        long long gByte = mByte * 1024;
        long long tByte = gByte * 1024;

        stringstream ss;
        if(amount > tByte)
            ss << (long long)(amount/tByte) << "." << (long long)((amount % tByte) / gByte) << " GB";
        else if(amount > gByte)
            ss << (long long)(amount/gByte) << "." << (long long)((amount % gByte) / mByte) << " GB";
        else if(amount > mByte)
            ss << (long long)(amount/mByte) << "." << (long long)((amount % mByte) / kByte) << " MB";
        else if(amount > kByte)
            ss << (long long)(amount/kByte) << "." << (long long)((amount % kByte) / byte) << " KB";
        else
            ss << (long long)(amount/byte) << "." << (long long)((amount % byte) / byte) << " bytes";

        return ss.str();
    }



    static void showMemoryUsage() {
        cout << "MEMORY USAGE: " << humanizeDataAmount(__getMemoryUsageInBytes()) << endl;
    }

private:

    static long long int __parseLine(char * line) {
        // This assumes that a digit will be found and the line ends in " Kb".
        int i = strlen(line);
        const char* p = line;
        while (*p <'0' || *p > '9') p++;
        line[i-3] = '\0';
        i = atoll(p);
        return i;
    }

    static long long int __getMemoryUsageInBytes() {
        FILE* file = fopen("/proc/self/status", "r");
        long long int result = -1;
        char line[128];

        while (fgets(line, 128, file) != NULL){
            if (strncmp(line, "VmRSS:", 6) == 0){
                result = __parseLine(line);
                break;
            }
        }
        fclose(file);

        return result * 1024;
    }

};

#endif // RLUTIL_H
