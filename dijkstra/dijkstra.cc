#include <cmath>
#include <queue>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string.h>
#include <map>
#include <climits>

using namespace std;
int debug = 0;

struct Edge;

struct Node {
  int node_index;
  int distance_from_source = INT_MAX;
  int distance_from_end = INT_MAX;
  vector<Edge> in_edges;
  vector<Edge> out_edges;
  int visited_forward = 0;
  int visited_backward = 0;
};

struct Edge {
    Node* source;
    Node* dest;
    bool water;
};

const int RESERVE_NODES = 100000;
const int MIN_FREE_NODES = 10;

std::vector<Node> all_nodes;
std::vector<Edge> water_edges;
int cur_unused_position = 0;

std::map<int, Node *> node_index;

Node *get_free_node() {
  if (all_nodes.size() < cur_unused_position + MIN_FREE_NODES) {
    all_nodes.resize(cur_unused_position + RESERVE_NODES);
  }
  Node *ret = &all_nodes[cur_unused_position];
  cur_unused_position++;
  return ret;
}

//Don't know if we'll need this
void reset_visited() {
  for (int i = 0; i < cur_unused_position; i++) {
    //all_nodes[i].visited = 0;
  }
}

//Don't know if we'll need this either
vector<Node *> get_indegree_zero_nodes() {
  vector<Node *> out;
  for (int i = 0; i < cur_unused_position; i++) {
    //if (all_nodes[i].previous.size() == 0) {
    //  out.push_back(&all_nodes[i]);
    //}
  }
  return out;
}

//In addition to creating the edge, set if this is a water path
void add_edge(int start_node, int end_node, bool water_path) {
  if (debug) {
    //cout << start_node << " " << end_node << endl;
  }
  Node *nodestart, *nodeend;
  if (node_index.find(end_node) == node_index.end()) {
    nodeend = get_free_node();
    nodeend->node_index = end_node;
    node_index[end_node] = nodeend;
  } else {
    nodeend = node_index[end_node];
  }
  if (node_index.find(start_node) == node_index.end()) {
    nodestart = get_free_node();
    nodestart->node_index = start_node;
    node_index[start_node] = nodestart;
  } else {
    nodestart = node_index[start_node];
  }
  nodestart->out_edges.resize(nodestart->out_edges.size()+1);
  Edge& node_start_edge = nodestart->out_edges.back();
  node_start_edge.source = nodestart;
  node_start_edge.dest = nodeend;
  node_start_edge.water = water_path;

  nodeend->in_edges.resize(nodeend->in_edges.size()+1);
  Edge& node_end_edge = nodeend->in_edges.back();
  node_end_edge.source = nodeend;
  node_end_edge.dest = nodestart;
  node_end_edge.water = water_path;

  //If this is an edge with water, add it to global vector water_edges
  if (water_path) {
    water_edges.push_back(node_start_edge);
  }
}

int traverse_to_beginning(Edge& in_edge, int cur_path_length, int *min_path_length) {
  Node *node = in_edge.dest;
  //cout << "B Examining: " << in_edge.dest->node_index << " " << cur_path_length << " " << *min_path_length << endl;
  if (node->visited_backward) {
    //cout << "Visited backward " << node->node_index << endl;
    if (node->distance_from_source < INT_MAX) {
      cur_path_length += node->distance_from_source;
      if (cur_path_length < *min_path_length) {
        cout << "B Set min path length: " << cur_path_length << endl;
        *min_path_length = cur_path_length;
        return *min_path_length;
      }
      return node->distance_from_source;
    }
    return INT_MAX;
  }
  if (node->node_index == 1) {
    //cout << "Got to beginning: " << node->node_index << endl;
    node->visited_backward = true;
    node->distance_from_source = 0;
    if (cur_path_length < *min_path_length) {
      //cout << "Set min path length: " << cur_path_length << endl;
      *min_path_length = cur_path_length;
      return *min_path_length;
    }
    return cur_path_length;
  }

  int min_ret = INT_MAX;
  for (int i = 0; i < node->in_edges.size(); i++) {
    int ret = 0;
    if (node->in_edges[i].water) {
      ret = traverse_to_beginning(node->in_edges[i], cur_path_length, min_path_length);
    } else {
      ret = traverse_to_beginning(node->in_edges[i], cur_path_length+1, min_path_length);
    }
    //cout << "Returned " << ret << " " << node->node_index << endl;
    node->in_edges[i].dest->visited_backward = true;
    node->in_edges[i].dest->distance_from_source = INT_MAX;
    if (ret >= 0) {
      node->in_edges[i].dest->distance_from_source = ret - cur_path_length;
      //cout << "Set distance from source " << ret - cur_path_length << endl;
      if (ret < min_ret) {
        min_ret = ret;
      }
    }
  }
  return min_ret;
}

