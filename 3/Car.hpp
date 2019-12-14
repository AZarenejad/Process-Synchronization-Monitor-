#include <iostream>
#include <string>

using namespace std;




class Car {
    public:
        Car(string path,int id_path, int id_car, int p);
        bool is_arrive();
        string next_move();
        void move();
        char get_position();
        string get_path();
    private:
        string path;
        int id_path;
        int id_car;
        int p ;
        char position;
};
