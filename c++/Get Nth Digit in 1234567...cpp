ll k;
 
ll getDig(ll smallBlock) {
	ll len = 1;
	ll num = 9;
	ll start = 1;
	for (; len < 18; len++) { // maybe it should be <= 18, did not test it on 10^18
		if (k <= len * num) break;
		k -= len * num;
		num *= 10;
		start *= 10;
	}
 
	ll skip = (k - 1) / len;
	start += skip;
	k -= skip * len;
 
	while (len > k) {
		start /= 10;
		len--;
	}
	return start % 10;
}
 
int main() {
	ios::sync_with_stdio(0);
	cin.tie(0), cout.tie(0);
 
	cin >> k;
	ll dig = getDig(1e12 + 10);
    cout << dig << "\n";
}