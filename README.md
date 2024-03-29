# Find ALL the common substrings among 2 strings

## Features
 - Implemented in C++
 - Can be loaded as a module in Python3
 - Support UTF8!!!

 The main purpose of creating this program was to
 have a faster alternative of difflib.get_matching_blocks function.

 The involved algorithm of this program is exactly the same as that of difflib.get_matching_blocks,
 however, since this program is a native C++, while difflib is native Python, this program is much FASTER!

 I'm pretty sure that this will be helpful especially if you want to find all the substrings, not just the longest,
 among huge strings.

 Also, as a Japanese, I want to emphasize that this program support UTF8!
 For those who rely on UTF8 including Japanese, Chinese, Korean, etc., this is for you.

## Usage & How to compile
 Since this is C++ program, you need to first compile it.
 I have added Makefile to make this process easier. You can just type command "make" to compile it, hopefully.
 You may get some error depending on your environment, then you can report an issue or just edit the Makefile.

 Once you compile this properly, you will have a library file name "*.so".
 You can import this from Python3 just like a normal module: "import <MODULE NAME HERE>".

 Sample Code (Assuming the generated module is named "findCommon.so")
 ```python
 import findCommon

 string0 = 'Tokyo, Japan'
 string1 = 'Yokohama, Japan'
 min_length = 2

 matches = findCommon.findCommon(string0, string1, min_length)
 ```

## Requirement
 To make the implementation of the interface between C++ and Python3 simpler, Boost.Python is used in this program.
 Therefore make sure that you have installed boost library in prior to the compilation, otherwise it will fail.
