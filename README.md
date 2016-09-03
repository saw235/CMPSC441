# CMPSC441
Assignments done for Operating Systems Class


Guidelines to all collaborators:
1) Coding styles should follow closely to Lab02.
2) Communication is key when merging. Notify collaborators before doing a merge when a merge conflict arise!
3) There should be function declarations (for good reasons) either in the header for each associated c code or right after the #includes
   if in the c-code itselfs.

4) Always ASK if unsure of anything, be it a git command or the structure of a code block.


USEFUL COMMANDS:

*****GCC*****
Single file
Compile: gcc -o programname code.c

Multiple files
Compile: gcc -c code1.c
         gcc -c code2.c
Link:    gcc -o programname code1.o code2.o

**************
*****GIT*****

Clone a repo: git clone https://github.com/saw235/CMPSC441.git
Stage and Commit:  git add filename; git commit -m "message";

Pull from repo and merge: git pull

If there is a merge conflict:
    1) Install kdiff3: sudo apt-get install kdiff3
    2) Run the command: git mergetool.
    3) Resolve all the conflict and click merge on toolbar to save.
    4) NOTIFY SOMEONE ELSE AND DO A REVIEW ON THE CODE BEFORE PUSHING IT TO MASTER REPO
    
