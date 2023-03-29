# C-Shell

### Created as a part of OSN (Operating Systems and Networks) course as an assignment.

### Brief Description:
Imitated a few of the traditional Linux terminal's commands by programming it in C.

### **Terminal commands:**
    $ make
    $ ./a.out
      

> Run on Ubuntu 20.0.4  

### **Functionality:**
>* Entering the terminal commands clears the screen and opens the shell that is programmed.  

>* Home folder (represented by ~) is the directory from which the shell is invoked.  

>* Press enter after a command is entered to indicate end of the input.
  
>* Directory switch and command execution occurs based on the command.
  
>* Enter -1 to exit the programmed shell.
  
### **File Information:**
  
*Note: every .c file has a corresponding .h file with the same name.
>* `bgfg.c` - contains functions related to the background and foreground processes. {spec 1 & part, spec 4, spec 6} 
>* `cd.c` - contains functions related to the cd command. {spec 1 cd part, spec 2 cd part}
>* `discover.c` - contains functions related to the discover command. {spec 7}
>* `func.c` - contains functions related to echo, pwd, and pinfo commands. {spec 2 echo and pwd parts, spec 5}
>* `headers.c` - contains all the header lines as well as an alphasort function that is used by both the ls and discover commands. {utility}
>* `history.c` - contains all the functions related to the history command. {spec 8}
>* `ls.c` - contains all the commands related to the ls command. {spec 3}
>* `readcheck.c` - contains all the utility functions that are needed to display the prompt in the shell, read the entered command, and check which command it is. {utility}
>* `makefile` - contains directions to run the shell on the terminal in Ubuntu. {extra}
>* `README.md`

### **Assumptions:**
>* `"cd -"` as the first command when the shell is run will give the current working directory as the previous working directory doesn't exist

>* "`&"` and `";"` are not the starting words of the input commands that can be entered.
  
>* Empty command just displays the prompt again with the current working directory and all the username and system info. 

>* All the errors are handled using perror. If there is an error, the program just shows there is an error and doesn't exit out of the programmed shell. The prompt is displayed again so that the user can enter a new command.
  
>* In the ls command, the files are showed alphabetically ignoring the upper and lower cases. Even the files starting with `.` are sorted by ignoring the `.`. For example, `Print.c` and `.print.c` and `print.c` will be printed one after the other. Basically, all the names starting with `p` will be displayed in the same place.
  
>* In the ls command, the same flag cannot be entered multiple times in the input line.
  
>* In the history command, `-1` is also counted as a command and is displayed in history.
  
>* In the discovery command, only one file name and one directory name can be entered, and directory name can be entered as it is or with a prefix of `./`
  
>* In the directory command, the only acceptable flags are `-d` and `-f`.
  
>* In the discover command, if there is a flag and a file name, and the flag is `-d`, it's gonna print the path to the directory that has the same name as entered in the command if it exists.
  
>* In the history command, if the command has a `;` in it, it is printed as two separate commands in history.
  
