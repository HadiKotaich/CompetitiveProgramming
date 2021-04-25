ll MOD;
std::unordered_map<ll, ll>log_a_mod;
ll MOD_INV_CURPOWER[20000];// MOD_INV_CURPOWER[i]=modinv(a^(i*radp))

ll powe(ll x, ll p) {
	if (p == 0) return 1;
	ll w = powe((x*x) % MOD, p / 2);
	if (p & 1) w = (w*x) % MOD;
	return w;
}
// modular inverse (MOD must be prime)
ll modinv(ll i) { return powe(i, MOD - 2); }


ll gcd(ll a, ll b) {
	ll r, i;
	while (b != 0) {
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

//if the log does not exist it return MOD+1
ll dicrete_modular_log(int a, int cand) {
	ll rad_p = std::sqrt(MOD);
	ll cur_pow = 1;
	ll factor = powe(a, rad_p) % MOD;
	factor += MOD;
	factor %= MOD;
	for (int i = 0; i <= rad_p + 1; i++) {
		ll comp = (cand*MOD_INV_CURPOWER[i]) % MOD;
		comp += MOD;
		comp %= MOD;
		if (log_a_mod.find(comp) != log_a_mod.end()) {
			return i*rad_p + log_a_mod[comp];
		}
		cur_pow = (cur_pow*factor) % MOD;
		cur_pow += MOD;
		cur_pow %= MOD;
	}
	return MOD + 1;
}

void init(ll a){
	ll rad_p = std::sqrt(MOD);
	ll cur_pow = 1;
	for (int i = 0; i <= rad_p + 1; i++) {
		if (log_a_mod.find(cur_pow) == log_a_mod.end())
			log_a_mod[cur_pow] = i;
		cur_pow *= a;
		cur_pow %= MOD;
		cur_pow += MOD;
		cur_pow %= MOD;
	}
	cur_pow = 1;
	ll factor = powe(a, rad_p) % MOD;
	factor += MOD;
	factor %= MOD;
	for (int i = 0; i <= rad_p + 1; i++) {
		MOD_INV_CURPOWER[i] = modinv(cur_pow);
		cur_pow *= factor;
		cur_pow %= MOD;
		cur_pow += MOD;
		cur_pow %= MOD;
	}

}