#include <stdio.h> 
#include <stdlib.h>     
#include <time.h>
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <map>
#include <tuple>
#include <sstream>
#include <vector>
#include <iterator>
#include <random>
#include <semaphore.h>
#include "Car.hpp"
#include "MonitorEdge.hpp"
#include "MonitorTotal.hpp"

using namespace std;
map<string,int> edges;    // store edge and h : for example AB:4
vector<tuple<string,int,int>> pathes;  //store (path , num_car , id_path)
map<string,MonitorEdge> edge_monitors;  // edge : monitor
MonitorTotal total_monitor;


void reading_file(string file_name){
    ifstream myfile(file_name);
    bool path_seen = false;
    int i = 1;
    if (myfile.is_open()){
        while(!myfile.eof()){
            string line;
            getline(myfile,line);
            if (path_seen){
                string path;
                string token;
                istringstream ss(line);
                while(getline(ss,token,'-')){
                    path+=token;
                }
                getline(myfile,line);
                istringstream s1(line);
                int num_car;
                s1 >> num_car;
                tuple<string,int,int> new_path = make_tuple(path ,num_car, i);
                pathes.push_back(new_path);
                i+=1;
            }
            else {
                if (line == "#"){
                    path_seen = true;
                    continue;
                }
                else{
                    istringstream ss(line);
                    string edge;
                    string node;
                    getline(ss,node,'-');
                    edge += node;
                    getline(ss,node,'-');
                    edge += node;
                    int h;
                    ss >> h;
                    edges[edge] = h;
                }
            }
        }
    }
}
int find_h_of_edge(string edge){
    map<string,int>::iterator it;
    it = edges.find(edge);
    if (it!=edges.end()){
        return it->second;
    }
    else{
        return -1;
    }
}
int num_car_in_path(string path){
    for(int i=0;i<pathes.size();i++){
        if (get<0>(pathes[i]) == path){
            return get<1>(pathes[i]);
        }
    }
    return -1;
}
int id_path(string path){
    for(int i=0;i<pathes.size();i++){
        if (get<0>(pathes[i]) == path){
            return get<2>(pathes[i]);
        }
    }
    return -1;
}
int generate_random_p(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 999);
    int p =  dis(gen) % 10 + 1;
    return p;
}
void make_file(string file_name){
    fstream file;
    file.open(file_name , ios::out);
    file.close();
}
string change_num_to_string(int num){
    // declaring output string stream 
    ostringstream str1; 
    // Sending a number as a stream into output 
    // string 
    str1 << num; 
    // the str() coverts number into string 
    return str1.str(); 
}
void write_info_of_one_edge_in_car_file(string file_name,char entrance_node_name,time_t entrance_time,
                                        char exit_node_name , time_t exit_time ,
                                        float emmission , float total_emmissin){
    fstream filestr;
    filestr.open (file_name, fstream::in | fstream::out | fstream::app);
    if(filestr.is_open()){
        filestr << entrance_node_name <<","<<entrance_time<<","<<exit_node_name<<","<<exit_time<<","<<emmission<<","<<total_emmissin<<endl;
    }
}

void making_monitors_for_edges(){
    map<string, int>::iterator it;
    for (it = edges.begin(); it != edges.end(); it++){
            // cout<<it->first<<endl;
            edge_monitors[it->first] = MonitorEdge();
       }
    total_monitor = MonitorTotal();
}

void move_car(string path , int id_path , int id_car , int p){
    string file_name = change_num_to_string(id_path) + "-" + change_num_to_string(id_car);
    Car c(path, id_path, id_car,p);
    make_file(file_name);
    string result;
    while(!c.is_arrive()){
        cout<<"car wants to move:  id_car:"<< id_car <<"  path:" <<c.get_path() << "   position:" <<c.get_position()<<endl;
        string next_move = c.next_move();
        char enter_node = next_move[0];
        time_t enter_time = time(NULL);
        int h_edge =  find_h_of_edge(next_move);
        if(h_edge==-1){
            cout<<"BUGG BUGG BUGG"<<endl;
        }
        float path_emmission = edge_monitors[next_move].calc_emission(p,h_edge);
        float total_emmission = total_monitor.add(path_emmission);
        char exit_node = next_move[1];
        time_t exit_time = time(NULL);

        // result+= enter_node+",";
        // result+=change_num_to_string(enter_time)+",";
        // result+=exit_node + ",";
        // result+=change_num_to_string(exit_time) +",";
        // result+=change_num_to_string(path_emmission)+",";
        // result+=change_num_to_string(total_emmission)+"\n";
        write_info_of_one_edge_in_car_file(file_name, enter_node, enter_time, 
                                                    exit_node, exit_time, path_emmission, total_emmission);
        cout<<"car moved:  id_car:"<< id_car <<"  path:" <<c.get_path() << "   from position:" <<c.get_position()<<" ";
        c.move();
        cout<<"to position: " <<c.get_position()<<endl;
    }
    cout <<"car:"<<id_car << "  in path:"<<c.get_path() << "  arrived****************" <<endl;
    // ofstream f(file_name);
    // f<<result;
}



int main(int argc, char const *argv[]){
    reading_file(argv[1]);
    making_monitors_for_edges();
    vector<thread> cars;
    int id_car = 1;
    for (int i=0; i<pathes.size() ;i++){
        int num_cars_in_path = get<1>(pathes[i]);
        for(int j=0; j<num_cars_in_path ;j++){
            cars.push_back( thread (move_car , get<0>(pathes[i]),get<2>(pathes[i]), id_car , generate_random_p()));
            id_car+=1;
        }
    }
    for (int i = 0;i<cars.size() ;i++){
        cars[i].join();
    }
    return 0;
}


