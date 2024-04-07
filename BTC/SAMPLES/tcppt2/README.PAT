(TCPPT11) Patches to Turbo C++ v1.01
====================================
To patch the Turbo C++ integrated environment, run the batch files
    PATTC TC.EXE

* If you are in the Turbo C++ bin directory, you may omit the path.
* If you supply a path, you must also state the name of the file to patch,
* for example:
      PATTC C:\TC\BIN\TC.EXE

This patche corrects the following problem:

[1]  Insert disk in drive A:
Intermittently, the a message was erroneously displayed prompting the
user to insert a disk in Drive A:.
