struct Edge {
	int u, v;
	ll w;
	Edge(int u, int v, ll w = 1) : u(u), v(v), w(w){}
};

struct Tree {
	int n, root;
	vector<vector<Edge>> edges;
	vi sz, dead;

	Tree(int n , int root = 1) :
		n(n), root(root), edges(n + 1), sz(n + 1), dead(n + 1){}

	void addEdgeDirected(int u, int v, int w = 1) {
		edges[u].push_back(Edge(u, v, w));
	}

	void addEdgeUndirected(int u, int v, int w = 1) {
		edges[u].push_back(Edge(u, v, w));
		edges[v].push_back(Edge(v, u, w));
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