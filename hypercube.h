#ifndef HYPER_CUBE_H
#define HYPER_CUBE_H

#include <fstream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

struct Node {
    std::string word;
    std::string pred;
    int g;
    int h;

    Node(const std::string& word, const std::string pred, int g) : word(word), pred(pred), g(g), h(0) {
        for (size_t i = 0; i < word.size(); i++) {
            if (word[i] == '0')
                h += 1;
        }
    }
};

struct NodePriority {
    // helper for binary comparison
    bool compare_binary(const Node& lhs, const Node& rhs) {
        for (size_t i = 0; i < lhs.word.size(); i++) {
            if (lhs.word[i] == '1' && rhs.word[i] == '0') {
                return true;
            } else if (lhs.word[i] == '0' && rhs.word[i] == '1') {
                return false;
            }
        }
        return false;
    }
    // comprator operator()
    bool operator()(const Node& lhs, const Node& rhs) {
        // TODO: implement priority for nodes.
        int lhs_f = lhs.g + lhs.h;
        int rhs_f = rhs.g + rhs.h;
        if (lhs_f > rhs_f) {
            return true;
        } else if (lhs_f == rhs_f && lhs.h > rhs.h) {
            return true;
        } else if (lhs_f == rhs_f && lhs.h == rhs.h && compare_binary(lhs, rhs)) {
            return true;
        } else {
            return false;
        }
    }
};

struct SearchResult {
    // Whether or not there is a path.
    bool found;

    // The node labels on the path.
    std::vector<std::string> path;

    // The number of expansions made during search.
    size_t expansions;

    // Convenience for if found.
    SearchResult(std::vector<std::string> path, size_t expansions) : found(true), path(path), expansions(expansions) {}

    // Convenience for if not found.
    SearchResult(bool found, size_t expansions) : found(found), expansions(expansions) {}
};

class HyperCube {
public:
    // TODO: take an input file stream containing the permitted nodes for the
    // n-hypercube and populate the hypercube permitted node labels with the
    // set of nodes it lists. Do not change the signature.
    void read(std::istream& input);

    // TODO: take the starting node label string and return a vector of strings
    // containing the node labels on the optimal path from the starting node to
    // the ending node. If not path is found, the returned vector should
    // contain the string "no path". The last string in the returned vector
    // should otherwise be the number of expansions.
    SearchResult search(const std::string& start) const;

private:
    std::set<std::string> permitted;
    // find and return all neighbors to a node
    std::vector<Node>
    find_neighbors(Node& node, std::priority_queue<Node, std::vector<Node>, NodePriority>& unvisited) const;
};

#endif
