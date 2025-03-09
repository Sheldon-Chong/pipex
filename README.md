# Pipex
Pipex is a project of the 42 core program where students must create a project that imitates the piping function of the shell programs like bash or zsh. Commands are passed as arguments when running the pipex program. The program runs each command, and the output of the previous command is passed into the next.

## 📖 Main concepts
- Piping and interprocess communication
- file descriptors (FD's) and redirection
- forking processes 

## ⬇️ Clone
```
git clone git@github.com:Sheldon-Chong/pipex.git
```

## ⏯️ Compile and run

```zsh
make
./pipex <args>...
```

## ❓ How to use Pipex

The format for input in Pipex is the following:
```zsh
./pipex file1 "cmd1" "cmd2" file2
```
The program takes the contents of the first file, and utilize it as the stdin (standard input) for `cmd1`. `cmd1` will then execute, and its output is passed as the input for `cmd2`, so on and so forth. The final argument is the output file, where the output of the last command will be passed to.

The equivalent in linux would be
```zsh
< file1 cmd1 | cmd2 > file2
```

Examples test-cases:
```zsh
./pipex input.txt "cat" "wc -l" output.txt
./pipex input.txt "grep error" "sort" output.txt
```

## My experience with this project
My approach to Pipex was rather unconventional. From other projects I've looked at, Pipex was approached in one of two main ways: 
- Processes are forked n amount of times (n being the amount of commands to be ran), and pipes are created to pipe one processes's output to the next
- Processes and pipes are created on the fly, and as the program forks the next process and forms the pipes, it will destroy the previous process and close its pipes.

My approach required at most 3 processes opened at once. The main process would fork a child, the child executes it's given command, the output of the child is then redirected into the input of the parent. The parent receives the input, destroys the child, then forks again and this repeats until the child executes the final command. (The 3rd process is created whenever a command is executed, so that the process will not be taken over be `execve()`)
