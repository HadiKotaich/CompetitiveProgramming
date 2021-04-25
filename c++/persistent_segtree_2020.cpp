struct StVal { // define
    ll v = 0; // optional
    StVal() {}
    StVal(const ll v) : v(v) {} // optional
    // how to merge a value with another value
    StVal(const StVal& v1, const StVal& v2)
    {
        v = v1.v + v2.v;
    }
};
struct PersistentSegtree {
    int NODES = 0; // next free ID
    vi leftChild, rightChild;
    vector<StVal>st;
    int n; // number of leaves in a single segment tree


    PersistentSegtree(int n) : n(n) { resize(20 * n); }

    // Usage: int root = build(0, n - 1);
    int build(int L, int R) {
        int M = (L + R) >> 1;
        if (L == R) return newleaf(StVal()); // construct as leaf
        else return newparent(build(L, M), build(M + 1, R)); // construct as parent
    }

    // int new_version_root = update(i, x, root);
    // this CHANGES THE VALUE OF Index i to x in the version of p
    int update(int i, ll x, int p) { return update(i, x, p, 0, n - 1); }
    int update(int i, ll x, int p, int L , int R ) {
        int M = (L + R) >> 1;
        if (L == R) return newleaf(x);
        if (i <= M) return newparent(update(i, x, leftChild[p], L, M), rightChild[p]);
        else        return newparent(leftChild[p], update(i, x, rightChild[p], M + 1, R));
    }

    StVal query(int l, int r, int p) { return (l <= r && l < n && r >= 0) ? query(l, r, p, 0, n - 1) : StVal(); }
    StVal query(int l, int r, int p, int L, int R) {
        if (l <= L && R <= r) return st[p];
        const int M = (L + R) >> 1;
        if (r <= M) return query(l, r, leftChild[p], L, M);
        if (M < l) return query(l, r, rightChild[p], M + 1, R);
        return StVal(query(l, r, leftChild[p], L, M), query(l, r, rightChild[p], M + 1, R));
    }

private:
    int newleaf(StVal value) {
        int p = ++NODES;
        if (p >= (int)st.size()) resize(p + 1);
        leftChild[p] = rightChild[p] = 0; // null
        st[p] = value;
        return p;
    }
    int newparent(int left, int right) {
        int p = ++NODES;
        if (p >= (int)st.size()) resize(p + 1);
        leftChild[p] = left;
        rightChild[p] = right;
        st[p] = StVal(st[left], st[right]);// immediately update value from children
        return p;
    }
    void resize(int p) {
        st.resize(p);
        leftChild.resize(p);
        rightChild.resize(p);
    }
};
