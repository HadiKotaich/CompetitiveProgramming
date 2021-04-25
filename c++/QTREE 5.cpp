/* 
You are given a tree (an acyclic undirected connected graph) with N nodes. 
The tree nodes are numbered from 1 to N. We define dist(a, b) as the number of edges on the path from node a to node b.
Each node has a color, white or black. All the nodes are black initially.
We will ask you to perfrom some instructions of the following form:
0 i : change the color of i-th node(from black to white, or from white to black).
1 v : ask for the minimum dist(u, v), node u must be white(u can be equal to v). 
Obviously, as long as node v is white, the result will always be 0.
*/

#define LOGN 18 // 2 ^ 18 = 262144
#define INF 1e9

struct Edge {
	int u, v;
	ll w;
	Edge(int u, int v, ll w = 1) : u(u), v(v), w(w){}
};

struct Tree {
	int n, root;
	vector<vector<Edge>> edges;
	vi sz, dead, parent;
	vll dist;

	Tree(int n , int root = 1) :
		n(n), root(root), edges(n + 1), sz(n + 1), dead(n + 1),
		parent(n + 1), dist(n + 1, INF) {}

	void addEdgeDirected(int u, int v, int w = 1) {
		edges[u].push_back(Edge(u, v, w));
	}

	void addEdgeUndirected(int u, int v, int w = 1) {
		edges[u].push_back(Edge(u, v, w));
		edges[v].push_back(Edge(v, u, w));
	}

	void DFS(int u, int par = -1){
		parent[u] = par;
		if(par == -1) dist[u] = 0;
		for(auto &edge: edges[u]){
			if(edge.v == par) continue;
			dist[edge.v] = dist[u] + edge.w;
			DFS(edge.v, u);
		}
	}
	void initLCA() {
		DFS(root, -1); // get parents
		height = vi(n + 1, - 1);
		anc = vvi(LOGN + 1, vi(n + 1, 0));
		FORI(i, n) anc[0][i] = parent[i];
		height[root] = 0;
		anc[0][root] = root;
		for (int i = 1; i <= n; i++) {
			if (height[i] == -1) getHeight(i);
		}
		for (int i = 1; i <= LOGN; i++) {
			for (int j = 1; j <= n; j++) {
				anc[i][j] = anc[i - 1][anc[i - 1][j]];
			}
		}
	}
	int lca(int u, int v) {
		if (height[u] < height[v]) swap(u, v);
		int diff = height[u] - height[v];
		REPD(k, LOGN, 0) if ((1 << k) <= diff) {
			diff -= 1 << k;
			u = anc[k][u];
		}
		if (u == v) return u;
		REPD(k, LOGN, 0) if (anc[k][u] != anc[k][v]) {
			u = anc[k][u];
			v = anc[k][v];
		}
		return parent[u];
	}
	ll distBetween(int u, int v) {
		int lcaNode = lca(u, v);
		ll ans = dist[u] + dist[v] - 2 * dist[lcaNode];
		return ans;
	}

	Tree CentroidDecomposition() {
		Tree CDTree(n);
		fill(all(dead), 0);
		fill(all(sz), 0);

		function<void(int, int)> rec = [&](int parent, int start) {
			const int centroid = OneCentroid(start);

			// insert code here

			if (parent != -1) {
				CDTree.addEdgeUndirected(parent, centroid);
			}
			else {
				CDTree.root = centroid;
			}
			dead[centroid] = true;
			for (auto it : edges[centroid]) if (!dead[it.v]) rec(centroid, it.v);
			dead[centroid] = false;
		};
		rec(-1, edges[0].empty() /* root: 0 or 1 */);
		return CDTree;
	}
private:
	vi height;
	vvi anc;
	int getHeight(int u) {
		if (height[u] == -1) height[u] = getHeight(parent[u]) + 1;
		return height[u];
	}

	int OneCentroid(const int root) {
		function<void(int, int)> get_sz = [&](int u, int prev) {
			sz[u] = 1;
			for (auto& it : edges[u]) {
				if (it.v != prev && !dead[it.v]) {
					get_sz(it.v, u);
					sz[u] += sz[it.v];
				}
			}
		};
		get_sz(root, -1);
		function<int(int, int)> dfs = [&](int u, int prev) {
			for (auto& it : edges[u]) {
				if (it.v != prev && !dead[it.v]) {
					if (sz[it.v] > sz[root] / 2) {
						return dfs(it.v, u);
					}
				}
			}
			return u;
		};
		return dfs(root, -1);
	}
};

void update(int u, Tree &cdtree, Tree &tree, vi &color, vector<multiset<ll>> &sets){
	int v = u;
	while(v != -1){
		if(color[u] == 0){
			sets[v].insert(tree.distBetween(u, v));
		}
		else{
			sets[v].erase(sets[v].find(tree.distBetween(u, v)));
		}
		v = cdtree.parent[v];
	}
	color[u] = 1 - color[u];
}

ll get(int u, Tree &cdtree, Tree &tree, vector<multiset<ll>> &sets){
	ll ans = INF;
	int v = u;
	while(v != -1){
		if(SZ(sets[v])){
			ll cur = *sets[v].begin() + tree.distBetween(u, v);
			ans = min(ans, cur);
		}
		v = cdtree.parent[v];
	}
	return (ans == INF ? -1 : ans);
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

	int n; cin>>n;
	Tree tree(n);
	for(int i = 0; i < n - 1; i++){
		int u, v; cin>>u>>v; tree.addEdgeUndirected(u, v);
	}

	vi color(n + 1, 0);
	vector<multiset<ll>> sets(n + 1);
	Tree CDtree = tree.CentroidDecomposition();
	CDtree.DFS(CDtree.root);
	tree.initLCA();
	int q; cin>>q;
	for(int i = 0; i < q; i++){
		int tp, v; cin>>tp>>v;
		if(tp == 0){
			update(v, CDtree, tree, color, sets);
		}
		else{
			cout<<get(v, CDtree, tree, sets)<<"\n";
		}
	}

}
