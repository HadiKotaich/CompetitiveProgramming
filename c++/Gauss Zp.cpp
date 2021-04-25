ll MOD = 1e9 + 7;
ll mpow(ll a, ll b, ll P = MOD) {
	a %= P; ll w = 1;
	while (b) {
		if (b % 2) w = (w * a) % P;
		a = (a * a) % P;
		b /= 2;
	}
	return w;
} 
inline ll rev(ll x, ll P) {
	return mpow(x, P - 2, P);
}
// for large P, add casts to ll where needed!
// returns number of free variables
// rank is n - gaussZp
int GaussZp(vector<vll> A, vll b, vll& x, ll P = MOD) {
	int m = SZ(A), n = SZ(A[0]), k, r;
	vi q;
	for (k = 0; k < min(m, n); k++) {
		int i, j;
		for (j = k; j < n; j++) for (i = k; i < m; i++) if (A[i][j]) goto found;
		break;
	found:
		if (j != k) FOR(t, m) swap(A[t][j], A[t][k]);
		q.pb(j);
		if (i != k) { swap(A[i], A[k]); swap(b[i], b[k]); }
		REP(j, k + 1, m - 1) if (A[j][k]) {
			ll l = (A[j][k] * rev(A[k][k], P)) % P;
			REP(i, k, n - 1) A[j][i] = (P + A[j][i] - (l * A[k][i]) % P) % P;
			b[j] = (P + b[j] - (l * b[k]) % P) % P;
		}
	}
	r = k; x.clear(); x.resize(n, 0);
	REP(k, r, m - 1) if (b[k]) return -1;
	REPD(k, r - 1, 0) {
		ll s = b[k];
		REP(j, k + 1, r - 1) s = (P + s - (A[k][j] * x[j]) % P) % P;
		x[k] = (s * rev(A[k][k], P)) % P;
	}
	REPD(k, r - 1, 0) swap(x[k], x[q[k]]);
	return n - r;
}