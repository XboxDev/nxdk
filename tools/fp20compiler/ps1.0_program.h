#ifndef _PS10_PROGRAM_H
#define _PS10_PROGRAM_H

#include "nvparse_errors.h"
#include "nvparse_externs.h"

#include <string>
#include <list>
#include <vector>
#include <map>

#define SCALE_NONE 0
#define SCALE_BY_TWO 1
#define SCALE_BY_FOUR 2
#define SCALE_BY_ONE_HALF 3

#define MASK_ALPHA 1
#define MASK_RGB 2
#define MASK_RGBA 3

namespace ps10
{

    struct constdef
    {
        std::string reg;
        float r,g,b,a;
    };

    extern std::map<int, std::pair<int,int> > constToStageAndConstMap;
    // extern std::map<int, GLenum> stageToTargetMap;
    extern std::map<int, int> stageToTargetMap;
    void invoke(std::vector<constdef> * c,
                std::list<std::vector<std::string> > * a,
                std::list<std::vector<std::string> > * b);
    
    // bool init_extensions();
}

#endif
