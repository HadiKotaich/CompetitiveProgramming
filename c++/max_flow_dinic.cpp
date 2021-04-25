// Adjacency list implementation of Dinic’s blocking flow algorithm.
// This is very fast in practice, and only loses to push-relabel flow.
//
// Running time:
// O(|V|ˆ2 |E|)
//
// INPUT:
// - graph, constructed using AddEdge()
// - source and sink
//
// OUTPUT:
// - maximum flow value
// - To obtain actual flow values, look at edges with capacity > 0
// (zero capacity edges are residual edges).
struct Edge {
    int u, v;
    ll cap, flow;
    int id;
    Edge() {}
    Edge(int u, int v, ll cap, int id = 0) : u(u), v(v), cap(cap), flow(0), id(id) {}
};
struct Dinic {
    // 0 based
    int N;
    vector<Edge> E;
    vector<vector<int>> g;
    vector<int> d, pt;
    Dinic(int N) : N(N), E(0), g(N), d(N), pt(N) {}
    void AddEdge(int u, int v, ll capUV, ll capVU = 0, int id = 0) {
        if (u != v) {
            E.emplace_back(Edge(u, v, capUV, id));
            g[u].emplace_back(E.size() - 1);
            E.emplace_back(Edge(v, u, capVU, id));
            g[v].emplace_back(E.size() - 1);
        }
    }
    bool BFS(int S, int T) {
        queue<int> q({ S });
        fill(d.begin(), d.end(), N + 1);
        d[S] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (u == T) break;
            for (int k : g[u]) {
                Edge& e = E[k];
                if (e.flow < e.cap && d[e.v] > d[e.u] + 1) {
                    d[e.v] = d[e.u] + 1;
                    q.emplace(e.v);
                }
            }
        }
        return d[T] != N + 1;
    }
    ll DFS(int u, int T, ll flow = -1) {
        if (u == T || flow == 0) return flow;
        for (int& i = pt[u]; i < g[u].size(); ++i) {
            Edge& e = E[g[u][i]];
            Edge& oe = E[g[u][i] ^ 1];
            if (d[e.v] == d[e.u] + 1) {
                ll amt = e.cap - e.flow;
                if (flow != -1 && amt > flow) amt = flow;
                if (ll pushed = DFS(e.v, T, amt)) {
                    e.flow += pushed;
                    oe.flow -= pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }
    ll MaxFlow(int S, int T) {
        ll total = 0;
        while (BFS(S, T)) {
            fill(pt.begin(), pt.end(), 0);
            while (ll flow = DFS(S, T))
                total += flow;
        }
        return total;
    }
};


/*
this gets the edges of the cuts, problem B moscow day 4 (thematic max flow contest)

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    
    int n, m; cin >> n >> m;
    Dinic dinic(n);
    map<int, pii> myMap;
    for (int i = 0; i < m; i++) {
        int u, v, w; cin >> u >> v >> w;
        dinic.AddEdge(u - 1, v - 1, w, w, i);
    }
    int flow =  dinic.MaxFlow(0, n - 1);
    set<int> ans;
    for (auto it : dinic.E) {
        if (dinic.d[it.u] == n + 1 && dinic.d[it.v] != n + 1 || dinic.d[it.v] == n + 1 && dinic.d[it.u] != n + 1) {
            ans.insert(it.id + 1);
        }
    }
    cout << ans.size() << " " << flow << "\n";
    for (auto it : ans) cout << it << " ";
    cout << "\n";
}

*/