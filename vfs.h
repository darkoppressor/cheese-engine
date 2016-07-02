/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef vfs_h
#define vfs_h

#include <string>
#include <map>

class Virtual_File_System{
private:

    //key: virtual file path
    //value: actual file path
    std::map<std::string,std::string> files;

public:

    Virtual_File_System();
};

class VFS{
public:

    ///
};

#endif
