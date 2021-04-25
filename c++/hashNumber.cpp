// class to hash a number mod multiple primes
// can be useful when you can point to a state with a sum
vll mod;
struct hashNumber{
	vll a;
	hashNumber(ll x = 0){
		a.resize(SZ(mod));
		for(int i = 0; i < SZ(mod); i++)
			a[i] = x % mod[i];
	}
	hashNumber operator+(hashNumber x){
		hashNumber res;
		for(int i = 0; i < SZ(mod); i++){
			res.a[i] = (a[i] + x.a[i]) % mod[i];
		}
		return res;
	}
	bool operator==(const hashNumber& x)const {
		return a == x.a;
	}
};