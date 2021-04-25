// choose k , n
ll MOD = 1e9 + 7;
vvl ch;
void init(int k, int n) {
	ch = vector<vll>(k + 1, vll(n + 1, 0));
	for (int i = 0; i < n; i++)
		ch[0][i] = 1;
	for (int i = 1; i <= k; i++) {
		for (int j = i; j <= n; j++) {
			ch[i][j] = ch[i - 1][j - 1] + ch[i][j - 1];
			ch[i][j] %= MOD;
		}
	}
}

ll choose(ll n, ll k){
	if(n < k) return 0;
	return ch[k][n];
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code for getting all divisors (not necessarly primes) and mobius function

void getDivisorsAndMobius(int mx, vector<vi> &divisors, vi &mobius) {
	divisors = vector<vi>(mx + 1);
	mobius = vi(mx + 1);
	for (int i = 1; i <= mx; i++){
		for (int j = i; j <= mx; j += i)
			divisors[j].push_back(i);
		if (i == 1)
			mobius[i] = 1;
		else if ((i / divisors[i][1]) % divisors[i][1] == 0)
			mobius[i] = 0;
		else
			mobius[i] = -mobius[i / divisors[i][1]];
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// counts the number of elements coprime with num in the array if cnt[d] is the number of elements that are multiples of d
int countCoprime(int num, vi &cnt, vi & mobius, vector<vi> & divisors) {
	ll ans = 0;
	for (auto it : divisors[num])
		ans += (ll)mobius[it] * cnt[it];
	return ans;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// very fast: for m = 10^7 works in 0.1s
std::vector<bool> sieve(int m) { // m = max_inclusive
	std::vector<bool> pr(m + 1, 1); pr[0] = pr[1] = 0;
	for (int x = 2; x*x <= m; ++x) if (pr[x])
		for (int y = x*x; y <= m; y += x) pr[y] = 0;
	return pr;
} 
vi listOfPrimes(int m) {
	// m=max_inclusive
	std::vector<bool> pr = sieve(m); vi ans;
	for(int i = 1; i <= m;i++)
		if (pr[i]) ans.push_back(i); 
	return ans;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef std::vector<std::pair<ll, int> > Fact;
Fact factorize(ll x) { // factorization: {<prime, multiplicity>}
	Fact res;
	for (int d = 2; (ll)d*d <= x; ++d) {
		if (x % d == 0) {
			int k = 0; while (x % d == 0) { x /= d; ++k; } res.push_back({ d, k });
		}
	}
	if (x != 1) res.push_back({ x, 1 }); return res;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int MxN = 10000000;
int spf[MxN + 1];

void sieve() {
	vi primes;
	spf[0] = spf[1] = 1;
	for (int i = 2; i <= MxN; i++) {
		if (spf[i] == 0) {
			primes.push_back(i);
			spf[i] = i;
		}
		for (int j = 0; j < SZ(primes) && primes[j] <= spf[i] && i * primes[j] <= MxN; j++)
			spf[i * primes[j]] = primes[j];
	}
}

vector<pii> factorize(int x){
	vector<pii> ret;
	while(x > 1){
		int y = spf[x];
		ret.push_back({y, 0});
		while(x % y == 0) x /= y, ret.back().second++;
	}
	return ret;
}

void getAllFactorsRec(int curFactor, int curPower, int curVal, vector<pii> &primeFactors, vi &ans){
	if(curFactor == SZ(primeFactors)) {
		ans.push_back(curVal);
		return;
	}
	getAllFactorsRec(curFactor + 1, 0, curVal, primeFactors, ans);
	if(curPower < primeFactors[curFactor].second)
		getAllFactorsRec(curFactor, curPower + 1, curVal * primeFactors[curFactor].first, primeFactors, ans);
}

vi getAllFactors(vector<pii> primeFactors){
	vi ans;
	getAllFactorsRec(0, 0, 1, primeFactors, ans);
	return ans;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ll powe(ll x, ll p) {
	if (p == 0) return 1;
	ll w = powe((x*x) % MOD, p / 2);
	if (p & 1) w = (w*x) % MOD;
	return w;
}
// modular inverse (MOD must be prime)
ll modinv(ll i) { return powe(i, MOD - 2); }


