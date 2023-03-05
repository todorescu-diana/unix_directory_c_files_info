# unix_directory_c_files_info
UNIX C program that prints info of .c files found in a given input directory.

# Functionalities
1. Traversing given input directory
2. Identifying .c files in the directory
3. Printing info about each file, according to given program arguments (see below)
4. For each file with size < 100KB, a symbolic link is created wit the file name, but without .c extension

# Arguments
The program takes 2 arguments:
1. directory absolute path in which to search for .c files
2. Options, in the form of: -<option1> <option2> ... <optionn>, where the possible options are:
  - n: prints the name of the file
  - u: prints user id
  - a: file permissions (user, group, others, and read, write, exec for each)
  - d: file size in bytes
  - c: file link coun
  - g: if this argument is present, for each .c file there will be a new process created, which compiles the file with gcc, and functionalities 3 & 4 (presented above) will be executed on separate processes
  - p: only if present simultaneously with g argument - the process which compiles the file with gcc sends the compile results to another process which filters only lines which contain the words "error" or "warning" and sends these lines to the parent process; the parent check the "error" and "warning" occurrences and calculates a score, as follows:
    - if none of the two words ever occurres: the result is 10
    - if "error" occurs at least once: result is 1
    - if "error" never occurs, but "warning" occurs more than 10 times: result is 2
    - if "error" never occurs, but "warning" occurs less than 10 times: result is 2 + 8*(10 – nr_warning)/10 
The parent process retrieves the state of each child process and will display it as "Procesul fiu cu PID <pid_fiu> s-a terminat cu codul <cod>." ("Child process with PID <child_pid> ended with code < code >."), and then it will halt.
  
# Running prorgam
1. Open command line, go to folder with .c program & test directory
2. Compile .c program: gcc unix_directory_files_info.c -o udfi_exec
3. Run program with args - example: ./udfi_exec <absolute_path_to_test_directory> -nuadcgp
