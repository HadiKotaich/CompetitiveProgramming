struct MaxBipartiteMatching {
    // 1 based, left: 1 .... left, right: left + 1, ..., left + right 
    int left, right, nodes;
    vi visited, match;
    vector<vi>adj;

    MaxBipartiteMatching(int _left, int _right)
    {
        left = _left;
        right = _right;
        nodes = left + right;
        adj = vector<vi>(nodes + 1);
    }

    MaxBipartiteMatching(int n, vector<vi>& G)
    {
        // identifies the partites in G and build the maxBPM struct
        auto colors = twoCol(n, G);
        assert(colors.size() > 0);

        left = right = 0;
        nodes = n;
        for (int x = 1; x <= n; x++) {
            if (colors[x] == 0) left++;
            else right++;
        }

        adj = vector<vi>(nodes + 1);
        vi id(nodes + 1, -1);
        int l = 1, r = left + 1;
        for (int x = 1; x <= n; x++) {
            if (colors[x] == 0) id[x] = l++;
            else id[x] = r++;
        }

        for (int x = 1; x <= n; x++) {
            if (colors[x] == 0) {
                for (auto y : G[x]) {
                    AddEdge(id[x], id[y]);
                }
            }
        }
    }

    void AddEdge(int l, int r) {
        assert(l >= 1 && l <= left && r >= left + 1 && r <= left + right);
        adj[l].push_back(r);
    }

    int Kuhn(bool greedy) {
        if (greedy) GreedyInit();
        else {
            match = vi(nodes + 1, -1);
        }
        for (int x = 1; x <= left; x++) {
            if (match[x] == -1) {
                visited = vi(nodes + 1, 0);
                Augment(x);
            }
        }
        int ans = 0;
        for (int x = 1; x <= left; x++) {
            if (match[x] != -1) ans++;
        }
        return ans;
    }

    vi MinimumVertexCover() {
        Kuhn(true);
        vector<vi> G(nodes + 1);
        for (int x = 1; x <= left; x++) {
            for (auto y : adj[x]) {
                if (y == match[x])
                    G[y].push_back(x);
                else
                    G[x].push_back(y);
            }
        }
        visited = vi(nodes + 1, 0);
        for (int x = 1; x <= left; x++) {
            if(match[x] == -1)
                dfs(x, G);
        }

        vi ans;
        for (int i = 1; i <= left; i++) {
            if (visited[i] == 0) ans.push_back(i);
        }
        for (int i = left + 1; i <= left + right; i++) {
            if (visited[i] == 1) ans.push_back(i);
        }
        return ans;
    }

    vi IndependentSet() {
        // clique is the independent set of G bar
        // where G bar contains the edge uv if G does not contain uv
        vi cover = MinimumVertexCover();
        set<int> st;
        for (auto it : cover) st.insert(it);
        vi independentSet;
        for (int i = 1; i <= nodes; i++) {
            if (st.count(i) == 0) independentSet.push_back(i);
        }
        return independentSet;
    }

private:
    bool Augment(int x) {
        if (visited[x] == 1) return false;
        visited[x] = 1;
        for (auto y : adj[x]) {
            if (match[y] == -1 || Augment(match[y])) {
                match[y] = x;
                match[x] = y;
                return true;
            }
        }
        return false;
    }

    void GreedyInit() {
        match = vi(nodes + 1, -1);
        for (int x = 1; x <= left; x++) {
            if (match[x] == -1) {
                for (auto y : adj[x]) {
                    if (match[y] == -1) {
                        match[x] = y;
                        match[y] = x;
                        break;
                    }
                }
            }
        }
    }

    vi twoCol(int n, vector<vi>& G) {
        vi ans(n + 1, 2);
        FORI(i, n) {
            if (ans[i] != 2) continue;
            deque<pii> q;
            q.pb(mp(i, 0));
            while (!q.empty()) {
                int v = q.front().fi, col = q.front().se;
                q.pop_front();
                if (ans[v] == 2) {
                    ans[v] = col;
                    FOREACH(it, G[v]) q.pb(mp(*it, 1 - col));
                }
                else if (ans[v] != col) return vi();
            }
        }
        return ans;
    }

    void dfs(int x, vector<vi> &G) {
        if (visited[x])return;
        visited[x] = 1;
        for (auto y : G[x]) {
            if (!visited[y]) dfs(y, G);
        }
    }
};