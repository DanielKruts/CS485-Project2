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

struct flow{
    string id;
    int trueSize; // Actual packet count
    int estSize; // Estimated count

    flow(): id(""), trueSize(0), estSize(0){}//Default constructor
};
struct countSketch{
    int d; // Depth
    int w; // Width
    vector<vector<int>> table; //2d table, table[d][w]

    countSketch(int d, int w): d(d), w(w), table(d, vector<int>(w, 0)){}
};

//Takes the input of flowID, arrayIndex(specifically the column index 0-2 for 3 to create 3 different seeds), and the integer w, which is the width of the table, or 3000
//The concatenated string is then modded with the width and returned. This is used as a helper function to create all 3 possible hashes for every given flowID
int hashFlow(const string& flowID, int arrayIndex, int w){
    //Creates a struct of hash with no name, that is of the string type and takes the argument of flowID_IndexNumber as what it hashes. This becomes the seed generated for hashing the index
    size_t seed = hash<string>{}(flowID + "_" + to_string(arrayIndex));
    return (int)(seed % w);
}

//Takes the input of flowID as a reference and the arrayIndex at wherever this flowID is at, then creates a seed similar to hashFlow function
//Returned value is always 1 (if even) or -1 (if odd)
//The function returns this to account for adding or subtracting different counts based off of this quality
int sign(const string& flowID, int arrayIndex){
    //Same format as seed in hashFlow function
    size_t seed = hash<string>{}(flowID + "_" + to_string(arrayIndex + 3));
    return (seed % 2 == 0) ? 1 : -1;//If even, return 1, else return -1
}

//Updates very similarly to count min sketch, however the difference is the use of the sign function, which returns a 1 or -1 depending on odd or even values hashed
//This makes it to where the estimations are a little more accurate by subtracting some counts and adding others
//Results in count sketch having underestimates rather than overestimates in counting
void update(countSketch& cs, const string& flowID, int packetCount){
    for(int i = 0; i < cs.d; i++){
        int col = hashFlow(flowID, i, cs.w);
        int s = sign(flowID, i);
        cs.table[i][col] += s * packetCount;
    }
}

//Finds the median value of a given flowID and the sketch. Different from count min sketch simply by taking the median value instead of the min as the considered value
int query(const countSketch& cs, const string& flowID){
    vector<int> estimates(cs.d, 0);
    for(int i = 0; i < cs.d; i++){
        int col = hashFlow(flowID, i, cs.w);
        int s = sign(flowID, i);
        estimates[i] = s * cs.table[i][col];
    }
    sort(estimates.begin(), estimates.end());
    return estimates[1];//Returns the median value after sorting, d is always 3, so estimates will always have 3 values
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
    return abs(a.estSize) > abs(b.estSize);
}

int main(){
    //Input file
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
    
    countSketch sketch(DEPTH, WIDTH);

    //Update
    for(int i = 0; i < n; i++){
        update(sketch, flows[i].id, flows[i].trueSize);
    }

    //Query
    for(int i = 0; i < n; i++){
        flows[i].estSize = query(sketch, flows[i].id);
    }

    //Makes the flows vector become descending order due to the helper function compareByEst
    double avgError = computeAvg(flows);

    //Sort
    sort(flows.begin(), flows.end(), compareByEst);

    //Output File
    ofstream output;
    output.open("countSketchOutput.txt");
    output << " avgError: " << avgError << endl;
    for(int i = 0; i <= 99; i++){
        output << setw(2) << i << " - " << "FlowID: " << setw(15) << flows[i].id << "  estSize: " << setw(5) << flows[i].estSize << "  trueSize: " << setw(5) << flows[i].trueSize << endl;
    }
    output.close();

    return 0;
}