#include<semaphore.h>
#include <condition_variable>
#include <iostream>
using namespace std;


class MonitorEdge {
    public:
        MonitorEdge();
        float calc_emission(int p , int h);
        void test();
    private:
        sem_t sem;
};