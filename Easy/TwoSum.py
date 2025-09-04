class Solution(object):
    def twoSum(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: List[int]
        """
        numbers = {}

        for i, num in enumerate(nums):
            x = target - num
            
            if x in numbers:
                return (i, numbers[x])

            else:
                numbers[num] = i

        
        return None








        