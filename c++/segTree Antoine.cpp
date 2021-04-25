#include <iostream>
#include <set>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <unordered_map>
#include <stack>

using namespace std;

typedef vector<int> vi;
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<pii, int> piii;


template<class V, class U> struct SegmenTree {
	int n;
	vector<V> st;
	vector<U> lazy;
	vector<int> leaves;
	vector<int> originalArray;

	SegmenTree(const int n) : n(n) {
		leaves.resize(n);
		originalArray.resize(n);
		init();
		lazy.resize(st.size());
	}

	void init() { init(1, 0, n - 1); }
	void init(const int si, const int lo, const int hi) { // init st[] and leaves[]
		if (lo == hi) {
			if (si >= (int)st.size()) st.resize(si + 1);
			st[si] = V(originalArray[lo]); //take from input array
			leaves[lo] = si;
		}
		else {
			const int mid = (lo + hi) >> 1;
			init(si << 1, lo, mid);
			init(si << 1 | 1, mid + 1, hi);
			st[si] = V(st[si << 1], st[si << 1 | 1]);
		}
	}

	// updates st[si] with lazy[si] and then gives its lazy val to lazy of its children
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

	// ------------------------------------------| ____ |---------------------------------------------------------
	// ------------------------------------------| NON-LAZY |---------------------------------------------------------

	void update(const int i, U u) { // single index update, [!!!] NON-LAZY SEGMENT TREE ONLY !!!
		int si = leaves[i];
		u.apply(st[si]);
		for (si >>= 1; si; si >>= 1)
			st[si] = V(st[si << 1], st[si << 1 | 1]);
	}

	// ------------------------------------------| ____ |---------------------------------------------------------
	// ------------------------------------------| OPTIONAL |---------------------------------------------------------

	// remove and return all non-zero values in a range, assuming all values >= 0
	vector<int> remAll(const int l, const int r) { vector<int> res; remAll(l, r, 1, 0, n - 1, res); return res; }
	void remAll(const int l, const int r, const int si, const int lo, const int hi, vector<int>& res) {
		updateLazy(si, lo, hi);
		if (!st[si] || hi < l || r < lo) return;
		if (lo == hi) {
			res.push_back(lo);
			st[si] = 0;
		}
		else {
			const int mid = (lo + hi) >> 1;
			remAll(l, r, si << 1, lo, mid, res);
			remAll(l, r, si << 1 | 1, mid + 1, hi, res);
			st[si] = V(st[si << 1], st[si << 1 | 1]);
		}
	}

	void updateAllLazy() { updateAllLazy(1, 0, n - 1); }
	void updateAllLazy(const int si, const int lo, const int hi) {
		updateLazy(si, lo, hi);
		if (lo != hi) {
			const int mid = (lo + hi) >> 1;
			updateAllLazy(si << 1, lo, mid);
			updateAllLazy(si << 1 | 1, mid + 1, hi);
		}
	}

	vector<V> retreive() {
		updateAllLazy();
		vector<V> res(n);
		for (int i = 0; i < n; ++i)
			res[i] = st[leaves[i]];
		return res;
	}
};

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

struct StUpdate { // define
	ll v = 0; // optional
	StUpdate() {}
	StUpdate(const ll v) : v(v) {} // optional

	// how to merge an update with another update
	StUpdate(const StUpdate& u1, const StUpdate& u2)
	{
		v = u1.v + u2.v;
	}

	// specifies how stUpdate will give its value to stVal v
	// for example how will lazy[si] give its value to st[si]
	void apply(StVal& v, const int lo, const int hi) { 
		v.v += this->v * (hi - lo + 1);
	}

	// non lazy
	void apply(StVal& v) {
		v.v += this->v;
	}

};

typedef SegmenTree<StVal, StUpdate> MyST;
