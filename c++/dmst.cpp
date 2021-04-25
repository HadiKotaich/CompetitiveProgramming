struct DSU {
    vi P, S;
    DSU(int n) {
        P.resize(n+1);
        S.resize(n+1, 1);
        for (int i = 1; i <= n; i++) P[i] = i;
    }
    ll find(int x) {
        if (x == P[x]) return x;
        return P[x] = find(P[x]);
    }
    bool merge(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return 0;
        if (S[a] < S[b]) swap(a, b);
        S[a] += S[b];
        P[b] = a;
        return 1;
    }
    int same(int a, int b) {
        return find(a) == find(b);
    }
};
 
struct Edge {ll a, b; ll w;};
 
struct Node {
    Edge key;
    Node *l, *r;
    ll delta;
    void prop() {
        key.w += delta;
        if (l) l->delta += delta;
        if (r) r->delta += delta;
        delta = 0;
    }
    Edge top() { prop(); return key; }
};
 
Node *merge(Node *a, Node *b) {
    if (!a || !b) return a ?: b;
    a->prop(), b->prop();
    if (a->key.w > b->key.w) swap(a, b);
    swap(a->l, (a->r = merge(b, a->r)));
    return a;
}
void pop(Node*& a) { a->prop(); a = merge(a->l, a->r); }
 
ll dmst(int n, int r, vector<Edge>& g) {
    DSU d(n);
    vector<Node*> heap(n);
    for (auto e: g) heap[e.b] = merge(heap[e.b], new Node{e});
    long long res = 0;
    vi seen(n, -1), path(n);
    seen[r] = r;
    for (int s = 0; s < n; s++) {
        int u = s, qi = 0, w;
        while (seen[u] < 0) {
            path[qi++] = u, seen[u] = s;
            if (!heap[u]) return -1;
            Edge e = heap[u]->top();
            heap[u]->delta -= e.w, pop(heap[u]);
            res += e.w, u = d.find(e.a); // found an edge
            if (seen[u] == s) {
                Node* cyc = 0;
                do cyc = merge(cyc, heap[w = path[--qi]]);
                while (d.merge(u, w));
                u = d.find(u);
                heap[u] = cyc, seen[u] = -1;
            }
        }
    }
    return res;
}
 
 
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);
    ll n; cin >> n;
    vector<Edge> edges;
    fo(i, 1, n+1){
      ll x, d; cin >> x >> d;
      f(j, n+1){
        int a; cin >> a;
        if(i != j) edges.pb({j, i, a});
      }
      edges.pb({x, i, d});
    }
 
    cout << dmst(n+1, 0, edges) << endl;
    return 0;
}