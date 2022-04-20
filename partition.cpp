#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>
#include <bitset>
#include <map>
#include <time.h>
#include <random>
#include <fstream>
#include <string.h>


using namespace std;

using ll = long long;
using ld = long double;

/*
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
*/

ll  kk_algorithm(vector<ll> v){
    if(v.size() == 0) {
        return 0;
    }

    multiset<ll> s;
    for (int i = 0; i < v.size(); ++i) {
        v[i] = abs(v[i]);
        if(v[i] > 0) {
            s.insert(v[i]);
        }
    }

    while(!s.empty()) {
        if(s.size() == 1) {
            return *s.begin();
        }
        auto it = s.end();
        it--;
        ll number1 = *it;
        it--;
        ll number2 = *it;
        s.erase(number1);
        s.erase(number2);
        if(number1 != number2) {
            s.insert(abs(number1 - number2));
        }
    }
    if(s.empty()) {
        return 0;
    }
}

ll  repeated_random(vector<ll> v, int tests){
    if(v.size() == 0) {
        return 0;
    }

    ll best_residual = -1;
    while(tests--) {
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

ll  prepartitioned_repeated_random(vector<ll> v, int tests){
    if(v.size() == 0) {
        return 0;
    }

    int n = v.size();

    ll best_residual = -1;
    while(tests--) {
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
        ll old_sign1 = signs[index1];
        ll old_sign2 = signs[index2];
        if (rand() % 2 == 0) { //changing only 1 of them
            ll new_sum = cur_sum - (2 * old_sign1) * v[index1];
            if(abs(cur_sum) > abs(new_sum)){
                cur_sum = new_sum;
                signs[index1] = (-1) * old_sign1;
            }
        } else { //changing both
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

    while(tests--) {
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

ld cooling_schedule(ld iter) {
    return 1e10 * std::pow(0.8, ceil(iter / 300));
}

bool make_random_decision(ld cur_sum, ld new_sum, ld iter){
    ld random_draw = (long double) rand() / (RAND_MAX);
    ld prob = exp( (abs(cur_sum) - abs(new_sum)) / cooling_schedule(iter));
    return random_draw < prob;
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

//(a small help function for testing procedure)
string find_method_by_value(map<string, int> keys, int value) {
    for(auto it = keys.begin(); it != keys.end(); it++) {
        if (it->second == value){
            return it->first;
        }
    }
}

int main(int argc, char **argv) {
    srand(239);

    if(!strcmp(argv[1], "0")) {
        int alg_code = stoi(argv[2]);
        int n = 100;

        ifstream input_file;
        input_file.open(argv[3], std::ios::in);
        if (!input_file.is_open()) {
            cout << "Incorrect file name" << endl;
            return 0;
        }

        vector<ll> v(n);
        for (int i = 0; i < n; ++i) {
            input_file >> v[i];
        }
        input_file.close();

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
    }

    if(atoi(argv[1]) <= 3 && atoi(argv[1]) >= 2) {

        //for big random numbers
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<unsigned long long> dis(1, (ll) 1e12);

        int sample_size;
        if (!strcmp(argv[1], "2")) {
            sample_size = 10;
        }

        if (!strcmp(argv[1], "3")) {
            sample_size = 50; // change to whatever you want
        }

        vector<vector<ll>> results(7);
        map<string, int> keys;
        keys["kk"] = 0;
        keys["rep_rand"] = 1;
        keys["part_rep_rand"] = 2;
        keys["hill"] = 3;
        keys["part_hill"] = 4;
        keys["ann"] = 5;
        keys["part_ann"] = 6;

        int tests = 25000;
        for (int i = 0; i < sample_size; i++) {
            int n = 100;
            vector<ll> v(n);
            for (int i = 0; i < n; ++i) {
                v[i] = dis(gen);
            }
            results[keys["kk"]].push_back(kk_algorithm(v));
            results[keys["rep_rand"]].push_back(repeated_random(v, tests));
            results[keys["part_rep_rand"]].push_back(prepartitioned_repeated_random(v, tests));
            results[keys["hill"]].push_back(hill_climbing(v, tests));
            results[keys["part_hill"]].push_back(prepartitioned_hill_climbing(v, tests));
            results[keys["ann"]].push_back(annealing(v, tests));
            results[keys["part_ann"]].push_back(prepartitioned_annealing(v, tests));
        }
        if (sample_size == 10) {
            for(int i = 0; i < 7; i++) {
                cout << "method = " << find_method_by_value(keys, i) << ":" << endl;
                for(int j = 0; j < sample_size; j++) {
                    cout << results[i][j] << " ";
                }
                cout << endl;
            }
        } else {
            for(int i = 0; i < 7; i++) {
                ofstream output;
                output.open(find_method_by_value(keys, i) + "_output");
                for(int j = 0; j < sample_size; j++) {
                    output << results[i][j] << endl;
                }
                output.close();
            }
        }
    }

    return 0;
}
