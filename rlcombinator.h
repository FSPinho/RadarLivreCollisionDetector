#ifndef COMBINATOR_H
#define COMBINATOR_H

#include <iostream>
#include <sstream>
#include <vector>

#include <rlcombinationexecutor.h>
#include <rlaircraft.h>
#include <rlutil.h>

using namespace std;

template<class T>
class Combinator {

    private:
        list<T> __ns;
        unsigned int __n;
        vector<CombinationExecutor<T>*> __combinationExecutors;
        CombinatorListener<T> * __listener;
        thread __combinatorThread;
        bool __useThreads;

        void __createExecutors() {
            unsigned int processors = Util::getProcessorsCount() - 2;
            processors = processors >= 1? processors: 1;
            for(unsigned int i = 0; i < processors; i++) {
                CombinationExecutor<T> * executor = new CombinationExecutor<T>(__listener);
                __combinationExecutors.push_back(executor);
            }
        }

        void __startExecutors() {
            for(auto executor: __combinationExecutors) {
                executor->start();
            }
        }

        void __joinExecutors() {
            for(unsigned int i = 0; i < __combinationExecutors.size(); i++) {
                __combinationExecutors[i]->join();
            }
        }

        void __stopExecutors() {
            for(unsigned int i = 0; i < __combinationExecutors.size(); i++) {
                __combinationExecutors[i]->stop();
            }
        }

        void __destroyExecutors() {
            Util::clear(__combinationExecutors);
        }

        void __run() {
            // cout << endl << "Combinator is started!" << endl << endl;

            // Threading
            // cout << "Creating executors..." << endl;
            if(__useThreads) {
                __createExecutors();
            }
            // cout << "Creating executors: done!" << endl;

            // The algorithm need more tham n elements
            if(__ns.size() < __n) return;

            stepCount = 0;
            expectedCombinations = __ns.size() * (__ns.size() - 1) / Math::factorial(__n);

            vector<unsigned int> indexes;

            for(unsigned int i = 0; i < __n; i++)
                indexes.push_back(i);

            vector<T> ns{ std::begin(__ns), std::end(__ns) };

            cout << endl << "GENERATING COMBINATIONS..." << endl;
            while(true) {
                /* Execution of combination */

                vector<T> * combinated = new vector<T>();
                for(auto i: indexes)
                    combinated->push_back(ns[i]);

                if(__useThreads) {
                    __combinationExecutors[stepCount % __combinationExecutors.size()]
                            ->addCombination(combinated);
                } else {
                    __listener->onCombine(*combinated);
                }

                /* End execution of combination */

                stepCount += 1;
                if(indexes[0] == ns.size() - __n)
                    break;

                for(unsigned int i = __n - 1; i > 0; i--) {
                    if(indexes[i] == ns.size() - (__n - i)) {
                        if(indexes[i - 1] + 1 != indexes[i]) {
                            indexes[i - 1] += 1;
                            for(unsigned int j = i; j < __n; j++)
                                indexes[j] = indexes[j - 1] + 1;
                            break;
                        } else {
                            continue;
                        }
                    } else {
                        indexes[i] += 1;
                        break;
                    }
                }

            }

            cout << endl << "COMBINATIONS GENERATED!" << endl;

            // End threading
            if(__useThreads) {

                __startExecutors();

                __stopExecutors();

                __joinExecutors();

                __destroyExecutors();

            }

            if(stepCount != expectedCombinations) {
                cout << "Combinatiom Error - expected: " << expectedCombinations << ", found: " << stepCount << endl;
            }
        }

    public:

        long expectedCombinations;
        long stepCount;

        Combinator(list<T> ns, int n, CombinatorListener<T> * listener = nullptr, bool useThreads = true)
            : __ns(ns), __n(n), __listener(listener), __useThreads(useThreads) {}

        ~Combinator() {
            Util::clear(__ns);
            Util::clear(__combinationExecutors);
        }

        void start(bool async=true) {
            __combinatorThread = thread(&Combinator::__run, this);
            if(!async)
                __combinatorThread.join();
        }

        string toString() {
            int buffer = 0;
            for(auto i: __combinationExecutors)
                buffer += i->getBufferSize();
            stringstream ss;
            ss << "";
            ss << "Combinator(Threads: " << (__useThreads? Util::getProcessorsCount() - 2: 1) << ", ";
            // ss << "Buffer: " << buffer << ")";
            ss << ")";
            //for(auto c: __combinationExecutors)
            //    ss << c->toString() << ", ";
            ss << "";
            return ss.str();
        }


};

#endif // COMBINATOR_H
