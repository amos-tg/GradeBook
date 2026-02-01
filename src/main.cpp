#include <fstream>
#include <filesystem>
#include <string>

using namespace std;


// no nullptr checking

/// returns int number of students. 
/// Loads the student names into (string *students), and the scores
/// into 2D-array (int **scores) in an identical order to which student names
/// are loaded into (string *students).
int getScores(int **scores, string *students);

/// Produces the average for each row of scores within (int **scores), and loads
/// it into (double *average_scores) in a row-based identical ordering. 
void averageScores(int **scores, double *average_scores);

// it is faster and more memory efficient to directly format the letter
// grades into the final outputted string on a line by line basis during iteration
// of average_scores to produce letter grades than it would be to store another
// array containing letter grades and have a seperate function which takes both 
// arrays to produce the formatted report.   

/// Assigns each student within (string *students) a letter grade based on their
/// score from (double *average_scores) and produces a neatly formatted column
/// aligned report showing, 
///   - 1. student name
///   - 2. average score
///   - 3. letter grade
string gradeFormat(double *average_scores, string *students);


const char *SCORES_FNAME = "StudentScores.txt";

// A reasonable max for the number of students in a class 
const int MAX_STUDENTS = 256;

// Number of scores for this particular class
const int NUM_SCORES = 5;


int main(void) {
  int scores[MAX_STUDENTS][NUM_SCORES];
  double average_scores[MAX_STUDENTS];
  string students[MAX_STUDENTS];

  return 0;
}
