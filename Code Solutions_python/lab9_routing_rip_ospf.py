# Sample inputs:
# Source router for OSPF / Dijkstra (e.g., A, B, C, D): A

import heapq

INF = float("inf")

# -------------------------------------------------------------
# 1. DISTANCE VECTOR ROUTING (RIP / BELLMAN-FORD)
# -------------------------------------------------------------
def distance_vector_routing(graph):
    print("=" * 60)
    print(" 1. DISTANCE VECTOR ROUTING PROTOCOL (RIP - BELLMAN-FORD)")
    print("=" * 60)
    nodes = sorted(list(graph.keys()))
    
    # Initialize Distance Tables: dist[u][v] = direct cost or INF
    dist = {u: {v: (0 if u == v else graph[u].get(v, INF)) for v in nodes} for u in nodes}
    next_hop = {u: {v: (v if v in graph[u] else None) for v in nodes} for u in nodes}

    iteration = 1
    updated = True
    while updated:
        updated = False
        print(f"\n--- Iteration {iteration}: Exchanging Distance Vectors with Neighbors ---")
        for u in nodes:
            for neighbor, link_cost in graph[u].items():
                for dest in nodes:
                    # Bellman-Ford equation: D_x(y) = min_v { c(x,v) + D_v(y) }
                    candidate_cost = link_cost + dist[neighbor][dest]
                    if candidate_cost < dist[u][dest]:
                        dist[u][dest] = candidate_cost
                        next_hop[u][dest] = neighbor
                        updated = True
        iteration += 1

    print("\n[Final Converged RIP Routing Tables]")
    for u in nodes:
        print(f"\nRouting Table for Router '{u}':")
        print(f"  {'Destination':<15} {'Cost':<10} {'Next Hop'}")
        print("  " + "-" * 35)
        for dest in nodes:
            hop = next_hop[u][dest] if next_hop[u][dest] is not None else "Local"
            print(f"  {dest:<15} {dist[u][dest]:<10} {hop}")
    return dist, next_hop

# -------------------------------------------------------------
# 2. LINK STATE ROUTING (OSPF / DIJKSTRA'S ALGORITHM)
# -------------------------------------------------------------
def dijkstra_link_state(graph, source):
    print("\n" + "=" * 60)
    print(f" 2. LINK STATE ROUTING PROTOCOL (OSPF - DIJKSTRA) FROM '{source}'")
    print("=" * 60)
    
    distances = {node: INF for node in graph}
    previous = {node: None for node in graph}
    distances[source] = 0
    
    # Priority queue stores (cost, current_node)
    pq = [(0, source)]
    visited = set()

    step = 1
    while pq:
        curr_dist, u = heapq.heappop(pq)
        if u in visited:
            continue
        visited.add(u)
        print(f"Step {step}: Selected Router '{u}' with minimum tentative cost {curr_dist}")
        step += 1

        for neighbor, weight in graph[u].items():
            if neighbor not in visited:
                new_dist = curr_dist + weight
                if new_dist < distances[neighbor]:
                    distances[neighbor] = new_dist
                    previous[neighbor] = u
                    heapq.heappush(pq, (new_dist, neighbor))
                    print(f"   --> Relaxed edge ({u} -> {neighbor}): New lowest cost = {new_dist}")

    return distances, previous

def reconstruct_path(previous, dest):
    path = []
    curr = dest
    while curr is not None:
        path.append(curr)
        curr = previous[curr]
    return " -> ".join(reversed(path))

if __name__ == "__main__":
    # Topology graph: Adjacency list with non-negative edge costs
    network_topology = {
        "A": {"B": 1, "C": 4},
        "B": {"A": 1, "C": 2, "D": 7},
        "C": {"A": 4, "B": 2, "D": 1},
        "D": {"B": 7, "C": 1},
    }

    # Run Distance Vector (RIP)
    distance_vector_routing(network_topology)

    # Run Link State (OSPF)
    src_input = input("\nEnter source router for OSPF (A, B, C, D) [default: A]: ").strip().upper() or "A"
    if src_input not in network_topology:
        src_input = "A"

    costs, prev_nodes = dijkstra_link_state(network_topology, src_input)
    print(f"\n[Shortest Path Tree (SPT) Computed by OSPF for Router '{src_input}']")
    print(f"  {'Destination':<15} {'Total Cost':<12} {'Complete Shortest Path'}")
    print("  " + "-" * 50)
    for dest in sorted(network_topology.keys()):
        path_str = reconstruct_path(prev_nodes, dest)
        print(f"  {dest:<15} {costs[dest]:<12} {path_str}")

"""
================================================================================
LAB EXPLANATION:
Lab 9 covers interior dynamic routing algorithms used at the Network layer:
Distance Vector Routing (Routing Information Protocol - RIP) and Link State Routing
(Open Shortest Path First - OSPF).

1. Distance Vector Routing (RIP / Bellman-Ford):
   - Principle: "Tell your neighbors about the entire world."
   - Each router maintains a vector of distances to all known destinations and periodically
     shares this vector only with directly connected immediate neighbors.
   - Updates routing table via the Bellman-Ford recurrence:
     D_x(y) = min_v { cost(x,v) + D_v(y) }
   - Characteristics: Simple computation, but slow convergence and susceptible to the
     "Count-to-Infinity" problem upon link failures.

2. Link State Routing (OSPF / Dijkstra's Algorithm):
   - Principle: "Tell the entire world about your immediate neighbors."
   - Each router floods Link-State Advertisements (LSAs) containing the status and cost of its
     directly connected links to all routers in the autonomous system (AS).
   - Every router reconstructs an identical, complete map (topology graph) of the network.
   - Each router runs Dijkstra's Shortest Path First (SPF) algorithm independently using a
     greedy priority queue, computing the shortest path tree (SPT) rooted at itself.
   - Characteristics: Fast convergence, loop-free routing, but requires higher memory and CPU.

CODE EXPLANATION:
- 'distance_vector_routing(graph)': Initializes distance matrix, iterates Bellman-Ford relaxations
  until distance values stabilize (convergence), and prints routing tables showing next hops.
- 'dijkstra_link_state(graph, source)': Implements Dijkstra's algorithm using a min-heap priority queue
  ('heapq') to extract unvisited nodes with minimum distance, relax adjacent edges, and track predecessors.
- 'reconstruct_path(previous, dest)': Backtracks predecessor pointers to print complete end-to-end paths.
================================================================================
"""
