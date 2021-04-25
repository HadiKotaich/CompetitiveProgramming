int mask = 5;
int cur = mask;
while (true) {
	cout << bitset<10>(cur).to_string() << "\n";
	if (cur == 0) break;
	else cur = (cur - 1) & mask;
}
	