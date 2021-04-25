int NumOfGoodCyclicalShifts(string s) {
	// it is the number of occurence of the minimum value in the prefix sum after we check equality of open and closed parenthesis
	if (s.size() % 2 != 0) return 0;

	vi occ(2 * s.size() + 5, 0);
	int curSum = 0;
	int numOfOpen = 0;
	for (auto it : s) {
		curSum += ((it == ')') ? -1 : 1);
		numOfOpen += (it == '(');
		occ[curSum + s.size()]++;
	}
	if (numOfOpen != s.size() / 2) return 0;

	int ans = 0;
	for (int i = 0; i < occ.size(); i++) {
		if (occ[i] != 0) return occ[i];
	}
}