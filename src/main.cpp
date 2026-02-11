#define MAX_NUM_SCORES 128
#define MAX_STUDENTS 256
#define UNREACHABLE -1
#define PRECISION 1 

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

/// returns int number of students. Loads the student names into 
/// (string *students), and the scores into 2D-array (int **scores) in an 
/// identical order to which student names are loaded into (string *students).
int getScores(
    int scores[][MAX_NUM_SCORES], string *students, filesystem::path fpath);

/// Produces the average for each row of scores within (int **scores), and loads
/// it into (double *average_scores) in a row-based identical ordering. 
void averageScores(
    int scores[][MAX_NUM_SCORES], double *average_scores, int num_students);

/// Assigns each student within (string *students) a letter grade based on their
/// score from (double *average_scores) and produces a neatly formatted column
/// aligned report showing, 
///   - 1. student name
///   - 2. average score
///   - 3. letter grade
string gradeFormat(double *average_scores, string *students, int num_students);

/// returns the letter grade as (char) based on the given (double average_score)
char letterGrade(double average_score);

/// returns a string generated from (double score) with (int decimal_precision)
/// number of decimal places.
string getDoubleStr(double score, int decimal_precision);

// global constant tracking number of test scores per student set to max the
// number of scores until the real number of scores is known.
int num_scores { MAX_NUM_SCORES };

int main(int argc, char *argv[]) {
  const char *ERR_NOARGS = 
    "Error: supply path of scores file: ./GradeBook ./path/to/scores";

  int scores[MAX_STUDENTS][MAX_NUM_SCORES];
  double average_scores[MAX_STUDENTS];
  string students[MAX_STUDENTS] {};


  if (argc < 2) {
    cerr << ERR_NOARGS << endl;
    exit(1);
  }

  // use argument given path as the scores path
  filesystem::path scores_path = argv[1];

  int num_stoods = getScores(scores, students, scores_path); 

  averageScores(scores, average_scores, num_stoods);

  string report { gradeFormat(average_scores, students, num_stoods) };

  cout << report << endl; 

  return 0;
}

int getScores(
    int scores[][MAX_NUM_SCORES], string *students, filesystem::path fpath) 
{
  const char *ERR_FOPEN = "Error: failed to open file";
  const char *ERR_FREAD = "Error: file read error";  

  // get scores file
  ifstream file { fpath };
  if (!file)
  {
    cerr << ERR_FOPEN << endl;
    exit(EXIT_FAILURE);
  }

  // read the student and score names into their respective arrays
  int nstd {};
  for (; nstd < MAX_STUDENTS && file >> students[nstd]; ++nstd)
  {
    for (int i {}; i < num_scores && i < MAX_NUM_SCORES; ++i) 
    {
      bool res = !(file >> scores[nstd][i]);

      if (res && num_scores == MAX_NUM_SCORES) 
      {
        num_scores = i;   
        file.clear();
        break;
      } 
      else if (res) 
      {
        file.clear();
        break;
      }
    }
  }

  if (file.eof()) 
  {
    return nstd;
  } 
  else if (!file) 
  {
    cerr << ERR_FREAD << endl;
    exit(EXIT_FAILURE);
  }

  return UNREACHABLE;
} 

void averageScores(
    int scores[][MAX_NUM_SCORES], double *average_scores, int num_students) 
{
  for (int i {}; i < num_students; ++i) 
  {
    double total {};

    for (int ii {}; ii < num_scores; ++ii) 
    {
      total += scores[i][ii];
    }

    average_scores[i] = total / num_scores;
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
  for (int i {}; i < num_students; ++i) 
  {
    if (students[i].size() > max_name) 
    {
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

  for (int i {}, max_lpad { max_name + 1 }; i < num_students; ++i) 
  {
    size_t lpad { max_lpad - students[i].size() };

    for (int i {}; i < lpad; ++i, format += ' ');   
    
    string score { getDoubleStr(average_scores[i], PRECISION) };
    
    // append average score
    format += students[i] + " | " + score + " | " + 
      letterGrade(average_scores[i]) + '\n';
  }

  return format;
}

char letterGrade(double average_score) {
  char grade;

  if (average_score >= 90.0 && average_score <= 100.0) 
  {
    grade = 'A';  
  }
  else if (average_score >= 80.0) 
  {
    grade = 'B';
  }
  else if (average_score >= 70.0) 
  {
    grade = 'C';
  }
  else if (average_score >= 60.0) 
  {
    grade = 'D';
  }
  else if (average_score >= 0.0) 
  {
    grade = 'F'; 
  }

  return grade;
}

string getDoubleStr(double score, int decimal_precision) {
  ostringstream score_str;
  score_str << fixed << setprecision(decimal_precision) << score;
  return score_str.str();
}  
