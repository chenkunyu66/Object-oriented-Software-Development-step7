/**
 * \file WorkshopWnd.cpp
 *
 * \author Charles Owen
 */

#include "stdafx.h"
#include <algorithm>    // std::random_shuffle

#include "WorkshopLib.h"
#include "WorkshopWnd.h"
#include "DoubleBufferDC.h"

#include <cassert>

using namespace std;
using namespace Gdiplus;

/// \cond 

// Frame duration in milliseconds
const int FrameDuration = 50;

/* Pixel values */
const int WorkshopWidth = 800;
const int WorkshopHeight = 600;

const int ButtonY = 510;    // Pixels

// Maximum speed for the bag in pixels per second
const double MaxBagSpeed = 200;

// How far to be off screen for the bag
const double BagOffScreen = 200;

// Duration of a time slice for our simulation
const double SimulationTimeslice = 0.001;

// Set true if we are using the platform
const bool UsePlatform = false;

// Set true if we are using the claw
const bool UseClaw = true;


IMPLEMENT_DYNAMIC(CWorkshopWnd, CWnd)

/// \endcond

/** Constructor */
CWorkshopWnd::CWorkshopWnd()
{
    srand((unsigned int)time(nullptr));

    LoadNames();

    mBagPositionX = BagOffScreen;
}

CWorkshopWnd::~CWorkshopWnd()
{
}

void CWorkshopWnd::PressNextOnList()
{
    if (mButtonNextEnabled)
    {
        mController->OnNextPressed();
        return;
    }
}

/**
* Simulate the pressing of the Naughty button
*/
void CWorkshopWnd::PressNaughty()
{
    if (mButtonNaughtyEnabled)
    {
        mController->OnNaughtyPressed();
        return;
    }

}

/**
* Simulate the pressing of the Ship button
*/
void CWorkshopWnd::PressShip()
{
    if (mButtonShipEnabled)
    {
        mController->OnShipPressed();
        return;
    }

}

void CWorkshopWnd::ClickOnGift(int col, int row)
{
    mController->OnGiftSelected(col, row);
}

void CWorkshopWnd::Update(double elapsed)
{
    while (elapsed >= SimulationTimeslice)
    {
        UpdateTimeslice();
        elapsed -= SimulationTimeslice;
    }
}


void CWorkshopWnd::Load()
{
    //
    // Load all of the images
    //
    LoadImage(mBackground, L"WorkshopLib/images/Workshop.png");
    LoadImage(mBag, L"WorkshopLib/images/Bag.png");
    LoadImage(mButtonNext, L"WorkshopLib/images/ButtonNext.png");
    LoadImage(mButtonNaughty, L"WorkshopLib/images/ButtonNaughty.png");
    LoadImage(mButtonShip, L"WorkshopLib/images/ButtonShip.png");

    mCoal.Load(this);
    LoadGifts();

	if (UsePlatform) 
	{
		mPlatform.Load(this);
	}

	if (UseClaw)
	{
		mClaw.Load(this);
	}
}

/// \cond

/**
 * Load some simple names to put on the bags
 */
void CWorkshopWnd::LoadNames()
{
    mNames.push_back(L"Timmy");
    mNames.push_back(L"Suzy");
    mNames.push_back(L"Anton");
    mNames.push_back(L"Harold");
    mNames.push_back(L"Katie");
    mNames.push_back(L"Mary");
    mNames.push_back(L"Noel");
    mNames.push_back(L"Carl");
    mNames.push_back(L"Liz");
    mNames.push_back(L"Billy");
    mNames.push_back(L"Fran");
    mNames.push_back(L"Greg");
    mNames.push_back(L"Judy");
    mNames.push_back(L"Len");
    mNames.push_back(L"Neil");
    mNames.push_back(L"Pam");
    mNames.push_back(L"Tommy");
    mNames.push_back(L"Art");

    std::random_shuffle(mNames.begin(), mNames.end());
}

void CWorkshopWnd::LoadGifts()
{
    const wstring Gifts[CWorkshopController::NumRows][CWorkshopController::NumCols ] = { 
        { L"Teddy.png", L"Lincoln.png", L"Barbie.png", L"Etch.png" },
        { L"Potato.png", L"Minion.png", L"Car.png", L"Phibes.png" },
        { L"Emmet.png", L"Spongebob.png", L"Cathy.png", L"Raptor.png" },
        { L"Elle.png", L"Bieber.png", L"Poo.png", L"Jon.png" }
    };

    for (int r = 0; r < CWorkshopController::NumRows; r++)
    {
        for (int c = 0; c < CWorkshopController::NumCols; c++)
        {
            LoadImage(mGifts[r][c], (wstring(L"WorkshopLib/images/") + Gifts[r][c]).c_str());
        }
    }

}

