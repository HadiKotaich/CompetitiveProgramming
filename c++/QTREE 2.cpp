/*
You are given a tree (an undirected acyclic connected graph) with N nodes, and edges numbered 1, 2, 3...N-1. Each edge has an integer value assigned to it, representing its length.

We will ask you to perfrom some instructions of the following form:

DIST a b : ask for the distance between node a and node b
or
KTH a b k : ask for the k-th node on the path from node a to node b

*/


// be careful everything is 1 based
// always do getParents() after you finish adding edges
// adjust N based on size
#define LOGN 17 // 2 ^ 17 = 131072
struct Tree {
	struct Edge {
		ll dest, weight;
		Edge(ll _dest = -1, ll _weight = 1) {
			dest = _dest;
			weight = _weight;
		}
	};
	int n, root;
	vector<vector<Edge>> adj;
	// start time 0 indexed, finishTime is 1 + biggestStartTime in my subtree
	vi subtreeCount, parent, startTime, finishTime, nodeStartingAt;
	vll dist;
	ll INF = (ll)1e15;

	Tree(int _nodes = 0, int _root = 1) { // 1 based
		n = _nodes;
		adj.resize(n + 1);
		root = 1;
	}
	void addEdgeDirected(int u, int v, int w = 1) {
		adj[u].push_back(Edge(v, w));
	}
	void addEdgeUndirected(int u, int v, int w = 1) {
		adj[u].push_back(Edge(v, w));
		adj[v].push_back(Edge(u, w));
	}

	void getSubtreeNodesCount() {
		// for each node how many nodes it has in its subtree
		subtreeCount = vi(n + 1, -1);
		for (int i = 1; i <= n; i++) {
			if (subtreeCount[i] == -1) {
				getSubtreeNodesCountRec(-1, i, subtreeCount);
			}
		}
	}
	ll getDiameter() {
		// gets the weight of the diameter of the tree
		return getDiameterRec(-1, root).first;
	}
	void getParents() {
		parent = vi(n + 1, -1);
		stack<int> st;
		st.push(root);
		while (!st.empty()) {
			int cur = st.top();
			st.pop();
			for (auto it : adj[cur]) {
				if (it.dest == parent[cur])continue;
				parent[it.dest] = cur;
				st.push(it.dest);
			}
		}
	}

	void DFS() {
		// gets the startTime and finishTime and nodeAtTime
		// only does it for the root
		int DFSTime = 0;
		startTime = vi(n + 1, -1);
		finishTime = vi(n + 1, -1);
		nodeStartingAt = vi(n + 1, -1);
		getParents();
		DFSRec(root, DFSTime);
	}

	void getDistArray() {
		dist = vll(n + 1, INF);
		dist[root] = 0;

		queue<int> q;
		q.push(root);
		while (!q.empty()) {
			int cur = q.front();
			q.pop();
			for (auto it : adj[cur]) {
				if (it.dest == parent[cur]) continue;
				dist[it.dest] = dist[cur] + it.weight;
				q.push(it.dest);
			}
		}
	}
	void initLCA() {
		pos.resize(n + 1);
		anc.resize(LOGN + 1);
		for (auto& it : anc) it.resize(n + 1, 0);
		getParents();
		FORI(i, n) anc[0][i] = parent[i];
		FORI(i, n) pos[i] = -1;
		pos[root] = 0;
		anc[0][root] = root;
		for (int i = 1; i <= n; i++) {
			if (pos[i] == -1) getPos(i);
		}
		for (int i = 1; i <= LOGN; i++) {
			for (int j = 1; j <= n; j++) {
				anc[i][j] = anc[i - 1][anc[i - 1][j]];
			}
		}
	}
	int lca(int u, int v) {
		if (pos[u] < pos[v]) swap(u, v);
		int diff = pos[u] - pos[v];
		REPD(k, LOGN, 0) if ((1 << k) <= diff) {
			diff -= 1 << k;
			u = anc[k][u];
		}
		// now they are equidistant from root
		if (u == v) return u;
		REPD(k, LOGN, 0) if (anc[k][u] != anc[k][v]) {
			u = anc[k][u];
			v = anc[k][v];
		}
		return parent[u];
	}
	ll distBetween(int u, int v) { // call getDist Array and initLca first
		int lcaNode = lca(u, v);
		ll ans = dist[u] + dist[v] - 2 * dist[lcaNode];
		return ans;
	}
	int kthParent(int u, int k) {
		//0 is self
		bitset<20> bitmask(k);
		int ans = u;
		for (int i = 0; i < 20; i++) {
			if (bitmask[i]) {
				ans = anc[i][ans];
			}
		}
		return ans;
	}

private:
	vi pos;
	vector<vi>anc; // temp

	int getPos(int u) {
		if (pos[u] == -1) pos[u] = getPos(parent[u]) + 1;
		return pos[u];
	}

	void DFSRec(int cur, int& DFSTime) {
		nodeStartingAt[DFSTime] = cur;
		startTime[cur] = DFSTime++;
		for (auto it : adj[cur]) {
			if (it.dest != parent[cur]) {
				DFSRec(it.dest, DFSTime);
			}
		}
		finishTime[cur] = DFSTime;
	}

	int getSubtreeNodesCountRec(int par, int cur, vi& subtreeNodesCount) {
		int mySize = 1;
		for (auto it : adj[cur]) {
			if (it.dest == par) continue;
			mySize += getSubtreeNodesCountRec(cur, it.dest, subtreeNodesCount);
		}
		return subtreeNodesCount[cur] = mySize;
	}
	pll getDiameterRec(int par, int cur) {
		ll diam = 0;
		ll maxHeight[3] = { 0, 0, 0 };
		for (auto it : adj[cur]) {
			if (it.dest == par) continue;
			pll subtreeDiam = getDiameterRec(cur, it.dest);
			diam = max(diam, subtreeDiam.first);

			maxHeight[0] = subtreeDiam.second + it.weight;
			sort(maxHeight, maxHeight + 3);
		}

		diam = max(diam, maxHeight[1] + maxHeight[2]);
		return { diam, maxHeight[2] };
	}
};

Tree tree;

int KthOnPath(Tree & tree2, int u, int v, int k) {
	// check if it is on u side or v side
	// get the right ancestor accordingly 
	int LCA = tree2.lca(u, v);
	
	int distToLCA = tree2.distBetween(u, LCA);
	int distToV = tree2.distBetween(u, v);

	k--;
	if (distToLCA < k) {
		swap(u, v);
		k = distToV - k;
	}

	int ans = tree2.kthParent(u, k);
	return ans;
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	
	int t;
	cin >> t;
	while (t--) {
		int n;
		cin >> n;
		tree = Tree(n);
		Tree tree2(n);
		for (int i = 0; i < n - 1; i++) {
			int u, v, w;
			cin >> u >> v >> w;
			tree.addEdgeUndirected(u, v, w);
			tree2.addEdgeUndirected(u, v);
		}
		tree.initLCA();
		tree2.initLCA();
		tree.getDistArray();
		tree2.getDistArray();

		while (true) {
			string query;
			cin >> query;

			if (query == "DIST") {
				int u, v;
				cin >> u >> v;
				cout << tree.distBetween(u, v) << "\n";
			}
			else if (query == "KTH") {
				int u, v, k;
				cin >> u >> v >> k;
				cout << KthOnPath(tree2, u, v, k) << "\n";
			}
			else if (query == "DONE") {
				break;
			}
		}
	}
}