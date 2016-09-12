# CMPSC441<br />
Assignments done for Operating Systems Class

|Folder Name| Description 
|-----------| -----------
|Lab02| Robot program that searches for Golds in the Map
|Lab03| Simulates the lifespans of clones in STARWARS. Test the capabilities of malloc


<br />
<br />

Guidelines to all collaborators:<br />
1) Coding styles should follow closely to Lab02.<br />
2) Communication is key when merging. Notify collaborators before doing a merge when a merge conflict arise!<br />
3) There should be function declarations (for good reasons) either in the header for each associated c code or right after<br />       the #includes if in the c-code itselfs. <br />
<br />
4) Always ASK if unsure of anything, be it a git command or the structure of a code block. <br />


USEFUL COMMANDS:

*****GCC*****
Single file<br />
Compile: gcc -o programname code.c
<br />
Multiple files<br />
Compile: gcc -c code1.c<br />
         gcc -c code2.c<br />
Link:    gcc -o programname code1.o code2.o<br />
<br />
**************
*****GIT*****
<br />
Clone a repo: git clone https://github.com/saw235/CMPSC441.git<br />
Stage and Commit:  git add filename; git commit -m "message";<br />
<br />
Pull from repo and merge: git pull<br />
<br />
If there is a merge conflict:<br />
    1) Install kdiff3: sudo apt-get install kdiff3<br />
    2) Run the command: git mergetool.<br />
    3) Resolve all the conflict and click merge on toolbar to save.<br />
    4) NOTIFY SOMEONE ELSE AND DO A REVIEW ON THE CODE BEFORE PUSHING IT TO MASTER REPO.
    
