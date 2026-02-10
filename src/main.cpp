#define NUM_SCORES 5
#define MAX_STUDENTS 256
#define UNREACHABLE -1

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

using namespace std;

/// returns int number of students. 
/// Loads the student names into (string *students), and the scores
/// into 2D-array (int **scores) in an identical order to which student names
/// are loaded into (string *students).
int getScores(
    int scores[][NUM_SCORES], string *students, filesystem::path fpath);

/// Produces the average for each row of scores within (int **scores), and loads
/// it into (double *average_scores) in a row-based identical ordering. 
void averageScores(
    int scores[][NUM_SCORES], double *average_scores, int num_students);

/// Assigns each student within (string *students) a letter grade based on their
/// score from (double *average_scores) and produces a neatly formatted column
/// aligned report showing, 
///   - 1. student name
///   - 2. average score
///   - 3. letter grade
string gradeFormat(double *average_scores, string *students, int num_students);

/// returns the letter grade as (char) based on the given (double average_score)
char letterGrade(double average_score);

/// iterates in reverse over all characters in (string &trimmed), removing all
/// (char pat) characters until a non (char pat) character is reached. 
void trimStrEnd(string &trimmed, char pat);

int main(void) {
  const char *SCORES_FNAME = "StudentScores.txt";

  int scores[MAX_STUDENTS][NUM_SCORES];
  double average_scores[MAX_STUDENTS];
  string students[MAX_STUDENTS] {};

  filesystem::path scores_path = 
    filesystem::current_path().parent_path() / SCORES_FNAME;

  int num_stoods = getScores(scores, students, scores_path); 

  averageScores(scores, average_scores, num_stoods);

  string report { gradeFormat(average_scores, students, num_stoods) };

  cout << report << endl; 

  return 0;
}

int getScores(
    int scores[][NUM_SCORES], string *students, filesystem::path fpath) 
{
  const char *ERR_FOPEN = "Error: failed to open file";
  const char *ERR_FREAD = "Error: file read error";  

  // get scores file
  ifstream file { fpath };
  if (!file) {
    cerr << ERR_FOPEN << endl;
    exit(EXIT_FAILURE);
  }

  // read the student and score names into their respective arrays
  int nstd {};
  for (; nstd < MAX_STUDENTS && file >> students[nstd]; ++nstd) {
    for (int i {}; i < NUM_SCORES && file >> scores[nstd][i]; ++i);
  }

  if (file.eof()) {
    return nstd;
  } else if (!file) {
    cerr << ERR_FREAD << endl;
    exit(EXIT_FAILURE);
  }

  return UNREACHABLE;
} 

void averageScores(
    int scores[][NUM_SCORES], double *average_scores, int num_students) 
{
  for (int i {}; i < num_students; ++i) {
    double total {};

    for (int ii {}; ii < NUM_SCORES; ++ii) {
      total += scores[i][ii];
    }

    average_scores[i] = total / NUM_SCORES;
  }
}

string gradeFormat(
    double *average_scores, string *students, int num_students) 
{
  constexpr size_t PRE_COLUMNS { sizeof(" | 90.3 | A") - 1 };
  const string HEADER { "Student Grades Report" };

  string format {};
  
  // extract the longest name length from the students array as the only 
  // dynamical length contributing to the column count.
  int max_name {};
  for (int i {}; i < num_students; ++i) {
    if (students[i].size() > max_name) {
      max_name = students[i].size();
    }
  }

  size_t total_cols { max_name + PRE_COLUMNS + 2 };
  int header_padding = total_cols - HEADER.size();

  // adds header to the report string
  for (int spaces {}; spaces < header_padding; ++spaces, format += ' ');
  format += HEADER;
  format += '\n';

  // adds a '-' seperator to the report string
  for (int hyphens {}; hyphens < total_cols; ++hyphens, format += '-');
  format += '\n';

  for (int i {}, max_lpad { max_name + 1 }; i < num_students; ++i) {
    size_t lpad { max_lpad - students[i].size() };

    for (int i {}; i < lpad; ++i, format += ' ');   
    
    // append average score and trim trailing zeros 
    string score { to_string(average_scores[i]) };
    trimStrEnd(score, '0');
    format += students[i] + " | " + score + " | " + 
      letterGrade(average_scores[i]) + '\n';
  }

  return format;
}

char letterGrade(double average_score) {
  char grade;

  if (average_score >= 90.0 && average_score <= 100.0) {
    grade = 'A';  
  } else if (average_score >= 80.0) {
    grade = 'B';
  } else if (average_score >= 70.0) {
    grade = 'C';
  } else if (average_score >= 60.0) {
    grade = 'D';
  } else if (average_score >= 0.0) {
    grade = 'F'; 
  }

  return grade;
}

void trimStrEnd(string &trimmed, char pat) {
  // iterate over the trimmed string in reverse
  int erase_idx {};
  for (size_t i { trimmed.size() - 1 }; i > 0; --i) {
    if (trimmed[i] == pat) {
      erase_idx = i; 
    } else if (erase_idx != 0) {
      break;
    } 
  }

  if (erase_idx != 0) 
    trimmed.erase(erase_idx);
}
