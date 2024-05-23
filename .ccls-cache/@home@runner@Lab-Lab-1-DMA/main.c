// Student and class info
/*
COP3502C Computer Science 1 - Lab 1 - Dynamic Memory Allocation
Lecturer:	  Dr. Tanvir Ahmed
Student name: Nicholas Newman
Student ID:	  5295926
Due date:	  5/23/24
*/
// Project Description
/*
In this program we have been asked to build a pointer array (courses) where all pointers point to a student structure pointer array where all these pointers point to 
student structures. Inside these student structures we have 3 integers an integer pointer and a char pointer. So these structures have pointers that point to arrays. 
We were given a main function and asked not to change it. We were also given the signatures for readCourses and the release_memory function. We are also given the 
files leak_detector_c.c and leak_detector_c.h that generate a file leak_info.txt to assist us in finding memory leaks. 
*/

#include <stdio.h>
#include <stdlib.h>
#include "leak_detector_c.h"

typedef struct student
{
  char *lname; //this will require DMA to store a string
  int assignment;
  int finalExam;
  int total;
  int *quizzes;//this will require DMA to store quizzes
}student;

student **readCourses(int *C, int *N, int *M) {

  scanf("%d", C);// C is an address which is why we did not use &C
  scanf("%d", N);// N is an address which is why we did not use &N
  scanf("%d", M);// M is an address which is why we did not use &M

  student **courses = (student**)malloc(*C * sizeof(student*)); /* This makes an array of C boxes called courses and the C boxes are large
          enough to hold the addresses of N student pointer... courses point to student pointers */

  for (int i = 0; i < *C; i++) {
    courses[i] = (student*)malloc(*N * sizeof(student));/*This is pointing each course box to an address holding student data*/

    for (int j = 0; j < *N; j++) {
      courses[i][j].quizzes = (int*)malloc(*M * sizeof(int));/*This is making an array of M quizzes pointed to by student j
               pointed to by course i*/

      courses[i][j].lname = (char*)malloc(50 * sizeof(char));/*This is making an array of 50-char boxes and the base address
               is being pointed to by student j pointed to by course i*/

    }// End of student loop generating lname and quizzes array
  }// End of courses loop generating students


  for (int i = 0; i < *C; i++) {// Itterating through each course
    for (int j = 0; j < *N; j++) {// Itterating through each student
      courses[i][j].total = 0;
      scanf("%s", courses[i][j].lname);
      scanf(" %d", &courses[i][j].assignment);
      for (int k = 0; k < *M; k++) {// Itterating through quizzes
        scanf("%d", &courses[i][j].quizzes[k]);
        courses[i][j].total += courses[i][j].quizzes[k];// beginning the summing of the total for course i and student j
      }// end of quizzes loop
      scanf(" %d", &courses[i][j].finalExam);

      courses[i][j].total += courses[i][j].assignment + courses[i][j].finalExam; // adding assignment grade and final exam grade to the total
    }// end of student loop
  }// End of course loop 

  return courses; // returning base address of courses array to courseList in the main function

}// Closes readCourses function

void printHighestTotal(student **courseList, int C, int N, int M) {
  int max = 0;

  student *tmp = malloc(sizeof(student));
  for (int i = 0; i < C; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = j + 1; k < N; k++) {
        if (courseList[i][j].total < courseList[i][k].total) {
            tmp[0] = courseList[i][j];
            courseList[i][j] = courseList[i][k];
            courseList[i][k] = tmp[0];
        }
      }
    }
  }

  for (int i = 0; i < C; i++) {
    if (courseList[i][0].total > max) {
      max = courseList[i][0].total;
    }
  }

  for (int i = 0; i < C; i++) {
      if (max == courseList[i][0].total) {
        printf("Name: %s\nAssignment: %d\nQuizzes: ", courseList[i][0].lname, courseList[i][0].assignment);
        for (int k = 0; k < M; k++) {
          printf("%d ", courseList[i][0].quizzes[k]);
        }
        printf("\nFinal exam: %d\nTotal: %d\nCourse number: %d", courseList[i][0].finalExam, courseList[i][0].total, i + 1);
      }
  }

  free(tmp);
}

void release_memory(student **st, int C, int N, int M) {

  for (int i = 0; i < C; i++) {// Itterating through each course
    for (int j = 0; j < N; j++) {// Itterating through each student in each course
      free(st[i][j].quizzes);
      free(st[i][j].lname);// this cleared out all of st[i]
    }
    free(st[i]);
  }
}// exit release_memory function

int main()
{
  atexit(report_mem_leak); //for memory leak detector.

  student **courseList;
  int C, N, M;

  courseList = readCourses(&C, &N, &M);/* recieving the base address of courses from readCourses function. We are also passing C, N, and M 
  by reference and actually setting these values in readCourses.*/ 
  printHighestTotal(courseList, C, N, M);// courseList is an address here. So passing by reference
  release_memory(courseList, C, N, M);// courseList is an address here. So passing by reference
  free(courseList);

  return 0;
}