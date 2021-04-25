struct bridgeGraph {
	struct Edge { // PRE: has to be bcc = -1 and bridge = 0 (constructors sets this)
		Edge* rev;
		int dest;
		int bcc; // OUT: number of biconnected Component
		bool bridge; // OUT: is it a bridge // B
		Edge(int v) : dest(v), bcc(-1), bridge(0) // B
		{};
	};
	int n; // IN: number of vertices
	vector<list<Edge>> adj; // IN: adjacency lists
	vi visit;
	int dfsTime;
	bridgeGraph(int _n) {
		n = _n;
		adj.resize(n + 1);
		visit.resize(n + 1);
		dfsTime = 1;
	}
	void add_edge(int a, int b) {
		adj[a].pb(Edge(b)); adj[b].pb(Edge(a));
		adj[a].back().rev = &adj[b].back();
		adj[b].back().rev = &adj[a].back();
	}
	int bccDFS(int v, bool root = 0) {
		int lo = visit[v] = ++dfsTime;
		FOREACH(it, adj[v]) {
			if (it->bcc != -1) continue;
			it->rev->bcc = -2;
			if (!visit[it->dest]) {
				int ulo = bccDFS(it->dest);
				lo = min(ulo, lo);
				it->bridge = it->rev->bridge = (ulo > visit[v]); // B
			}
			else lo = min(lo, visit[it->dest]);
		}
		return lo;
	}
	void computeBCC() {
		visit = vi(n + 1, 0);
		dfsTime = 1;
		FORI(i, n) if (!visit[i]) bccDFS(i, 1); // maybe change FORI to FOR
	}
};