# Wadarchive
Compressed archive library for packaging data in an obscure format. Easily included in any C++ project.

# Installation

 1. Clone repository to local (to init submodules do `git clone --recurse-submodules https://github.com/alexsaves/wadarchive.git`)
 1. Initialize & update submodules (`git submodule init`, `git submodule update`)

You're done! (Optional) If you want to build the wadarchive CLI executable:

 1. Update Makefile directories (see `UPDATE THESE LINES` section)
 1. Run `make release`

A quick way to do things with submodules is:
 ```
 git clone --recurse-submodules https://github.com/alexsaves/wadarchive.git
 make release
 ```
# Command Line usage

The general usage to archive a folder is...
```
wadarchive -a -s [SOURCE FOLDER] -d [DESTINATION]
```
To extract an archive:
```
wadarchive -x -s [SOURCE WAD] -d [DESTINATION]
```
You can also tell it to be silent:
```
wadarchive -q -a -s [SOURCE FOLDER] -d [DESTINATION]
```
