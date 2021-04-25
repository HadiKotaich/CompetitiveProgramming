ll Xor(ll n){
	// get xor of elements from 1 to n
	switch (n & 3) // n % 4  
	{
	case 0: return n;     // if n is multiple of 4 
	case 1: return 1;     // If n % 4 gives remainder 1   
	case 2: return n + 1; // If n % 4 gives remainder 2     
	case 3: return 0;     // If n % 4 gives remainder 3   
	}
}

ll rangeXor(ll left, ll right) {
	// [left, right] inclusive
	ll ans = (left == 0) ? Xor(right):  Xor(left - 1) ^ Xor(right);
	return ans;
}
