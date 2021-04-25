struct StVal {
	ll v = 0; StVal() {}
	StVal(const ll _v) : v(_v) {}
	StVal(const StVal& v1, const StVal& v2) { v = max(v1.v, v2.v); }
};
struct MyST {
	int n;
	vector<StVal> st;
	vector<int> leaves;
	vector<ll> originalVal;

	MyST(const int n = 1) : n(n) {
		leaves.resize(n);
		originalVal.resize(n);
		init();
	}

	void init() { init(1, 0, n - 1); }

	void init(const int si, const int lo, const int hi) {
		if (lo == hi) {
			if (si >= (int)st.size()) st.resize(si + 1);
			st[si] = StVal(originalVal[lo]);
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

	void update(const int i, ll c) {
		int si = leaves[i];
		st[si] = StVal(c);
		for (si >>= 1; si; si >>= 1)
			st[si] = StVal(st[si << 1], st[si << 1 | 1]);
	}
};
// usage: if value on edge, just add edges and init. else you should add values for nodes than init
// 1 based
// [in[v], out[v]] --> subtree of v
// [in[nxt[v]], in[v]] --> path from v to last vertex in ascending heavy path from v
// sz[v] size of subtree of v
// in[v] pos of v in segtree
// out[v] pos in segtree of last element in subtree of v
// nxt[v] last vertex in ascending heavy path from v
// val[v] is the value on v
struct HeavyLight {
	struct Edge {
		int dest, id;
		ll weight;
		Edge(int _dest, ll _weight = 1, int _id = 0) {
			dest = _dest;
			weight = _weight;
			id = _id;
		}
	};
	int n, root, isValueOnEdge = 1; //1 if value on edge
	vector<vector<Edge>> adj;
	vi sz, in, out, nxt, parent, depth, edgeToNode;
	vll val;
	MyST segtree;
	HeavyLight (int _n) {
		n = _n;
		val = vll(n + 1, 0);
		segtree = MyST(n);
		root = 1;
		adj = vector<vector<Edge>>(n + 1);
		edgeToNode = vi(n + 1);
	}
	void init() {
		// parent[root] = nxt[root] = 1
		// depth[root] = 1
		in = vi(n + 1, 0), out = vi(n + 1, 0), nxt = vi(n + 1, 1);
		sz = vi(n + 1, 0), parent = vi(n + 1, 1), depth = vi(n + 1, 0);
		int t = 0;
		dfs_sz(root, parent[root]);
		dfs_hld(root, parent[root], t);
		for (int i = 1; i <= n; i++)
			segtree.originalVal[in[i]] = val[i];
		segtree.init();
	}
	void addEdgeUndirected(int u, int v, ll w = 0, int id = 0) {
		adj[u].push_back(Edge(v, w, id));
		adj[v].push_back(Edge(u, w, id));
	}
	void addEdgeDirected(int u, int v, ll w = 0, int id = 0) {
		adj[u].push_back(Edge(v, w, id));
	}
	StVal query (int u, int v) {
		int lca = LCA(u, v);
		StVal ansU = queryUp(u, lca);
		StVal ansV = queryUp(v, lca);
		StVal ans = StVal(ansU, ansV);
		return ans;
	}
	// change here
	void update(int u, ll x) {
		if (isValueOnEdge) {
			u = edgeToNode[u];
		}
		val[u] = x;
		segtree.update(in[u], x);
	}
private:
    void dfs_sz (int cur, int par) {
        sz[cur] = 1;
        for (auto& u : adj[cur]) {
			if (u.dest == par) continue;
            dfs_sz(u.dest, cur);
            sz[cur] += sz[u.dest];
            if (adj[cur][0].dest == par || sz[u.dest] > sz[adj[cur][0].dest]) {
                swap(u, adj[cur][0]);
            }
        }
    }
    void dfs_hld (int cur, int par, int &t) {
		parent[cur] = par;
		depth[cur] = (par == 0 ? 1 : depth[par] + 1);
		in[cur] = t++;
        for (auto u : adj[cur]) {
			if (u.dest == par) continue;
			if (isValueOnEdge) {
				edgeToNode[u.id] = u.dest;
				val[u.dest] = u.weight;
			}
            nxt[u.dest] = (u.dest == adj[cur][0].dest ? nxt[cur] : u.dest);
            dfs_hld(u.dest, cur, t);
        }
        out[cur] = t - 1;
    }
	StVal queryUp (int u, int lca) {
		StVal ans(0); // specify default
		while (true) {
			if (nxt[u] == nxt[lca]) {
				ans = StVal(ans, segtree.query(in[lca] + isValueOnEdge, in[u]));
				break;
			}
			ans = StVal(ans, segtree.query(in[nxt[u]], in[u]));
			u = parent[nxt[u]];
		}
		return ans;
	}
	int LCA (int u, int v) {
		while (true) {
			if (nxt[u] == nxt[v])
				return (depth[u] < depth[v] ? u : v);
			if (depth[nxt[u]] > depth[nxt[v]]) swap(u, v);
			v = parent[nxt[v]];
		}
	}
};