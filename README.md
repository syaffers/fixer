fixer
=====

Crappy program to convert infix/postfix/prefix strings to their counterparts. Not really done yet so don't hold your breath. The program can convert strings from an input file (reading line-by-line) or via the `stdin`. Up to you.

Usage
-----

### Compiling

Super easy:

    gcc expr.c -o <output filename>

e.g.

    gcc expr.c -o expr

### Running

Even easier:

    ./<output filename>

e.g.

    ./expr

You have an option to choose a file during the program runtime. Don't worry.

Important Files
-----

+ `expr`: The compiled program. Compiled in Ubuntu 14.04, recompile from the origin `expr.c` for best results.
+ `expr.c`: The source code for the program. Where the magic happens.
+ `input.txt`: The file that stores all the read lines from `stdin` in "console mode"
+ `output.txt`: The file that stores the output for uses in "console mode"
