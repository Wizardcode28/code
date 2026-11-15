/*
Sample inputs:
Source Router index for OSPF (0 for A, 1 for B, 2 for C, 3 for D): 0
*/

#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int INF = 9999;
const int N = 4;

void distance_vector(const int cost[N][N]) {
    int dist[N][N], next_hop[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            dist[i][j] = cost[i][j];
            next_hop[i][j] = (cost[i][j] != INF && i != j) ? j : -1;
        }
    }

    bool updated = true;
    while (updated) {
        updated = false;
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

    cout << "\n--- 1. Distance Vector Routing (RIP) ---\n";
    for (int i = 0; i < N; i++) {
        cout << "Routing Table for Router " << static_cast<char>('A' + i) << ":\n";
        for (int j = 0; j < N; j++) {
            if (next_hop[i][j] != -1)
                cout << "  To " << static_cast<char>('A' + j) << ": Cost = " << dist[i][j] << ", Next Hop = " << static_cast<char>('A' + next_hop[i][j]) << "\n";
            else
                cout << "  To " << static_cast<char>('A' + j) << ": Cost = " << dist[i][j] << ", Next Hop = None\n";
        }
    }
}

void dijkstra(const int cost[N][N], int src) {
    vector<int> dist(N);
    vector<bool> visited(N, false);
    for (int i = 0; i < N; i++) dist[i] = cost[src][i];
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

        for (int v = 0; v < N; v++) {
            if (!visited[v] && cost[u][v] != INF && dist[u] + cost[u][v] < dist[v]) {
                dist[v] = dist[u] + cost[u][v];
            }
        }
    }

    cout << "\n--- 2. Link State / Dijkstra (OSPF) from Router " << static_cast<char>('A' + src) << " ---\n";
    for (int i = 0; i < N; i++) {
        cout << "  To " << static_cast<char>('A' + i) << ": Shortest Cost = " << dist[i] << "\n";
    }
}

int main() {
    int cost[N][N] = {
        {0,   1,   4,   INF},
        {1,   0,   2,   7},
        {4,   2,   0,   1},
        {INF, 7,   1,   0}
    };

    distance_vector(cost);

    int src = 0;
    cout << "\nEnter source router for OSPF (0:A, 1:B, 2:C, 3:D) [default 0]: ";
    string line;
    if (getline(cin, line) && !line.empty()) {
        try { src = stoi(line); } catch (...) { src = 0; }
    }
    if (src < 0 || src >= N) src = 0;

    dijkstra(cost, src);
    return 0;
}
