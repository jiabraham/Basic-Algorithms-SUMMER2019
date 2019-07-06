#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <string.h>

using namespace std;



int getLongestSubseqLength(std::vector<int> nums) {
  int longest = 1;
  int n = nums.size();
  for (int i = 0; i < nums.size(); i++) {
    int cur_longest = 1;
    for (int j = i; j < nums.size()-1; j++) {
      //cout << "Check " << j+1 << " " << j << " " << nums[j+1] << " " << nums[j] << endl;
      if (nums[j+1] % nums[j] == 0) {
	cur_longest++;
	if (cur_longest > longest) {
	  longest = cur_longest;
	  //cout << "Setting " << j+1 << " " << j << " LCS " << longest << endl;
	}
      }
      else {
	break;
      }
    }
  }
  return longest;
}

int main() {
  vector<int> nums;
  nums.push_back(1);
  nums.push_back(2);
  nums.push_back(3);
  nums.push_back(8);
  nums.push_back(16);
  nums.push_back(32);
  nums.push_back(64);
  cout << "Longest Subsequence = " << getLongestSubseqLength(nums) << endl;
  
}
