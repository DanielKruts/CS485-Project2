#include <iostream>
#include <fstream>//For input files
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <functional>

const int NUM_IDS = 10000;
const int DEPTH = 3;
const int WIDTH = 3000;

using namespace std;

//References are used on numerous parameters in functions to ensure there are no extra copies made that ultimately slow down the process of countMinSketch

struct flow{
    string id;
    int trueSize = 0; // Actual packet count
    int estSize = 0; // Estimated count

    flow(string id, int trueSize, int estSize): id(id), trueSize(trueSize), estSize(estSize){}
};
struct countMin{
    int d; // Depth
    int w; // Width
    vector<vector<int>> table; //2d table, table[d][w]

    countMin(int d, int w): d(d), w(w), table(d, vector<int>(w, 0)){}
};

//Takes the input of flowID, arrayIndex(specifically the column index 0-2 for 3 to create 3 different seeds), and the integer w, which is the width of the table, or 3000
//The concatenated string is then modded with the width and returned. This is used as a helper function to create all 3 possible hashes for every given flowID
int hashFlow(const string& flowID, int arrayIndex, int w){
    size_t seed = hash<string>{}(flowID + "_" + to_string(arrayIndex));
    return (int)(seed % w);
}

//Updates the sketch by looping through every row and hashing the flowID to incremement every relevant counter
//Every counter updated all at once with one call, as per cms(count min sketch)
void update(countMin& cms, const string& flowID, int packetCount){
    for(int i = 0; i < cms.d; i++){
        int col = hashFlow(flowID, i, cms.w);
        cms.table[i][col] += packetCount;
    }
}

//Finds the minimum value from a given flowID, which is found by comparing the counter of each value in the table
int query(const countMin& cms, const string& flowID){
    int minVal = INT_MAX;//INT_MAX used to prevent any possiblity of not finding a smaller value
    for(int i = 0; i < cms.d; i++){
        int col = hashFlow(flowID, i, cms.w);
        minVal = min(minVal, cms.table[i][col]);
    }
    return minVal;
}

//Computes the average error between the estimated count and the true count
//Estimated size - True size because the estimate should always be greater than or equal to the True size
double computeAvg(const vector<flow>& flows){
    double totalError = 0;
    for(const flow& f : flows){
        totalError += abs(f.estSize - f.trueSize);
    }
    return totalError / flows.size();
}

int main(){
    fstream input;
    input.open("project2input.txt");




    input.close();
    return 0;
}