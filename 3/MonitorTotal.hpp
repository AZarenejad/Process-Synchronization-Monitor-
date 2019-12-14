
#include <semaphore.h>



class MonitorTotal {
    private:
        float total_sum;
        sem_t sem;

    public:
        MonitorTotal();
        float add(float emission);
};