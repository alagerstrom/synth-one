//
// Created by Andreas Lagerstrom on 2018-02-17.
//

#ifndef SYNTHONE_ABSTRACTMODULE_H
#define SYNTHONE_ABSTRACTMODULE_H


#import "IPlugStructs.h"
#import "IGraphics.h"

class AbstractModule {
public:
    virtual void draw(IGraphics * iGraphics, IBitmap * knob) = 0;
    virtual void handleParamChange(int paramIndex) = 0;
};


#endif //SYNTHONE_ABSTRACTMODULE_H
