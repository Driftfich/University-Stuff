#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tStudent
{
    char *name;
    int matrNr;
    int noteBel;
    float noteKl;
} tStud;

tStud *read_stu(FILE *f) {
    int len = fgetc(f);
    if (len == EOF) return NULL;

    tStud *new_stu = malloc(sizeof(tStud));
    if (!new_stu) return NULL;
//     for (i=0; i<4; i++) {
//         int len = fgetc(f);
//         if (len = '\0') {
//             if (i>0) free(new_stu->name);
//             free(new_stu);
//             return NULL;
//         }
//
//         if (i==0) char *name = malloc(sizeof(char) * (len+1));
//
//         for (i=0; i<len; i++) {
//
//     }

    char *name = malloc(len+1);
    if (!name) {free(new_stu); return NULL;}
    fread(name, len, 1, f);
    new_stu->name=name;

    len = fgetc(f);
    if (len == EOF) {free(new_stu->name); free(new_stu); return NULL;}
    char *tmp = malloc(len + 1);
    fread(tmp, len, 1, f);
    tmp[len]=0;
    new_stu->matrNr=atoi(tmp);

    len = fgetc(f);
    if (len == EOF) {free(new_stu->name); free(new_stu); return NULL;}
    fread(tmp, len, 1, f);
    tmp[len]=0;
    new_stu->noteBel=atoi(tmp);

    len = fgetc(f);
    if (len == EOF) {free(new_stu->name); free(new_stu); return NULL;}
    fread(tmp, len, 1, f);
    tmp[len]=0;
    new_stu->noteKl=atof(tmp);

    free(tmp);

    return new_stu;
}

int main(int argc, char *argv[0]) {
    if (argc < 2) {
        puts("No file name given to read in.\n");
        exit(1);
    }

    char file[strlen(argv[1]) + 1];
    char file_out[strlen(argv[1]) + 1];
    strcpy(file, argv[1]);
    strcpy(file_out, strtok(argv[1], "."));
    strcat(file_out, ".csv");
    printf("Reading from file with name %s\n", file);
    printf("Writing to file with name %s\n", file_out);


    FILE *src = fopen(file, "rb");
    FILE *dst = fopen(file_out, "w");

    tStud *new_stu;
    while (1==1) {
            new_stu=read_stu(src);
            if (!new_stu) break;
            printf("%s, %d, %d, %f\n", new_stu->name, new_stu->matrNr, new_stu->noteBel, new_stu->noteKl);
            fprintf(dst, "%s, %d, %d, %f\n", new_stu->name, new_stu->matrNr, new_stu->noteBel, new_stu->noteKl);
            free(new_stu->name);
            free(new_stu);
    }

    fclose(src);
    fclose(dst);
    return 0;
}
