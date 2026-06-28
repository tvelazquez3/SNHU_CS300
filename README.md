# SNHU_CS300
DSA: Analysis and Design

The goal of these proects was to design and implement an advising program for ABC University that allows advisors to efficiently load, search, and display course information. Over the course of these projects, I analyzed several different data structures to determine which one would provide the best balance of performance, memory usage, and functionality for the system. My analysis concluded that a binary search tree was the best choice here because it supports efficient searching while naturally printing courses in alphanumeric order through an in-order traversal.

I began by comparing vectors, hash tables, and binary search trees to understand their strengths and weaknesses in terms of runtime complexity and memory usage. This helped me select the most appropriate structure before writing the application. Understanding data structures is important because the choice will directly affect how efficiently data is stored, searched, and maintained as a program becomes larger and more complex. 

One of the biggest challenges was correctly loading the course data while ensuring that every prerequisite actaully existed in the input file. I solved this by first storing all course numbers, validating the prerequisites after reading the file, and only inserting valid courses into the binary search tree. 

This project reinforced the importance of planning before writing code. Instead of immediately beginning implementation, I spent time analyzing data structures, designed program flow, and developed the solution based on those decisions. It was also nice to gain experience breaking a larger project down into smaller, more digestable functions. 

This project taught me the value of writing organized, modular code. I used descriptive function names, separated responsibilities to helper functions, and kept related code together within the binary search tree class. This made the program easier to read, debug, and expand on. 
