#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

int main(){
	ordered_set<int>  s;
	s.insert(1); 
	s.insert(3);
	cout << s.order_of_key(2) << endl; // the number of elements in the s less than 2
	cout << *s.find_by_order(0) << endl; // print the 0-th smallest number in s(0-based)
}

/// multiset
template <typename T>
using ordered_multiset = tree<T, null_type, less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;

int main(){
	ordered_multiset<int>  s;
	s.insert(1); 
	s.insert(3);
	s.insert(2);
	cout << s.order_of_key(3) << endl; // the number of elements in the s less than 2
	s.erase(s.upper_bound(2));
	cout << s.order_of_key(3) << endl; // the number of elements in the s less than 2
	cout << *s.find_by_order(0) << endl; // print the 0-th smallest number in s(0-based)
}
