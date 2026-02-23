#include <iostream>
#include <fstream>
#include <vector>
#include <string>

const int NUM_IDS = 10000;

using namespace std;

struct flow{
    string id;
    int trueSize; // Actual packet count
    int estSize; // Estimated count
    double error; // |estSize - trueSize|
};

struct count{
    int d; // Depth
    int w; // Width
    vector<int> dw;
};

int main(){
    //Input file
    fstream inputTxt;
    inputTxt.open("project2input.txt");

    





    inputTxt.close();
    return 0;
}