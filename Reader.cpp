#include <iostream>
#include <fstream>
#include "./Headers/Reader.h"


//TODO: Find another way, I don't fell ok doing this on C++
std::string read_file(){
    std::string content;
    std::string full_file;
    std::ifstream stream("./script.play");
    while( getline(stream, content)){
        full_file += content;
        full_file.push_back('\n');
    }
    stream.close();
    return full_file;
}