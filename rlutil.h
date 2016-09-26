#ifndef RLUTIL_H
#define RLUTIL_H

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
        return sysconf(_SC_NPROCESSORS_ONLN);
    }

};

#endif // RLUTIL_H
