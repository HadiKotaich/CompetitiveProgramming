struct Graph {
	struct Edge {
		int id;
		int dest;
		ll weight;

		Edge(int _dest = -1, ll _weight = 1, int _id = 0) {
			dest = _dest;
			weight = _weight;
			id = _id;
		}

		bool operator<(const Edge& other) const {
			return dest < other.dest;
		}
	};

	int n;
	vector<vector<Edge>> adj;
	vll dist;
	ll INF = (ll)1e15;

	Graph(int nodes = 0) {
		n = nodes;
		adj.resize(n + 1);
	}

	void addEdgeUndirected(int u, int v, ll w = 1, int id = 0) {
		adj[u].push_back(Edge(v, w, id));
		adj[v].push_back(Edge(u, w, id));
	}

	void addEdgeDirected(int u, int v, ll w = 1) {
		adj[u].push_back(Edge(v, w));
	}
	
	
	void reverseEdges() {
		vector<vector<Edge>> adjReversed(n + 1);
		for (int i = 1; i <= n; i++) {
			for (auto it : adj[i]) {
				adjReversed[it.dest].push_back(Edge(i, it.weight, it.id));
			}
		}
		adj = adjReversed;
	}

	void dijkstra(int src) {
		dist = vll (n + 1, INF);
		set<pll> st;
		dist[src] = 0;
		st.insert({ 0, src });

		while (st.size() > 0) {
			int curNode = st.begin()->second;
			ll curDist = st.begin()->first;
			st.erase(st.begin());

			if (dist[curNode] != curDist) continue;
			for (auto it : adj[curNode]) {
				ll temp = curDist + it.weight;
				if (temp < dist[it.dest]) {
					dist[it.dest] = temp;
					st.insert({ temp, it.dest });
				}
			}
		}
	}

	pair<Tree, ll> MST() {
		// returns Tree, Cost
		ll cost = 0;
		Tree mst(n + 1);
		vector<bool> done(n + 1, 0);
		priority_queue<pair<ll, pii> > q;
		q.push({ 0, {-1, 1} }); // minus cost, from, to
		while (!q.empty()) {
			int v = q.top().se.se, d = -q.top().fi, from = q.top().se.fi;

			q.pop(); 
			if (done[v]) continue; 
			done[v] = 1; 
			cost += d;
			if (from != -1) 
				mst.addEdgeUndirected(from, v, d);
			for(auto it: adj[v]) {
				if (!done[it.dest]) {
					q.push({ -it.weight, {v, it.dest} });
				}
			}
		}
		return { mst, cost };
	}
};
