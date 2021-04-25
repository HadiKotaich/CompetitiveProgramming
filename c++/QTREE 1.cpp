/*
You are given a tree (an acyclic undirected connected graph) with N nodes, and edges numbered 1, 2, 3...N-1.

We will ask you to perfrom some instructions of the following form:

CHANGE i ti : change the cost of the i-th edge to ti
or
QUERY a b : ask for the maximum edge cost on the path from node a to node b

*/


struct StVal {
	ll v = 0; StVal() {}
	StVal(const ll _v) : v(_v) {}
	StVal(const StVal& v1, const StVal& v2) { v = max(v1, v2); }
	operator ll() const { return v; }
};
struct MyST {
	int n;
	vector<StVal> st;
	vector<int> leaves;

	MyST(const int n = 1) : n(n) {
		leaves.resize(n);
		init();
	}

	void init() { init(1, 0, n - 1); }

	void init(const int si, const int lo, const int hi) {
		if (lo == hi) {
			if (si >= (int)st.size()) st.resize(si + 1);
			st[si] = StVal();
			leaves[lo] = si;
		}
		else {
			const int mid = (lo + hi) >> 1;
			init(si << 1, lo, mid);
			init(si << 1 | 1, mid + 1, hi);
			st[si] = StVal(st[si << 1], st[si << 1 | 1]);
		}
	}

	StVal query(const int l, const int r) { return (l <= r && l < n && r >= 0) ? query(l, r, 1, 0, n - 1) : StVal(); }
	StVal query(const int l, const int r, const int si, const int lo, const int hi) {
		if (l <= lo && hi <= r) return st[si];

		const int mid = (lo + hi) >> 1;
		if (r <= mid) return query(l, r, si << 1, lo, mid);
		if (mid < l) return query(l, r, si << 1 | 1, mid + 1, hi);
		return StVal(query(l, r, si << 1, lo, mid), query(l, r, si << 1 | 1, mid + 1, hi));
	}

	void update(const int i, int c) {
		int si = leaves[i];
		st[si] = StVal(c);
		for (si >>= 1; si; si >>= 1)
			st[si] = StVal(st[si << 1], st[si << 1 | 1]);
	}
};
struct HeavyLight {
	// 0 based
	struct edge {
		int dest, id;
		ll w;
		edge(int _dest = -1, ll _w = 1, int _id = -1) {
			dest = _dest;
			w = _w;
			id = _id;
		}
	};
	const int isValueOnEdge = 1;  // switch to 0 for value on node
	vector<vector<edge>> adj;

	// heavy[cur] is the heavy node chosen for cur
	// root[cur] is the root of the chain of cur
	// segTreePos[cur] is the pos of cur in the segment tree
	// edges[id] is the one Ditrected edge based on the bigRoot
	vi parent, depth, heavy, root, segTreePos;
	vector<edge> edges;

	int bigRoot, n;
	ll queryRes;
	MyST segTree;

	HeavyLight(int _n = 0, int _root = 0) {
		n = _n;
		adj.resize(n);
		parent.resize(n, -1);
		depth.resize(n, -1);
		heavy.resize(n, -1);
		root.resize(n, -1);
		segTreePos.resize(n, -1);
		edges.resize(n - 1);
		bigRoot = 0;
		segTree = MyST(n);
	}
	void init() {
		buildChains();
		for (auto it : edges) {
			updatePos(it.dest, it.w);
		}
	}

	void addEdgeUndirected(int u, int v, ll w, int id) {
		adj[u].push_back(edge(v, w, id));
		adj[v].push_back(edge(u, w, id));
	}

	void addEdgeDirected(int u, int v, ll w, int id) {
		adj[u].push_back(edge(v, w, id));
	}

	int dfs_hld(int v) {
		int size = 1, maxSubtree = 0;
		for (auto it : adj[v]) {
			int u = it.dest, edgeIdx = it.id;
			if (u != parent[v]) {
				edges[edgeIdx] = it;
				parent[u] = v, depth[u] = depth[v] + 1;
				int childTreeSize = dfs_hld(u);
				if (childTreeSize > maxSubtree)
					heavy[v] = u, maxSubtree = childTreeSize;
				size += childTreeSize;
			}
		}
		return size;
	}
	void buildChains() {
		heavy = vi(n, -1);
		parent[bigRoot] = -1, depth[bigRoot] = 0;
		dfs_hld(bigRoot);

		// Connect chains to its root. Map chain to segment tree part
		for (int chainRoot = 0, pos = 0; chainRoot < n; ++chainRoot) {
			if (parent[chainRoot] == -1 || heavy[parent[chainRoot]] != chainRoot) {
				for (int j = chainRoot; j != -1; j = heavy[j])  // iterate on a chain
					root[j] = chainRoot, segTreePos[j] = pos++;
			}
		}
	}
	void queryChain(int l, int r) {
		queryRes = max(queryRes, segTree.query(l, r).v);
	}
	int queryPath(int u, int v) {
		queryRes = 0;  // be careful from u = v for isValueOnEdge
		for (; root[u] != root[v]; v = parent[root[v]]) {
			if (depth[root[u]] > depth[root[v]])
				swap(u, v);
			queryChain(segTreePos[root[v]], segTreePos[v]);
		}
		if (depth[u] > depth[v])
			swap(u, v);
		if (!isValueOnEdge || u != v)
			queryChain(segTreePos[u] + isValueOnEdge, segTreePos[v]);
		return queryRes;  // u = LCA node
	}

	// For value on DIRECTED edge (f, t, value), call update_node(t, value)
	void updatePos(int treeNode, int value) {
		segTree.update(segTreePos[treeNode], value);
	}
};

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);


	int t;
	cin >> t;
	while (t--) {
		int N;
		cin >> N;
		HeavyLight tree(N);
		for (int i = 0; i < N - 1; i++) {
			int u, v, w;
			cin >> u >> v >> w;
			tree.addEdgeUndirected(u - 1, v - 1, w, i);
		}

		tree.init();

		while (true) {
			string query;
			cin >> query;
			if (query == "DONE") break;
			else if (query == "CHANGE") {
				int id, val;
				cin >> id >> val;
				tree.updatePos(tree.edges[id - 1].dest, val);
			}
			else {
				int u, v;
				cin >> u >> v;
				cout << tree.queryPath(u - 1, v - 1) << "\n";
			}
		}
	}
	return 0;
}