struct TwoSAT{
	int n;
	vvi g, gt;
	// all will be initialized in solve
	vector<bool> used;
	vi order, comp;
	vector<bool> assignment;

	TwoSAT(int n) : n(2*n), g(2 * n), gt(2 * n){}
	// usage: to add a clause a or b => addClause(2 * a, 2 * b)
	// usage: to add a clause a or not b => addClause(2 * a, 2 * b + 1)
	void OR(int u, int v){
		addImplication(NOT(u), v);
		addImplication(NOT(v), u);
	}

	void XOR(int u, int v) {
		// or but not both, CNF: (a v b) & (!a v !b)  ==> E.g. can't be true for (1, 1) / (0, 0)
		OR(u, v);
		OR(NOT(u), NOT(v));
	}

	void XNOR(int u, int v){
		// both or none, CNF: (!a v b) & (a v !b)  ==> E.g. can't be true for (1, 0) / (0, 1)
		OR(u, NOT(v));
		OR(v, NOT(u));
	}

	void forceValue(int u, bool b){
		if(b){
			addImplication(NOT(u), u);
		}
		else{
			addImplication(u, NOT(u));
		}
	}

	bool solve() {
	    used.assign(n, false);
	    for (int i = 0; i < n; ++i) {
	        if (!used[i])
	            dfs1(i);
	    }

	    comp.assign(n, -1);
	    for (int i = 0, j = 0; i < n; ++i) {
	        int v = order[n - i - 1];
	        if (comp[v] == -1)
	            dfs2(v, j++);
	    }

	    assignment.assign(n / 2, false);
	    for (int i = 0; i < n; i += 2) {
	        if (comp[i] == comp[i + 1])
	            return false;
	        assignment[i / 2] = comp[i] > comp[i + 1];
	    }
	    return true;
	}
	static int NOT(int u){
		return u ^ 1;
	}
private:
	void addImplication(int u, int v){
		g[u].push_back(v);
		gt[v].push_back(u);
	}
	void dfs1(int v) {
	    used[v] = true;
	    for (int u : g[v]) {
	        if (!used[u])
	            dfs1(u);
	    }
	    order.push_back(v);
	}
	void dfs2(int v, int cl) {
	    comp[v] = cl;
	    for (int u : gt[v]) {
	        if (comp[u] == -1)
	            dfs2(u, cl);
	    }
	}
};