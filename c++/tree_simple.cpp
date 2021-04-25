struct Edge {
	int u, v;
	ll w;
	Edge(int u, int v, ll w = 1) : u(u), v(v), w(w){}
	bool operator<(const Edge &other) {
		if(w == other.w){
			pii p1 = {u, v}, p2 = {other.u, other.v};
			return p1 < p2;
		}
		else return w < other.w;
	}
};
 
struct Tree {
	int n, root;
	vector<vector<Edge>> edges;

	Tree(int n, int root = 1) : n(n), root(root), edges(n + 1) {}
 
	void addEdgeDirected(int u, int v, int w = 1) {
		edges[u].push_back(Edge(u, v, w));
	}
 
	void addEdgeUndirected(int u, int v, int w = 1) {
		edges[u].push_back(Edge(u, v, w));
		edges[v].push_back(Edge(v, u, w));
	}
 
	void DFS(int u, int par = -1){
		for(auto &edge: edges[u]){
			int v = edge.v;
			if(v == par) continue;
			DFS(v, u);
		}
	}
};