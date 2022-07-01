//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Fps.h"

void Fps::update()
{
    if (mClock.getElapsedTime().asSeconds() >= 1.f)
    {
        mFps = mFrame;
        mFrame = 0;
        mClock.restart();
    }

    ++mFrame;
}
