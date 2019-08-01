#include <cmath>
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
    vector<Node *> next;
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
    } else {
        nodeend = node_index[end_node];
    }
    if (node_index.find(start_node) == node_index.end()) {
        nodestart = get_free_node();
        nodestart->key = start_node;
        node_index[start_node] = nodestart;
        nodestart->next.push_back(nodeend);
    } else {
        nodestart = node_index[start_node];
        nodestart->next.push_back(nodeend);
    }
    //cout << node_index[start_node]->key << " " << node_index[end_node]->key << endl;
    //cout << "Size: " << node_index[start_node]->next.size() << " " << node_index[end_node]->next.size() << endl;
}

bool cycle_detection(Node *start, std::vector<int>& room_seq) {
    //cout << "Room seq: ";
    //for (const auto& ind : room_seq) {
    //    cout << ind << " ";
    //}
    //cout << endl;
    //cout << "Next size: " << start->next.size() << endl;
    for (int i = 0; i < start->next.size(); i++) {
        std::vector<int> cur_path = room_seq;
        if (start->visited == 2) continue;
        start->visited = 1;
        if (start->next.size() != 0) {
            Node* cur = start->next[i];
            //cout << "Looking at: " << cur->key << endl;
            std::vector<int>::iterator p = find(cur_path.begin(), cur_path.end(), cur->key);
            if (p != cur_path.end()) {
                cur_path.erase(cur_path.begin(), p);
                //cout << "Found cyle " << cur->key << endl;
                room_seq = cur_path;
                return true;
            }
            cur_path.push_back(cur->key);
            if (cycle_detection(cur, cur_path)) {
                //cout << "Returning 1 after recursion." << endl; 
                room_seq = cur_path;
                return true;
            }
        }
        //cout << "Looking at: " << cur->key << endl;
    }
    //cout << "Return 0" << endl;
    start->visited = 2;
    return false;
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
    
    for (int start_node = 0; start_node < num_of_rooms; start_node++) {
        if (node_index.find(start_node) != node_index.end()) {
            std::vector<int> room_seq;
            room_seq.push_back(node_index[start_node]->key);
            if (cycle_detection(node_index[start_node], room_seq)) {
                // Output the room seq
                cout << "1" << endl;
                for (const auto& seq : room_seq) {
                    cout << seq << " ";
                }
                return 0;
            }
        }
    }
    cout << 0 << endl;
    return 0;
}
