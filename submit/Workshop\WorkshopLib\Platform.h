/**
 * \file Platform.h
 *
 * \author Charles Owen
 *
 * \cond
 */
#pragma once

#include <memory>

class CWorkshopWnd;


class AFX_EXT_CLASS CPlatform
{
public:
    CPlatform();
    virtual ~CPlatform();

	/** \brief Copy constructor disabled */
	CPlatform(const CPlatform &) = delete;
	/** \brief Assignment operator disabled */
	void operator=(const CPlatform &) = delete;

    void Draw(Gdiplus::Graphics *graphics);
    void Load(CWorkshopWnd *wnd);
    void Update(double elapsed);

    /**
     * Set the platform movement speed
     * \param speedX Speed from -1 to 1. 1 moves right 
     * \param speedY Speed from -1 to 1. 1 moves up 
     */
    void SetSpeed(double x, double y) { mSpeedX = x; mSpeedY = y; }

    /**
     * Get the platform position X
     * \return Relative platform position
     *
     * A value of 0 is centered on the first bin (left to right), 1 is centerned 
     * on the second, etc. Valid values should be 0-4, where 4 is 
     * where we dump into the bag 
     */
    double GetX() { return mPositionX; }

    /**
     * Get the platform position Y
     * \return Relative platform position
     *
     * A value of 0 is in front of the first bin (bottom), 1 is in front of 
     * on the second, etc. Valid values should be 0-3, where 3 is 
     * the top most bin.
     */
    double GetY() { return mPositionY; }

	/**
	 * Acquire the gift that is at a given column and row for the platform
	 *
	 * This makes a copy of the gift on the platfrom so we
	 * can move it to the bag.
	 * \param col Column number
	 * \param row Row number
	 */
    void Acquire(std::shared_ptr<Gdiplus::Bitmap> gift) { mGift = gift; mSlide = 0; mSlideRate = 0; }
    void BagIt();
    bool IsBagItDone();

    std::shared_ptr<Gdiplus::Bitmap> GetGift() { return mGift; }

private:
    enum BaggingStates {Tipping, TipDrop, Dropping, Untipping, Done};

    void SetBaggingState(BaggingStates state);
    void ServiceBaggingState();

    std::shared_ptr<Gdiplus::Bitmap>  mPlatform;
    std::shared_ptr<Gdiplus::Bitmap>  mPlatformTop;

	// Any gift we are currently carrying
	std::shared_ptr<Gdiplus::Bitmap> mGift;

    double mPositionX;
    double mPositionY;

    double mSpeedX = 0;
    double mSpeedY = 0;

    float mTip = 0;         ///< Top tip amount in degrees
    float mTipRate = 0;     ///< Top tip amount in degrees per second

    float mSlide = 0;       ///< Amount we have slide item off platform
    float mSlideRate = 0;   ///< Rate we are sliding the item

    BaggingStates mBaggingState = Untipping;
};

/// \endcond
