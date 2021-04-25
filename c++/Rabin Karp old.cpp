struct RabinKarp { // Rabin Karp
	const int mod[2] = { (int)1e9 + 7, (int)1e9 + 9 };
	const int prime[2] = { 179, 233 };
	vector<vll> pPow, h;

	RabinKarp(string& s) {
		pPow.resize(s.size() + 9, vll(2));
		h.resize(s.size() + 9, vll(2));
		init(s);
	}

	void init(const string& s) {
		if (!pPow[0][0]) {
			pPow[0][0] = pPow[0][1] = 1;
			for (int i = 1; i <= s.size(); ++i)
				for (int z = 0; z < 2; ++z)
					pPow[i][z] = pPow[i - 1][z] * prime[z] % mod[z];
		}
		for (int i = 0; i < (int)s.size(); ++i)
			for (int z = 0; z < 2; ++z)
				h[i + 1][z] = (h[i][z] * prime[z] + s[i]) % mod[z];
	}

	array<ll, 2> getHash(const string & s) {
		array<ll, 2> curhash = { 0, 0 };
		for (int i = 0; i < (int)s.size(); ++i)
			for (int z = 0; z < 2; ++z)
				curhash[z] = (curhash[z] * prime[z] + s[i]) % mod[z];

		return curhash;
	}

	array<ll, 2> query(const int l, const int r) {
		array<ll, 2> res;
		for (int z = 0; z < 2; ++z) {
			res[z] = (h[r + 1][z] - h[l][z] * pPow[r - l + 1][z]) % mod[z];
			if (res[z] < 0)
				res[z] += mod[z];
		}
		return res;
	}

};

ll to_ll(array<ll, 2> a) {
	return ((a[0] << 32) + a[1]);
}

/*
// gets the hash of s[l, r] repeated m times
array<ll, 2> extendedHash(const int l, const int r, const int m){
		auto res = query(l, r);
		int len = (r - l + 1);
		// 1 + p^len + p ^ 2len + ... + p (m - 1) len
		// (p ^(len)(m + 1) - 1 ) / ( p ^ len - 1)
		for(int z = 0; z < 2; ++z)
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

*/
