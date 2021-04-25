struct Edge {
	int u, v;
	ll w;
	Edge(int u, int v, ll w = 1) : u(u), v(v), w(w){}
	bool operator<(const Edge &other) {
		if(w == other.w){
			pii p1 = {u, v}, p2 = {other.u, other.v};
			return p1 < p2;
		}
		else return w < other.w;
	}
};

struct Graph {
	int n;
	vector<vector<Edge>> edges;
	const ll INF = 1e15;
	Graph(int n) : n(n), edges(n + 1) {}

	void addEdgeDirected(int u, int v, int w = 1) {
		edges[u].push_back(Edge(u, v, w));
	}

	void addEdgeUndirected(int u, int v, int w = 1) {
		edges[u].push_back(Edge(u, v, w));
		edges[v].push_back(Edge(v, u, w));
	}

	vll dijkstra(int src) {
		vll dist = vll (n + 1, INF);
		set<pll> st;
		dist[src] = 0;
		st.insert({ 0, src });

		while (st.size() > 0) {
			int curNode = st.begin()->second;
			ll curDist = st.begin()->first;
			st.erase(st.begin());

			if (dist[curNode] != curDist) continue;
			for (auto it : edges[curNode]) {
				ll temp = curDist + it.w;
				if (temp < dist[it.v]) {
					dist[it.v] = temp;
					st.insert({ temp, it.v });
				}
			}
		}

		return dist;
	}

	vvl floydWarshall(){
		vvl dist(n + 1, vll(n + 1, INF));
		for(int i = 1; i <= n; i++){
			dist[i][i] = 0;
			for(auto &it: edges[i]){
				dist[i][it.v] = min(dist[i][it.v], it.w);
			}
		}
		for(int k = 1; k <= n; k++){
			for(int i = 1; i <= n; i++){
				for(int j = 1; j <= n; j++){
					dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
				}
			}
		}
		return dist;
	}

	vi toposort() {
		vi ins(n+1,0), res;
		FORI(i,n) FOREACH(it,edges[i]) ++ins[it->v];
		deque<int> q; FORI(i,n) if (ins[i] == 0) q.pb(i);
		while (!q.empty()) {
			int v = q.front(); q.pop_front();
			FOREACH(it,edges[v]) if ((--ins[it->v]) == 0) q.pb(it->v);
			res.pb(v);
		}
		return SZ(res)==n ? res : vi();
	}
};