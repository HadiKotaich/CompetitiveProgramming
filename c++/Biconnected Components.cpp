///// when clearing be careful to clear adj[i] from i = 1 till n
///// do not define n in main
///// do not define st in main 


// lines denoted [C], [B], [A], [L] necessary only if we're looking for:
// numbering of biconnected [C]omponents, [B]ridges, [A]rticulation points, [L]owfunction
// every Bridge forms its own biconnected Component


#define N 2000007
struct Edge { // PRE: has to be bcc = -1 and bridge = 0 (constructors sets this)
	Edge* rev;
	int dest;
	int bcc; // OUT: number of biconnected Component
	bool bridge; // OUT: is it a bridge // B
	Edge(int v) : dest(v), bcc(-1)
		, bridge(0) // B
	{};
};
int n; // IN: number of vertices, be careful not to write int n in MAIN
list<Edge> adj[N]; // IN: adjacency lists
int visit[N];
bool artp[N]; // OUT: is it an articulation point // A
int bcc_num; // OUT: number of biconnected Components // C
int low[N]; // OUT: Low function // L
void add_edge(int a, int b) {
	adj[a].pb(Edge(b)); adj[b].pb(Edge(a));
	adj[a].back().rev = &adj[b].back();
	adj[b].back().rev = &adj[a].back();
} 
stack<Edge*> st; // C
int dfsTime;
int bccDFS(int v, bool root = 0) {
	int lo = visit[v] = ++dfsTime;
	FOREACH(it, adj[v]) {
		if (it->bcc != -1) continue;
		st.push(&*it); // C
		it->rev->bcc = -2;
		if (!visit[it->dest]) {
			int ulo = bccDFS(it->dest);
			lo = min(ulo, lo);
			it->bridge = it->rev->bridge = (ulo > visit[v]); // B
			if (ulo >= visit[v]) { // AC
				artp[v] = !root; root = 0; // A
				Edge* edge; // C
				do { // C
					edge = st.top(); st.pop(); // C
					edge->bcc = edge->rev->bcc = bcc_num; // C
				} while (edge != &*it); // C
				++bcc_num; // C
			} // AC
		}
		else lo = min(lo, visit[it->dest]);
	}
	low[v] = lo; // L
	return lo;
} 
void computeBCC() {
	fill(artp, artp + n + 1, 0); // A
	fill(visit, visit + n + 1, 0);
	dfsTime = 1;
	bcc_num = 0; // C
	FORI(i, n) if (!visit[i]) bccDFS(i, 1); // maybe change FORI to FOR
}