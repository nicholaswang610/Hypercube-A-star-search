#include "hypercube.h"

#include <iostream>

using namespace std;

// read permitted nodes
void HyperCube::read(istream& input) {
    int num_nodes;
    input >> num_nodes;
    input.ignore();  // get rid of newline
    for (int i = 0; i < num_nodes; i++) {
        string node;
        getline(input, node);
        permitted.insert(node);
    }
}

// find and return all neighbor nodes
vector<Node> HyperCube::find_neighbors(Node& node, priority_queue<Node, vector<Node>, NodePriority>& unvisited) const {
    vector<Node> neighbors;
    // find every node that differs by 1 character
    for (size_t i = 0; i < node.word.size(); i++) {
        string adjacent_node = node.word;
        if (node.word[i] == '0') {
            adjacent_node[i] = '1';
        } else {
            adjacent_node[i] = '0';
        }
        // check that the node is in the set of permitted nodes
        // if so, add it to the list of neighbors
        if (permitted.find(adjacent_node) != permitted.end()) {
            neighbors.push_back(Node(adjacent_node, node.word, node.g + 1));
        }
    }
    return neighbors;
}

// helper for trace_back_path
void trace_back_helper(std::map<std::string, Node, std::less<string>>& visited, vector<string>& path, Node& node) {
    // stop recursion at start node
    if (node.pred.empty()) {
        path.push_back(node.word);
        return;
    }
    trace_back_helper(visited, path, visited.at(node.pred));
    path.push_back(node.word);
}

// recursively build the path to the end/goal node
vector<string> trace_back_path(std::map<std::string, Node, std::less<string>>& visited, const string& end) {
    vector<string> path;
    Node& end_node = visited.at(end);
    trace_back_helper(visited, path, end_node);
    return path;
}

SearchResult HyperCube::search(const string& start) const {
    // Initialize ending node of all 1's.
    std::string end(start.size(), '1');
    // Instantiate priority queue to use for A* algorithm.
    std::priority_queue<Node, std::vector<Node>, NodePriority> unvisited;
    // Initialize the number of expansions used by the algorithm and path.
    size_t expansions = 0;
    std::vector<std::string> path;
    // map to lookup explored nodes by string and access predecessors
    std::map<std::string, Node, std::less<string>> visited;

    Node start_node(start, "", 0);
    unvisited.push(start_node);
    while (true) {
        // no path exists
        if (unvisited.size() == 0) {
            return SearchResult(false, expansions);
        }
        Node curr_node = unvisited.top();
        if (visited.find(curr_node.word) == visited.end()) {
            expansions++;
            visited.emplace(curr_node.word, curr_node);
        }
        unvisited.pop();
        vector<Node> neighbors = find_neighbors(curr_node, unvisited);
        for (size_t i = 0; i < neighbors.size(); i++) {
            map<string, Node, less<string>>::iterator it = visited.find(neighbors[i].word);
            // add to priority queue if unexplored or f value (g + h) is smaller than the explored one
            if (it == visited.end() || (neighbors[i].g + neighbors[i].h) < (it->second.g + it->second.h)) {
                unvisited.push(neighbors[i]);
            }
        }
        // break from loop if end node is found (top of priority queue)
        if (unvisited.top().word == end) {
            visited.emplace(end, unvisited.top());
            break;
        }
    }
    // trace back the predecessors to build the path
    path = trace_back_path(visited, end);
    return SearchResult(path, expansions);
}
