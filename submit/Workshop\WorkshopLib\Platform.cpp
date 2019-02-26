/**
 * \file Platform.cpp
 *
 * \author Charles Owen
 * \cond
 */

#include "stdafx.h"
#include "Platform.h"
#include "Claw.h"
#include "WorkshopWnd.h"

#include <cassert>

const double Speed = 1;

const double BaggingColumn = 4;
const double BaggingRow = 1.5;


CPlatform::CPlatform()
{
    mPositionX = BaggingColumn;
    mPositionY = BaggingRow;
}


CPlatform::~CPlatform()
{
}

void CPlatform::Load(CWorkshopWnd *wnd)
{
    wnd->LoadImage(mPlatform, L"WorkshopLib/images/Platform.png");
    wnd->LoadImage(mPlatformTop, L"WorkshopLib/images/PlatformTop.png");
}


void CPlatform::Draw(Gdiplus::Graphics *graphics)
{
    float x = (float)(CWorkshopWnd::GiftsX + mPositionX * CWorkshopWnd::GiftsDX);
    float y = (float)(CWorkshopWnd::GiftsY + (CWorkshopController::NumRows - mPositionY) * CWorkshopWnd::GiftsDY - 15);

    graphics->DrawImage(mPlatform.get(), x, y + mPlatformTop->GetHeight(), (float)mPlatform->GetWidth(), (float)mPlatform->GetHeight());

    auto state = graphics->Save();
    float w = (float)mPlatformTop->GetWidth();
    float h = (float)mPlatformTop->GetHeight();

    graphics->TranslateTransform(x + w, y + h);
    graphics->RotateTransform(mTip);
    graphics->TranslateTransform(-w, -h);
    graphics->DrawImage(mPlatformTop.get(), 0.0f, 0.0f, w, h);

    if (mGift != nullptr)
    {
        float gw = (float)mGift->GetWidth();
        float gh = (float)mGift->GetHeight();
        graphics->DrawImage(mGift.get(), (w - gw) / 2 + mSlide, -gh, gw, gh);
    }

    graphics->Restore(state);
}

void CPlatform::Update(double elapsed)
{
    mPositionX += elapsed * mSpeedX * Speed;
    mPositionY += elapsed * mSpeedY * Speed;
    mTip += (float)(mTipRate * elapsed);
    mSlide += (float)(mSlideRate * elapsed);

    ServiceBaggingState();
}

/**
* Cause the platform to drop the gift into the bag
*
* Check when this is done using the function IsBagItDone()
*/
void CPlatform::BagIt()
{
    //
    // Ensure they are actually there
    //
    double Tolerance = 0.05;
    double pc = GetX();
    double pr = GetY();
    double col = BaggingColumn;
    double row = BaggingRow;

    assert(col > pc - Tolerance && col < pc + Tolerance);
    assert(row > pr - Tolerance && row < pr + Tolerance);

    SetBaggingState(Tipping);
}

/**
* Determine if the BagIt operation is done
* \return true if BagIt operation is done
*/
bool CPlatform::IsBagItDone()
{
    return mBaggingState == Done;
}


void CPlatform::SetBaggingState(BaggingStates state)
{
    mBaggingState = state;

    switch (mBaggingState)
    {
    case Tipping:
        mTipRate = 45;
        break;

    case TipDrop:
        mSlideRate = 100;
        break;

    case Dropping:
        mTipRate = 0;
        break;

    case Untipping:
        mSlide = 0;
        mSlideRate = 0;
        mGift = nullptr;
        mTipRate = -45;
        break;

    case Done:
        mTip = 0;
        mTipRate = 0;
        break;
    }
}

void CPlatform::ServiceBaggingState()
{
    switch (mBaggingState)
    {
    case Tipping:
        if (mTip > 45)
        {
            SetBaggingState(TipDrop);
            break;
        }
        break;

    case TipDrop:
        if (mTip >= 90)
        {
            SetBaggingState(Dropping);
            break;
        }
        break;

    case Dropping:
        if (mSlide > 125)
        {
            SetBaggingState(Untipping);

        }
        break;

    case Untipping:
        if (mTip <= 0)
        {
            SetBaggingState(Done);
        }
        break;
    }
}

/// \endcond
