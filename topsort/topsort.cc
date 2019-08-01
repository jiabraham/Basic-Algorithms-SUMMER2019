#include <cmath>
#include <bits/stdc++.h>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string.h>
#include <map>
using namespace std;


struct Node {
    int key;
    int visited;
    vector<Node *> previous;
    
};

const int RESERVE_NODES = 100000;
const int MIN_FREE_NODES = 10;
std::vector<Node> all_nodes;
int cur_unused_position = 0;

std::map<int, Node *> node_index;

Node *get_free_node() {
    if (all_nodes.size() < cur_unused_position + MIN_FREE_NODES) {
        all_nodes.resize(cur_unused_position + RESERVE_NODES);        
    }
    Node *ret = &all_nodes[cur_unused_position];
    ret->visited = 0;
    cur_unused_position++;
    return ret;
}

void add_edge(int start_node, int end_node) {  
    //cout << start_node << " " << end_node << endl;
    Node *nodestart, *nodeend;
    if (node_index.find(end_node) == node_index.end()) {
        nodeend = get_free_node();
        nodeend->key = end_node;
        node_index[end_node] = nodeend;
        nodestart->previous.push_back(nodeend);
    } else {
        nodeend = node_index[end_node];
        nodestart->previous.push_back(nodeend);
    }
    if (node_index.find(start_node) == node_index.end()) {
        nodestart = get_free_node();
        nodestart->key = start_node;
        node_index[start_node] = nodestart;
    } else {
        nodestart = node_index[start_node];
    }
    //cout << node_index[start_node]->key << " " << node_index[end_node]->key << endl;
    //cout << "Size: " << node_index[start_node]->next.size() << " " << node_index[end_node]->next.size() << endl;
}

vector<int> topsort () {
    priority_queue <int, vector<int>, greater<int>> pq;
    for ()
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int num_of_rooms;
    cin >> num_of_rooms;
    
    num_of_rooms+=1;
    int num_of_passages;
    cin >> num_of_passages;

    for (int i = 0; i < num_of_passages; i++) {
        int room_num1;
        cin >> room_num1;
        int room_num2;
        cin >> room_num2;
        add_edge(room_num1, room_num2);
    }
    
    
    cout << 0 << endl;
    return 0;
}

