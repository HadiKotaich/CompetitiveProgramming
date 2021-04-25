/*
dp(k,i)=min for all j less than or equal i, {dp(k−1, j - 1)+C(j,i)}
where C(k,j) is some cost function.
Say 1≤i≤n and 1≤j≤m, and evaluating C takes O(1) time. 
Let opt(i,j) be the value of k that minimizes the above expression. If opt(i,j)≤opt(i,j+1) for all i,j, then we can apply divide-and-conquer DP. 
*/
ll INF = 1e18;
ll C(ll i, ll j);
vll dp_before, dp_cur;
void compute(int l, int r, int optl, int optr){
    if (l > r) return;
    int mid = (l + r) >> 1;
    pair<long long, int> best = {INF, -1};
    for (int j = optl; j <= min(mid, optr); j++) {
        best = min(best, {dp_before[j - 1] + C(j, mid), j});
    }
    dp_cur[mid] = best.first;
    int opt = best.second;
    compute(l, mid - 1, optl, opt);
    compute(mid + 1, r, opt, optr);
}