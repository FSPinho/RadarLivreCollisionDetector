#ifndef COMBINATIONEXECUTOR_H
#define COMBINATIONEXECUTOR_H

#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include <list>

#include <rlutil.h>

using namespace std;

template<class T>
class Combination {
    public:
        Combination(T a, T b) : a(a), b(b) {}

        T a;
        T b;
};

template<class T>
class CombinatorListener {
    public:
        virtual void onCombine(Combination<T> * combination) = 0;
};

template<class T>
class CombinationExecutor {

    private:
        int __id;
        bool __running;
        int __combinationCount;
        CombinatorListener<T> * __listener;
        list<Combination<T>* > __combinations;
        recursive_mutex __combinationsMutex;
        thread __cThread;

        void __lock() {
            __combinationsMutex.lock();
        }

        void __unlock() {
            __combinationsMutex.unlock();
        }

        Combination<T> * __popCombination() {
                Combination<T> * c = nullptr;
                //__lock();
                    if(__hasCombinations()) {
                        c = __combinations.front();
                        __combinations.pop_front();
                        __combinationCount++;
                    }
                //__unlock();

            return c;
        }

        bool __hasCombinations() {
            // __lock();
            // int size = __combinations.size();
            // __unlock();
            // return size > 0;
            return __combinations.size() > 0;
        }

        void __run() {

            while(__hasCombinations()) {

                Combination<T> * c = __popCombination();
                if(c != nullptr) {
                    __listener->onCombine(c);
                    delete c;
                }

            }

        }

    public:
        static int SERIAL;

        CombinationExecutor(CombinatorListener<T> * listener)
            : __id(SERIAL++), __combinationCount(0) {
            __listener = listener;
        }

        ~CombinationExecutor() {
            __combinations.clear();
        }

        void start(bool async=true) {
            __running = true;

            __cThread = thread(&CombinationExecutor<T>::__run, this);
            if(!async)
                __cThread.join();
        }

        void stop() {
            __running = false;
        }

        void join() {

            __cThread.join();

        }

        void addCombination(Combination<T> * c) {
            // __lock();
                __combinations.push_back(c);
            // __unlock();
        }

        int getBufferSize() {
            return __combinations.size();
        }

        string toString() {
            char buff[1000];
            sprintf(buff, "b=%05d", int(__combinations.size()));
            return string(buff);
        }

};

template<class T>
int CombinationExecutor<T>::SERIAL = 0;

#endif // COMBINATIONEXECUTOR_H
