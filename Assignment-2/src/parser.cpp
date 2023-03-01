#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;
int main(){
    // parse a file
    fstream file;
    file.open("cube.obj");
    if(!file.is_open()){
        cout<<"File not found"<<endl;
        return 0;
    }
    string line;
    while(getline(file,line)){
        stringstream ss(line);
        string word;
        ss>>word;
        if(word=="v"){
            float x,y,z;
            ss>>x>>y>>z;
            cout<<"Vertex: "<<x<<" "<<y<<" "<<z<<endl;
        }
        else if(word=="f"){
            int a,b,c;
            ss>>a>>b>>c;
            cout<<"Face: "<<a<<" "<<b<<" "<<c<<endl;
        }
    }

}

