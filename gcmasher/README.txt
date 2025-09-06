
GC Masher
Command line bruteforce optimizer for GCC parameters
Marq/Fit 11.8.2005
Use as you wish

A little tool that runs through a list of command line parameters and finally
prints out the combination that produced the smallest executable. See
datafile.txt to learn how it works. The thingy was originally made for 4k
intro compression but can probably be used for other purposes as well.
It might take "some" time to run a complex list of parameters, but what the
hell: they are free bytes for you with little extra effort. Let the computer
do the work for you. Especially when crafting compressed code it is very
difficult for a normal human being to tell which flags produce the best result.

In its present state GC Masher probably works only in Unix-like systems but
it should not be a big deal to change the source for Cygwin or similar
platform. For speed optimization you should check Acovea.

During a lengthy run you can check the current top combination like this:
sort -n results.txt | head -n 1
Ctrl-c doesn't do much, so if you should want to break it:
killall gcmasher

The latest version should always be here:
http://ftp.kameli.net/pub/fit/misc/

