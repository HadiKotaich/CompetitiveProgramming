#include <iostream>
#include <set>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <unordered_map>
#include <stack>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <list>
#include <map>
#include <bitset>
#include <queue>
#include <functional>  
#include <numeric>      
#include <assert.h>
#include <unordered_set>

#define REP(i,a,b) for (int i = (a); i <= (b); ++i)
#define REPD(i,a,b) for (int i = (a); i >= (b); --i)
#define FORI(i,n) REP(i,1,n)
#define FOR(i,n) REP(i,0,int(n)-1)
#define mp make_pair
#define pb push_back
#define SZ(x) int((x).size())
#define DBG(v) cerr << #v << " = " << (v) << endl;
#define FOREACH(i,t) for (auto i = t.begin(); i != t.end(); ++i)
#define fi first
#define se second

using namespace std;

typedef vector<int> vi;
typedef long long ll;
typedef vector<ll> vll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef pair<pii, int> piii;
typedef pair<pll, ll>plll;
typedef pair<int, ll>pil;

ll M[100100];
ll B[100100];
ll I[100100];
class ConvexHullTrick {
public:
	
	int si, pt;
	ConvexHullTrick() {
		si = 0; pt = 0;
	}
	void init() { si = 0; pt = 0; }
	bool bad(int a, int b, int c) {
		return (B[b] - B[a]) * (M[a] - M[c]) >= (B[c] - B[a]) * (M[a] - M[b]);
	}
	void add(ll m, ll b, ll ind) {
		M[si] = m; B[si] = b; I[si] = ind; si++;
		while (si >= 3 && bad(si - 3, si - 2, si - 1)) {
			M[si - 2] = M[si - 1];
			B[si - 2] = B[si - 1];
			I[si - 2] = I[si - 1];
			si--;
		}
		if (pt >= si) pt = si - 1;
	}
	ll query(ll x) {
		if (si == 0) {
			return (1ll << 60);
		}
		if (pt >= si) {
			pt = si - 1;
		}
		while (pt < si - 1 && M[pt + 1] * x + B[pt + 1] < M[pt] * x + B[pt]) {
			pt++;
		}
		return M[pt] * x + B[pt];
	}
};

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);

	int n, m, p;
	cin >> n >> m >> p;

	vll d(n - 1, 0), h(m, 0), t(m, 0);
	for (int i = 0; i < n - 1; i++) {
		cin >> d[i];
	}

	for (int i = 0; i < m; i++) {
		cin >> h[i] >> t[i];
	}
	vll dist(n, 0);
	for (int i = 1; i < n; i++) {
		dist[i] = dist[i - 1] + d[i - 1];
	}

	ll ans = 0;
	vll realTime(m, 0);
	for (int i = 0; i < m; i++) {
		//ans += max(0LL, dist[h[i] - 1] - t[i]);
		realTime[i] = t[i] - dist[h[i] - 1];
	}

	map<ll, ll> occ;
	for (int i = 0; i < m; i++) {
		occ[realTime[i]]++;
	}

	vector<pll> TimeWeights;
	for (auto it : occ)
		TimeWeights.push_back(it);
	reverse(TimeWeights.begin(), TimeWeights.end());

	int N = TimeWeights.size();
	vll s(N, 0);
	s[N - 1] = TimeWeights[N - 1].second;
	for (int i = N - 2; i >= 0; i--) {
		s[i] = TimeWeights[i].second + s[i + 1];
	}

	ll total = 0;
	ll temp = TimeWeights[0].first;
	for (int i = 0; i < N; i++) {
		total += (temp - TimeWeights[i].first) * TimeWeights[i].second;
	}

	ConvexHullTrick convexHull;
	vector<vll> dp(p, vll(N, 0));

	for (int feeders = 0; feeders <= p - 1; feeders++) {
		for (int cur = N - 1; cur >= 0; cur--) {
			dp[feeders][cur] = max(0LL, -convexHull.query(TimeWeights[cur].first));
		}
		convexHull.init();
		for (int cur = N - 1; cur >= 0; cur--) {
			convexHull.add(-s[cur], s[cur] * TimeWeights[cur].first - dp[feeders][cur], cur);
		}
	}

	ans -= dp[p - 1][0];
	ans += total;

	cout << ans << "\n";
}