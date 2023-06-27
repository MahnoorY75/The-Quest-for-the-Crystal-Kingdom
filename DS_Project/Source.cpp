#include <iostream>
#include <fstream>
#include <limits>
using namespace std;
const int INF = numeric_limits<int>::max() / 2;
#include <ctime>
#include <cstdlib>
const int ROWS = 20;
const int COLS = 20;
int Total_Score = 0;
int Jewel = 0;
int Potion = 0;
int Weapon = 0;
int checkid = 0;

class Node {
public:
    int id;
    char reward_type;
    int reward_score;
    int count;
    int height;
    Node* left;
    Node* right;

    Node(int id, char reward_type)
        : id(id), reward_type(reward_type), count(1), height(1), left(nullptr), right(nullptr) {
        switch (reward_type) {
        case 'J':
            reward_score = 50;
            break;
        case 'W':
            reward_score = 30;
            break;
        case 'P':
            reward_score = 70;
            break;
        default:
            reward_score = 0;
            break;
        }
    }
};

class AVLTree {
public:
    Node* root;

    AVLTree() : root(nullptr) {}

    int height(Node* node) {
        return node ? node->height : 0;
    }

    int balanceFactor(Node* node) {
        return height(node->left) - height(node->right);
    }

    void updateHeight(Node* node) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* balance(Node* node) {
        updateHeight(node);

        if (balanceFactor(node) == 2) {
            if (balanceFactor(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        if (balanceFactor(node) == -2) {
            if (balanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
        return node;
    }

    Node* insert(Node* node, int id, char reward_type) {
        if (!node) {
            return new Node(id, reward_type);
        }
        if (id < node->id) {
            node->left = insert(node->left, id, reward_type);
        }
        else if (id > node->id) {
            node->right = insert(node->right, id, reward_type);
        }
        else {
            node->count++;
            node->reward_score += node->reward_score / (node->count - 1);
        }
        return balance(node);
    }

    Node* findMin(Node* node) {
        return node->left ? findMin(node->left) : node;
    }

    Node* removeMin(Node* node) {
        if (!node->left) {
            return node->right;
        }
        node->left = removeMin(node->left);
        return balance(node);
    }

    Node* remove(Node* node, int id) {
        if (!node) {
            return nullptr;
        }
        if (id < node->id) {
            node->left = remove(node->left, id);
        }
        else if (id > node->id) {
            node->right = remove(node->right, id);
        }
        else
        {
            if (node->count > 1) {
                node->count--;
                node->reward_score -= node->reward_score / (node->count + 1);
            }
            else {
                Node* left = node->left;
                Node* right = node->right;
                delete node;

                if (!right) {
                    return left;
                }
                Node* min = findMin(right);
                min->right = removeMin(right);
                min->left = left;
                return balance(min);
            }
        }
        return balance(node);
    }

    void add(int id, char reward_type) {
        root = insert(root, id, reward_type);
    }

    void remove(int id) {
        root = remove(root, id);
    }
    void Count(Node* node) {
        if (!node) {
            return;
        }

        Count(node->left);
        /*cout << "ID: " << node->id
            << ", Reward Type: " << node->reward_type
            << ", Reward Score: " << node->reward_score
            << ", Count: " << node->count
            << std::endl;*/
        if (node->reward_type == 'J')
            Jewel += node->count;
        else if (node->reward_type == 'W')
            Weapon += node->count;
        else if (node->reward_type == 'P')
            Potion += node->count;
        Total_Score += node->reward_score;
        Count(node->right);
    }

    void checke(Node* node,char a) {
        if (!node) {
            return;
        }

        checke(node->left,a);
        /*cout << "ID: " << node->id
            << ", Reward Type: " << node->reward_type
            << ", Reward Score: " << node->reward_score
            << ", Count: " << node->count
            << std::endl;*/
        if (node->reward_type == a)
            checkid = node->id;
        checke(node->right,a);
    }
};


class Graph {
public:
    int numVertices;
    int** adjMatrix;
    int** next;
    int parent[ROWS * COLS];
    Graph(int numVertices) {
        this->numVertices = numVertices;
        adjMatrix = new int* [numVertices];
        for (int i = 0; i < numVertices; i++) {
            adjMatrix[i] = new int[numVertices];
            for (int j = 0; j < numVertices; j++) {
                adjMatrix[i][j] = (i == j) ? 0 : INF;
            }
        }
        next = new int* [numVertices];
        for (int i = 0; i < numVertices; i++) {
            next[i] = new int[numVertices];
            for (int j = 0; j < numVertices; j++) {
                if (i == j || adjMatrix[i][j] == INF) {
                    next[i][j] = -1;
                }
                else {
                    next[i][j] = j;
                }
            }
        }

    }

    void addEdge(int i, int j, int weight) {
        adjMatrix[i][j] = weight;
        adjMatrix[j][i] = weight;
    }

    void removeEdge(int i, int j) {
        adjMatrix[i][j] = INF;
        adjMatrix[j][i] = INF;
    }

    bool isEdge(int i, int j) {
        return adjMatrix[i][j] != 0 && adjMatrix[i][j] != INF;
    }
    void print() {
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                if(adjMatrix[i][j]==1)
                std::cout << i<<" " << j << " = ";
            }
            std::cout << std::endl;
        }
    }
    void floydWarshall() {
        //next = new int* [numVertices];
        for (int i = 0; i < numVertices; i++) {
            next[i] = new int[numVertices];
            for (int j = 0; j < numVertices; j++) {
                if (i == j || adjMatrix[i][j] == INF) {
                    next[i][j] = -1;
                }
                else {
                    next[i][j] = j;
                }
            }
        }
        for (int k = 0; k < numVertices; k++) {
            for (int i = 0; i < numVertices; i++) {
                for (int j = 0; j < numVertices; j++) {
                    if (adjMatrix[i][k] != INF && adjMatrix[k][j] != INF &&
                        adjMatrix[i][k] + adjMatrix[k][j] < adjMatrix[i][j]) {

                        adjMatrix[i][j] = adjMatrix[i][k] + adjMatrix[k][j];
                        //cout << adjMatrix[i][j] << endl;
                        next[i][j] = next[i][k];
                        //cout << next[i][j] << endl;
                    }
                }
            }
        }
    }

    void printShortestPath(int src, int dest, AVLTree& inventory, char** map) {
        if (src < 0 || src >= numVertices || dest < 0 || dest >= numVertices) {
            cerr << "Invalid source or destination node" << endl;
            return;
        }

        if (next[src][dest] == -1) {
            cerr << "No path exists between nodes " << src << " and " << dest << "." << endl;
            return;
        }
        int check = 0;
        cout << "Shortest path between nodes " << src << " and " << dest << ": " << endl << "( " << src / 20 << " , " << src % 20 << " )" << endl;
        int currentNode = src;
        while (currentNode != dest) {
            currentNode = next[currentNode][dest];
            if (map[currentNode / 20][currentNode % 20] == 'J' || map[currentNode / 20][currentNode % 20] == 'P' || map[currentNode / 20][currentNode % 20] == 'W')
            {
               // cout << map[currentNode / 20][currentNode % 20] << endl;
                int id = rand() % 201;
                if (check == 0)
                    inventory.add(100, map[currentNode / 20][currentNode % 20]);
                else
                    inventory.add(id, map[currentNode / 20][currentNode % 20]);
                check++;
            }
            if (map[currentNode / 20][currentNode % 20] == '@' || map[currentNode / 20][currentNode % 20] == '&' || map[currentNode / 20][currentNode % 20] == '$')
            {
                char a;
                if (map[currentNode / 20][currentNode % 20] == '&')
                    a = 'J';
                else if (map[currentNode / 20][currentNode % 20] == '$')
                    a = 'P';
                else if (map[currentNode / 20][currentNode % 20] == '@')
                    a = 'W';

                inventory.checke(inventory.root, a);
                if (checkid != 0)
                    inventory.remove(checkid);
                //cout << "Check ID" << checkid << endl;
                checkid = 0;
            }
            if (map[currentNode / 20][currentNode % 20] == '%')
            {
                //cout << "You Lose THe Game " << endl;
                //exit(1);
                //goto jump;
            }
            cout << " -> " << "( "<<currentNode/20<<" , "<<currentNode%20<<" )";
        }
        cout << endl;
    }




    int getShortestPathDistance(int src, int dest) {
        if (src < 0 || src >= numVertices || dest < 0 || dest >= numVertices) {
            cerr << "Invalid source or destination node" << endl;
            return -1;
        }

        if (adjMatrix[src][dest] == INF) {
            cerr << "No path exists between nodes " << src << " and " << dest << "." << endl;
            return -1;
        }

        return adjMatrix[src][dest];
    }

    int minKey(int key[], bool mstSet[]) {
        int min = INF;
        int min_index = 0;

        for (int v = 0; v < numVertices; v++) {
            if (mstSet[v] == false && key[v] < min) {
                //cout << v << endl;
                min = key[v];
                min_index = v;
            }
        }

        return min_index;
    }

    void primMST() {

        int key[ROWS * COLS];
        bool mstSet[ROWS * COLS];

        for (int i = 0; i < numVertices; i++) {
            key[i] = INF;
            mstSet[i] = false;
        }

        key[0] = 0;
        parent[0] = -1;

        for (int count = 0; count < numVertices - 1; count++) {
            int u = minKey(key, mstSet);

            mstSet[u] = true;

            for (int v = 0; v < numVertices; v++) {
                if (adjMatrix[u][v] && mstSet[v] == false && adjMatrix[u][v] < key[v]) {
                    parent[v] = u;
                    key[v] = adjMatrix[u][v];
                }
            }
        }

        // print the MST
        for (int i = 1; i < numVertices; i++) {
            //cout << parent[i] << " - " << i << endl;
        }
    }

    // This function prints the path from the source to the destination
    void printPrimPath(int src, int dst) {
        if (src == dst) {
            cout << "( " << src / 20 << " , " << src % 20 << " )" << " ";
            return;
        }
        //cout << "( " << dst / 20 << " , " << dst % 20 << " )" << " ";
        printPrimPath(src, parent[dst]);
        cout << "( " << dst / 20 << " , " << dst % 20 << " )" << " ";
    }


    ~Graph() {
        for (int i = 0; i < numVertices; i++) {
            delete[] adjMatrix[i];
            delete[] next[i];
        }
        delete[] adjMatrix;
        delete[] next;
    }
};

//class Edge {
//public:
//    int src, dest, weight;
//};

//class DSU {
//    int* parent;
//    int* rank;
//    int n;
//public:
//    DSU(int n) {
//        this->n = n;
//        parent = new int[n];
//        rank = new int[n];
//        for (int i = 0; i < n; i++) {
//            parent[i] = i;
//            rank[i] = 0;
//        }
//    }

//    int find(int x) {
//        if (x != parent[x])
//            parent[x] = find(parent[x]);
//        return parent[x];
//    }

//    void union_sets(int x, int y) {
//        x = find(x);
//        y = find(y);
//        if (x != y) {
//            if (rank[x] < rank[y])
//                std::swap(x, y);
//            parent[y] = x;
//            if (rank[x] == rank[y])
//                rank[x]++;
//        }
//    }

//    ~DSU() {
//        delete[] parent;
//        delete[] rank;
//    }
//};

//void KruskalMST(Graph& graph) {
//    int V = graph.numVertices;

//    // Step 1: Convert the adjacency matrix into an edge list
//    // Count the number of edges
//    int numEdges = 0;
//    for (int i = 0; i < V; i++) {
//        for (int j = i + 1; j < V; j++) {
//            if (graph.isEdge(i, j)) {
//                numEdges++;
//            }
//        }
//    }

//    // Allocate the array and fill it
//    Edge* edges = new Edge[numEdges];
//    int edgeIndex = 0;
//    for (int i = 0; i < V; i++) {
//        for (int j = i + 1; j < V; j++) {
//            if (graph.isEdge(i, j)) {
//                edges[edgeIndex].src = i;
//                edges[edgeIndex].dest = j;
//                edges[edgeIndex].weight = graph.adjMatrix[i][j];
//                edgeIndex++;
//            }
//        }
//    }

//    // Step 2: Sort the edges according to their weights
//    std::sort(edges, edges + numEdges, [](Edge a, Edge b) {
//        return a.weight < b.weight;
//        });

//    // Step 3: Apply Kruskal's algorithm using a DSU
//    DSU dsu(V);
//    Edge* mst = new Edge[V - 1];
//    int mstIndex = 0;
//    for (int i = 0; i < numEdges; i++) {
//        Edge& edge = edges[i];
//        if (dsu.find(edge.src) != dsu.find(edge.dest)) {
//            mst[mstIndex++] = edge;
//            dsu.union_sets(edge.src, edge.dest);
//        }
//    }

//    // Printing the MST
//    int totalWeight = 0;
//    for (int i = 0; i < V - 1; i++) {
//        Edge& edge = mst[i];
//        std::cout << "Edge (" << edge.src << ", " << edge.dest << "), Weight: " << edge.weight << "\n";
//        totalWeight += edge.weight;
//    }
//    std::cout << "Total weight: " << totalWeight << "\n";

//    delete[] edges;
//    delete[] mst;
//}

// ... Node and Graph class definitions ...

int minDistance(int dist[], bool visited[], int numVertices) {
    int minValue = std::numeric_limits<int>::max();
    int minIndex = -1;

    for (int i = 0; i < numVertices; i++) {
        if (!visited[i] && dist[i] < minValue) {
            minValue = dist[i];
            minIndex = i;
        }
    }

    return minIndex;
}



void printPath(int parent[], int dest, int source) {
    //cout << dest << endl;
    if (parent[dest] == -1) {
        std::cout << "(" << dest / 20 << "," << dest % 20 << ")";
        return;
    }
    printPath(parent, parent[dest], source);
    std::cout << " -> " << "(" << dest / 20 << "," << dest % 20 << ")";
}

void dijkstra(Graph& graph, int src, int dest,AVLTree& inventory,char** map) {
    int numVertices = graph.numVertices;
    int check = 0;
    int dist[20 * 20];
    bool visited[20 * 20];
    int parent[20 * 20];
    srand(time(0));

    for (int i = 0; i < numVertices; i++) {
        dist[i] = std::numeric_limits<int>::max();
        visited[i] = false;
        parent[i] = -1;
    }

    dist[src] = 0;

    for (int count = 0; count < numVertices - 1; count++) {
        int u = minDistance(dist, visited, numVertices);

        // Stop the loop if all vertices are visited
        if (u == -1) {
            break;
        }

        visited[u] = true;

        for (int v = 0; v < numVertices; v++) {
            if (!visited[v] && graph.isEdge(u, v) && dist[u] != std::numeric_limits<int>::max() &&
                dist[u] + graph.adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + graph.adjMatrix[u][v];
                
                if (map[v / 20][v % 20] == 'J' || map[v / 20][v % 20] == 'P' || map[v / 20][v % 20] == 'W')
                {
                   // cout << map[v / 20][v % 20] << endl;
                    int id = rand() % 201;
                    if(check==0)
                        inventory.add(100, map[v / 20][v % 20]);
                    else
                        inventory.add(id, map[v / 20][v % 20]);
                    check++;
                }
                if (map[v / 20][v % 20] == '@' || map[v / 20][v % 20] == '&' || map[v / 20][v % 20] == '$')
                {
                    char a;
                    if (map[v / 20][v % 20] == '&')
                        a = 'J';
                    else if (map[v / 20][v % 20] == '$')
                        a = 'P';
                    else if (map[v / 20][v % 20] == '@')
                        a = 'W';

                    inventory.checke(inventory.root, a);
                    if(checkid!=0)
                        inventory.remove(checkid);
                    //cout << "Check ID" << checkid << endl;
                    checkid = 0;
                }
                if (map[v / 20][v % 20] == '%')
                {
                    //cout << "You Lose THe Game " << endl;
                    //exit(1);
                    //goto jump;
                }
                parent[v] = u;
            }
        }
    }
    //inventory.remove(100);
    std::cout << "Shortest path: ";
    std::cout << dest;
    printPath(parent, dest, src);
    std::cout << std::endl;
    std::cout << "Path cost: " << dist[dest] << std::endl;
}

int weight(char val)
{
    int num = 0;
    if (val == '%')
        num = 100;
    else if (val == '$')
        num = 70;
    else if (val == '&')
        num = 50;
    else if (val == '@')
        num = 30;
    else if (val == 'C')
        num = 1;
    else if (val == 'W')
        num = 7;
    else if (val == 'J')
        num = 5;
    else if (val == 'P')
        num = 3; 

    return num;
}
char** readMapFromFile(string filename) {
    char** map = new char* [20];
    for (int i = 0; i < 20; i++) {
        map[i] = new char[20];
    }

    ifstream file;
    file.open(filename);
    if (!file) {
        cout << "Unable to open file " << filename << endl;
        return map;
    }

    char c;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            file >> c;
            map[i][j] = c;
        }
    }

    file.close();
    return map;
}
void make_graph(Graph& graph, char** map)
{

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int currentNode = i * COLS + j;

            if (map[i][j] != '#') {
                if (i > 0 && map[i - 1][j] != '#') {
                    graph.addEdge(currentNode, (i - 1) * COLS + j, weight(map[i][j]));
                }
                if (j > 0 && map[i][j - 1] != '#') {
                    graph.addEdge(currentNode, i * COLS + (j - 1), weight(map[i][j]));
                }
                if (i < ROWS - 1 && map[i + 1][j] != '#') {
                    graph.addEdge(currentNode, (i + 1) * COLS + j, weight(map[i][j]));
                }
                if (j < COLS - 1 && map[i][j + 1] != '#') {
                    graph.addEdge(currentNode, i * COLS + (j + 1), weight(map[i][j]));
                }
            }
        }
    }

}
int main() {

    char** map = readMapFromFile("D:\\Project\\Map.txt");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << map[i][j] << " ";
        }cout << endl;
    }

    bool chek = true;
    while (chek)
    {
        jump:
        Graph graph(ROWS * COLS);
        make_graph(graph, map);
        //graph.print();
        int playerLocation = 0;
        int crystalLocation = -1;
        // Find the player and crystal locations
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                if (map[i][j] == '*')
                    crystalLocation = i * COLS + j;
        string Name;
        AVLTree inventory;
        int choice = 0;
        Total_Score = 0;
        Jewel = 0;
        Potion = 0;
        Weapon = 0;
        system("cls");
        system("color 01");
        cout << "Welcome To The Quest for the Crystal Kingdom" << endl;
        cout << " Please Enter your Name : " << endl;
        cin >> Name;
        cout << " Please Enter your Choice : " << endl;
        cout << "1)Shortest Path " << endl;
        cout << "2)MST " << endl;
        cout << "3)Exit " << endl;
        cin >> choice;
        switch (choice)
        {
            case 1:
                cout << " Please Enter your Choice : " << endl;
                cout << "1)Shortest Path By Dijkstra " << endl;
                cout << "2)Shortest Path By Floyd " << endl;
                cin >> choice;
                    switch (choice)
                    {
                        case 1:
                            system("cls");
                            system("color 02");
                            cout << " Please Enter your Choice : " << endl;
                            cout << "1)Dijkstra with Default Location " << endl;
                            cout << "2)Dijkstra with Custom Location " << endl;
                            cin >> choice;
                            switch (choice)
                            {
                                case 1:
                                    
                                    cout << playerLocation << "  " << crystalLocation << endl;
                                    dijkstra(graph, playerLocation, crystalLocation, inventory, map);
                                    cout << "Inventory contents:" << std::endl;
                                    inventory.Count(inventory.root);
                                    cout << endl << endl << Name << " Your Total Score Is : " << Total_Score << endl;
                                    cout << " Jewel : " << Jewel << endl;
                                    cout << " Weapon : " << Weapon << endl;
                                    cout << " Potion : " << Potion << endl;
                                    cout << endl << endl << "Enter 1 If you want to discontinue the game " << endl;
                                    cin >> choice;
                                    if (choice == 1)
                                        exit(1);
                                    break;
                                case 2:
                                    int a = 0, b = 0,c=0,d=0;
                                    cout << " Please Enter Player Location : " << endl;
                                    cin >> a >> b;
                                    cout << " Please Enter Destination Location : " << endl;
                                    cin >> c >> d;
                                    cout << a * COLS + b << "  " << c * ROWS + d << endl;
                                    dijkstra(graph, a * COLS + b, c * ROWS + d, inventory, map);
                                    cout << "Inventory contents:" << std::endl;
                                    inventory.Count(inventory.root);
                                    cout << endl << endl << Name << " Your Total Score Is : " << Total_Score << endl;
                                    cout << " Jewel : " << Jewel << endl;
                                    cout << " Weapon : " << Weapon << endl;
                                    cout << " Potion : " << Potion << endl;
                                    cout << endl << endl << "Enter 1 If you want to discontinue the game " << endl;
                                    cin >> choice;
                                    if (choice == 1)
                                        exit(1);
                                    break;
                            }
                            break;
                        case 2:
                            system("cls");
                            system("color 03");
                            cout << " Please Enter your Choice : " << endl;
                            cout << "1)Floyd with Default Location " << endl;
                            cout << "2)Floyd with Custom Location " << endl;
                            cin >> choice;
                            switch (choice)
                            {
                                case 1:
                                    graph.floydWarshall();
                                    cout << playerLocation << "  " << crystalLocation << endl;
                                    graph.printShortestPath(playerLocation, crystalLocation,inventory,map);
                                    cout << "Inventory contents:" << std::endl;
                                    inventory.Count(inventory.root);
                                    cout << endl << endl << Name << " Your Total Score Is : " << Total_Score << endl;
                                    cout << " Jewel : " << Jewel << endl;
                                    cout << " Weapon : " << Weapon << endl;
                                    cout << " Potion : " << Potion << endl;
                                    cout << endl << endl << "Enter 1 If you want to discontinue the game " << endl;
                                    cin >> choice;
                                    if (choice == 1)
                                        exit(1);
                                    break;
                                case 2:
                                    int a = 0, b = 0, c = 0, d = 0;
                                    cout << " Please Enter Player Location : " << endl;
                                    cin >> a >> b;
                                    cout << " Please Enter Destination Location : " << endl;
                                    cin >> c >> d;
                                    graph.floydWarshall();
                                    graph.printShortestPath(a * COLS + b, c * ROWS + d, inventory, map);
                                    cout << "Inventory contents:" << std::endl;
                                    inventory.Count(inventory.root);
                                    cout << endl << endl << Name << " Your Total Score Is : " << Total_Score << endl;
                                    cout << " Jewel : " << Jewel << endl;
                                    cout << " Weapon : " << Weapon << endl;
                                    cout << " Potion : " << Potion << endl;
                                    cout << endl << endl << "Enter 1 If you want to discontinue the game " << endl;
                                    cin >> choice;
                                    if (choice == 1)
                                        exit(1);
                                    break;
                            }
                        break;
                    }
                break;
            case 2:
                system("cls");
                system("color 04");
                cout << " Please Enter your Choice : " << endl;
                cout << "1)MST by PRIMS Algorithm " << endl;
                cout << "2)MST by Kruskal's Algorithm " << endl;
                cin >> choice;
                switch (choice)
                {
                case 1:
                    
                    int a = 0, b = 0, c = 0, d = 0;
                    cout << " Please Enter Destination Location : " << endl;
                    cin >> a >> b;
                    graph.primMST();
                    cout << a * COLS + b << "  " << c * ROWS + d << endl;
                    graph.printPrimPath(0 , a * ROWS + b);
                    cout << endl << endl << "Enter -1 If you want to discontinue the game " << endl;
                    cin >> choice;
                    if (choice == 1)
                        exit(1);
                    break;
                }
        
                break;
            case 3:
                chek = false;
        }
    }
    return 0;
}
