#include <iostream>
#include <iomanip>
#include <fstream>//For input files
#include <vector>
#include <string>
#include <algorithm>

const int NUM_IDS = 10000;
const int DEPTH = 3;
const int WIDTH = 3000;

using namespace std;

//References are used on numerous parameters in functions to ensure there are no extra copies made that ultimately slow down the process of countMinSketch

struct flow{
    string id;
    int trueSize; // Actual packet count
    int estSize; // Estimated count

    flow(): id(""), trueSize(0), estSize(0){}//Default constructor
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
    //Creates a struct of hash with no name, that is of the string type and takes the argument of flowID_IndexNumber as what it hashes. This becomes the seed generated for hashing the index
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

//Helper function used to sort my vector by the estimated size. Making the highest counted flows go to the top of the vector
bool compareByEst(const flow& a, const flow& b) {
    return a.estSize > b.estSize;
}

int main(){
    fstream input;
    input.open("project2input.txt");
    int n;
    input >> n;

    //Read file
    vector<flow> flows;
    for(int i = 0; i < n; i++){
        flow f;
        input >> f.id >> f.trueSize;
        flows.push_back(f);
    }
    input.close();

    //Initialize sketch
    countMin sketch(DEPTH, WIDTH);

    //Update
    for(int i = 0; i < n; i++){
        update(sketch, flows[i].id, flows[i].trueSize);
    }

    //Query
    for(int i = 0; i < n; i++){
        flows[i].estSize = query(sketch, flows[i].id);
    }

    //Avg error
    double avgError = computeAvg(flows);

    //Makes the flows vector become descending order due to the helper function compareByEst
    sort(flows.begin(), flows.end(), compareByEst);

    //Output file
    ofstream output;
    output.open("countMinOutput.txt");
    output << " avgError: " << avgError << endl;
    for(int i = 0; i <= 99; i++){
        output << setw(2) << i << " - " << "FlowID: " << setw(15) << flows[i].id << "  estSize: " << setw(5) << flows[i].estSize << "  trueSize: " << setw(5) << flows[i].trueSize << endl;
    }

    output.close();
    return 0;
}