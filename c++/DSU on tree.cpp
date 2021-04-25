void merge(int u, int v, vector<map<int, unordered_set<string>>*>& maps, const int cur) {
	if (maps[u]->size() < maps[v]->size()) swap(u, v);
	for (auto height : *maps[v])
		for(auto name: height.second)
			(*maps[u])[height.first].insert(name);
	maps[cur] = maps[u];
}

void dsuOnTree(int cur, vector<map<int, unordered_set<string>>*> & maps, vector<string> &names, vector<map<int, int>>& ans) {
	maps[cur] = new map<int, unordered_set<string>>();
	for (auto it : tree.adj[cur]) {
		if (it.dest == tree.parent[cur]) continue;
		dsuOnTree(it.dest, maps, names, ans);
		merge(cur, it.dest, maps, cur);
	}
	(*maps[cur])[tree.dist[cur]].insert(names[cur]);
	for (auto &it : ans[cur]) {
		it.second = (*maps[cur])[it.first].size();
	}
}
