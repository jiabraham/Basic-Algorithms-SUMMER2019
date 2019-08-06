#include <cmath>
#include <queue>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string.h>
#include <map>

using namespace std;
int debug = 0;

struct Node {
  int key;
  int visited;
  int in_degree;
  vector<Node *> previous;
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


void reset_visited() {
  for (int i = 0; i < cur_unused_position; i++) {
    all_nodes[i].visited = 0;
  }
}

vector<Node *> get_indegree_zero_nodes() {
  vector<Node *> out;
  for (int i = 0; i < cur_unused_position; i++) {
    if (all_nodes[i].previous.size() == 0) {
      out.push_back(&all_nodes[i]);
    }
  }
  return out;
}


void add_edge(int start_node, int end_node) {  
  if (debug) {
    cout << start_node << " " << end_node << endl;
  }
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
  } else {
    nodestart = node_index[start_node];
  }
  nodeend->previous.push_back(nodestart);
  nodeend->in_degree = nodeend->previous.size();
  nodestart->next.push_back(nodeend);
  if (debug) {
    cout << node_index[start_node]->key << " " << node_index[end_node]->key << endl;
    cout << "Size: " << node_index[start_node]->next.size() << " " << node_index[end_node]->next.size() << endl;
  }
}

bool topsort (bool cycle, int num_of_rooms, vector<int>& topsort_output) {
  /* Step 1: Compute in-degree for each vertex */
  vector<Node *> indegree0 = get_indegree_zero_nodes();
  //cout << "calculate indegree0 vector" << " indegree0.size() = " << indegree0.size()  << endl;
  if (indegree0.size() == 0) {
    cycle = true;
    return cycle;
  }
  if (debug) cout  << "Indegree 0 number of items: " << indegree0.size() << endl;
  
  priority_queue<int, vector<int>, greater<int> > pq;
  int visited_counter = 0;

  for (int i = 0; i < indegree0.size(); i++) {
    pq.push(indegree0[i]->key);
  }
  while (pq.empty() == false) {
    //Take top element of the queue
    int temp = pq.top();
    topsort_output.push_back(temp);

    //Remove node key from queue
    pq.pop();
    
    // cout << temp << " ";
    //Cycle through indegree0 vector to get the node of the top element
    int matched_index = -1;
    for (int i = 0; i < indegree0.size(); i++) {
        if (indegree0[i]->key == temp) {
            //cout << "indegree0[i]->key = " << indegree0[i]->key << endl;

            //Increment the visited counter
            visited_counter++;

            matched_index = i;
    
    //Cycle through all the next elements of that node to decrement in_degree
        for (int j = 0; j < indegree0[i]->next.size(); j++) {
          indegree0[i]->next[j]->in_degree--;
          //cout << "indegree0[i]->next[j]->key = " << indegree0[i]->next[j]->key << " next[j]->in_degree = " << indegree0[i]->next[j]->in_degree << endl;
      
          //Check if indegree of any neighbors is 0
          if (indegree0[i]->next[j]->in_degree == 0) {
            //cout << "indegree0[i]->next[j]->key = " << indegree0[i]->next[j]->key << " " << "indegree0[i]->next[j]->in_degree = " << indegree0[i]->next[j]->in_degree << endl;
            //Put new indegree 0 node keys into the queue
            pq.push(indegree0[i]->next[j]->key);
            //cout << indegree0[i]->next[j]->key << endl;

        

            //Put new indegree 0 nodes into the indegree0 vector
            indegree0.push_back(indegree0[i]->next[j]);
          }
        }
        break;
      }
    }
    
    //Remove node from indegree0 matrix
    indegree0.erase(indegree0.begin() + matched_index);

    //Print out the current key
    //cout << temp << " ";
  }

  //Set cycle to true in case of topsort being impossible
  //cout << "visited_counter = " << visited_counter << endl;
  //cout << "num_of_rooms = " << num_of_rooms << endl;
  if (visited_counter != num_of_rooms)cycle=true;
  //cout << "visited counter = " << visited_counter << " all_nodes.size() = " << all_nodes.size() << endl;
  return cycle;
}

int main() {
  /* Enter your code here. Read input from STDIN. Print output to STDOUT */
  int num_of_rooms;
  cin >> num_of_rooms;
  
  int num_of_passages;
  cin >> num_of_passages;

  int start_node = 0;
  for (int i = 0; i < num_of_passages; i++) {
    int room_num1;
    cin >> room_num1;

    if (i==0) start_node = room_num1;
    
    int room_num2;
    cin >> room_num2;
    add_edge(room_num1, room_num2);
  }
  bool cycle = false;
  vector<int> out;
  if (topsort(cycle, num_of_rooms, out)) cout << -1 << endl;
  else {
    for (int i = 0; i < out.size(); i++) {
      cout << out[i] << " ";
    }
    cout << endl;
  }
  return 0;
}

