## Part A
Run the below command to run the code for Part-A :
```
g++ lab4.cpp -o lab4
./lab4 -M 32 -V 32 -P 512 -i infile -o outfile
```
or
```
make lab4
./lab4 -M 32 -V 32 -P 512 -i infile -o outfile
```
To clean output files use
```
make clean
```

Below are the flags supported :
| Name | Default Value | Description |
| :---: | :-------------: | :----------- |
| `-V`| 32 | Virtual Memory Size |
| `-M`| 32 | Main Memory Size |
| `-P`| 512 | Page Size | 
| `-i`| infile | input file consisting list of commands |
| `-o`| outfile | output file name to print the output in | 

## Part B

Valgrind is a memory-bug detector tool
• To install
```
  sudo apt-get install valgrind
```
• Command to run valgrind
```
  gcc prog.c -o prog
  valgrind prog  
```

Check man page of valgrind for more information.Submit the c file along with a text file with your answer and the output of your program with valgrind command
1. Write a program that creates an array of integers called data of size 100 using malloc; then, set data[100] to zero. What happens when you run this program? What happens when you run this program using valgrind? Is the program correct?
 - It doesnt give any error while running, but in the valgrind output we can see a prompt telling invalid write of size 4. Valgrind being a memory debugger catches the illegal write.
2. Create a program that allocates an array of integers called data of size 100 using malloc, frees them, and then tries to print the value of one of the elements of the array. What happens when you run this program? What happens when you run this program using valgrind? Is the program correct?
 - It doesnt give any runtime error, but in the valgrind output we can see a prompt telling invalid read of size 4. Because we tried to read an already freed page which is illegal.


