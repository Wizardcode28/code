# Sample inputs:
# Source router for OSPF (e.g. A, B, C, D): A

INF = float("inf")

def distance_vector(graph):
    nodes = list(graph)
    dist = {u: {v: (0 if u == v else graph[u].get(v, INF)) for v in nodes} for u in nodes}
    next_hop = {u: {v: (v if v in graph[u] else None) for v in nodes} for u in nodes}

    updated = True
    while updated:
        updated = False
        for u in nodes:
            for neighbor, link_cost in graph[u].items():
                for dest in nodes:
                    new_cost = link_cost + dist[neighbor][dest]
                    if new_cost < dist[u][dest]:
                        dist[u][dest] = new_cost
                        next_hop[u][dest] = neighbor
                        updated = True
    return dist, next_hop

def dijkstra(graph, start):
    import heapq
    dist = {node: float("inf") for node in graph}
    prev = {node: None for node in graph}
    dist[start] = 0
    pq = [(0, start)]

    while pq:
        d, u = heapq.heappop(pq)
        if d > dist[u]:
            continue
        for v, weight in graph[u].items():
            if dist[u] + weight < dist[v]:
                dist[v] = dist[u] + weight
                prev[v] = u
                heapq.heappush(pq, (dist[v], v))
    return dist, prev

def get_path(prev, dest):
    path = []
    curr = dest
    while curr is not None:
        path.append(curr)
        curr = prev[curr]
    return " -> ".join(reversed(path))

if __name__ == "__main__":
    net = {
        "A": {"B": 1, "C": 4},
        "B": {"A": 1, "C": 2, "D": 7},
        "C": {"A": 4, "B": 2, "D": 1},
        "D": {"B": 7, "C": 1},
    }

    print("--- 1. Distance Vector Routing (RIP) ---")
    distances, hops = distance_vector(net)
    for node in net:
        print(f"Routing Table for Router {node}:")
        for dest in net:
            print(f"  Destination {dest} -> Cost: {distances[node][dest]}, Next Hop: {hops[node][dest]}")

    print("\n--- 2. Link State Routing / Dijkstra (OSPF) ---")
    src = input("Enter source router for OSPF (A/B/C/D) [default: A]: ").strip().upper() or "A"
    if src not in net:
        src = "A"
    dist, prev = dijkstra(net, src)
    print(f"Shortest Paths from Router {src}:")
    for dest in net:
        print(f"  To {dest}: Cost = {dist[dest]}, Path: {get_path(prev, dest)}")
