//function that gets the number of pairs with distance <= K
const int MaxN = (int)1e5 + 9;
int sz[MaxN + 9];
bool dead[MaxN + 9];

Tree tree;

ll numOfPairsWithDistLess(int M,vll &occ) {
	// gets the number of distinct pairs (i, j) i != j with dist less than M
	// based on the occurences array given

	ll ans = 0;
	vll partialSum(occ.size());
	partial_sum(occ.begin(), occ.end(), partialSum.begin());

	for (int i = 0; i <= min(M, (int)occ.size() - 1); i++) {
		ans += occ[i] * partialSum[min(M - i, (int)partialSum.size() - 1)];
		if (2 * i <= M) ans -= occ[i];
	}
	ans /= 2;
	return ans;
}

void getDistOccArray(int cur, int parent, int depth, vll &temp) {
	// gets the distance counts that occurs in the subtree of cur
	if (depth >= (int)temp.size())
		temp.resize(depth + 1, 0);
	++temp[depth];
	for (auto it : tree.adj[cur]) 
		if (it.dest != parent && !dead[it.dest]) // make sure it is in my subtree in the CDTree
			getDistOccArray(it.dest, cur, depth + 1, temp);
}

ll calc(int M, int centroid) {
	// gets the number of pairs with dist less than M such that the
	// smallest label on the path between them is centroid
	ll ans = 0;
	vll distOcc;

	for (auto it : tree.adj[centroid]) {
		if (dead[it.dest]) continue;
		vll temp;
		getDistOccArray(it.dest, centroid, 1, temp);
		ans -= numOfPairsWithDistLess(M, temp); // remove all path passing in the Centroid and goind down in the same subtree

		if (temp.size() > distOcc.size())
			distOcc.resize(temp.size(), 0);
		for (int i = 0; i < temp.size(); i++) {
			distOcc[i] += temp[i];
		}
	}
	ans += accumulate(distOcc.begin(), distOcc.begin() + min(M + 1, (int)distOcc.size()),0);
	ans += numOfPairsWithDistLess(M, distOcc);
	return ans;
}

int OneCentroid(const int root) {
	function<void(int, int)> get_sz = [&](int u, int prev) {
		sz[u] = 1;
		for (auto& it : tree.adj[u]) {
			if (it.dest != prev && !dead[it.dest]) {
				get_sz(it.dest, u);
				sz[u] += sz[it.dest];
			}
		}
	};
	get_sz(root, -1);
	function<int(int, int)> dfs = [&](int u, int prev) {
		for (auto& it : tree.adj[u]) {
			if (it.dest != prev && !dead[it.dest]) {
				if (sz[it.dest] > sz[root] / 2) {
					return dfs(it.dest, u);
				}
			}
		}
		return u;
	};
	return dfs(root, -1);
}

// puts in ans the number of nodes with distance less than M
Tree CentroidDecomposition(const int M, ll &ans /* spe */) {
	Tree CDTree(tree.n);
	function<void(int, int)> rec = [&](int parent, int start) {
		const int centroid = OneCentroid(start);
		ans += calc(M, centroid); // spe
		if (parent != -1) {
			CDTree.addEdgeUndirected(parent, centroid);
		}
		else {
			CDTree.root = centroid;
		}
		dead[centroid] = true;
		for (auto u : tree.adj[centroid]) if (!dead[u.dest]) rec(centroid, u.dest);
		dead[centroid] = false;
	};
	rec(-1, tree.adj[0].empty() /* root: 0 or 1 */);

	return CDTree;
}