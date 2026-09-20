# Fitness & Nutrition CLI Tracker
#### Video Demo:  [ HERE](https://youtu.be/P8ZJgfaPiNo)
#### Description:

My final project for CS50 is a Command-Line Interface (CLI) application written in C that utilizes a SQLite database to track strength training workouts and nutritional supplementation. As a Systems Engineering student, I wanted to apply systems logic to build a tool that solves a personal need: having a fast, reliable, and distraction-free way to log specific exercises, such as hip thrusts and back extensions, and monitor my daily intake of supplements like whey protein isolate and creatine monohydrate directly from the terminal.

By Valerie Marín - Lechería, Venezuela.

## Features
*   **Workout Tracking:** Users can input the date, exercise name, sets, reps, and weight lifted.
*   **Nutrition Tracking:** Allows users to log the exact grams of supplements consumed on a given date.
*   **Automated Database Setup:** The program automatically generates the `fitness.db` file and the necessary tables.
*   **Strict Input Validation:** Ensures that all dates are logically correct (YYYY-MM-DD) and prevents the input of negative values for weights and grams.

## Files in this Project
*   `main.c`: This is the core source code file. It contains the C logic, including the `main` loop for the interactive menu, functions for database initialization (`setup_database`), data insertion (`log_workout`, `log_nutrition`), data retrieval (`view_history`), and robust error handling (`is_valid_date`).
*   `fitness.db`: A SQLite database file generated automatically upon the first execution. It contains two tables: `workouts` and `nutrition`.

## Design Choices
When designing this application, I had to make several technical decisions. Initially, I considered building a web application, but I chose a C-based CLI approach to solidify my understanding of memory management, pointers, and native SQLite integration in C. I wanted a lightweight environment that I could easily compile and run.

One major design choice was the implementation of the `is_valid_date` function. Instead of relying on complex external time libraries, I built a custom validation system that parses the string format and checks for logical month and day boundaries. Furthermore, I implemented `do-while` loops for all numerical inputs. This guarantees that users cannot accidentally break the database by entering negative sets, reps, or weights.

I also designed the SQLite database schema to be simple yet scalable. Using primary keys with `AUTOINCREMENT` allows for easy future expansions, such as deleting or updating specific rows. The callback function provided by the SQLite API was adapted to cleanly format the query results in the terminal.

In the future, I plan to expand this project by adding a feature to calculate the total weekly volume lifted and exporting the data to a CSV file for statistical analysis.

## Conclusion
Building this CLI Fitness & Nutrition Tracker was a challenging yet incredibly rewarding experience that tied together everything I learned throughout CS50. From understanding memory management and pointers in C to integrating a real-world SQLite database, this project pushed me to think like a software engineer and apply computational logic to a personal passion. I am proud to have created a practical, functional tool that I can genuinely use in my daily life to track my gym progress and nutrition. This course has given me a strong foundation in computer science, and I am excited to continue my journey in software development. Thank you CS50!
