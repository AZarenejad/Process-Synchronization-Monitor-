#include "MonitorEdge.hpp"


MonitorEdge::MonitorEdge(){
    sem_init(&this->sem,0,1);
}

float MonitorEdge::calc_emission(int p,int h){
    sem_wait(&this->sem);
    float result = 0.0;
    for(float k = 0; k<1000000; k++){
        result += k / (1000000 * p * h ) ;
    }
    sem_post(&this->sem);
    return result;
}
