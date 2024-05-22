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

void release_memory(student **st, int C, int N, int M);

student **readCourses(int C, int N, int M) {
  printf("%d %d %d\n", C, N, M);
  student **student2Ptr = (student**)malloc(C * sizeof(student*)); /* This makes an array of C boxes called courses and the C boxes are large
                                  enough to hold the addresses of N student pointer... courses point to student*/

  for (int i = 0; i < C; i++) {
    student2Ptr[i] = (student*)malloc(N * sizeof(student));/*This is pointing each course box to an address holding student data*/

    for (int j = 0; j < N; j++) {
      student2Ptr[i][j].quizzes = (int*)malloc(M * sizeof(int));/*This is making an array of M quizzes pointed to by student j 
                                   pointed to by course i*/

      student2Ptr[i][j].lname = (char*)malloc(50 * sizeof(char));/*This is making an array of 50 char boxes and the base address 
                                   is being pointed to by student j pointed to by course i*/

    }// End of student loop generating lname and quizzes array
  }// End of courses loop generating students


  for (int i = 0; i < C; i++) {// Itterating through each course
    printf("\n\t\t\t----------\n\t\t\t Course %d\n\t\t\t----------", i+1);
    for (int j = 0; j < N; j++) {// Itterating through each student
      student2Ptr[i][j].total = 0;
      printf("\nStudent %d\n", j+1);
      printf("Enter the name of the student: ");
      scanf("%s", student2Ptr[i][j].lname);
      printf("Enter the assignment score: ");
      scanf(" %d", &student2Ptr[i][j].assignment);
      printf("Enter the quiz grade(s) for this student: ");
      for (int k = 0; k < M; k++) {// Itterating through each students quizzes
        scanf("%d", &student2Ptr[i][j].quizzes[k]);
        student2Ptr[i][j].total += student2Ptr[i][j].quizzes[k];// beginning the summing of the total for course i and student j
      }// end of quizzes loop
      printf("Enter the final exam score: ");
      scanf(" %d", &student2Ptr[i][j].finalExam);

      student2Ptr[i][j].total += student2Ptr[i][j].assignment + student2Ptr[i][j].finalExam; // adding assignment grade and final exam grade to the total
    }// end of student loop
 }// End of course loop 



  return student2Ptr; // returning base address of courses array to courseList in the main function

}// Closes readCourses function

void printHighestTotal(student **courseList, int C, int N, int M) {
  int max = 0;
  for (int i = 0; i < C; i++) {// Itterating through each course
    for (int j = 0; j < N; j++) {// Itterating through each student in each course
      if (courseList[i][j].total > max) {// Finding the highest total. If course-student-total is greater than max
        max = courseList[i][j].total;// Then max is set to course-student-total
      }
    }
  }// Ends course-student-total itteration for finding max number

  for (int i = 0; i < C; i++) {// Itterating through each course
    for (int j = 0; j < N; j++) {// Itterating through each student in each course
      if (max == courseList[i][j].total) {// If course-student-total is equal to max, print out this students info. 
        printf("\nName: %s\nAssignment: %d\nquizzes: ", courseList[i][j].lname, courseList[i][j].assignment);
        for (int k = 0; k < M; k++) {// itterate through course-student-quizzes
          printf("%d ", courseList[i][j].quizzes[k]);
        }
        printf("\nFinal exam: %d\nTotal: %d\nCourse number: %d\n", courseList[i][j].finalExam, courseList[i][j].total, i+1);

      }
    }
  }// Closes out course-student-total itteration for printing highest total

}




void release_memory(student **st, int C, int N, int M){

  printf("In release memory\n");
  
  for (int i = 0; i < C; i++) {// Itterating through each course
    for (int j = 0; j < N; j++) {// Itterating through each student in each course
      for(int k = 0; k < M; k++){// Itterating through each student's quizzes
        free(&st[i][j].quizzes[k]);
        free(&st[i][j].lname[k]);
      }
      free(st[i]);
    }
    free(st);
}
printf("Exiting program\n");
}// exit release_memory function









int main()
{
  atexit(report_mem_leak); //for memory leak detector.

  student **courseList;

  int C, N, M;

  printf("Enter the number of courses: ");
  scanf("%d", &C);
  printf("Enter the number of students: ");
  scanf("%d", &N);
  printf("Enter the number of quizzes: ");
  scanf("%d", &M);

  //passing reference of C, N, and M so that we get to know what we have in the first line
  courseList = readCourses(C, N, M);// recieving the base address of courses from readCourses function
  printHighestTotal(courseList, C, N, M);
  release_memory(courseList, C, N, M);
  free(courseList);
  printf("\nmemory cleared");

  return 0;
}