#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include <math.h>
#include <bitset>
#include <map>
#include <deque>
#include <unordered_map>
#include <stack>
#include <unistd.h>
#include <queue>
#include <time.h>

#include <fstream>
#include <stdio.h>
#include <string.h>
#include <assert.h>


using namespace std;

using ll = long long;
using ld = long double;

ll  dynamic_solution(vector<ll> v){
    ll sum = 0;
    ll limit = 5000000;
    for (int i = 0; i < v.size(); ++i) {
        v[i] = abs(v[i]);
        sum += v[i];
    }
    if(sum > limit) {
        cout << "sum of inputs is too large for the problem";
        return -1;
    }
    if(v.size() == 0){
        return 0;
    }
    if(v.size() == 1){
        return v[0];
    }
    vector<bool> difference(sum + 1, false);
    difference[v[0]] = true;
    for(int i = 1; i < v.size(); i++) {
        vector<bool> updated_difference(sum + 1, false);
        for(int j = 0; j < difference.size(); j++) {
            if(difference[j]) {
                updated_difference[j + v[i]] = true;
                updated_difference[abs(j - v[i])] = true;
            }
        }
        for (int j = 0; j < difference.size(); ++j) {
            difference[j] = updated_difference[j];
        }
    }
    int index = 0;
    while(!difference[index]){
        index++;
    }
    return index;
}

ll  kk_algorithm(vector<ll> v){
    if(v.size() == 0) {
        return 0;
    }

    map<ll, ll> differences;
    for (int i = 0; i < v.size(); ++i) {
        v[i] = abs(v[i]);
    }

    for (int i = 0; i < v.size(); ++i) {
        if(v[i] > 0) {
            differences[v[i]]++;
        }
    }
    while(differences.size() > 0) {
        if(differences.size() == 1) {
            ll number = differences.begin()->first;
            ll count = differences.begin()->second;
            return (count % 2) * number;
        }
        auto it = differences.end(); it--;
        ll number1 = it->first;
        if(it->second % 2 == 0) {
            differences.erase(it);
            continue;
        } else {
            differences.erase(it);
            it--;
            ll number2 = it->first;
            if (it->second == 1) {
                differences.erase(it);
            }
            differences[abs(number1 - number2)]++;
        }
    }
    if(differences.size() == 0) {
        return 0;
    }
}

ll  prepartitioned_repeated_random(vector<ll> v, int tests){
    if(v.size() == 0) {
        return 0;
    }

    int n = v.size();

    ll best_residual = -1;
    while(tests--){
        vector<int> p(n);
        for (int i = 0; i < n; ++i) {
            p[i] = rand() % n;
        }
        vector<ll> a(n, 0);
        for (int j = 0; j < n; ++j) {
            a[p[j]] += v[j];
        }
        ll kk_output = kk_algorithm(a);
        if(best_residual == -1 or kk_output < best_residual) {
            best_residual = kk_output;
        }
    }
    return best_residual;
}

ll  repeated_random(vector<ll> v, int tests){
    if(v.size() == 0) {
        return 0;
    }

    ll best_residual = -1;
    while(tests--){
        ll cur_sum = 0;
        for (int i = 0; i < v.size(); ++i) {
            cur_sum += (2 * (rand() % 2) - 1) * v[i];
        }
        if(best_residual == -1 || best_residual > abs(cur_sum)){
            best_residual = abs(cur_sum);
        }
    }
    return best_residual;
}

ll  prepartitioned_hill_climbing(vector<ll> v, int tests){
    if(v.size() == 0) {
        return 0;
    }

    int n = v.size();

    vector<int> p(n);
    for (int i = 0; i < n; ++i) {
        p[i] = rand() % n;
    }
    vector<ll> a(n, 0);
    for (int j = 0; j < n; ++j) {
        a[p[j]] += v[j];
    }

    ll kk_output = kk_algorithm(a);

    while(tests--){
        int index = rand() % n, new_index = rand() % n;
        int old_index = p[index];
        p[index] = new_index;
        vector<ll> a_new(n, 0);
        for (int j = 0; j < n; ++j) {
            a_new[p[j]] += v[j];
        }
        ll kk_new_output = kk_algorithm(a_new);
        if(kk_new_output < kk_output) {
            kk_output = kk_new_output;
        } else {
            p[index] = old_index;
        }
    }
    return kk_output;
}

ll  hill_climbing(vector<ll> v, int tests){
    if(v.size() == 0) {
        return 0;
    }

    int n = v.size();

    ll cur_sum = 0;
    vector<ll> signs(n);
    for (int i = 0; i < v.size(); ++i) {
        signs[i] = (2 * (rand() % 2) - 1);
        cur_sum += signs[i] * v[i];
    }
    while(tests--){
        int index1 = rand() % n, index2 = rand() % n;
        if (rand() % 2 == 0) { //changing only 1 of them
            ll old_sign1 = signs[index1];
            ll new_sum = cur_sum - (2 * old_sign1) * v[index1];
            if(abs(cur_sum) > abs(new_sum)){
                cur_sum = new_sum;
                signs[index1] = (-1) * old_sign1;
            }
        } else { //changing both
            ll old_sign1 = signs[index1];
            ll old_sign2 = signs[index2];
            ll new_sum = cur_sum - (2 * old_sign1) * v[index1] - (2 * old_sign2) * v[index2];
            if(abs(cur_sum) > abs(new_sum)){
                cur_sum = new_sum;
                signs[index1] = (-1) * old_sign1;
                signs[index2] = (-1) * old_sign2;
            }
        }
    }
    return abs(cur_sum);
}

