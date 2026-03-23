#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define PIPEREAD 0
#define PIPEWRITE 1
#define LINESIZE 1024

#define FILE "./easypipe.c"

void err_exit (const char *msg)
{
  perror(msg);
  exit(EXIT_FAILURE);
}

void err_cont (const char *msg)
{
  perror(msg);
  fprintf(stderr, "Trying to continue.\n");
  return;
}

int main(int argc, char* argv[])
{
  int ret, fd[2], rfd;
  pid_t pid, wpid;
  ssize_t in_father, in_son;
  char buf_father, buf_son[LINESIZE];

  ret = pipe(fd);
  if (ret == -1)
    err_exit("pipe() failed.\n");

  pid = fork();
  if ((int) pid == -1)
    err_exit("fork() failed.\n");

  if (pid == 0) {
    /* *** Anfang Code des Sohnes ************************************ */
    ret = close(fd[PIPEWRITE]); /* schlieÃŸe unbenutzten (Schreib-)Deskriptor */
    if (ret == -1)
      err_cont("Closing (unused) pipe write descriptor in son failed.");

    /* Schleife: Lesen aus der Pipe und Ausgabe nach stdout */
    in_son = read(fd[PIPEREAD], &buf_son, LINESIZE);
    if (in_son == -1)
      err_cont("Reading from pipe read descriptor in son failed.");
    while (in_son != 0) {
      ret = write(1, &buf_son, in_son);  /* Schreibe auf das Terminal */
      if (ret == -1)
	err_cont("Writing to stdout in son failed. WTF?");
      in_son = read(fd[PIPEREAD], &buf_son, LINESIZE); /* Lies aus Pipe */
      if (in_son == -1) {
	err_cont("Reading from pipe read descriptor in son failed. Closing it.");
	in_son = 0;
      }
    }
    ret = close(fd[PIPEREAD]); /* schlieÃŸe benutzten Deskriptor */
    if (ret == -1)
      err_cont("Closing (used) pipe read descriptor in son failed.");
    printf("Son exits.\n");
    exit(EXIT_SUCCESS); /* beendet den Sohn */
    /* *** Ende Code des Sohnes ************************************** */
  }
  else {
    /* *** Anfang Code des Vaters ************************************ */
    rfd = open(FILE, O_RDONLY);  /* Ã–ffne Datei zum Lesen */
    if (rfd == -1)
      err_exit("Open on input file in father failed.");
    ret = close(fd[PIPEREAD]); /* schlieÃŸe unbenutzten (Lese-)Deskriptor */
    if (ret == -1)
      err_cont("Closing (unused) pipe read descriptor in father failed.");

    /* Schleife: Lesen aus Datei und Schreiben in Pipe */
    in_father = read(rfd, &buf_father, 1);
    if (in_father == -1)
      err_cont("Reading from file in father failed.");
    while (in_father != 0) {
      ret = write(fd[PIPEWRITE], &buf_father, 1);
      if (ret == -1)
	err_cont("Writing to pipe in father failed.");
      in_father = read(rfd, &buf_father, 1);
      if (in_father == -1) {
	err_cont("Reading from file in father failed. Closing it.");
	in_father = 0;
      }
    }
    ret = close(fd[PIPEWRITE]); /* schlieÃŸe benutzte Deskriptoren */
    if (ret == -1)
      err_cont("Closing (used) pipe write descriptor in father failed.");
    ret = close(rfd);
    if (ret == -1)
      err_cont("Closing input file in father failed.");

    wpid = wait(NULL); /* warte auf Ende des Sohnes */
    if (wpid == -1)
      err_cont("wait() in father failed.");

    printf("Father exits.\n");
    exit(EXIT_SUCCESS);
    /* *** Ende Code des Vaters ************************************* */
  }
}
