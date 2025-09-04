class Solution(object):
    def isPalindrome(self, x):
        """
        :type x: int
        :rtype: bool
        """

        if x < 0:
            return False
        
        original = x
        isPalindrome = 0

        while x > 0:
            last_digit = x % 10

            isPalindrome = isPalindrome * 10 + last_digit

            x = x // 10
            

        if isPalindrome == original:
            return True
        else:
            return False

        
# Best Solution

# class Solution(object):
#     def isPalindrome(self, x):
#         """
#         :type x: int
#         :rtype: bool
#         """
#         n_0 = str(x)
#         n_1 = n_0[::-1]

#         if n_0 == n_1:
#             return True
#         return False