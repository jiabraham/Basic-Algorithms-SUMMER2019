#include <bits/stdc++.h>
#include <bits/stl_vector.h>
#include <vector>
#include <cmath>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>


using namespace std;
int debug = 0;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);
vector<int> karatsuba(vector<int>& a, vector<int>& b);

/*
 * Complete the 'smellCosmos' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER_ARRAY a
 *  2. INTEGER_ARRAY b
 */

vector<int> smellCosmos(vector<int>& a, vector<int>& b) {
    int a_size = a.size();
    vector<int> out = karatsuba(a, b);
    return out;
}

void displayVector(const vector<int>& a, const string& label) {
    if (debug == 0) return;
    
    cout << label <<"(" << a.size() <<  "): ";
    for (int i = 0; i < a.size(); i++) {
        cout << a[i] << " ";
    }
    cout << endl;
}


//Do we have to pass in all factor vectors and products for each call?
vector<int> karatsuba(vector<int>& a, vector<int>& b) {
    vector<int> product;
    
    displayVector(a, "a");
    displayVector(b, "b");
    if (a.size() == 4 && b.size() == 4) {
        product.resize(7);
        product[0] = a[0]*b[0];
        product[1] = a[0]*b[1] + b[0]*a[1];
        product[2] = a[0]*b[2] + a[1]*b[1] + a[2]*b[0];
        product[3] = a[0]*b[3] + a[1]*b[2] + a[2]*b[1] + b[0]*a[3];
        product[4] = a[1]*b[3] + a[2]*b[2] + a[3]*b[1];
        product[5] = a[2]*b[3] + a[3]*b[2];
        product[6] = a[3]*b[3];
        return product; 
    }
    if (a.size() == 2 && b.size() == 2) {
      product.resize(3);
      product[0] = a[0]*b[0];
      product[2] = a[1]*b[1];
      product[1] = a[0]*b[1] + b[0]*a[1];
      return product;
    }

    if (a.size() == 1 && b.size() == 1) {
        //Check for correctness of middle squares
        product.push_back(a[0]*b[0]);
        displayVector(product, "Product");
        return product;
    }
    int order = a.size();
    
    int mid = order/2;
    int rmid = order - mid;
    
    
    if (debug) cout << "MID: " << mid << endl;
    vector<int> a_low(a.begin(),a.begin()+mid);
    vector<int> a_high(a.begin()+mid, a.end());
    vector<int> b_low(b.begin(),b.begin()+mid);
    vector<int> b_high(b.begin()+mid, b.end());
    
    //Calculate low indices
    if (debug) cout << "LOW" << endl;
    vector<int> low_portion = karatsuba(a_low, b_low);
    
    vector<int> midveca(a_high.size(), 0);
    displayVector(a_low, "a_low");
    displayVector(a_high, "a_high");
    displayVector(b_low, "b_low");
    displayVector(b_high, "b_high");
    
    for (int i=0; i <a_high.size(); i++) {
        if (i >= a_low.size()) {
          midveca[i] = a_high[i];
        } else {
          midveca[i] = a_low[i] + a_high[i]; 
        }
    }
    vector<int> midvecb(b_high.size(), 0);
    for (int i=0; i < b_high.size(); i++) {
        if (i >= b_low.size()) {
          midvecb[i] = b_high[i];
        } else {
          midvecb[i] = b_low[i] + b_high[i]; 
        }
    }
    if (debug) cout << "MID_PORTION: " << endl;
    vector<int> mid_portion = karatsuba(midveca, midvecb);
    if (debug) cout << "HIGH PORTION: " << endl;
    
    vector<int> high_portion = karatsuba(a_high, b_high);
    
    displayVector(low_portion, "low_portion");
    displayVector(mid_portion, "mid_portion");
    displayVector(high_portion, "high_portion");
    for (int i = 0; i < mid_portion.size(); i++) {
      if (i < low_portion.size()) {
        mid_portion[i] -= (low_portion[i] + high_portion[i]);
      } else {
    mid_portion[i] -= high_portion[i];
      }
    }
    product.resize(2*order-1, 0);
    int corr = 0;
    if (order % 2 == 1) corr = 1;
    for (int i = 0; i < high_portion.size(); i++) {
        product[i+order - corr] += high_portion[i];
        product[i+mid] += mid_portion[i];
        if (i < low_portion.size()) product[i] += low_portion[i];
    }
    displayVector(product, "PRODUCT:");
    return product;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));


    string n_temp;
    getline(cin, n_temp);

    int n = stoi(ltrim(rtrim(n_temp)));

    string a_temp_temp;
    getline(cin, a_temp_temp);

    vector<string> a_temp = split(ltrim(rtrim(a_temp_temp)));

    vector<int> a(n+1);

    for (int i = 0; i < n+1; i++) {
        int a_item = stoi(a_temp[i]);

        a[i] = a_item;
    }

    string b_temp_temp;
    getline(cin, b_temp_temp);

    vector<string> b_temp = split(ltrim(rtrim(b_temp_temp)));

    vector<int> b(n+1);

    for (int i = 0; i < n+1; i++) {
        int b_item = stoi(b_temp[i]);

        b[i] = b_item;
    }
    
    vector<int> result = smellCosmos(a, b);

       
    for (int i = 0; i < result.size(); i++) {
        fout << result[i];

        if (i != result.size() - 1) {
            fout << " ";
        }
    }

    fout << "\n";

    fout.close();
    
    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
