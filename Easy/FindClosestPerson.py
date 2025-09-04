class Solution(object):
    def findClosest(self, x, y, z):
        """
        :type x: int
        :type y: int
        :type z: int
        :rtype: int
        """
        pos1 = abs(x - z)
        pos2 = abs(y - z)
        

        if pos1 < pos2:
            return 1
        elif pos2 < pos1: 
            return 2
        else: 
           return 0

    
        