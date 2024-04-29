#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

int N, O, V, D;
struct Node {
    map<int, int> children;//[clildnode, children]
};

map<int, Node> tree;
//map<int, int> nodeDepths;
map<int, int> parents;

void addNode(int parent, int child, int distance) {
    tree[parent].children[child] = distance;
    parents[child] = parent;
}

/*void setDepths(int node, int depth) {
    nodeDepths[node] = depth;
    for (const auto& child : tree[node].children) {
        if (child.first != parents[node]) {
            setDepths(child.first, depth + 1);
        }
    }
}*/

//debug
void printTree() {
    cout << "\nCurrent Tree Structure:\n";
    for (const auto& pair : tree) {
        cout << "Node " << pair.first << " has children: ";
        for (const auto& child : pair.second.children) {
            if (child.first != parents[pair.first]) {
                cout << "(" << child.first << ", Distance: " << child.second << ") ";
            }
        }
        cout << "\n";
    }
}

void deleteNode(int node) {
    if (tree.find(node) == tree.end()) return;
    int parent = parents[node];
    if (parent == -1) return;
    for (auto& child : tree[node].children) {
        tree[parent].children[child.first] = child.second;
        parents[child.first] = parent;
    }
    tree[parent].children.erase(node);
    parents.erase(node);
    tree.erase(node);
    //printTree();
}
map<int, pair<long long int, long long int>> memo;
pair<long long int, long long int> dfs(int node, long long int parent) {
    if (memo.count(node)) {
        return memo[node];
    }

    long long int max1 = 0;
    long long int max2 = 0;

    for (const auto& child : tree[node].children) {
        if (child.first != parent) {
            long long int childDist = dfs(child.first, node).first + child.second;
            if (childDist > max1) {
                max2 = max1;
                max1 = childDist;
            } else if (childDist > max2) {
                max2 = childDist;
            }
        }
    }

    memo[node] = {max1, max2};
    return memo[node];
}
void check(bool last) {
    memo.clear();
    int lengg = tree.size();
    if(lengg == 1){
        cout << "Maximum Value: " << D  << "\nRoot of the Path: " << "0" << "\n";
    }
    long long int maxdistt=INT64_MIN;
    long long int chilroot;
    long long int overall_second_longest = INT64_MIN;
    long long int firstlong = INT64_MIN;
    for (auto& node : tree) {
        if(node.first == -1) continue;
        //cout << "now node: " << node.first << endl;
        overall_second_longest = INT64_MIN;
        firstlong = INT64_MIN;
        pair<long long int, long long int> lengths = dfs(node.first, -1);
        firstlong = max(firstlong, lengths.first);
        overall_second_longest = max(overall_second_longest, lengths.second);

        long long int overall = tree[parents[node.first]].children[node.first] + firstlong + overall_second_longest;//node.first
        if(overall > maxdistt){
            maxdistt = overall;
            chilroot = node.first;
        }
        
    }
    if(last){
        cout << "Final Root: " << chilroot << endl;
        return;
    }
    cout << "Maximum Value: " << maxdistt  << "\nRoot of the Path: " << chilroot << "\n";
}

int main() {
    //cin.tie(0);
    //ios_base::sync_with_stdio(false);
    cin >> N >> O;
    cin >> V >> D;
    parents[V] = -1;
    tree[parents[V]].children[V] = D;
    tree[V];
    //setDepths(V, 0);

    int P, C, Di;
    for (int i = 0; i < N; ++i) {
        cin >> P >> C >> Di;
        addNode(P, C, Di);
    }
    //setDepths(V, 0);
    //printTree();
    string operation;
    for (int i = 0; i < O; ++i) {
        cin >> operation;
        if (operation == "Check") {
            check(0);
        } else if (operation == "Add") {
            int A, B, C;
            cin >> A >> B >> C;
            addNode(A, B, C);
            //setDepths(V, 0);
        } else if (operation == "Delete") {
            int B;
            cin >> B;
            deleteNode(B);
                //printTree();
        }
    }
    //check(0);
    check(1);
    return 0;
}
