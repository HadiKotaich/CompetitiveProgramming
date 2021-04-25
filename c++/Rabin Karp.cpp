struct RabinKarp { // Rabin Karp
	const int mod[2] = { (int)1e9 + 7, (int)1e9 + 9 };
	const int prime[2] = { 179, 233 };
	vector<vll> pPow, h;
	int modSz;

	RabinKarp(string& s, int modSz = 2) : modSz(modSz) {
		pPow.resize(s.size() + 9, vll(modSz));
		h.resize(s.size() + 9, vll(modSz));
		init(s);
	}

	void init(const string& s) {
		if (!pPow[0][0]) {
			for(int z = 0; z < modSz; z++)
				pPow[0][z] = 1;
			for (int i = 1; i <= SZ(s); ++i)
				for (int z = 0; z < modSz; ++z)
					pPow[i][z] = pPow[i - 1][z] * prime[z] % mod[z];
		}
		for (int i = 0; i < SZ(s); ++i)
			for (int z = 0; z < modSz; ++z)
				h[i + 1][z] = (h[i][z] * prime[z] + s[i]) % mod[z];
	}

	array<ll, 2> getHash(const string & s) {
		array<ll, 2> curhash = { 0, 0 };
		for (int i = 0; i < SZ(s); ++i)
			for (int z = 0; z < modSz; ++z)
				curhash[z] = (curhash[z] * prime[z] + s[i]) % mod[z];

		return curhash;
	}

	array<ll, 2> query(const int l, const int r) {
		array<ll, 2> res = {0, 0};
		for (int z = 0; z < modSz; ++z) {
			res[z] = (h[r + 1][z] - h[l][z] * pPow[r - l + 1][z]) % mod[z];
			if (res[z] < 0)
				res[z] += mod[z];
		}
		return res;
	}

	ll queryLL(const int l, const int r){
		auto a = query(l, r);
		return ((a[0] << 32) + a[1]);
	}
};
/*
	// returns the hash of (s[l, r] repeated m times)
	array<ll, 2> extendedHash(const int l, const int r, const int m){
		auto res = query(l, r);
		int len = (r - l + 1);
		// 1 + p^len + p ^ 2len + ... + p (m - 1) len
		// (p ^(len)(m + 1) - 1 ) / ( p ^ len - 1)
		for(int z = 0; z < modSz; ++z)
		{
			ll plen = pPow[len][z];
			res[z] *= powe(plen, m, mod[z]);
			res[z] %= mod[z];
			res[z] *= modinv(plen - 1, mod[z]);
			res[z] %= mod[z];
			if (res[z] < 0)
				res[z] += mod[z];
		}
		return res;
	}
	// returns the hash from l to r without index x, i.e. the string s[l ... x - 1] + s[x + 1 ... r]
	array<ll, 2> query_minus(const int l, const int r, const int x) {
		array<ll, 2> res = {0, 0};
		for (int z = 0; z < modSz; ++z) {
			if(x >= l && x <= r){
				ll left = (h[x][z] - h[l][z] * pPow[x - l][z]) % mod[z];
				ll right = (h[r + 1][z] - h[x + 1][z] * pPow[r - x][z]) % mod[z];
				res[z] = (right + left * pPow[r - x][z]) % mod[z];
			}
			else res[z] = (h[r + 1][z] - h[l][z] * pPow[r - l + 1][z]) % mod[z];
			if (res[z] < 0)
				res[z] += mod[z];
		}
		return res;
	}
*/

