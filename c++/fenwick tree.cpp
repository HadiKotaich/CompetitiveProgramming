struct FenwickTree { // 0 based
	ll n; vll a;
	vll tadd, tmul, tsq; //only for range
	FenwickTree(int _n) {
		n = _n;
		a.resize(n);
		tadd.resize(n); //only for range
		tmul.resize(n); //only for range
		tsq.resize(n); //only for range
	}
	void add(int i, ll x) { // val[n] += x
		add_help(a, i, x);
	}
	ll sum(int i) {// val[0] + val[1] + ... + val[n]
		return sum_help(a, i);
	}
	ll sumbetween(int le, int ri) {// val[le] +...+ val[ri]. le=0 is ok
			return sum_help(a, ri) - sum_help(a, le - 1);
	}
	// returns min p st sum[0,p] > s (or n if no such p exists)
	int lowerBound(ll s) {
		int pos = -1;
		for (int bs = 1 << (31 - __builtin_clz(n)); bs != 0; bs /= 2) {
			int nxt = pos + bs;
			if (nxt < n && s >= a[nxt]) { s -= a[nxt]; pos = nxt; }
		}
		return pos + 1;
	}

	// RANGE QUERIES + RANGE UPDATES
	void addOnSegment(ll le, ll ri, ll x) { // add to [le..ri]: [x,x,...,x]
		add_help(tmul, le, x); add_help(tadd, le, -x * (le - 1));
		add_help(tmul, ri, -x); add_help(tadd, ri, x * ri);
	}
	void addSaw(ll le, ll ri, ll x) {// add to [le,ri]: [x, 2x, ..., (ri - le + 1)x]
		add_help(tsq, le, x); add_help(tadd, le, -x * le * (le - 1) / 2);
		add_help(tsq, ri + 1, -x); add_help(tadd, ri + 1, x * ri * (ri + 1) / 2);
		addOnSegment(le, ri, -x * (le - 1));
	}
	void addBackSaw(ll le, ll ri, ll x) {// add to [le, ri]: [(ri - le + 1)x, ..., 2x, x]
		addSaw(le, ri, -x); addOnSegment(le, ri, (ri - le + 2) * x);
	}
	ll query(ll i) {// get sum[0..i]
		ll tmp = i * (i + 1) / 2;
		ll ans = sum_help(tadd, i) + sum_help(tmul, i) * i + sum_help(tsq, i) * tmp;
		return ans;
	}
	ll query(ll l, ll r){
		ll ans = query(r);
		if(l > 0) ans -= query(l - 1);
		return ans;
	}
	// RANGE QUERIES + RANGE UPDATES end

private:
	void add_help(vll &v, ll i, ll x) {
		for (; i < n; i |= i + 1) v[i] += x;
	}
	ll sum_help(vll& v, ll i) {
		ll s = 0;
		while (i >= 0) {
			s += v[i];
			i = (i & (i + 1)) - 1;
		}
		return s;
	}
};