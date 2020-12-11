#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iterator>
#include <numeric>
#include <algorithm>

using namespace std; 

vector<double> calculations(vector<int> cylinder_moves) {
    vector<double> calculations;
    double total, avg, sq_sum, stdev;
    total = accumulate(cylinder_moves.begin(), cylinder_moves.end(), 0);
    avg = total / cylinder_moves.size();
    sq_sum = inner_product(cylinder_moves.begin(), cylinder_moves.end(), cylinder_moves.begin(), 0);
    stdev = sqrt(sq_sum / cylinder_moves.size() - avg * avg);

    calculations.push_back(total);
    calculations.push_back(avg);
    calculations.push_back(stdev);

    return calculations;
}

vector<int> fcfs_disk(vector<int> requests, int head_start) {
    int seek_time = 0;
    vector<int> cylinder_moves;
    for(vector<int>::iterator it = requests.begin(); it != requests.end(); ++it) {
        seek_time = abs(head_start - *it);
        cylinder_moves.push_back(seek_time);
        head_start = *it;
    }

    return cylinder_moves;
}

vector<int> sstf_disk(vector<int> requests, int head_start) {
    int seek_time = 0;
    int min = abs(head_start - requests[0]);
    int next_cyl;
    vector<int> cylinder_moves;
    while(requests.size() > 0) {
        for(vector<int>::iterator it = requests.begin(); it != requests.end(); ++it) {
            //cout << *it << " Diff: " << abs(head_start-*it) << " Min: " << min << endl;
            
            if(abs(head_start-*it) <= min) {
                min = abs(head_start - *it);
                next_cyl = *it;
            }
        }

        cylinder_moves.push_back(min);
        requests.erase(remove(requests.begin(), requests.end(), next_cyl), requests.end());
        head_start = next_cyl;
        min = abs(head_start - requests[0]);
    }

    return cylinder_moves;
}

vector<int> scan_disk(vector<int> requests, int head_start, int num_cylinders) {
    int add_seek = 0;
    vector<int> cylinder_moves;
    sort(requests.begin(), requests.end());
    int min = *requests.begin();
    int max = *(requests.end()-1);

    // Getting index of head starting position
    int index;
    for(vector<int>::iterator it = requests.begin(); it != requests.end(); ++it) {
        if(*it < head_start && head_start < *(it+1)) {
            index = it - requests.begin();
        }
    }

    // Left traversal
    for(vector<int>::iterator it = requests.begin()+index; it != requests.begin()-1; --it) {
        if(head_start == min) {
            cylinder_moves.push_back(abs(head_start-*it)+(2*min));
            head_start = *it;
        }
        else {
            cylinder_moves.push_back(abs(head_start-*it));
            head_start = *it;  
        }
    }

    // Add an offset to the head start to account for min to 0 traversal
    head_start -= 2*min;

    // Right traversal
    for(vector<int>::iterator it = requests.begin()+(index+1); it != requests.end(); ++it) {
        if(head_start == max) {
            cylinder_moves.push_back(abs(head_start-*it+(abs(max-num_cylinders))));
        }
        else {
            cylinder_moves.push_back(abs(head_start-*it));
            head_start = *it;  
        }
    }

    return cylinder_moves;
}

int main() {
    ifstream infile("requests.txt");
    string a;
    vector<int> requests;
    int head_start;

    while(getline(infile,a,',')) {
        requests.push_back(stoi(a));
    }

    cout << "\nEnter the head starting position: ";
    cin >> head_start;

    cout << "\nalgorithm\t\tnumber of cylinders moved\ttotal\tavg\tstdev\n";
    cout << string(80, '-') << "\n";

    vector<int> cylinder_moves;
    vector<double> calcs; 

    // FCFS Disk Algorithm
    cylinder_moves = fcfs_disk(requests, head_start);
    calcs = calculations(cylinder_moves);

    cout << "fcfs\t\t\t";
    for(vector<int>::iterator it = cylinder_moves.begin(); it != cylinder_moves.end(); ++it) {
        cout << *it << " ";
    }
    cout << "\t" << int(calcs[0]) << "\t" << calcs[1] << "\t" << calcs[2] << endl;

    // SSTF Disk Algorithm
    cylinder_moves = sstf_disk(requests, head_start);
    calcs = calculations(cylinder_moves);

    cout << "sstf\t\t\t";
    for(vector<int>::iterator it = cylinder_moves.begin(); it != cylinder_moves.end(); ++it) {
        cout << *it << " ";
    }
    cout << "\t\t" << int(calcs[0]) << "\t" << calcs[1] << "\t" << calcs[2] << endl;

    // SCAN Disk Algorithm (with 200 cylinders)
    cylinder_moves = scan_disk(requests, head_start, 200);
    calcs = calculations(cylinder_moves);

    cout << "scan\t\t\t";
    for(vector<int>::iterator it = cylinder_moves.begin(); it != cylinder_moves.end(); ++it) {
        cout << *it << " ";
    }
    cout << "\t\t" << int(calcs[0]) << "\t" << calcs[1] << "\t" << calcs[2] << endl << endl;

    return 0;
}
