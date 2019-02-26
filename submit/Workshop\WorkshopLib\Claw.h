/**
 * \file Claw.h
 * \author Charles B. Owen
 *
 * Class that represents and manages the claw
 */
#pragma once

#include <memory>

class CWorkshopWnd;

/**
 * Class that represents and manages the claw
 */
class AFX_EXT_CLASS CClaw
{
public:
	CClaw();
	virtual ~CClaw();

	/// The appropriate column to move to for bagging
	const static double BaggingColumn;

	/// The appropriate row to move to for bagging
	const static double BaggingRow;

	/// Row we move the claw to to park
	const static double ParkingRow;

	/** \brief Copy constructor disabled */
	CClaw(const CClaw &) = delete;
	/** \brief Assignment operator disabled */
	void operator=(const CClaw &) = delete;

	/**
	 * Set the claw movement speed
	 * \param x Speed from -1 to 1. 1 moves right
	 * \param y Speed from -1 to 1. 1 moves up
	 */
	void SetSpeed(double x, double y) { mSpeedX = x; mSpeedY = y; }

	/**
	 * Get the claw position X
	 * \return Relative claw position
	 *
	 * A value of 0 is centered on the first bin (left to right), 1 is centered
	 * on the second, etc. Valid values should be 0-4, where 4 is
	 * where we dump into the bag
	 */
	double GetX() { return mPositionX; }

	/**
	 * Get the claw position Y
	 * \return Relative claw position
	 *
	 * A value of 0 is in front of the first bin (bottom), 1 is in front of
	 * on the second, etc. Valid values should be 0-3, where 3 is
	 * the top most bin.
	 */
	double GetY() { return mPositionY; }

	/**
	 * Acquire the gift that is at a given column and row for the claw
	 *
	 * This makes a copy of the gift on the claw so we
	 * can move it to the bag. This operation takes one second to complete.
	 * \param col Column number
	 * \param row Row number
	 */
	void Acquire(int col, int row);

	/**
	 * Set the speed of the pinch motor.
	 * \param s Speed. 0=stopped, 1=closing, 2=opening
	 */
	void SetPinchSpeed(double s) { mPinchSpeed = s; }

	/**
	 * Get the amount of the pinch.
	 * \return 0 to 1, where 0 is open, 1 is closed
	 */
	double GetPinch() const { return mPinch; }

	/**
	 * Drop any package currently held by the claw.
	 */
	void Drop();

	/**
	 * Determine if the gift drop operation is completed.
	 * \return true if drop operation is completed.
	 */
	bool DropComplete();

	/**
	 * Get whatever gift is currently being held by the claw
	 * \return Gift bitmap pointer.
	 */
	std::shared_ptr<Gdiplus::Bitmap> GetGift() { return mGift; }

	/** \cond */

	void Load(CWorkshopWnd * wnd);
	void Draw(Gdiplus::Graphics * graphics);

	void Update(double elapsed);

private:
	std::shared_ptr<Gdiplus::Bitmap>  mClaw;
	std::shared_ptr<Gdiplus::Bitmap>  mClaw1;
	std::shared_ptr<Gdiplus::Bitmap>  mClaw2;

	double mPositionX;
	double mPositionY;

	double mSpeedX = 0;
	double mSpeedY = 0;

	double mPinch = 0;		// Pinch value from 0 to 1
	double mPinchSpeed = 0;	// Pinch change speed

	double mDrop = 0;		// Amount of drop
	double mDropSpeed = 0;	// Dropping speed

	// Any gift we are currently carrying
	std::shared_ptr<Gdiplus::Bitmap> mGift;

	CWorkshopWnd *mWorkshopWnd = nullptr;

	/** \endcond */
};

