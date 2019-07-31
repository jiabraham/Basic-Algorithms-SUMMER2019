#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <string.h>
using namespace std;

int debug = 0;
int debug_backtrack = 0;

//
int getNeighborValue(const std::vector<std::vector<int>>& dp_table, int row_index, int column_index, bool match,
                        std::vector<std::vector<int>>& path_table) {
    int row_neighbor = row_index - 1;
    int column_neighbor = column_index - 1;
    int score = 0;
    int path_action = 1;
    if (row_neighbor < 0) {
        if (match) {
            score = -column_index + 2;
        }
        else {
            if (column_neighbor >= 0) {
                score = dp_table[row_index][column_neighbor] - 1;
                path_action = 2;
            }
            else {
                score = -1;
                path_action = 4;
            }
        }
        
    } else if (column_neighbor < 0) {
        if (match) {
            score = -row_index + 2;
        }
        else {
            if (row_neighbor >= 0) {
                score = dp_table[row_neighbor][column_index] - 1;
                path_action = 3;
            }
            else {
                score = -1;
                path_action = 4;
            }
        }
    } else {
        if (match) {
            score = dp_table[row_neighbor][column_neighbor] + 2;
        } else {
            if (dp_table[row_index][column_neighbor] > dp_table[row_neighbor][column_index]) {
                score = dp_table[row_index][column_neighbor] - 1;
                path_action = 2;
            } else {
                score = dp_table[row_neighbor][column_index] - 1;
                path_action = 3;
            }
            if (dp_table[row_neighbor][column_neighbor] - 1 > score) {
                score = dp_table[row_neighbor][column_neighbor] - 1;
                path_action = 4;
            }
        }
    }
    path_table[row_index][column_index] = path_action;
    if (debug) cout << row_index << " " << column_index << " score " << score << " path " << path_action << endl;
    return score;
    
}

int main() {
    string seq1;
    string seq2;
    
    cin >> seq1;
    cin >> seq2;
    
    int seq1_size = seq1.size();
    int seq2_size = seq2.size();
    
    bool swap = false;
    if (seq1_size > seq2_size) {
        string temp = seq2;
        seq2 = seq1;
        seq1 = temp;
        int temporary = seq2_size;
        seq2_size = seq1_size;
        seq1_size =temporary;
        swap = true;
    }

    //Dynamic Programming table and Path matrix declaration
    std::vector<std::vector<int>> DP_table;
    DP_table.resize(seq2_size, std::vector<int>(seq1_size, 0));
    
    std::vector<std::vector<int>> Path_matrix;
    Path_matrix.resize(seq2_size, std::vector<int>(seq1_size, 0));
    

    if (debug) cout << "Seq1 size: " << seq1_size << " seq 2 size = " << seq2_size << endl;
    for (int row_index = 0; row_index < seq2_size; ++row_index) {
        for (int column_index = 0; column_index < seq1_size; ++column_index) {
            bool match = false;
            if (seq1.substr(column_index,1)==(seq2.substr(row_index,1))) {
                match = true;
            }
            DP_table[row_index][column_index] = getNeighborValue(DP_table, row_index, column_index, match, Path_matrix);
        }
    }
 
    //getScore function end
    //getAlignment function start
    string final_seq1 = "";
    string final_seq2 = "";
    int i = seq1_size-1;
    int j = seq2_size-1;
    while(j > -1) {
        if (debug_backtrack) {
            cout << " i= " << i << " j= " << j << endl;
        }
  
        if (i >= 0) {
           if (Path_matrix[j][i] == 1 || 
                Path_matrix[j][i] == 4) {
                string temp = seq1.substr(i,1);
                temp.append(final_seq1);
                final_seq1 = temp;
                temp = seq2.substr(j,1);
                temp.append(final_seq2);
                final_seq2 = temp;
                j--;
                i--;
                if (debug_backtrack) {
                    cout << "1. append 2 chars 1: " << final_seq1 << " 2: " << final_seq2 << " " << i << " " << j << endl;
                }
            } else if (Path_matrix[j][i] == 2) {
                string temp = seq1.substr(i,1);
                temp.append(final_seq1);
                final_seq1 = temp;
                temp = "_";
                temp.append(final_seq2);
                final_seq2 = temp;
                i--;
                if (debug_backtrack) {
                    cout << "2. append 1 chars 1: " << final_seq1 << " 2: " << final_seq2 << endl;
                }
            } else if (Path_matrix[j][i] == 3) {
                string temp = seq2.substr(j,1);
                temp.append(final_seq2);
                final_seq2 = temp;
                temp = "_";
                temp.append(final_seq1);
                final_seq1 = temp;
                j--;
                if (debug_backtrack) {
                    cout << "3. append 1 chars 1: " << final_seq1 << " 2: " << final_seq2 << endl;
                }
            }
        } else if (j >= 0) {
            string temp = seq2.substr(j,1);
            temp.append(final_seq2);
            final_seq2 = temp;
            temp = "_";
            temp.append(final_seq1);
            final_seq1 = temp;
            j--;
            if (debug_backtrack) {
                cout << "4. append 1 chars 1: " << final_seq1 << " 2: " << final_seq2 << endl;
            }
        }
    }
    if (debug > 2) {
        //getAlignment function end
        std::cout << ""<< endl;
        std::cout << "DP_table" << endl;

        //Print DP_table start
    
        for (int i = 0; i < seq2_size; ++i)
        {
            for (int j = 0; j < seq1_size; ++j)
            {
                std::cout << DP_table[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }
    if (debug > 1) {
        //Print DP_table end
        std::cout << ""<< endl;
        std::cout << "Path_matrix" << endl;
        //Print Path_matrix start
        for (int i = 0; i < seq2_size; ++i)
        {
            for (int j = 0; j < seq1_size; ++j)
            {
                std::cout << Path_matrix[i][j] << ' ';
            }
            std::cout << std::endl;
        }
        //Print Path_matrix end
    }
    if (swap) {
        cout << DP_table[seq2_size-1][seq1_size-1] << endl;
        cout << final_seq2 << endl;
        cout << final_seq1 << endl;
    }
    else {
        cout << DP_table[seq2_size-1][seq1_size-1] << endl;
        cout << final_seq1 << endl;
        cout << final_seq2 << endl;
    }
    return 0;
}
