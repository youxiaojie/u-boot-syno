/****************************************************************************
*
*                         Ultra Long Period Timer
*
*  ========================================================================
*
*    The contents of this file are subject to the SciTech MGL Public
*    License Version 1.0 (the "License"); you may not use this file
*    except in compliance with the License. You may obtain a copy of
*    the License at http://www.scitechsoft.com/mgl-license.txt
*
*    Software distributed under the License is distributed on an
*    "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
*    implied. See the License for the specific language governing
*    rights and limitations under the License.
*
*    The Original Code is Copyright (C) 1991-1998 SciTech Software, Inc.
*
*    The Initial Developer of the Original Code is SciTech Software, Inc.
*    All Rights Reserved.
*
*  ========================================================================
*
* Language:     ANSI C
* Environment:  32-bit Windows VxD
*
* Description:  OS specific implementation for the Zen Timer functions.
*
****************************************************************************/

/*---------------------------- Global variables ---------------------------*/

static ulong    frequency = 1193180;

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
REMARKS:
Initialise the Zen Timer module internals.
****************************************************************************/
#define __ZTimerInit()

/****************************************************************************
REMARKS:
Call the assembler Zen Timer functions to do the timing.
****************************************************************************/
#define __LZTimerOn(tm)     VTD_Get_Real_Time(&tm->start.high,&tm->start.low)

/****************************************************************************
REMARKS:
Call the assembler Zen Timer functions to do the timing.
****************************************************************************/
static ulong __LZTimerLap(
    LZTimerObject *tm)
{
    CPU_largeInteger    lap,count;
    VTD_Get_Real_Time(&lap.high,&lap.low);
    _CPU_diffTime64(&tm->start,&lap,&count);
    return _CPU_calcMicroSec(&count,frequency);
}

/****************************************************************************
REMARKS:
Call the assembler Zen Timer functions to do the timing.
****************************************************************************/
#define __LZTimerOff(tm)    VTD_Get_Real_Time(&tm->end.high,&tm->end.low)

/****************************************************************************
REMARKS:
Call the assembler Zen Timer functions to do the timing.
****************************************************************************/
static ulong __LZTimerCount(
    LZTimerObject *tm)
{
    CPU_largeInteger tmCount;
    _CPU_diffTime64(&tm->start,&tm->end,&tmCount);
    return _CPU_calcMicroSec(&tmCount,frequency);
}

/****************************************************************************
REMARKS:
Define the resolution of the long period timer as microseconds per timer tick.
****************************************************************************/
#define ULZTIMER_RESOLUTION     1000

/****************************************************************************
REMARKS:
Read the Long Period timer value from the BIOS timer tick.
****************************************************************************/
static ulong __ULZReadTime(void)
{
    CPU_largeInteger count;
    VTD_Get_Real_Time(&count.high,&count.low);
    return (count.low * 1000.0 / frequency);
}

/****************************************************************************
REMARKS:
Compute the elapsed time from the BIOS timer tick. Note that we check to see
whether a midnight boundary has passed, and if so adjust the finish time to
account for this. We cannot detect if more that one midnight boundary has
passed, so if this happens we will be generating erronous results.
****************************************************************************/
ulong __ULZElapsedTime(ulong start,ulong finish)
{ return finish - start; }
