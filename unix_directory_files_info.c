#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>

const char* get_filename_ext(const char *filename) {    // functie ce returneaza extensia fisierului cu numele filename dat ca parametru
    const char* dot = strrchr(filename, '.');
    
    if(!dot || dot == filename) return "";
    return dot + 1;
}

char* get_filename_without_ext(char *filename) {   // functie ce returneaza numele fisierului cu numele complet char *filename fara extensie
    char *filename_copy;
    if( strlen(filename) > 0 ) {
        filename_copy = (char*) malloc ( strlen(filename) * sizeof(char) ); // copie a filename-ului folosita pentru strtok, care modifica string-ul folosit
    }
    else return "";
    
    strcpy(filename_copy, filename);
    
    const char* dot = strrchr(filename_copy, '.');  // ultima aparitie a caracterului '.' in numele fisierului
    
    char *filename_without_ext;
    filename_without_ext = strtok(filename_copy, ".");  // primul token gasit in string (impartit de .) => nume fisier fara extensie
    
    if(!dot || dot == filename_copy) return ""; // invalid
    
    return filename_without_ext;
}

bool is_path_last_character_slash(char *path) {    // functie care verifica daca path-ul dar ca parametru de tip char* se termina cu caracterul '/'
    
    int path_length = strlen(path);
    
    if(path_length > 0) {
        char path_last_character = path[path_length - 1];
        
        if(path_last_character == '/') return true;
        else return false;
    } else return false;
    
}

char *append_full_path_without_slash_to_filename(char *path, char *filename) {  // functie ce returneaza path-ul complet care fisierul cu numele char *filename, append-uind path-ul fara '/' intre <path> si <filename> - functie apelata cand path-ul se termina deja cu caracterul '/', deci nu mai trebuie adaugat din nou ( <path><filename> )
    char *filename_with_path;
    
    filename_with_path = (char*) malloc ( (strlen(path) + 1 + strlen(filename)) * sizeof(char) );   // +1 pentru terminatorul de sir '\0'
    
    strcpy(filename_with_path, path);
    strcat(filename_with_path, filename);
    
    return filename_with_path;
}

char *append_full_path_with_slash_to_filename(char *path, char *filename) { // functie ce returneaza path-ul complet care fisierul cu numele char *filename, append-uind path-ul cu '/' intre <path> si <filename> - functie apelata cand path-ul nu se termina cu caracterul '/', deci trebuie adaugat ( <path>/<filename> )
    char *filename_with_path;
    
    filename_with_path = (char*) malloc ( (strlen(path) + 2 + strlen(filename)) * sizeof(char) );   // +2 pentru caracterul '/' + terminatorul de sir '\0'
    
    strcpy(filename_with_path, path);
    strcat(filename_with_path, "/");
    strcat(filename_with_path, filename);
    
    return filename_with_path;
}

void compile_file_with_gcc(char *filename, char *dir_path) {
    
    char *total_command = (char*) malloc ( (16 + strlen(filename) + strlen(filename) - 2 + strlen(dir_path)) * sizeof(char) );
    strcpy(total_command, "cd ");
    strcat(total_command, dir_path);
    
    char * filename_without_ext = get_filename_without_ext(filename);
    
    if(strcmp(filename_without_ext, "")) {   // functia get_filename_without_ext returneaza string-ul gol "" daca numele fisierului nu e valid ( de tip <nume>.<extensie> )
        
        strcat(total_command, " && gcc ");
        strcat(total_command, filename);
        strcat(total_command, " -o ");
        strcat(total_command, filename_without_ext);
   
        system(total_command);
   }
    
    free(total_command);
}
int count_occurences(char *string, char *substring) {   // functie ce returneaza numarul de aparitii al string-ului char *substring in string-ul char *string
    int count = 0;
   
    for(int i = 0; string[i] != '\0'; i++) {
        int j;
        for(j = 0; substring[j] != '\0' && substring[j] == string[i+j]; j++);
       
        if(substring[j] == '\0') {
            count ++;
        }
    }
   
    return count;
}

