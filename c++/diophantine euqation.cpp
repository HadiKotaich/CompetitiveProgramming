// a*x - b*y = return value = GCD(a,b).
ll egcd(ll a, ll b, ll &x, ll &y) { // watch out: MINUS B*Y
	if (b == 0) {
		x = 1;
		y = 0;
		return a;
	}
	ll d = egcd(b, a%b, y, x);
	y = a - x*(a / b) - y;
	x = b - x;
	return d;
}

struct sol {
	bool valid;
	ll x0, coeffX;
	ll y0, coeffY;

	sol(bool _valid = false,ll _x0 = 0, ll _coeffX = 0, ll _y0 = 0, ll _coeffY = 0) {
		x0 = _x0;
		coeffX = _coeffX;
		y0 = _y0;
		coeffY = _coeffY;
		valid = _valid;
	}

	void print() {
		if (!valid) {
			cout << "sol does not exist\n";
		}
		else {
			cout << "x= " << x0 << "+ t* " << coeffX<<"\n";
			cout << "y= " << y0 << "+ t* " << coeffY << "\n";
		}
	}
};

sol diophSol(ll coeffX, ll coeffY, ll C) {
	ll x0 = 0;
	ll y0 = 0;
	ll g = egcd(coeffX, coeffY, x0, y0);
	if (C%g != 0) {
		return sol();
	}
	else {
		sol ans;
		ans.valid = true;
		ans.x0 = x0*(C / g);
		ans.y0 = y0*(-C / g);
		ans.coeffX = coeffY / g;
		ans.coeffY = -coeffX / g;
		return ans;
	}
}

pll getAns(ll coeffX, ll coeffY, ll C) {
	sol s = diophSol(coeffX, coeffY, C);
	pll ans = { C,1 };
	if (s.valid){
		//
	}
	return ans;
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0), cout.tie(0);
	
	int T;
	cin >> T;
	preCalc();
	while (T--) {
		ll n;
		cin >> n;
		pll ans = { n - 1, 1 };
		for (int k = 1; k < 55; k++) {
			ll coeffA = fib[k - 1];
			ll coeffB = fib[k];
			pll cur = getAns(coeffA, coeffB, n);
			ans = min(ans, cur);
		}
		cout << ans.second <<" "<<ans.first << "\n";
	}

	//system("pause");
	return 0;
}