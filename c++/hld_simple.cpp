struct Edge {
	int u, v;
	ll w;
	Edge(int u, int v, ll w = 1) : u(u), v(v), w(w){}
};
 
struct Tree {
	int n, root;
	vector<vector<Edge>> edges;
	vi sz;
	Tree(int n, int root = 1) : n(n), root(root) {
		edges.resize(n + 1);
		sz.resize(n + 1);
		depth.resize(n + 1);
		heavyLeaf.resize(n + 1);
	}
 
	void addEdgeDirected(int u, int v, int w = 1) {
		edges[u].push_back(Edge(u, v, w));
	}
 
	void addEdgeUndirected(int u, int v, int w = 1) {
		edges[u].push_back(Edge(u, v, w));
		edges[v].push_back(Edge(v, u, w));
	}
 
	void DFS(int u, int par = -1){
		sz[u] = 1;
		for(auto &edge: edges[u]){
			int v = edge.v;
			if(v == par) continue;
			DFS(v, u);
			sz[u] += sz[v];
			if(par == edges[u][0].v || sz[v] > sz[edges[u][0].v]) {
	           		swap(edge, edges[u][0]);
	        	}
		}
	}
};