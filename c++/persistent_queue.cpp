#define fi first
#define se second
 
const int N = 200007;
typedef pair<int, int> ii;
 
ii ver[N];
int val[N];
 
int pred[200007][32];
 
int t;
 
void push(int v, int x) {
	t++;
	pred[t][0] = ver[v].se;
 
	for (int i = 1; i < 30; ++i) {
		pred[t][i] = pred[pred[t][i - 1]][i - 1];
	}
 
	ver[t] = ii(ver[v].fi, t);
	val[t] = x;
}
 
int pop(int v) {
	t++;
	int pai = ver[v].fi;
 
	int x = ver[v].se;
	for (int i = 30; i >= 0; --i) {
		if (pred[x][i] > pai) x = pred[x][i];
	}
	pai = x;
 
	ver[t] = ii(pai, ver[v].se);
	return val[pai];
}