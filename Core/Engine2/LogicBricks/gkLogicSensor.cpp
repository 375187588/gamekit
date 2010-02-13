/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "gkLogicSensor.h"
#include "gkLogicController.h"
#include "gkLogicManager.h"


gkLogicSensor::gkLogicSensor(gkGameObject *object, const gkString &name)
:       gkLogicBrick(object, name), m_freq(0), m_tick(0), m_pulse(PULSE_NONE), m_invert(false), m_positive(false),
        m_suspend(false), m_tap(false)
{
}


void gkLogicSensor::tick(void)
{
    if (m_suspend) return;


    bool doQuery = false;
    if (++m_tick > m_freq) {
        doQuery = true;
        m_tick = 0;
    }

    if (doQuery) {

        // test
        m_positive = query();

        if (m_pulse != PULSE_NONE) {
            // default to positive if negitave
            // is set as well
            if (!(m_pulse & PULSE_POSITIVE)) {
                if (m_pulse & PULSE_NEGATIVE)
                    m_positive = !m_positive;
            }
        }

        // lastly invert
        if (m_invert)
            m_positive = !m_positive;


        // dispatch results
        if (m_positive) 
            gkLogicManager::getSingleton().push(this);


        // TODO destroy ?
        if (m_tap)
            m_suspend = true;
    }
}

void gkLogicSensor::dispatch(void)
{
    if (!m_controllers.empty())
    {
        // send signal to all controllers

        utListIterator<Controllers> it(m_controllers);
        while (it.hasMoreElements())
            it.getNext()->relay();
    }

}