ld cooling_schedule(ld iter) {
    return 1e10 * std::pow(0.8, ceil(iter / 300));
}

bool make_random_decision(ld cur_sum, ld new_sum, ld iter){
    ld random_draw = (long double) rand() / (RAND_MAX);
    ld prob = exp( (abs(cur_sum) - abs(new_sum)) / cooling_schedule(iter));
    return random_draw < prob;
}


ll  prepartitioned_annealing(vector<ll> v, int tests){
    if(v.size() == 0) {
        return 0;
    }

    int n = v.size();

    vector<int> p(n);
    for (int i = 0; i < n; ++i) {
        p[i] = rand() % n;
    }
    vector<ll> a(n, 0);
    for (int j = 0; j < n; ++j) {
        a[p[j]] += v[j];
    }

    ll kk_output = kk_algorithm(a);
    ll best_output = kk_output;

    for(int iter = 0; iter < tests; iter++){
        int index = rand() % n, new_index = rand() % n;
        int old_index = p[index];
        p[index] = new_index;
        vector<ll> a_new(n, 0);
        for (int j = 0; j < n; ++j) {
            a_new[p[j]] += v[j];
        }
        ll kk_new_output = kk_algorithm(a_new);
        if(kk_new_output < kk_output or make_random_decision(kk_output, kk_new_output, iter + 1)) {
            kk_output = kk_new_output;
        } else {
            p[index] = old_index;
        }
        best_output = min(best_output, kk_new_output);
    }
    return best_output;
}

ll  annealing(vector<ll> v, int tests){
    if(v.size() == 0) {
        return 0;
    }

    int n = v.size();

    ll cur_sum = 0;
    vector<ll> signs(n);
    for (int i = 0; i < v.size(); ++i) {
        signs[i] = (2 * (rand() % 2) - 1);
        cur_sum += signs[i] * v[i];
    }
    ll best_sum = cur_sum;
    ll new_sum;
    for(int iter = 0; iter < tests; iter++){
        int index1 = rand() % n, index2 = rand() % n;
        if (rand() % 2 == 0) { //changing only 1 of them
            ll old_sign1 = signs[index1];
            new_sum = cur_sum - (2 * old_sign1) * v[index1];
            if(abs(cur_sum) > abs(new_sum) or make_random_decision(cur_sum, new_sum, iter + 1)){
                cur_sum = new_sum;
                signs[index1] = (-1) * old_sign1;
            }
        } else { //changing both
            ll old_sign1 = signs[index1];
            ll old_sign2 = signs[index2];
            new_sum = cur_sum - (2 * old_sign1) * v[index1] - (2 * old_sign2) * v[index2];
            if(abs(cur_sum) > abs(new_sum) or make_random_decision(cur_sum, new_sum, iter + 1)){
                cur_sum = new_sum;
                signs[index1] = (-1) * old_sign1;
                signs[index2] = (-1) * old_sign2;
            }
        }
        if(abs(best_sum) > abs(new_sum)){
            best_sum = new_sum;
        }
    }
    return abs(best_sum);
}

int main(int argc, char **argv) {
    srand(239);

    /*
    vector<ll> inp = vector<ll>{10, 3, 4};

    cout << repeated_random(inp, 2) << endl;
    cout << hill_climbing(inp, 5) << endl;
    cout << annealing(inp, 4) << endl;

    cout << prepartitioned_repeated_random(inp, 1) << endl;
    cout << endl;
    cout << prepartitioned_hill_climbing(inp, 3) << endl;

    cout << endl;
    cout << prepartitioned_annealing(inp, 1) << endl;

    //cout << kk_algorithm(inp) << endl;
    */

    if(!strcmp(argv[1], "0")) {
        int alg_code = stoi(argv[2]);
        int n = 5;

        ifstream input_file(argv[3]);
        if (!input_file.is_open()) {
            cout << "Incorrect file name" << endl;
            return 0;
        }

        vector<ll> v(n);
        for (int i = 0; i < n; ++i) {
            input_file >> v[i];
        }

        int tests = 25000;
        switch (alg_code) {
            case 0:
                cout << kk_algorithm(v) << endl;
                break;
            case 1:
                cout << repeated_random(v, tests) << endl;
                break;
            case 2:
                cout << hill_climbing(v, tests) << endl;
                break;
            case 3:
                cout << annealing(v, tests) << endl;
                break;
            case 11:
                cout << prepartitioned_repeated_random(v, tests) << endl;
                break;
            case 12:
                cout << prepartitioned_hill_climbing(v, tests) << endl;
                break;
            case 13:
                cout << prepartitioned_annealing(v, tests) << endl;
                break;
            default:
                cout << "incorrect code for algorithm" << endl;
                break;
        }
        return 0;
    }
}
