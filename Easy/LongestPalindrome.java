class Solution {
    public int longestPalindrome(String s) {
        if (s == null || s.isEmpty()) {
            return 0;
        }

        int[] charCounts = new int[128];
        for (int i = 0; i < s.length(); i++) {
            charCounts[s.charAt(i)]++;
        }

        int maxLength = 0;
        boolean hasOddCount = false;

        for (int count : charCounts) {
            if (count % 2 == 0) {
                maxLength += count;
            } else {
                maxLength += count - 1;
                hasOddCount = true;
            }
        }

        if (hasOddCount) {
            maxLength += 1;
        }
        return maxLength;
    }
}
