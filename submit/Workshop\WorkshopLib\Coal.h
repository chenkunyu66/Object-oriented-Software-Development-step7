/**
 * \file Coal.h
 * \author Charles B. Owen
 * \cond
 */

#include <memory>

class CWorkshopWnd;

#pragma once
class AFX_EXT_CLASS CCoal
{
public:
    CCoal();
    ~CCoal();

    void Draw(Gdiplus::Graphics *graphics);
    void Load(CWorkshopWnd *wnd);
    void Update(double elapsed);

    void LoadCoal();
    bool IsCoalLoaded();

    int GetCnt() { return mCnt; }

private:
    enum States {Opening, Dropping, Closing, Done};

    void SetState(States state);
    void ServiceState();

    States mState = Done;

    std::shared_ptr<Gdiplus::Bitmap>  mCoalChute;
    std::shared_ptr<Gdiplus::Bitmap>  mCoalDoor;
    std::shared_ptr<Gdiplus::Bitmap>  mCoalLump;

    float mDoorScale = 1;
    float mDoorScaleSpeed = 0;

    float mDrop = 0;
    float mDropSpeed = 0;

    int mCnt = 0;
};

/// \endcond