BEGIN_MESSAGE_MAP(CWorkshopWnd, CWnd)
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_WM_LBUTTONDOWN()
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


/** The workshop width. Based on the width of the background image
* \return Width in pixels */
int CWorkshopWnd::GetWidth()
{
    return WorkshopWidth;
}

/** The workshop height. Based on the height of the background image
* \return Height in pixels */
int CWorkshopWnd::GetHeight()
{
    return WorkshopHeight;
}

void CWorkshopWnd::OnPaint()
{
    if (mFirstDraw)
    {
        SetTimer(1, FrameDuration, nullptr);
        mFirstDraw = false;
    }

    // Handle time management for the simulation
    TimeManagement();

    CPaintDC paintDC(this); // device context for painting

    CDoubleBufferDC dbDC(&paintDC);

    Graphics graphics(dbDC.m_hDC);    // Create GDI+ graphics context

    int w, h;
    CRect rect;
    GetClientRect(&rect);
    w = rect.Width();
    h = rect.Height();
    int bottom = mBackground->GetHeight();

    //
    // Background
    //

    Draw(&graphics, mBackground, 0, 0);

    //
    // Gifts
    //


    for (int r = 0; r < CWorkshopController::NumRows; r++)
    {
        for (int c = 0; c < CWorkshopController::NumCols; c++)
        {
            int cx = (GiftsDX - mGifts[r][c]->GetWidth()) / 2;
            Draw(&graphics, mGifts[r][c], GiftsX + c * GiftsDX + cx, GiftsY + (CWorkshopController::NumRows - r - 1) * GiftsDY);
        }
    }

    //
    // The platform
    //

	if (UsePlatform)
	{
		mPlatform.Draw(&graphics);
	}

    //
    // Coal
    //
    mCoal.Draw(&graphics);

	//
	// The Claw
	//
	if (UseClaw)
	{
		mClaw.Draw(&graphics);
	}

    //
    // Bag
    //
    const int BagY = 307;
    const int BagX = 667;
    int x = (int)(BagX + mBagPositionX);
    int y = (int)(BagY + mBagPositionY);
    Draw(&graphics, mBag, x, y);

    FontFamily fontFamily(L"Arial");
    Gdiplus::Font font(&fontFamily, 15);

    SolidBrush white(Color(255, 255, 255));
    graphics.DrawString(mNames[mName].c_str(),  // String to draw
        -1,         // String length, -1 means it figures it out on its own
        &font,      // The font to use
        PointF((float)x + 49, (float)y + 35),   // Where to draw (top left corner)
        &white);    // The brush to draw the text with



    //
    // Buttons
    //
    DrawButtons(&graphics);

}


void CWorkshopWnd::DrawButtons(Gdiplus::Graphics *graphics)
{
    const int ButtonGap = 15;   // Pixels

    int buttonTotal = mButtonNext->GetWidth() +
        mButtonNaughty->GetWidth() +
        mButtonShip->GetWidth() + ButtonGap * 2;

    int x = (GetWidth() - buttonTotal) / 2;

    mButtonNextX = x;
    if (mButtonNextEnabled)
    {
        Draw(graphics, mButtonNext, x, ButtonY);
    }

    x += mButtonNext->GetWidth() + ButtonGap;
    mButtonNaughtyX = x;

    if (mButtonNaughtyEnabled)
    {
        Draw(graphics, mButtonNaughty, x, ButtonY);
    }

    x += mButtonNaughty->GetWidth() + ButtonGap;
    mButtonShipX = x;

    if (mButtonShipEnabled)
    {
        Draw(graphics, mButtonShip, x, ButtonY);
    }

}





void CWorkshopWnd::OnTimer(UINT_PTR nIDEvent)
{
    Invalidate();
    UpdateWindow();

    CWnd::OnTimer(nIDEvent);
}


void CWorkshopWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    //
    // Standard buttons
    //

    if (mButtonNextEnabled && ButtonTest(point, mButtonNextX, ButtonY, mButtonNext))
    {
        mController->OnNextPressed();
        return;
    }

    if (mButtonNaughtyEnabled && ButtonTest(point, mButtonNaughtyX, ButtonY, mButtonNaughty))
    {
        mController->OnNaughtyPressed();
        return;
    }

    if (mButtonShipEnabled && ButtonTest(point, mButtonShipX, ButtonY, mButtonShip))
    {
        mController->OnShipPressed();
        return;
    }

    //
    // Click on gifts
    //
    if (point.x >= GiftsX && point.y >= GiftsY)
    {
        int c = (point.x - GiftsX) / GiftsDX;
        int r = (point.y - GiftsY) / GiftsDY;
        if (c < CWorkshopController::NumCols && r < CWorkshopController::NumRows) 
        {
            mController->OnGiftSelected(c, CWorkshopController::NumRows - r - 1);
        }
        
    }

    CWnd::OnLButtonDown(nFlags, point);
}

bool CWorkshopWnd::ButtonTest(CPoint point, int x, int y, std::shared_ptr<Gdiplus::Bitmap> &bitmap)
{
    return point.x >= x && point.x <= x + (int)bitmap->GetWidth() &&
        point.y >= y && point.y <= y + (int)bitmap->GetHeight();
}


int CWorkshopWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    Load();



    return 0;
}




BOOL CWorkshopWnd::OnEraseBkgnd(CDC* pDC)
{
    return FALSE;
}

/** Load an image into a local bitmap pointer
* \param bitmap Reference to shared_ptr we load into
* \param name The filename to load */
void CWorkshopWnd::LoadImage(std::shared_ptr<Gdiplus::Bitmap> &bitmap, const wchar_t *name)
{
    bitmap = shared_ptr<Bitmap>(Bitmap::FromFile(name));
    if (bitmap->GetLastStatus() != Ok)
    {
        wstring msg(L"Failed to open: ");
        msg += name;
        AfxMessageBox(msg.c_str());
    }
}

/**
* Draw a bitmap it's native size.
* \param bitmap Bitmap declared as a shared_ptr to a Gdiplus::Bitmap
* \param x X location in pixels
* \param y Y location in pixels
*/
void CWorkshopWnd::Draw(Gdiplus::Graphics *graphics, std::shared_ptr<Gdiplus::Bitmap> &bitmap, int x, int y)
{
    graphics->DrawImage(bitmap.get(), x, y, bitmap->GetWidth(), bitmap->GetHeight());
}

/**
* Set the elevator controller we are using for this elevator.
* @param c Elevator controller
*/
void CWorkshopWnd::SetController(std::shared_ptr<CWorkshopController> c)
{
    mController = c;
    mController->SetWorkshop(this);
    mController->Initialize();
}


/** Handle once-per-draw time management
*
* Handles updating the time management component and
* calls to the local Update function.
*/
void CWorkshopWnd::TimeManagement()
{
    mTimeManagement.Update();

    while (mTime < mTimeManagement.GetTime())
    {
        UpdateTimeslice();
        mTime += SimulationTimeslice;
    }

}


/** Update in time
*/
void CWorkshopWnd::UpdateTimeslice()
{
    double elapsed = SimulationTimeslice;

    //
    // Move the bag
    //
    mBagPositionX += mBagSpeed * elapsed;
    if (mBagPositionX < -90) 
    {
        mBagPositionY += elapsed * 400;
    }

    //
    // Platform
    //
	if (UsePlatform)
	{
		mPlatform.Update(elapsed);
	}

	//
	// Claw
	//
	if (UseClaw)
	{
		mClaw.Update(elapsed);
	}

	//
	// Coal
	//
    mCoal.Update(elapsed);

    if (mController != nullptr)
    {
        mController->Service();
    }

}


void CWorkshopWnd::SetConveyerSpeed(double speed)
{
    // Actual stored speed is in pixels per second
    mBagSpeed = speed * MaxBagSpeed;
}

double CWorkshopWnd::GetBagPosition()
{
    return mBagPositionX / BagOffScreen;
}

void CWorkshopWnd::NewBag()
{
    mName++;
    if (mName >= mNames.size()) {
        mName = 0;
    }

    mBagPositionX = BagOffScreen;
    mBagPositionY = 0;
}



std::shared_ptr<Gdiplus::Bitmap> CWorkshopWnd::GetGift(int col, int row)
{
	assert(col >= 0);
	assert(col < CWorkshopController::NumCols);
	assert(row >= 0);
	assert(row < CWorkshopController::NumRows);

	return mGifts[row][col];
}


void CWorkshopWnd::Press(bool next, bool naughty, bool ship)
{
	if (next)
	{
		PressNextOnList();
	}

	if (naughty)
	{
		PressNaughty();
	}

	if (ship)
	{
		PressShip();
	}
}


/// \endcond
