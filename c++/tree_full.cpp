#define LOGN 18 // 2 ^ 18 = 262144, works for n <= 5 * 1e5
#define INF 1e9

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

struct Tree {
	int n, root;
	vector<vector<Edge>> edges;
	vi height, parent; vll dist;
	vi start, finish; int t; // for subtree queries
	vvi anc; vvl lift; ll defaultVal; // initialized in initLCA

	Tree(int n, int root = 1) : n(n), root(root), edges(n + 1), height(n + 1), parent(n + 1), dist(n + 1), start(n + 1), finish(n + 1), t(-1) {}

	void addEdgeDirected(int u, int v, int w = 1) {
		edges[u].push_back(Edge(u, v, w));
	}

	void addEdgeUndirected(int u, int v, int w = 1) {
		edges[u].push_back(Edge(u, v, w));
		edges[v].push_back(Edge(v, u, w));
	}

	void DFS(int u, int par = -1){
		if(par == -1) dist[u] = 0;
		parent[u] = par; start[u] = ++t;
		height[u] = (par == -1 ? 0 : height[par] + 1);
		for(auto &edge: edges[u]){
			int v = edge.v;
			if(v == par) continue;
			dist[v] = dist[u] + edge.w;
			DFS(v, u);
		}
		finish[u] = t;
	}

	int diameter() {
		int src1 = 1, src2 = -1, diam = 0;
		DFS(src1);
		auto farthest = [&](vll &dist){
			auto mx = max_element(dist.begin() + 1, dist.end());
			diam = *mx;
			return distance(dist.begin(), mx);
		};
		src2 = farthest(dist);
		DFS(src2);
		src1 = farthest(dist);
		return diam;
	}

	inline ll lcaUpdate(ll u, ll v){
		return min(u, v);
	}

	void initLCA(ll defVal = 0) {
		defaultVal = defVal;
		DFS(root, -1);
		anc = vvi(LOGN + 1, vi(n + 1, 0));
		lift = vvl(LOGN + 1, vll(n + 1, defaultVal));
		for(int i = 1; i <= n; i++) {
			anc[0][i] = (i == root ? root : parent[i]);
			lift[0][i] = (i == root ? defaultVal : dist[i] - dist[parent[i]]);
		}
		for (int i = 1; i <= LOGN; i++) {
			for (int j = 1; j <= n; j++) {
				anc[i][j] = anc[i - 1][anc[i - 1][j]];
				lift[i][j] = lcaUpdate(lift[i - 1][j], lift[i - 1][anc[i - 1][j]]);
			}
		}
	}

	int lca(int u, int v) {return query(u, v).first;}

	pair<int, ll> query(int u, int v) {
		if (height[u] < height[v]) swap(u, v);
		int diff = height[u] - height[v];
		ll val = defaultVal;
		REPD(k, LOGN, 0) if ((1 << k) <= diff) {
			diff -= 1 << k;
			val = lcaUpdate(val, lift[k][u]);
			u = anc[k][u];
		}
		if (u == v) return {u, val};
		REPD(k, LOGN, 0) if (anc[k][u] != anc[k][v]) {
			val = lcaUpdate(val, lift[k][u]);
			u = anc[k][u];
			val = lcaUpdate(val, lift[k][v]);
			v = anc[k][v];
		}
		val = lcaUpdate(val, lcaUpdate(lift[0][u], lift[0][v]));
		return {parent[u], val};
	}

	int kthParent(int u, int k) { //0 is self
		bitset<21> bitmask(k);
		int ans = u;
		for (int i = 0; i < 21; i++) if (bitmask[i]) ans = anc[i][ans];
		return ans;
	}
};


/*
int findCenter(vvi &adj){
	vi deg(adj.size());
	for(int i = 0; i < adj.size(); i++){
		deg[i] = adj[i].size();
	}
	int rem = adj.size();
	queue<int> leafs;
	for(int i = 0; i < adj.size(); i++) if(deg[i] == 1) leafs.push(i);
	while(rem > 2){
		int u = leafs.front(); leafs.pop(); rem--;
		deg[u] = 0;
		for(auto it: adj[u]) {
			deg[it]--;
			if(deg[it] == 1) leafs.push(it);
		}
	}
	for(int i = 0; i < adj.size(); i++) if(deg[i] == 1) return i;
	return -1;
}
*/