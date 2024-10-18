#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(){
    ifstream inpFile("artigo.csv");
    ofstream outFile("artigomenor.csv");

    string line; 

    int i = 0;

    while(i < 1000){
        getline(inpFile, line);
        outFile << line << endl;
        i++;
    }
}