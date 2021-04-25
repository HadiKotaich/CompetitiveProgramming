#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
using namespace std;

typedef long long ll;

namespace fft
{
	struct num
	{
		double x, y;
		num() { x = y = 0; }
		num(double x, double y) :x(x), y(y) {}
	};
	inline num operator+(num a, num b) { return num(a.x + b.x, a.y + b.y); }
	inline num operator-(num a, num b) { return num(a.x - b.x, a.y - b.y); }
	inline num operator*(num a, num b) { return num(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
	inline num conj(num a) { return num(a.x, -a.y); }

	int base = 1;
	vector<num> roots = { {0,0},{1,0} };
	vector<int> rev = { 0,1 };
	const double PI = acosl(-1.0);

	void ensure_base(int nbase)
	{
		if (nbase <= base) return;
		rev.resize(1 << nbase);
		for (int i = 0;i < (1 << nbase);i++)
			rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (nbase - 1));
		roots.resize(1 << nbase);
		while (base < nbase)
		{
			double angle = 2 * PI / (1 << (base + 1));
			for (int i = 1 << (base - 1);i < (1 << base);i++)
			{
				roots[i << 1] = roots[i];
				double angle_i = angle * (2 * i + 1 - (1 << base));
				roots[(i << 1) + 1] = num(cos(angle_i), sin(angle_i));
			}
			base++;
		}
	}

	void fft(vector<num>& a, int n = -1) {

		if (n == -1) n = a.size();
		assert((n & (n - 1)) == 0);
		int zeros = __builtin_ctz(n);
		ensure_base(zeros);
		int shift = base - zeros;

		for (int i = 0;i < n;i++) {
			if (i < (rev[i] >> shift)) {
				num tmp = a[i];
				a[i] = a[rev[i] >> shift];
				a[rev[i] >> shift] = tmp;
			}
		}

		for (int k = 1;k < n;k <<= 1)
		{
			for (int i = 0;i < n;i += 2 * k)
			{
				for (int j = 0;j < k;j++)
				{
					num z = a[i + j + k] * roots[j + k];
					a[i + j + k] = a[i + j] - z;
					a[i + j] = a[i + j] + z;
				}
			}
		}
	}

	vector<num> fa, fb;

	void fillf(vector<int>& k, vector<num>& fk, const int sz, const int m) {
		for (int i = 0; i < k.size(); i++) {
			int x = (k[i] % m + m) % m;
			fk[i] = num(x & ((1 << 15) - 1), x >> 15);
		}
		fill(fk.begin() + k.size(), fk.begin() + sz, num{ 0,0 });
		fft(fk, sz);
	}

	vector<int> multiply_mod(vector<int>& a, vector<int>& b, const int m, const int eq = 0)
	{
		int need = a.size() + b.size() - 1;
		int nbase = 0;
		while ((1 << nbase) < need) nbase++;
		ensure_base(nbase);
		int sz = 1 << nbase;
		if (sz > (int)fa.size()) fa.resize(sz);

		fillf(a, fa, sz, m);

		if (sz > (int)fb.size()) fb.resize(sz);
		if (eq) copy(fa.begin(), fa.begin() + sz, fb.begin());
		else fillf(b, fb, sz, m);

		double ratio = 0.25 / sz;
		num r2(0, -1), r3(ratio, 0), r4(0, -ratio), r5(0, 1);
		for (int i = 0;i <= (sz >> 1);i++)
		{
			int j = (sz - i) & (sz - 1);
			num a1 = (fa[i] + conj(fa[j]));
			num a2 = (fa[i] - conj(fa[j])) * r2;
			num b1 = (fb[i] + conj(fb[j])) * r3;
			num b2 = (fb[i] - conj(fb[j])) * r4;
			if (i != j)
			{
				num c1 = (fa[j] + conj(fa[i]));
				num c2 = (fa[j] - conj(fa[i])) * r2;
				num d1 = (fb[j] + conj(fb[i])) * r3;
				num d2 = (fb[j] - conj(fb[i])) * r4;
				fa[i] = c1 * d1 + c2 * d2 * r5;
				fb[i] = c1 * d2 + c2 * d1;
			}
			fa[j] = a1 * b1 + a2 * b2 * r5;
			fb[j] = a1 * b2 + a2 * b1;
		}
		fft(fa, sz);fft(fb, sz);
		vector<int> res(need);
		for (int i = 0;i < need;i++)
		{
			ll aa = fa[i].x + 0.5;
			ll bb = fb[i].x + 0.5;
			ll cc = fa[i].y + 0.5;
			res[i] = (aa + ((bb % m) << 15) + ((cc % m) << 30)) % m;
		}
	}
}