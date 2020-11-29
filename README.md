Spacekill
==

Occasionally outside your control you need to run an application in docker that might write to disk. If that program doesn't clean up for some reason eventually your docker container might become full.

A lot of applications will catch write errors and attempt to handle them. This can be a disadavantage when trying to handle disk space failures as the application will still pass health checks but incorrectly serve requests.

`spacekill` solves this by overriding the `write` call with a version that checks error codes for `ENOSPC` and `EDQUOT` (free space, and disk quota full errors). If it detects this, it termiates the program.

Compiling
--
```
gcc -Wall -fPIC -shared -o spacekill.so spacekill.c
```

Using
--
Start your program with an LD_PRELOAD set to `spacekill.so`
```
LD_PRELOAD=spacekill.so python example.py
```

An example docker file is included.

Exit code
--
`91 - spacekill termianted the application because a write failed`

Notes
--
I'm not a C dev. I have no idea what horrible things this will cause. I have not tested this in production. I have barely tested this myself. This doesn't check if the file descriptor is pointing to a real file. This was made in literally in a couple of hours after a long drive.