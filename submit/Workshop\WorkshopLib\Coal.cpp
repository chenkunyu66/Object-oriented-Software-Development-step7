/**
* \file Coal.cpp
* \author Charles B. Owen
* \cond
*/

#include "stdafx.h"
#include "Coal.h"
#include "WorkshopWnd.h"

const int CoalX = 673;

CCoal::CCoal()
{
}


CCoal::~CCoal()
{
}


void CCoal::Draw(Gdiplus::Graphics *graphics)
{
    graphics->DrawImage(mCoalChute.get(), (float)CoalX, 0.0f, (float)mCoalChute->GetWidth(), (float)mCoalChute->GetHeight());

    if (mState == Dropping)
    {
        graphics->DrawImage(mCoalLump.get(), (float)CoalX, mDrop, (float)mCoalLump->GetWidth(), (float)mCoalLump->GetHeight());
    }

    auto save = graphics->Save();
    graphics->TranslateTransform(0, 17);
    graphics->ScaleTransform(1, mDoorScale);
    graphics->TranslateTransform(0, -17);
    graphics->DrawImage(mCoalDoor.get(), (float)CoalX, 0.0f, (float)mCoalDoor->GetWidth(), (float)mCoalDoor->GetHeight());
    graphics->Restore(save);


}

void CCoal::Load(CWorkshopWnd *wnd)
{
    wnd->LoadImage(mCoalChute, L"WorkshopLib/images/CoalChute.png");
    wnd->LoadImage(mCoalDoor, L"WorkshopLib/images/CoalDoor.png");
    wnd->LoadImage(mCoalLump, L"WorkshopLib/images/CoalLump.png");
}

void CCoal::Update(double elapsed)
{
    mDoorScale += (float)(mDoorScaleSpeed * elapsed);
    mDrop += (float)(mDropSpeed * elapsed);
    ServiceState();
}


/**
* Cause coal to be dropped into the bag
*/
void CCoal::LoadCoal()
{
    SetState(Opening);
}

/**
* Determine if the process of dropping coal is done
* \return true if coal has been loaded
*/
bool CCoal::IsCoalLoaded()
{
    return mState == Done;
}

void CCoal::SetState(States state)
{
    mState = state;

    switch (mState)
    {
    case Opening:
        mDoorScaleSpeed = -1.0f;
        break;

    case Dropping:
        mDoorScaleSpeed = 0;
        mDrop = 10;
        mDropSpeed = 200;
        break;

    case Closing:
        mDoorScaleSpeed = 1.0f;
        mDrop = 0;
        mDropSpeed = 0; 
        break;

    case Done:
        mDoorScale = 1;
        mDoorScaleSpeed = 0;
        mCnt++;
        break;
    }

}

void CCoal::ServiceState()
{
    switch (mState)
    {
    case Opening:
        if (mDoorScale < 0.5)
        {
            SetState(Dropping);
        }
        break;

    case Dropping:
        if (mDrop > 300)
        {
            SetState(Closing);
        }
        break;

    case Closing:
        if (mDoorScale >= 1) 
        {
            SetState(Done);
        }
        break;
    }
}

/// \endcond