int traverse_to_end(Edge& in_edge, int end_node_index, int cur_path_length, int *min_path_length, bool *reached_final, bool debug_path) {
  Node *node = in_edge.dest;
  if (debug_path) cout << "++";
  cout << "F Examining: " << in_edge.dest->node_index << " " << end_node_index << " " << cur_path_length << " " << *min_path_length << endl;
  if (node->visited_forward) {
    if (debug_path) cout << "++";
    cout << "DE: " << node->distance_from_end;
    if (node->distance_from_end < INT_MAX) {

      cur_path_length += node->distance_from_end;
      if (cur_path_length < *min_path_length) {
        cout << "F Set min path length: " << cur_path_length << endl;
        *min_path_length = cur_path_length;
        return *min_path_length;
      }
      return node->distance_from_end;
    }
    return INT_MAX;
  }
  if (node->node_index == end_node_index) {
    *reached_final = true;
    cout << "**Got to end: " << node->node_index << endl;
    node->visited_forward = true;
    node->distance_from_end = 0;
    if (cur_path_length < *min_path_length) {
      cout << "Set min path length: " << cur_path_length << endl;
      *min_path_length = cur_path_length;
      return *min_path_length;
    }
    return cur_path_length;
  }
  if (node->out_edges.empty()) {
    /* Mark the path as being visited, but no path to end */
    node->distance_from_end = -1;
    node->visited_forward = true;
    return -1;
  }
  int min_ret = INT_MAX;
  for (int i = 0; i < node->out_edges.size(); i++) {
    int ret = 0;
    if (debug_path) cout << "++Looking at: " << node->out_edges[i].dest->node_index;
    if (node->out_edges[i].water) {
      ret = traverse_to_end(node->out_edges[i], end_node_index, cur_path_length, min_path_length, reached_final, debug_path);
    } else {
      ret = traverse_to_end(node->out_edges[i], end_node_index, cur_path_length+1, min_path_length, reached_final, debug_path);
    }
    if (*reached_final) {
      cout << "**Returned " << ret << " " << node->node_index << endl;
    } else {
      cout << "Returned " << ret << " " << node->node_index << endl;
    }
    node->out_edges[i].dest->visited_forward = true;
    node->out_edges[i].dest->distance_from_end = INT_MAX;
    if (ret >= 0) {
      node->out_edges[i].dest->distance_from_end = ret - cur_path_length;
      if (debug_path) cout << "++";
      cout << "F Set distance from end " << ret - cur_path_length << endl;
      if (ret < min_ret) {
        min_ret = ret;
      }
    }
  }
  return min_ret;
}

int main() {
  /* Enter your code here. Read input from STDIN. Print output to STDOUT */
  int num_of_rooms;
  cin >> num_of_rooms;

  int num_of_passages;
  cin >> num_of_passages;

  int start_node = 0;
  int water_count = 0;
  for (int i = 0; i < num_of_passages; i++) {
    int room_num1;
    cin >> room_num1;

    if (i==0) start_node = room_num1;

    int room_num2;
    cin >> room_num2;

    int reservoir;
    cin >> reservoir;
    if (reservoir == 1) {
      water_count++;
      add_edge(room_num1, room_num2, true);
    }
    else {
      add_edge(room_num1, room_num2, false);
    }
  }

  //Print minumum path distance minus paths with reservoir
  int min_path_length = INT_MAX;
  for (int i = 0; i < water_count; i++) {
    int cur_path_length_to_beginning = INT_MAX;
    traverse_to_beginning(water_edges[i], 0, &cur_path_length_to_beginning);
    int cur_path_length_to_end = INT_MAX;
    bool debug_path = false;
    if (water_edges[i].dest->node_index==45) debug_path = true;
    bool reached_end = false;
    traverse_to_end(water_edges[i], num_of_rooms, 0, &cur_path_length_to_end, &reached_end, debug_path);
    if (reached_end) cout << "**";
    cout << "Q: " << water_edges[i].dest->node_index << " " << cur_path_length_to_beginning << " " << cur_path_length_to_end << endl;
    if (cur_path_length_to_beginning < INT_MAX && cur_path_length_to_end < INT_MAX) {
        int cur_path_length = cur_path_length_to_beginning + cur_path_length_to_end;
        if (cur_path_length < min_path_length && cur_path_length >= 0) {
          min_path_length = cur_path_length;
          cout << "P: " << water_edges[i].dest->node_index << " " << min_path_length;
        }
    }
  }
  if (min_path_length == INT_MAX) {
    min_path_length = -1;
  }
  cout << min_path_length << endl;
  //bool ret = backward_dijkstra(num_of_rooms, all_nodes[num_of_rooms-1], forward, min_path, water_count)
  //if (ret) {
  //  cout << min_path << endl;
  //}
  //else {
  //  cout << -1 << endl;
  //}
  return 0;
}
