# 1. String Reversal
# 👉 Question: Write a function to reverse a string.
# Python Example:

def reverse_string(s: str) -> str:
    return s[::-1]

# Test
print(reverse_string("hello"))  # "olleh"

#
# 2. Palindrome Check
# 👉 Question: Check if a string is a palindrome (same forward and backward).
# Python Example:

def is_palindrome(s: str) -> bool:
    return s == s[::-1]

print(is_palindrome("racecar"))  # True
print(is_palindrome("hello"))    # False
#

# 3. Find Max in a List/Array
# 👉 Question: Given an array of integers, return the maximum number.
# Python Example:

def find_max(arr):
    max_val = arr[0]
    for num in arr:
        if num > max_val:
            max_val = num
    return max_val

print(find_max([3, 5, 2, 9, 1]))  # 9
#
# 4. Unit Test Example (QA-Style)
# 👉 Question: Write a test for a function that adds two numbers.
# Python (pytest):

def add(a, b):
    return a + b


def test_add():
    assert add(2, 3) == 5
    assert add(-1, 1) == 0
    assert add(0, 0) == 0
#


#
# 👨‍💻 Serge (CTO at Reppa): Hi Jay, nice to meet you. Let’s jump right in.
# Here’s a warmup:
# 👉 Problem: Write a function that takes a list of integers and returns only the even numbers.
# Input: [1, 2, 3, 4, 5, 6]
# Output: [2, 4, 6]
# As you code, please:
# 1. Explain your approach.
# 2. Write the code.
# 3. Walk through a test case.
#
# You said: give me the script/answer
# ChatGPT said: (scripted guidance and examples)
#
# Step 1 (explanation): Loop through numbers; keep those where num % 2 == 0.
# Step 2 (code):
# Python (list comprehension) example:
# def filter_even_numbers(numbers):
#     return [num for num in numbers if num % 2 == 0]
#
# JavaScript example removed.
#
# Step 3 (example run): Input [1,2,3,4,5,6] -> Output [2,4,6]
# Wrap-up: O(n) time; % checks divisibility; filter/loop keeps evens.
#
# Your refined explanation: Iterate; use modulo 2; 0 => even, keep; else skip.
#
# Active Python implementation (loop):

def filter_even_numbers(numbers):
    even_numbers = []
    for num in numbers:
        if num % 2 == 0:
            even_numbers.append(num)
    return even_numbers

# Example
print(filter_even_numbers([1, 2, 3, 4, 5, 6]))  # Output: [2, 4, 6]