void print_file_err() { // functie ce afiseaza eroarea setata in errno, daca e cazul
        switch(errno) {
            case EACCES:
            printf("EACCES");
            break;

            case EBADF:
            printf("EBADF");
            break;

            case EFAULT:
            printf("EFAULT");
            break;

            case ELOOP:
            printf("ELOOP");
            break;
                            
            case ENAMETOOLONG:
            printf("ENAMETOOLONG");
            break;
                            
            case ENOENT:
            printf("ENOENT");
            break;
                            
            case ENOMEM:
            printf("ENOMEM");
            break;
                            
            case ENOTDIR:
            printf("ENOTDIR");
            break;
                            
            case EOVERFLOW:
            printf("EOVERLFOW");
            break;
        }
}

void print_file_permissions(struct stat st) {   // functie ce afiseaza permisiunile unui fisier pentru utilizator, grup si altii
        printf("%s\n", "Utilizator:");
        printf("%s", "Read - ");
        if (st.st_mode & S_IRUSR) {
            printf("%s\n", "DA");
        } else {
            printf("%s\n", "NU");
        }
            printf("%s", "Write - ");
        if (st.st_mode & S_IWUSR) {
            printf("%s\n", "DA");
        } else {
            printf("%s\n", "NU");
        }
            printf("%s", "Exec - ");
        if (st.st_mode & S_IXUSR) {
            printf("%s\n", "DA");
        } else {
            printf("%s\n", "NU");
        }
                               
        printf("%s\n", "Grup:");
        printf("%s", "Read - ");
        if(st.st_mode & S_IRGRP) {
            printf("%s\n", "DA");
        } else {
            printf("%s\n", "NU");
        }
        printf("%s", "Write - ");
        if(st.st_mode & S_IWGRP) {
            printf("%s\n", "DA");
        } else {
            printf("%s\n", "NU");
        }
            printf("%s", "Exec - ");
        if(st.st_mode & S_IXGRP) {
            printf("%s\n", "DA");
        } else {
            printf("%s\n", "NU");
        }
                               
        printf("%s\n", "Altii:");
        printf("%s", "Read - ");
        if (st.st_mode & S_IROTH) {
            printf("%s\n", "DA");
        } else {
            printf("%s\n", "NU");
        }
        printf("%s", "Write - ");
        if (st.st_mode & S_IWOTH) {
            printf("%s\n", "DA");
        } else {
            printf("%s\n", "NU");
        }
            printf("%s", "Exec - ");
        if (st.st_mode & S_IXOTH) {
            printf("%s\n", "DA");
        } else {
            printf("%s\n", "NU");
        }
        printf("\n");
}

