//
//  version.hpp.in
//
// Copyright (c) Boucher Lab. All rights reserved.
// Licensed under the GNU license. See the LICENSE file in the repository root for full license information.

#ifndef version_hpp
#define version_hpp

#include <string>
#include <iostream>

namespace afm
{

struct Version
{
    static std::string AFM_GIT_BRANCH;
    static std::string AFM_GIT_COMMIT_HASH;
    static int AFM_MAJOR;
    static int AFM_MINOR;
    static int AFM_PATCH;
    
    static void print();
};

}

#endif //version_hpp
