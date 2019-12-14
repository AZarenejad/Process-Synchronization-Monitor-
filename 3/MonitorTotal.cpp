#include "MonitorTotal.hpp"





MonitorTotal::MonitorTotal(){
    sem_init(&this->sem , 0 , 1);
    this->total_sum = 0;
}


float MonitorTotal::add(float emission){
    sem_wait(&sem);
    total_sum+=emission;
    sem_post(&sem);
    return total_sum;
}