int main(int argc, char* argv[]) {
    const int BSIZE = 100;
    char buf[BSIZE]; // buffer folosit pentru citire pipe cerinta 6  
   
    DIR *d;
    struct dirent *dir;
    if(argc == 3) {
        char* path = argv[1];   // variabila ce retine al doilea argument dat din command line: path-ul catre directorul de parcurs
        char* command = argv[2];    // variabila ce retine al 3-lea argument
   
        struct stat st1;        // variabila ce retine informatii despre fisierul director
        if( stat( argv[1], &st1 ) != 0) {
            perror("Eroare director.\n");
            exit(EXIT_FAILURE);
        } else {
            if( !S_ISDIR( st1.st_mode ) ) {  // verificare ca path-ul indicat de al doilea argument reprezinta un director
                perror("Error: Argumentul nr 2. dat nu este de tip director.\n");
                exit(EXIT_FAILURE);
            }
            else {
            
                d = opendir(path);
                if (d) {
                struct stat st; // variabila ce retine informatii despre fisierul curent
                char *filename_with_full_path;  // variabila ce va retine path-ul intreg catre fiecare fisier din director, pentru stat()
                
                while ((dir = readdir(d)) != NULL) {    // parcurgere director fisier cu fisier
                    
                    // concatenare path intreg inainte de numele fisierului curent
                    
                    // verificare daca ultimul caracter al path-ului catre director este "/", pentru concatenare
                    
                    if( is_path_last_character_slash(path) ) {  // ultimul caracter al path-ului este "/"
                        filename_with_full_path = (char*) malloc ( (strlen(path) + 1 + strlen(dir->d_name)) * sizeof(char) );
                        filename_with_full_path = append_full_path_with_slash_to_filename(path, dir->d_name);
                        
                    } else {    // ultimul caracter al path-ului nu este "/"
                        filename_with_full_path = (char*) malloc ( (strlen(path) + 2 + strlen(dir->d_name)) * sizeof(char) );
                        filename_with_full_path = append_full_path_with_slash_to_filename(path, dir->d_name);
                    }
                    
                    if(stat(filename_with_full_path, &st) < 0) {    // eroare fisier
                        print_file_err();
                        exit(EXIT_FAILURE);
                    }
                    if(*get_filename_ext(dir->d_name) == 'c' && strlen(get_filename_ext(dir->d_name)) == 1) {    // verificare ca fisierul curent are extensia .c
                        if( strlen(argv[2]) >= 1 ) {    // verificare ca al 3-lea argument din linia de comanda este valid: are lungime ma mare decat 1: minim 2 caractere: - si o litera
                            if(argv[2][0] == '-') {     // verificare ca al 3-lea argument din linia de comanda este valid: primul caracter este "-"
                                
                                if( strchr(command, 'g') ) {
                                    
                                    pid_t pid1; // proces 1: cerinta 3 - afisare informatii fisier in functie de al 3-lea argument dat
                                    int status1;
                               
                                    pid1 = fork();
                               
                                    if( pid1 < 0 ) {
                                        perror("Fork nereusit.\n");
                                        exit(EXIT_FAILURE);
                                    }
                                
                                    if(pid1 == 0) { // codul fiului
                                
                                        if( strchr(command, 'n') ) {
                                            printf("Nume: fisier: %s\n\n", dir->d_name);
                                        }
                                        if( strchr(command, 'u') ) {
                                            printf("Identificator utilizator: %d\n\n", st.st_uid);
                                        }
                                        if( strchr(command, 'a') ) {
                                            print_file_permissions(st);
                                        }
                                        if( strchr(command, 'd') ) {
                                            printf("Dimensiunea in octeti: %d\n\n", st.st_size);
                                        }
                                        if( strchr(command, 'c') ) {
                                            printf("Contor legaturi fisier: %d\n\n", st.st_nlink);
                                        }
                                
                                        exit(0);
                                    }
           
                                    // codul parintelui
           
                                    waitpid(pid1, &status1, 0);
                               
                                    printf("1 - Procesul fiu cu PID %d s-a terminat cu codul %d.\n", pid1, status1);
                                   
                                    kill(-pid1, SIGKILL);
                                    
                                } else {    // cerinta 3 fara procese - nu este prezenta optiunea 'g' in argumentul 3
                                    
                                    if( strchr(command, 'n') ) {
                                            printf("Nume fisier: %s\n\n", dir->d_name);
                                        }
                                        if( strchr(command, 'u') ) {
                                            printf("Identificator utilizator: %d\n\n", st.st_uid);
                                        }
                                        if( strchr(command, 'a') ) {
                                            print_file_permissions(st);
                                        }
                                        if( strchr(command, 'd') ) {
                                            printf("Dimensiunea in octeti: %d\n\n", st.st_size);
                                        }
                                        if( strchr(command, 'c') ) {
                                            printf("Contor legaturi fisier: %d\n\n", st.st_nlink);
                                        }
                                        
                                }
            
                                if( strchr(command, 'g') && !strchr(command, 'p') ) {    // argumentul 3 contine 'g', dar nu contine 'p' => nu trebuie apelat un al doilea fork() pentru un proces nou pentru cerinta 6 care sa ruleze in paralel cu cerinta 5
                                    
                                    pid_t pid2; // proces 2: cerinta 5 - compilare fisier cu lansarea in executie a gcc
                                    int status2;
                               
                                    pid2 = fork();
                               
                                    if( pid2 < 0 ) {
                                        perror("Fork nereusit.\n");
                                        exit(EXIT_FAILURE);
                                    }  
                                    if(pid2 == 0) { // codul fiului - doar codul fiului poate ajunge aici (doar in fiu fork() returneaza 0)
                            
                                        compile_file_with_gcc(dir->d_name, path);
                                        exit(0); // apel necesar pentru a se opri codul fiului astfel incat acesta sa nu execute si codul parintelui
                                        
                                    }
                               
                                    // codul parintelui
                                   
                                    waitpid(pid2, &status2, 0);
                               
                                    printf("2 - Procesul fiu cu PID %d s-a terminat cu codul %d.\n\n", pid2, status2);
                                   
                                    kill(-pid2, SIGKILL);
                                    
                                } else if( strchr(command, 'g') && strchr(command, 'p') ) { // argumentul 3 contine si 'g' si 'p' => trebuie apelat un al doilea fork() pentru un proces nou pentru cerinta 6 care sa ruleze in paralel cu cerinta 5
                                    
                                    int fd1[2]; // cele doua capete ale primului pipe
                                    int fd2[2]; // cele doua capete ale celui de al doilea pipe
                                    
                                    if(pipe(fd1) == -1) {
                                        perror("Pipe nereusit.\n");
                                        exit(EXIT_FAILURE);
                                    }
                                    
                                    if(pipe(fd2) == -1) {
                                        perror("Pipe nereusit.\n");
                                        exit(EXIT_FAILURE);
                                    }
                                        
                                
                                    pid_t pid2; // proces 2: cerinta 5 - compilare fisier cu lansarea in executie a gcc
                                    int status2;
                                    pid_t pid4; // proces 4: cerinta 6 - pipes, "error" & "warning"
                                    int status4;
                                    
                                    pid2 = fork();
                               
                                    if( pid2 < 0 ) {
                                        perror("Fork nereusit.\n");
                                        exit(EXIT_FAILURE);
                                    }  
                                    
                                    if(pid2 == 0) { // codul fiului - doar codul fiului poate ajunge aici (doar in fiu fork() returneaza 0)
                                        
                                        pid4 = fork();
                                    
                                        if( pid4 < 0 ) {
                                            perror("Fork nereusit.\n");
                                            exit(EXIT_FAILURE);
                                        }  
                                    
                                        if(pid4 == 0) { // codul fiului - doar codul fiului poate ajunge aici (doar in fiu fork() returneaza 0)
                            
                                            close(fd1[1]);  // inchidere capat scriere primul pipe
                                            close(fd2[0]);  // inchidere capat citire al doilea pipe
                                         
                                            dup2(fd1[0], 0);    // redirectare stdin in capatul de citire al primului pipe
                                            dup2(fd2[1], 1);     // redirectare stdout in capatul de scriere al celui de-al doilea pipe
                                            
                                            close(fd1[0]);  // inchidere capat citire primul pipe
                                            close(fd2[1]);  // inchidere capat scriere al doilea pipe
                                        
                                            execlp("grep", "grep", "\\(warning\\|error\\)", NULL);  // executare instructiune grep pentru filtrare error / warning
                                            
                                            perror("Eroare");   // daca exista eroare de la execlp grep se afiseaza si se termina
                                            exit(EXIT_FAILURE);
                                        
                                        }
                                    
                                        //codul parintelui
                                    
                                        close(fd1[0]);  // inchidere capat citire pipe1
                                        close(fd2[0]);  // inchidere capat citire pipe2
                                        close(fd2[1]);  // inchidere capat scriere pipe2
                                        dup2(fd1[1], 2);    // redirectare stderr (unde vor fi scrise rezultatele compilarii cu erori / warnings) catre capatul de scriere al primului pipe
                                        close(fd1[1]);  // inchidere capat scriere pipe1
                                    
                                        
                                        compile_file_with_gcc(dir->d_name, path);   // compilare fisier, actiune in urma careia rezultatele cu erori / warnings vor fi scrise in fd[1]
                                        
                                        exit(0); // apel necesar pentru a se opri codul fiului astfel incat acesta sa nu execute si codul parintelui
                                        
                                    }
                                    
                                    // codul parintelui
                                    
                                    int result = 0; // variabila in care se va retine rezultatul cerut
                                    int error_count = 0;
                                    int warning_count = 0;
                                    
                                   
                                    close(fd1[0]);  // inchidere capat citire pipe1
                                    close(fd1[1]);  // inchidere capat scriere pipe1
                                    close(fd2[1]);  // inchidere capat scriere pipe2
                                    
                                    char buff[BSIZE];   // buffer folosit la citirea din pipe
                                    FILE *stream;   // file pointer folosit la citirea din pipe
                                    stream = fdopen(fd2[0],"r");
                                    
                                    
                                    if(stream == NULL) {
                                        perror("Eroare fisier stream.\n");
                                        exit(EXIT_FAILURE);
                                    }
                                    
                                    while(fgets(buff, BSIZE, stream)) { // citire din pipe pana la terminare
                                        if(strstr(buff, "error")) { // intalnire cuvant "error" => incrementare counter error
                                            error_count ++;
                                        } else {
                                            if(strstr(buff, "warning")) {   // intalnire cuvant "warning" => incrementare counter warning
                                                warning_count ++;
                                            }
                                        }
                                    }
                                    
                                    if(error_count == 0 && warning_count == 0) {
                                        result = 10;   
                                    } else {
                                        if(error_count > 0) {
                                            result = 1;
                                        } else {
                                            if(warning_count > 10 ) {
                                                result = 2;
                                            } else {
                                             result = 2 + 8 * (10 - warning_count) /10;   
                                            }
                                        }
                                    }
                                    
                                    close(fd2[0]);  // inchidere capat citire pipe2
                                    
                                    printf("\nerror count: %d ; warning count: %d ; Rezultat: %d\n\n", error_count, warning_count, result);
                                  
                                    waitpid(pid2, &status2, 0);
                                    printf("2 - Procesul fiu cu PID %d s-a terminat cu codul %d.\n\n", pid2, status2);
                                   
                                    kill(-pid2, SIGKILL);
                                    
                                    waitpid(pid4, &status4, 0);
                                    printf("4 - Procesul fiu cu PID %d s-a terminat cu codul %d.\n\n", pid4, status4);
                                   
                                    kill(-pid4, SIGKILL);
                                    
                                }
                            } else {
                                
                                perror("Comanda trebuie sa inceapa cu caracterul -.\n");
                                exit(EXIT_FAILURE);
                                
                            }
                        } else {
                            perror("Error: Comanda invalida.\n");
                            exit(EXIT_FAILURE);
                        }
                        if(st.st_size < 1000000) {
                            
                            int l;
                            
                            if( strchr(command, 'g') ) {    // cerinta 4 executata cu procese - argumentul 3 contine optiunea 'g'
                                
                                pid_t pid3; // proces 3: cerinta 4 - creare symbolic link
                                int status3;
                               
                                pid3 = fork();
                           
                                if( pid3 < 0 ) {
                                    perror("Fork nereusit.\n");
                                    exit(EXIT_FAILURE);
                                }
                           
                                if( pid3 == 0 ) {   // codul fiului
                                    
                                    l = symlink(realpath(dir->d_name, NULL), get_filename_without_ext(dir->d_name));
                                    if(l == 0) {
                                        printf("Legatura simbolica creata cu succes.\n");
                                    } else {
                                        perror("Eroare la crearea legaturii simbolice.\n");
                                    }
                               
                                    exit(0);
                                }
                           
                                // codul parintelui
                               
                                waitpid(pid3, &status3, 0);
                               
                                printf("3 - Procesul fiu cu PID %d s-a terminat cu codul %d.\n\n", pid3, status3);
                                   
                                kill(-pid3, SIGKILL);
                                
                            } else {    // cerinta 4 executata fara procese - argumentul 3 nu contine optiunea 'g'
                                
                                    l = symlink(realpath(dir->d_name, NULL), get_filename_without_ext(dir->d_name));
                                    if(l == 0) {
                                        printf("Legatura simbolica creata cu succes.\n");
                                    } else {
                                        perror("Eroare la crearea legaturii simbolice.\n");
                                    }
                                    
                            }
                        }
                        
                        printf("\n\n");
                    }
                }
                
            closedir(d);
            
            }
        }
    }
  } else {
      
      perror("Numar gresit argumente.\n");
      exit(EXIT_FAILURE);
      
  }
  return 0;
}
