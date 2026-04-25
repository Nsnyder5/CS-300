#Course Advising System - Hash Table Implementation
What was the problem you were solving in the projects for this course? 

The primary problem was to design and implement a course advising system for ABCU that could process raw data files and manage a large catalog of courses. The system needed to provide academic advisors with a reliable way to look up course titles and prerequisites while ensuring that the underlying data was valid and consistent.

How did you approach the problem? 

I considered why data structures are important to understand. I approached the problem by evaluating the performance tradeoffs of three different data structures: Vectors, Hash Tables, and Binary Search Trees. Understanding data structures was critical because the choice directly impacts the efficiency of the software; for example, a Hash Table or Map was chosen for the final implementation because it offers constant time average case search, which is much faster than the linear search required for a Vector. This ensures that as the course catalog grows, advisors do not experience delays when searching for specific course information.

How did you overcome any roadblocks you encountered while going through the activities or project? 

One significant roadblock was ensuring that every listed prerequisite actually existed in the source file before creating a course entry. I overcame this by implementing a two pass file loading logic: the first pass reads the entire file to collect all valid course numbers into a set, and the second pass validates the prerequisites against that set before final storage. I also addressed potential formatting issues by using a custom trim function and comma splitting logic to handle extra whitespace or empty parameters within the data file.

How has your work on this project expanded your approach to designing software and developing programs? 

This project expanded my approach by shifting my focus from simple functionality to computational efficiency and Big O analysis. I learned to choose algorithms and data structures based on the most frequent user actions, such as prioritizing the lightning fast lookups of a Hash Table even if it meant a slightly higher complexity for sorting. It also reinforced the importance of planning with pseudocode to map out complex logic like recursive tree insertions before writing the actual C plus plus code.

How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable? 

My work has evolved toward a more modular and defensive programming style. I now write programs that are divided into clear, single purpose functions such as loading data, printing the full catalog, and searching for individual courses, which makes the code much easier to maintain or update. Additionally, I prioritized readability by using descriptive naming conventions and consistent formatting, and I improved adaptability by using standard library containers that can handle varying amounts of data without requiring core logic changes.
