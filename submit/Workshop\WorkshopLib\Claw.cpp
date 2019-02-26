#include "stdafx.h"
#include "Claw.h"
#include "WorkshopWnd.h"

#include <cassert>

const double ClawStartColumn = 1;
const double ClawStartRow = 2;

// Offset to make claw appear in the right position relative to a gift
const double ClawOffsetY = -70;

// Maximum rotation when pinching
const double PinchAngle = 20;

// Speed of the moving claw in cells per second
const double Speed = 2;

const double CClaw::BaggingColumn = 4.9;
const double CClaw::BaggingRow = 1.5;
const double CClaw::ParkingRow = 4;

// Distance to drop to get to the bag
const double ToBag = CClaw::BaggingRow - 1;

// Speed to drop the gift in cells per second
const double DropSpeed = 2;


CClaw::CClaw()
{
	mPositionX = BaggingColumn;
	mPositionY = ParkingRow;
}


CClaw::~CClaw()
{
}

/// \cond

void CClaw::Load(CWorkshopWnd *wnd)
{
	mWorkshopWnd = wnd;

	wnd->LoadImage(mClaw, L"WorkshopLib/images/Claw.png");
	wnd->LoadImage(mClaw1, L"WorkshopLib/images/Claw1.png");
	wnd->LoadImage(mClaw2, L"WorkshopLib/images/Claw2.png");
}


void CClaw::Draw(Gdiplus::Graphics *graphics)
{
	// Compute bottom left corner of the cell
	float x = (float)(CWorkshopWnd::GiftsX + mPositionX * CWorkshopWnd::GiftsDX);
	float y = (float)(CWorkshopWnd::GiftsY + (CWorkshopController::NumRows - mPositionY) * CWorkshopWnd::GiftsDY - 15);

	//
	// Left claw
	//
	float pivotY = (float)(y + ClawOffsetY - 53);

	auto state = graphics->Save();
	graphics->TranslateTransform((float)(x + 50), pivotY);
	graphics->RotateTransform((float)(-mPinch * PinchAngle));
	graphics->TranslateTransform(-29, -4);

	graphics->DrawImage(mClaw1.get(), 0.0f, 0.0f, (float)mClaw1->GetWidth(),
		(float)mClaw1->GetHeight());

	graphics->Restore(state);

	//
	// Right claw
	//
	state = graphics->Save();
	graphics->TranslateTransform((float)(x + 85), pivotY);
	graphics->RotateTransform((float)(mPinch * PinchAngle));
	graphics->TranslateTransform(-2, -4);

	graphics->DrawImage(mClaw2.get(), 0.0f, 0.0f, (float)mClaw2->GetWidth(),
		(float)mClaw2->GetHeight());

	graphics->Restore(state);

	graphics->DrawImage(mClaw.get(), 
		(float)(x - mClaw->GetWidth() / 2 + CWorkshopWnd::GiftsDX / 2),
		(float)(y - mClaw->GetHeight() + ClawOffsetY), 
		(float)mClaw->GetWidth(), 
		(float)mClaw->GetHeight());

	if (mGift != nullptr)
	{
		float gw = (float)mGift->GetWidth();
		float gh = (float)mGift->GetHeight();
		graphics->DrawImage(mGift.get(), 
			(float)(x - gw / 2 + CWorkshopWnd::GiftsDX / 2),
			(float)(y - gh - 6 + mDrop * DropSpeed * CWorkshopWnd::GiftsDY),
			gw, gh);
	}
}

void CClaw::Update(double elapsed)
{
	mPositionX += elapsed * mSpeedX * Speed;
	mPositionY += elapsed * mSpeedY * Speed;
	mPinch += elapsed * mPinchSpeed;
	if (mPinch < 0)
	{
		mPinch = 0;
		mPinchSpeed = 0;
	}
	else if (mPinch > 1)
	{
		mPinch = 1;
		mPinchSpeed = 0;
	}

	mDrop += mDropSpeed * elapsed;

	if (mDrop >= ToBag)
	{
		mDropSpeed = 0;
		mGift = nullptr;
		mDrop = 0;
	}
}

/// \endcond

void CClaw::Acquire(int col, int row)
{
	//
	// Ensure they are actually there
	//
	double Tolerance = 0.05;
	double pc = GetX();
	double pr = GetY();
	assert(col > pc - Tolerance && col < pc + Tolerance);
	assert(row > pr - Tolerance && row < pr + Tolerance);

	mGift = mWorkshopWnd->GetGift(col, row);
	SetPinchSpeed(1);
}


void CClaw::Drop()
{
	// Must be holding the gift
	assert(mGift != nullptr);

	mPinchSpeed = -1;
	mDropSpeed = 1;
}


bool CClaw::DropComplete()
{
	return mDropSpeed == 0 && mPinchSpeed == 0;
}
