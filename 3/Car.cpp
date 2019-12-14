#include "Car.hpp"



Car::Car(string path , int id_path , int id_car, int p){
    this->path = path ;
    this -> id_path = id_path ;
    this ->id_car = id_car;
    this -> p = p ;
    this->position = path[0];
}

char Car::get_position(){
    return position;
}
string Car::get_path(){
    return path;
}

bool Car::is_arrive(){
    return ( position == this->path[(this->path).size()-1]);
}

string Car::next_move(){
    if(is_arrive()){
        return "-1";
    }
    for(int i=0;i<this->path.size();i++){
        if ( this->path[i] == position){
            return path.substr(i,2);
        }
    }
}

void Car::move(){
    if(is_arrive()){
        return;
    }
    for(int i=0;i<path.size();i++){
        if(position == path[i]){
            position = path[i+1];
            break;
        }
    }
}