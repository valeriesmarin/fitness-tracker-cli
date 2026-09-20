#include <cs50.h>
#include <ctype.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
bool is_valid_date(string date);
void setup_database(sqlite3 *db);
void log_workout(sqlite3 *db);
void log_nutrition(sqlite3 *db);
void view_history(sqlite3 *db);
void delete_record(sqlite3 *db);

int main(void)
{
    sqlite3 *db;
    int rc;

    // Open the SQLite database connection (creates file if it doesn't exist)
    rc = sqlite3_open("fitness.db", &db);
    if (rc)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Initialize tables if they don't exist
    setup_database(db);

    int choice = 0;

    // Main application loop
    while (choice != 5)
    {
        printf("\n===================================\n");
        printf("    FITNESS & NUTRITION TRACKER    \n");
        printf("===================================\n");
        printf("1. Log a new workout\n");
        printf("2. Log nutrition / supplements\n");
        printf("3. View history\n");
        printf("4. Delete a record\n");
        printf("5. Exit\n");
        printf("===================================\n");

        choice = get_int("Choose an option (1-5): ");

        switch (choice)
        {
            case 1:
                log_workout(db);
                break;
            case 2:
                log_nutrition(db);
                break;
            case 3:
                view_history(db);
                break;
            case 4:
                delete_record(db);
                break;
            case 5:
                printf("\nKeep up the good work. Goodbye!\n");
                break;
            default:
                printf("\nInvalid option. Please choose between 1 and 5.\n");
        }
    }

    // Close the database connection safely
    sqlite3_close(db);
    return 0;
}

// Callback function to process and print SQLite SELECT results
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        printf("%s: %s | ", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

// Validates if a date string strictly follows YYYY-MM-DD format and logical values
bool is_valid_date(string date)
{
    if (strlen(date) != 10)
        return false;
    if (date[4] != '-' || date[7] != '-')
        return false;

    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3)
        return false;

    if (year < 2000 || year > 2100)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;

    // Basic month length check
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return false;
    if (month == 2 && day > 29)
        return false;

    return true;
}

// Creates the required tables in the database if they don't already exist
void setup_database(sqlite3 *db)
{
    char *zErrMsg = 0;

    const char *sql_workouts = "CREATE TABLE IF NOT EXISTS workouts ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "date TEXT NOT NULL, "
                               "exercise TEXT NOT NULL, "
                               "sets INTEGER NOT NULL, "
                               "reps INTEGER NOT NULL, "
                               "weight REAL NOT NULL);";

    const char *sql_nutrition = "CREATE TABLE IF NOT EXISTS nutrition ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "date TEXT NOT NULL, "
                                "supplement TEXT NOT NULL, "
                                "grams REAL NOT NULL);";

    sqlite3_exec(db, sql_workouts, 0, 0, &zErrMsg);
    sqlite3_exec(db, sql_nutrition, 0, 0, &zErrMsg);
}

// Handles user input and database insertion for workouts
void log_workout(sqlite3 *db)
{
    char *zErrMsg = 0;
    string date;

    printf("\n--- Log Workout ---\n");

    // Robust date input loop
    do
    {
        date = get_string("Date (YYYY-MM-DD): ");
    }
    while (!is_valid_date(date));

    string exercise = get_string("Exercise name (e.g., Hip Thrust): ");

    int sets;
    do
    {
        sets = get_int("Sets: ");
    }
    while (sets <= 0);

    int reps;
    do
    {
        reps = get_int("Reps: ");
    }
    while (reps <= 0);

    float weight;
    do
    {
        weight = get_float("Weight in kg: ");
    }
    while (weight < 0);

    char sql[512];
    sprintf(sql,
            "INSERT INTO workouts (date, exercise, sets, reps, weight) VALUES ('%s', '%s', %i, %i, "
            "%.2f);",
            date, exercise, sets, reps, weight);

    int rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        printf("Workout logged successfully!\n");
    }
}

// Handles user input and database insertion for nutrition/supplements
void log_nutrition(sqlite3 *db)
{
    char *zErrMsg = 0;
    string date;

    printf("\n--- Log Nutrition ---\n");

    do
    {
        date = get_string("Date (YYYY-MM-DD): ");
    }
    while (!is_valid_date(date));

    string supplement = get_string("Supplement (e.g., Whey Protein, Creatine): ");

    float grams;
    do
    {
        grams = get_float("Amount in grams: ");
    }
    while (grams <= 0);

    char sql[512];
    sprintf(sql, "INSERT INTO nutrition (date, supplement, grams) VALUES ('%s', '%s', %.2f);", date,
            supplement, grams);

    int rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        printf("Nutrition logged successfully!\n");
    }
}

// Queries and displays data from the database
void view_history(sqlite3 *db)
{
    char *zErrMsg = 0;
    printf("\n--- View History ---\n");
    printf("1. View Workouts\n");
    printf("2. View Nutrition\n");

    int view_choice = get_int("What do you want to see? (1 or 2): ");
    char *sql;

    if (view_choice == 1)
    {
        sql = "SELECT id, date, exercise, sets, reps, weight FROM workouts ORDER BY date DESC;";
        printf("\n--- Workout History ---\n");
    }
    else if (view_choice == 2)
    {
        sql = "SELECT id, date, supplement, grams FROM nutrition ORDER BY date DESC;";
        printf("\n--- Nutrition History ---\n");
    }
    else
    {
        printf("Invalid choice.\n");
        return;
    }

    int rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

// Deletes a specific record by ID from either table
void delete_record(sqlite3 *db)
{
    char *zErrMsg = 0;
    printf("\n--- Delete Record ---\n");
    printf("1. Delete from Workouts\n");
    printf("2. Delete from Nutrition\n");

    int table_choice = get_int("Choose table (1 or 2): ");

    if (table_choice != 1 && table_choice != 2)
    {
        printf("Invalid choice. Returning to main menu.\n");
        return;
    }

    printf("(Tip: If you don't know the ID, go back and use 'View history' first)\n");
    int id = get_int("Enter the ID of the record you want to delete: ");

    char sql[256];
    if (table_choice == 1)
    {
        sprintf(sql, "DELETE FROM workouts WHERE id = %i;", id);
    }
    else
    {
        sprintf(sql, "DELETE FROM nutrition WHERE id = %i;", id);
    }

    int rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        printf("Record deleted successfully!\n");
    }
}
