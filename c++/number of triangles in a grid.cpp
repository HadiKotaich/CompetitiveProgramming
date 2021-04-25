// gets the number of triangles in a grid using O(n^2 m^2) precalc and O(1) query

ll dp[101][101];

ll choose3(ll n) {
	ll ans = n * (n - 1LL) * (n - 2LL);
	ans /= 6;
	return ans;
}

// gets the number of triangles in the grid NM
ll calc(int n, int m) {
	ll ans = choose3(n * m);
	ans -= n * choose3(m);
	ans -= m * choose3(n);
	ans -= dp[n][m];
	return ans;
}

long long gcd(long long a, long long b) {
	long long r;
	while (b != 0) {
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

// (x3 - x2) / (y3 - y2) = (x2 - x1) / (y2 - y1)
// x3 = x2 + t (x2 - x1)/g

// gets the number of degenerate triangles having a point in (x1, y1) and that are completely in the grid x1y1
ll cnt(int x1, int y1) {
	ll ans = 0;
	for (int x2 = 2; x2 < x1; x2++) {
		for (int y2 = 2; y2 < y1; y2++) {
			ll g = gcd(x1 - x2, y1 - y2);
			ans += min((g * (x2 - 1)) / (x1 - x2), (g * (y2 - 1)) / (y1 - y2));
		}
	}
	return ans * 2;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL); cout.tie(NULL);
	mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

	int T; cin >> T;
	// dp[i][j] = dp[i - 1][j - 1] - dp[i - 1][j] - dp[i][j - 1] + cnt(i, j)
	int mx = 100;
	for (int i = 0; i <= mx; i++) dp[0][i] = dp[i][0] = 0;
	for (int i = 1; i <= mx; i++) {
		for (int j = 1; j <= mx; j++) {
			dp[i][j] = - dp[i - 1][j - 1] + dp[i - 1][j] + dp[i][j - 1] + cnt(i, j);
			if (i <= 2 || j <= 2) assert(dp[i][j] == 0);
		}
	}
	for(int t = 1; t <= T; t++){
		ll n, m; cin >> n >> m;
		cout << calc(n, m) << "\n";
	}
}