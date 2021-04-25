const ll INF = 1LL << 61; bool cmpA;

struct Line { // ax+b. xl = the x-coord of intersection with previous line in set
	ll a, b;
	mutable long double xl;
	Line(ll a1, ll b1, long double xl1) {
		a = a1;
		b = b1;
		xl = xl1;
	}
	bool operator < (const Line& l) const {
		if (cmpA) return a < l.a; else return xl
			< l.xl;
	}
};
struct DynamicHull : std::multiset<Line> {
	iterator prev(iterator y) { return --y; } // skip if using c++0x/c++11
	iterator next(iterator y) { return ++y; } // skip if using c++0x/c++11
	bool bad(iterator y) {
		iterator z = next(y), x;
		if (y == begin()) {
			if (z == end()) return 0;
			return y->a == z->a && y->b <= z->b;
		}
		x = prev(y);
		if (z == end()) return y->a == x->a && y->b <= x->b;
		return 1.0L * (x->b - y->b) * (z->a - y->a) >= 1.0L * (y->b - z->b) * (y->a -
			x->a);
	}
	void add(ll a, ll b) { // add a line ax+b to the convex hull
		cmpA = 1; iterator y = insert(Line(a, b, -INF));
		if (bad(y)) { erase(y); return; }
		while (next(y) != end() && bad(next(y))) erase(next(y));
		while (y != begin() && bad(prev(y))) erase(prev(y));
		if (next(y) != end()) next(y)->xl = 1.0L * (y->b - next(y)->b) / (next(y)->a -
			y->a);
		if (y != begin()) y->xl = 1.0L * (y->b - prev(y)->b) / (prev(y)->a - y->a);
	}
	ll eval(ll x) { // find max ax+b, over all lines added, for given x
		if (empty()) return -INF;
		cmpA = 0; iterator it = prev(lower_bound(Line(0, 0, 1.0L * x)));
		return it->a * x + it->b;
	}
};

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);

	int n, k;
	cin >> n >> k;

	vi weights(n);
	for (auto& it : weights)
		cin >> it;


	ll totalCost = 0;
	for (int i = 0; i < n; i++) {
		totalCost += (ll)(i)* weights[i];
	}

	vll s(n, 0);
	s[n - 1] = weights[n - 1];
	for (int i = n - 2; i >= 0; i--) {
		s[i] = s[i + 1] + weights[i];
	}

	DynamicHull convexHull;
	vector<vll> dp(k + 1, vll(n, 0));

	for (int leaves = 0; leaves <= k - 1; leaves++) {
		for (int cur = n - 1; cur >= 0; cur--) {
			dp[leaves][cur] = max(0LL, convexHull.eval(cur));
		}

		for (int cur = n - 1; cur >= 0; cur--) {
			convexHull.add(-s[cur], dp[leaves][cur] + s[cur] * cur);
		}
	}
	cout << totalCost - dp[k - 1][0] << "\n";
	return 0;
}