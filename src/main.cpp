#define NUM_SCORES 5
#define MAX_STUDENTS 256
#define UNREACHABLE -1

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <stdlib.h>

using namespace std;

/// returns int number of students. 
/// Loads the student names into (string *students), and the scores
/// into 2D-array (int **scores) in an identical order to which student names
/// are loaded into (string *students).
int getScores(int scores[][NUM_SCORES], string *students, filesystem::path fpath);

/// Produces the average for each row of scores within (int **scores), and loads
/// it into (double *average_scores) in a row-based identical ordering. 
void averageScores(int scores[][NUM_SCORES], double *average_scores);

/// Assigns each student within (string *students) a letter grade based on their
/// score from (double *average_scores) and produces a neatly formatted column
/// aligned report showing, 
///   - 1. student name
///   - 2. average score
///   - 3. letter grade
string gradeFormat(double *average_scores, string *students);

int main(void) {
  const char *SCORES_FNAME = "StudentScores.txt";

  int scores[MAX_STUDENTS][NUM_SCORES];
  double average_scores[MAX_STUDENTS];
  string students[MAX_STUDENTS] {};

  filesystem::path scores_path = 
    filesystem::current_path().parent_path() / SCORES_FNAME;

  int num_stoods = getScores(scores, students, scores_path); 

  return 0;
}

int getScores(int scores[][NUM_SCORES], string *students, filesystem::path fpath) {
  const char *ERR_FOPEN = "Error: failed to open file";
  const char *ERR_FREAD = "Error: file read error";  

  ifstream file { fpath };
  if (!file) {
    cerr << ERR_FOPEN << endl;
    exit(EXIT_FAILURE);
  }

  int stood_count {};
  for (; stood_count < MAX_STUDENTS && file >> *students++; ++stood_count) {
    for (int i {}; i < NUM_SCORES && file >> *scores[i]; ++i);
  }

  if (file.eof()) {
    return stood_count;
  } else if (!file) {
    cerr << ERR_FREAD << endl;
    exit(EXIT_FAILURE);
  }

  return UNREACHABLE;
} 
