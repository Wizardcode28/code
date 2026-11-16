/*
Sample inputs:
Source router for OSPF (0 for A, 1 for B, 2 for C, 3 for D): 0
*/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

const int INF = 9999;
const int N = 4;

void distance_vector_routing(const int cost[N][N]) {
    cout << "============================================================\n";
    cout << " 1. DISTANCE VECTOR ROUTING PROTOCOL (RIP - BELLMAN-FORD)\n";
    cout << "============================================================\n";

    int dist[N][N], next_hop[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            dist[i][j] = cost[i][j];
            next_hop[i][j] = (cost[i][j] != INF && i != j) ? j : -1;
        }
    }

    int iteration = 1;
    bool updated = true;
    while (updated) {
        updated = false;
        cout << "\n--- Iteration " << iteration++ << ": Exchanging Distance Vectors ---\n";
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next_hop[i][j] = next_hop[i][k];
                        updated = true;
                    }
                }
            }
        }
    }

    cout << "\n[Final Converged RIP Routing Tables]\n";
    for (int i = 0; i < N; i++) {
        cout << "\nRouting Table for Router '" << static_cast<char>('A' + i) << "':\n";
        cout << "  " << left << setw(15) << "Destination" << setw(10) << "Cost" << "Next Hop\n";
        cout << "  " << string(35, '-') << "\n";
        for (int j = 0; j < N; j++) {
            string hop = (next_hop[i][j] != -1) ? string(1, static_cast<char>('A' + next_hop[i][j])) : "Local";
            cout << "  " << left << setw(15) << static_cast<char>('A' + j) << setw(10) << dist[i][j] << hop << "\n";
        }
    }
}

void dijkstra_link_state(const int cost[N][N], int src) {
    cout << "\n============================================================\n";
    cout << " 2. LINK STATE ROUTING PROTOCOL (OSPF - DIJKSTRA) FROM '" << static_cast<char>('A' + src) << "'\n";
    cout << "============================================================\n";

    vector<int> dist(N);
    vector<int> prev(N, -1);
    vector<bool> visited(N, false);

    for (int i = 0; i < N; i++) {
        dist[i] = cost[src][i];
        if (cost[src][i] != INF && i != src) prev[i] = src;
    }
    visited[src] = true;

    for (int count = 0; count < N - 1; count++) {
        int min_d = INF, u = -1;
        for (int i = 0; i < N; i++) {
            if (!visited[i] && dist[i] < min_d) {
                min_d = dist[i];
                u = i;
            }
        }
        if (u == -1) break;
        visited[u] = true;
        cout << "Selected Router '" << static_cast<char>('A' + u) << "' with tentative cost " << min_d << "\n";

        for (int v = 0; v < N; v++) {
            if (!visited[v] && cost[u][v] != INF && dist[u] + cost[u][v] < dist[v]) {
                dist[v] = dist[u] + cost[u][v];
                prev[v] = u;
                cout << "   --> Relaxed edge (" << static_cast<char>('A' + u) << " -> "
                     << static_cast<char>('A' + v) << "): New lowest cost = " << dist[v] << "\n";
            }
        }
    }

    cout << "\n[Shortest Path Tree (SPT) Computed by OSPF for Router '" << static_cast<char>('A' + src) << "']\n";
    cout << "  " << left << setw(15) << "Destination" << setw(12) << "Total Cost" << "Complete Path\n";
    cout << "  " << string(45, '-') << "\n";

    for (int i = 0; i < N; i++) {
        string path = "";
        int curr = i;
        while (curr != -1) {
            path = static_cast<char>('A' + curr) + (path.empty() ? "" : " -> " + path);
            curr = (curr == src) ? -1 : prev[curr];
        }
        cout << "  " << left << setw(15) << static_cast<char>('A' + i) << setw(12) << dist[i] << path << "\n";
    }
}

int main() {
    int cost[N][N] = {
        {0,   1,   4,   INF},
        {1,   0,   2,   7},
        {4,   2,   0,   1},
        {INF, 7,   1,   0}
    };

    distance_vector_routing(cost);

    int src = 0;
    string line;
    cout << "\nEnter source router for OSPF (0:A, 1:B, 2:C, 3:D) [default 0]: ";
    if (getline(cin, line) && !line.empty()) {
        try { src = stoi(line); } catch (...) { src = 0; }
    }
    if (src < 0 || src >= N) src = 0;

    dijkstra_link_state(cost, src);
    return 0;
}

/*
================================================================================
LAB EXPLANATION:
Lab 9 covers interior dynamic routing algorithms used at the Network layer:
Distance Vector Routing (RIP) and Link State Routing (OSPF).

1. Distance Vector Routing (RIP / Bellman-Ford):
   - Routers share their distance vector with immediate neighbors.
   - Updates via Bellman-Ford equation: D_x(y) = min_v { c(x,v) + D_v(y) }.
   - Converges iteratively across iterations.

2. Link State Routing (OSPF / Dijkstra):
   - Routers flood link state advertisements to build an identical network topology graph.
   - Computes shortest paths using Dijkstra's greedy algorithm.
   - Fast convergence, loop-free shortest path tree.

CODE EXPLANATION:
- 'distance_vector_routing()': Iterates Bellman-Ford relaxation until table convergence.
- 'dijkstra_link_state()': Computes shortest path tree and reconstructs the full hop-by-hop path.
================================================================================
*/
