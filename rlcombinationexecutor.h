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
class PairCombination {
    public:
        PairCombination(T a, T b) : a(a), b(b) {}

        T a;
        T b;
};


template<class T>
class PairCombinatorListener {
    public:
        virtual bool isCombination(T a, T b) = 0;
        virtual void onCombine(PairCombination<T> * combination) = 0;
};

template<class T>
class AsyncPairCombinationExecutor {

    private:
        int __id;
        bool __running;
        int __combinationCount;
        PairCombinatorListener<T> * __listener;
        list<PairCombination<T>* > __combinations;
        recursive_mutex __combinationsMutex;
        thread __cThread;

        void __lock() {
            __combinationsMutex.lock();
        }

        void __unlock() {
            __combinationsMutex.unlock();
        }

        PairCombination<T> * __popCombination() {
                PairCombination<T> * c = nullptr;
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

                PairCombination<T> * c = __popCombination();
                if(c != nullptr) {
                    __listener->onCombine(c);
                    delete c;
                }

            }

        }

    public:
        static int SERIAL;

        AsyncPairCombinationExecutor(PairCombinatorListener<T> * listener)
            : __id(SERIAL++), __combinationCount(0) {
            __listener = listener;
        }

        ~AsyncPairCombinationExecutor() {
            __combinations.clear();
        }

        void start(bool async=true) {
            __running = true;

            __cThread = thread(&AsyncPairCombinationExecutor<T>::__run, this);
            if(!async)
                __cThread.join();
        }

        void stop() {
            __running = false;
        }

        void join() {

            __cThread.join();

        }

        void addCombination(PairCombination<T> * c) {
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
int AsyncPairCombinationExecutor<T>::SERIAL = 0;

#endif // COMBINATIONEXECUTOR_H
