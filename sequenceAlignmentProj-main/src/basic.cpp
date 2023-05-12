#include <algorithm>
#include <iostream>
#include <map>
#include <stack>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <cctype>
using namespace std;

const int GapPenalty = 30;
string result1("");
string result2("");
int opt_value=0;
int result_judge=0;
long getTotalMemory();
int MissPenality(char, char);
string * ReadFileInput(const string& FileName);


//Refer to the attachment by CSCI570_Spring_FinalProject.pdf
long getTotalMemory() {
    struct rusage usage;
    int returnCode = getrusage(RUSAGE_SELF, &usage);
    if(returnCode == 0) {
        return usage.ru_maxrss;
    } else {
        //It should never occur. Check man getrusage for more info todebug.
        return -1;
    }
}

string* ConstructString(vector< vector<int> >& DP, const string& s1, const string& s2)
{
    int i = DP.size() - 1;
    int j = DP[0].size() - 1;
    string* result = new string[2];
    string ConstructString1 = s1;
    string ConstructString2 = s2;

    while (i > 0 || j > 0)
    {
        if (i >=1 && j>=1 && DP[i][j] == (DP[i-1][j - 1] + MissPenality(s1[i - 1], s2[j - 1])))
        {
            i--;
            j--;
        }
        else if (i >= 1 && DP[i][j] == (DP[i-1][j] + GapPenalty)) {
            ConstructString2.insert(j ,"_");
            i--;
        }
        else if (j >=1 && DP[i][j] == (DP[i][j-1] + GapPenalty)) {
            ConstructString1.insert(i , "_");
            j--;
        }
    }
    result1 += ConstructString1;
    result2 += ConstructString2;
    result[0] = ConstructString1;
    result[1] = ConstructString2;
    return result;
}
//hardcode the GapPenalty and MissPenality
int MissPenality(char x, char y)
{
    int minus = abs(x - y);
    int MPenality;
    switch (minus) {
        case 'C' - 'A':
            MPenality = 110;
            break;
        case 'G' - 'A':
            MPenality = 48;
            break;
        case 'T' - 'A':
            MPenality = 94;
            break;
        case 'G' - 'C':
            MPenality = 118;
            break;
        case 'T' - 'C':
            MPenality = 48;
            break;
        case 'T' - 'G':
            MPenality = 110;
            break;
        case 0:
            MPenality = 0;
            break;
        default: MPenality = 30;
            break;
    };
    return MPenality;
}

bool isNum(string str) {  //Determine if a character is a number
    for (int i = 0; i < str.size(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

string * ReadFileInput(const string& FileName) {

    string String1, String2;
    string* result = new string[2];
    int index;
    ifstream inputFile;

    inputFile.open(FileName);
    if (!inputFile) {
        cout << "Unable to open file";
        exit(1);
    }

    string input;
    if (!inputFile.eof()) {
        inputFile >> String1;
    }
    while (!inputFile.eof() && (inputFile >> input) && isNum(input)) {
        cout << input << endl;
        int index = stoi(input) + 1;
        String1 = String1.substr(0, index) + String1 + String1.substr(index);
    }
    String2 = string(input);
    while (!inputFile.eof() && (inputFile >> input) && isNum(input)) {
        int index = stoi(input) + 1;
        String2 = String2.substr(0, index) + String2 + String2.substr(index);
    }
    result[0] = String1;
    result[1] = String2;

    // Close file
    inputFile.close();
    return result;
}

//Dynamic programming basic solution method
vector< vector<int> > DP_Basic(const string& s1, const string& s2) {
    int x = s1.size() + 1;
    int y = s2.size() + 1;
    vector< vector<int> > DP(x, vector<int>(y));

    DP[0][0] = 0;
    for (int i = 1; i < x; i++) {
        DP[i][0] = i * GapPenalty;
    }
    for (int j = 1; j < y; j++) {
        DP[0][j] = j * GapPenalty;
    }

    for (int i = 1; i < x; i++) {
        for (int j = 1; j < y; j++) {
            DP[i][j] =
                min(DP[i - 1][j - 1] + MissPenality(s1[i - 1], s2[j - 1]),
                    min(DP[i - 1][j] + GapPenalty, DP[i][j - 1] + GapPenalty));
        }
    }
    opt_value=DP[x-1][y-1];
    return DP;
}

int main(int argc, char *argv[]) {
    struct timeval begin, end;
    gettimeofday(&begin, 0);

    result1 = "";
    result2 = "";

    string* originalInput = ReadFileInput(argv[1]);


    auto DP_Table = DP_Basic(originalInput[0], originalInput[1]);
    string* orignialOutput=ConstructString(DP_Table, originalInput[0], originalInput[1]);

    double problemSize=originalInput[0].length()+originalInput[1].length();
    double totalmemory = getTotalMemory();
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double totaltime = seconds*1000 + microseconds*1e-3;

    ofstream OutFile;
    OutFile.open(argv[2]);
    if (!OutFile) {
        cout << " could not be opened for writing";
        exit(1);
    }

    OutFile << opt_value<< endl;
    OutFile << "Problem Size:" << problemSize << endl;
    OutFile << result1 << endl;
    OutFile << result2 << endl;
    OutFile << "TimeCost:"<<totaltime << endl;
    OutFile << "Memorycost:"<<totalmemory<<endl;

    OutFile.close();
    return 0;
}