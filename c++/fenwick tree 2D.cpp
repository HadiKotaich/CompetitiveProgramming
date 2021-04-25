struct FT {
	int n;
	vector<int> vals;
	FT(int n) :
			n(n), vals(vector<int>(n + 1, 0)) {
	}
 
	void update(int i, const int a) {
		for (++i; i <= n; i += i & -i)
			vals[i] += a;
	}
	int query(int i) const { // sum from 0 ... i
		assert(i >= -1 && i < n);
		int sum = 0;
		for (++i; i; i -= i & -i)
			sum += vals[i];
		return sum;
	}
	int query(int i, int j) const {
		return query(j) - query(i - 1);
	}
};
 
struct FT2 {
	FT ft = FT(0);
	vector<int> keys;
 
	void preKey(const int key) {
		keys.emplace_back(key);
	}
 
	void process() {
		sort(keys.begin(), keys.end());
		keys.erase(unique(keys.begin(), keys.end()), keys.end());
		ft = FT(keys.size());
	}
 
	void update(const int key, const int val) {
		auto it = lower_bound(keys.begin(), keys.end(), key);
		assert(it != keys.end() && *it == key);
		ft.update(it - keys.begin(), val);
	}
 
	int query(const int key) {
		auto it = upper_bound(keys.begin(), keys.end(), key);
		return it == keys.begin() ? 0 : ft.query(prev(it) - keys.begin());
	}
 
	int query(const int keyL, const int keyR) {
		return query(keyR) - query(keyL - 1);
	}
};
 
struct FT1 {
	int n;
	vector<FT2> ft2s;
 
	FT1(const int n) :
			n(n), ft2s(vector<FT2>(n + 1)) {
	}
 
	void preKey(int x, const int y) {
		for (++x; x <= n; x += x & -x)
			ft2s[x].preKey(y);
	}
 
	void process() {
		for (FT2 &ft2 : ft2s)
			ft2.process();
	}
 
	void update(int x, const int y, const int val) {
		for (++x; x <= n; x += x & -x)
			ft2s[x].update(y, val);
	}
 
	int query(int x, const int yl, const int yr) {
		assert(x >= -1 && x < n);
 
		int sum = 0;
		for (++x; x; x -= x & -x)
			sum += ft2s[x].query(yl, yr);
		return sum;
	}
	int query(const int xl, const int xr, const int yl, const int yr) {
		return query(xr, yl, yr) - query(xl - 1, yl, yr);
	}
};
 

// Usage: 
// create FT1(n) n is the maxmium coordinate
// go over all update queries and call Ft1.prekey(i, j)
// call FT1.process()
// now you are ready to process all queries and updates
// make your queries 1 based not zero based