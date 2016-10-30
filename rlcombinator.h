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
class PairCombinator {

    private:
        list<T> __ns;
        unsigned int __n;
        vector<AsyncPairCombinationExecutor<T>*> __combinationExecutors;
        PairCombinatorListener<T> * __listener;
        thread __combinatorThread;
        bool __useThreads;
        bool __skipCombinations;

        void __createExecutors() {
            unsigned int processors = Util::getProcessorsCount();
            processors = processors >= 1? processors: 1;
            for(unsigned int i = 0; i < processors; i++) {
                AsyncPairCombinationExecutor<T> * executor = new AsyncPairCombinationExecutor<T>(__listener);
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

            // Threading
            if(__useThreads) {
                __createExecutors();
            }

            // The algorithm need more tham n elements
            if(__ns.size() < __n) return;

            stepCount = 0;
            skippedCombinations = 0;
            expectedCombinations = __ns.size() * (__ns.size() - 1) / Math::factorial(__n);

            vector<unsigned int> indexes;

            for(unsigned int i = 0; i < __n; i++)
                indexes.push_back(i);

            vector<T> ns{ std::begin(__ns), std::end(__ns) };

            cout << "EXPECTED COMBINATIONS: " << expectedCombinations << endl;

            while(true) {
                /* Execution of combination */

                /*
                vector<T> * combinated = new vector<T>();
                for(auto i: indexes)
                    combinated->push_back(ns[i]);
                */

                if(__skipCombinations && !__listener->isCombination(ns[indexes[0]], ns[indexes[1]])) {
                    skippedCombinations++;
                } else {
                    PairCombination<T> * c = new PairCombination<T>(ns[indexes[0]], ns[indexes[1]]);

                    if(__useThreads) {
                        __combinationExecutors[stepCount % __combinationExecutors.size()]
                                ->addCombination(c);
                    } else {
                        __listener->onCombine(c);
                        delete c;
                    }

                }

                /* End execution of combination */

                /* Moving the indexes */
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
        long skippedCombinations;
        long stepCount;

        PairCombinator(list<T> ns, PairCombinatorListener<T> * listener = nullptr, bool useThreads = true, bool skipCombinations = true)
            : __ns(ns), __n(2), __listener(listener), __useThreads(useThreads), __skipCombinations(skipCombinations) {}

        ~PairCombinator() {
            Util::clear(__ns);
            Util::clear(__combinationExecutors);
        }

        void start(bool async=true) {
            __combinatorThread = thread(&PairCombinator::__run, this);
            if(!async)
                __combinatorThread.join();
        }

        string toString() {
            int buffer = 0;
            for(auto i: __combinationExecutors)
                buffer += i->getBufferSize();
            stringstream ss;
            ss << "";
            ss << "(ths: " << (__useThreads? Util::getProcessorsCount(): 1) << ", ";
            ss << "bff: " << buffer << ")";
            // ss << ")";
            //for(auto c: __combinationExecutors)
            //    ss << c->toString() << ", ";
            ss << "";
            return ss.str();
        }


};

#endif // COMBINATOR_H
