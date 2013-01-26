# Linux Process Injection Tool

This is a process injection tool for Linux written in C. 

A demo of the code running can be found here: http://ascii.io/a/1749

## How It Works

First, it attaches to the victim process with ptrace. It then searches through memory for the address to mmap in the process, it runs mmap and captures the return value. Then, it copies the shellcode into the process's memory and sets the process's instruction pointer to the shellcode.

The shellcode begins with a stub that simply forks, the child continues running the payload, the parent returns to the main process. This may be improved later for stealth. The rest of the shellcode can be anything from a bindshell to hooking functions.

## Compiling

To compile the code just run:

```
    $ make
```

## Running

At the moment, the code only works against the test program provided, with a little tinkering it can be made to work with any process though. So to run the test program (which simply prints it's pid and then loops):

```
    $ ./test
    12094
```

Next, run the parasite program.

```
    $ ./parasite 12094
```

The first time it is run it will hang, ctrl+c out of it and then rerun it, it will work the second time. This is a known bug and will be fixed in the future. At this point, a shell will be listening on port 4444, you can connect to it with netcat.

## Credit

(c) 2012 jtRIPper

http://blackhatlibrary.net/

jtRIPper@tormail.org
