struct StVal {
	ll v = 0;
	StVal() {}
	StVal(const ll v) : v(v) {}
	StVal(const StVal& v1, const StVal& v2){
		v = v1.v + v2.v;
	}
};
 
struct StUpdate {
	ll v = 0;
	StUpdate() {}
	StUpdate(const ll v) : v(v) {}
	StUpdate(const StUpdate& u1, const StUpdate& u2){
	}
	void apply(StVal& v, const int lo, const int hi) {
	}
};
 
template<class V, class U> struct SegmenTree {
	int n;
	vector<V> st;
	vector<U> lazy;
 
	SegmenTree(const int n) : n(n) {
		init();
		lazy.resize(st.size());
	}
 
	void init() { init(1, 0, n - 1); }
	void init(const int si, const int lo, const int hi) {
		if (lo == hi) {
			if (si >= (int)st.size()) st.resize(si + 1);
			st[si] = V();
		}
		else {
			const int mid = (lo + hi) >> 1;
			init(si << 1, lo, mid);
			init(si << 1 | 1, mid + 1, hi);
			st[si] = V(st[si << 1], st[si << 1 | 1]);
		}
	}
 
	void updateLazy(const int si, const int lo, const int hi) {
		lazy[si].apply(st[si], lo, hi);
		if (lo != hi) {
			lazy[si << 1] = U(lazy[si << 1], lazy[si]);
			lazy[si << 1 | 1] = U(lazy[si << 1 | 1], lazy[si]);
		}
		lazy[si] = U();
	}
 
	V query(const int l, const int r) { return (l <= r && l < n && r >= 0) ? query(l, r, 1, 0, n - 1) : V(); }
	V query(const int l, const int r, const int si, const int lo, const int hi) {
		updateLazy(si, lo, hi);
		if (l <= lo && hi <= r) return st[si];
		const int mid = (lo + hi) >> 1;
		if (r <= mid) return query(l, r, si << 1, lo, mid);
		if (mid < l) return query(l, r, si << 1 | 1, mid + 1, hi);
		return V(query(l, r, si << 1, lo, mid), query(l, r, si << 1 | 1, mid + 1, hi));
	}
 
	// gets the index of the kth one
	int kthOne(int k) { return kthOne(k, 1, 0, n - 1);}
	int kthOne(int k, const int si, const int lo, const int hi){
		updateLazy(si, lo, hi);
		if(st[si].v < k) return -1;
		if(lo == hi) return lo;
		const int mid = (lo + hi) >> 1;
		updateLazy(si<<1, lo, mid);
		int left = st[si<<1].v;
		if(left >= k) return kthOne(k, si<<1, lo, mid);
		else return kthOne(k - left, si<<1 | 1, mid + 1, hi);
	}

	// gets the smallest index j greater than l where a[j] >= k
	// works with max segment tree
	int lowerBound(int k, int l) { return lowerBound(k, l, 1, 0, n - 1);}
	int lowerBound(int k, const int l, const int si, const int lo, const int hi){
		if(hi < l) return -1;
		updateLazy(si, lo, hi);
		if(st[si].v < k) return -1;
		if(lo == hi) return lo;
		const int mid = (lo + hi) >> 1;
		int left = (mid < l) ? -1 : lowerBound(k, l, si << 1, lo, mid);
		if(left != -1) return left;
		else{
			return lowerBound(k, l, si << 1 | 1, mid + 1, hi);
		}
	}
 
	void update(const int l, const int r, const U u) { if (l <= r) update(l, r, u, 1, 0, n - 1); }
	void update(const int l, const int r, const U& u, const int si, const int lo, const int hi) {
		if (l <= lo && r >= hi) {
			lazy[si] = U(lazy[si], u);
			updateLazy(si, lo, hi);
		}
		else {
			updateLazy(si, lo, hi);
			if (l > hi || r < lo) return;
			const int mid = (lo + hi) >> 1;
			update(l, r, u, si << 1, lo, mid);
			update(l, r, u, si << 1 | 1, mid + 1, hi);
			st[si] = V(st[si << 1], st[si << 1 | 1]);
		}
	}
};
typedef SegmenTree<StVal, StUpdate> MyST;