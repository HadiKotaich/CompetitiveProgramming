struct ZAlgo {

	vi Z, matches;

	// Fills Z array for given string str[] 
	// Z[i] is the length of the match between
	// suffix starting at i and the suffix starting at 0
	void getZarr(vi& str) {
		Z = vi(str.size(), 0);
		int n = str.size();
		int L, R, k;
		L = R = 0;
		for (int i = 1; i < n; ++i) {
			if (i > R) {
				L = R = i;
				while (R < n && str[R - L] == str[R])
					R++;
				Z[i] = R - L;
				R--;
			}
			else {
				k = i - L;

				if (Z[k] < R - i + 1)
					Z[i] = Z[k];
				else {
					L = i;
					while (R < n && str[R - L] == str[R])
						R++;
					Z[i] = R - L;
					R--;
				}
			}
		}
		Z[0] = str.size();
	}

	// returns all occurrences of pattern in text using Z algo 
	void getMatches(vi& text, vi& pattern, int specialChar = -1) {
		vi concat(pattern);
		concat.push_back(specialChar);
		concat.insert(concat.end(), text.begin(), text.end());
		int l = concat.size();

		getZarr(concat);

		matches = vi();
		for (int i = pattern.size() + 1; i < l; ++i) {
			if (Z[i] == pattern.size())
				matches.push_back(i - pattern.size() - 1);//-pattern.lenght() because of the concatination
		}
	}

	void getMatches(string& text, string& pattern, int specialChar = -1) {
		vi textVec = FromStringToVec(text);
		vi patternVec = FromStringToVec(pattern);
		getMatches(textVec, patternVec, specialChar);
	}

	vi FromStringToVec(string &s) {
		vi ans;
		ans.insert(ans.end(), s.begin(), s.end());
		return ans;
	}
};