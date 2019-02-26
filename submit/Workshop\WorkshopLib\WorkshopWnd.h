/**
 * \file WorkshopWnd.h
 * Workshop simulation class
 *
 * \author Charles Owen
 * \version 1.01 11-25-2015 Initial Development
 * \version 1.02 11-24-2018 Added the Claw
 */

#pragma once
#include <memory>
#include <string>
#include <vector>

#include "WorkshopController.h"
#include "TimeManagement.h"
#include "Platform.h"
#include "Claw.h"
#include "Coal.h"

/**
 * Workshop simulation class
 *
 * This class simulates a workshop at a secret location
 * that manages delivery of toys for a certain unnamed
 * individual.
 *
 * You do not directly access the functions in CWorkshopWnd.
 * Instead, use the functions in CWorkshopController for 
 * managing all Workshop operations.
 */
class AFX_EXT_CLASS CWorkshopWnd : public CWnd
{

public:
    /**
    * Constructor
    */
	CWorkshopWnd();

    /**
    * Destructor
    */
	virtual ~CWorkshopWnd();

    /**
     * Simulate the pressing of the NextOnList button
     */
    void PressNextOnList();

    /**
     * Simulate the pressing of the Naughty button
     */
    void PressNaughty();

    /**
     * Simulate the pressing of the Ship button
     */
    void PressShip();

	/**
	 * Press any of the three buttons
     *
	 * DO NOT USE THIS IN YOUR CONTROLLER
	 *
	 * This function is used for testing to ensure the 
	 * system does not respond to a button press that 
	 * it should not respond to.
	 * \param next Press the Next On List button
	 * \param naughty Press the Naughty button
	 * \param ship Press the Ship button.
	 */
	void Press(bool next, bool naughty, bool ship);

    /**
     * Simulate clicking on a gift
     * \param col Column we click on
     * \param row Row we click on
     */
    void ClickOnGift(int col, int row);

    /**
     * Get the bitmap for one of the gifts by column and row.
     * \param col The gift column
     * \param row The gift row
     * \return Reference to the gift bitmap
     */
    std::shared_ptr<Gdiplus::Bitmap> GetGift(int col, int row);

    /**
    * Update the simulation based on time
    *
    * This is normally only called from testing. It advances the simulation
    * by some elapsed time. Note that this does not have to be
    * real time.
    *
    * This will advance in millisecond increments only.
    *
    * \param elapsed Time to advance the simulation in seconds.
    */
    void Update(double elapsed);

    /**
     * Is the Next On List button enabled?
     * \return true if enabled 
     */
    bool IsButtonNextEnabled() { return mButtonNextEnabled;  }

    /**
    * Is the Naughty button enabled?
    * \return true if enabled
    */
    bool IsButtonNaughtyEnabled() { return mButtonNaughtyEnabled; }

    /**
    * Is the Ship button enabled?
    * \return true if enabled
    */
    bool IsButtonShipEnabled() { return mButtonShipEnabled; }

    /**
     * Load all images from files
     *
     * This is only used for Test, since it is automatically
     * called when the window is created when not in test
     */
    void Load();

    /**
     * Get the total number of coal loading that have been done.
     *
     * This is provided for testing purposes to ensure the
     * coal process is being done correctly 
     * \return Total number of time the coal loading process has completed.
     */
    int GetCoalCount() { return mCoal.GetCnt(); }

    /// \cond

    static const int GiftsX = 14;
    static const int GiftsY = 25;
    static const int GiftsDX = 132;
    static const int GiftsDY = 106;

    int GetWidth();
    int GetHeight();

    void SetController(std::shared_ptr<CWorkshopController> c);

    //
    // Bag operations
    //
    
    /**
    * Set the speed of the bag conveyer belt
    *
    * \param speed Speed to set. Positive values move the bag
    * to the right, negative values to the left. Values should
    * range from -1 to 1 (max left speed to max right speed)
    */
    void SetConveyerSpeed(double speed);

    /**
    * Get the bag position
    *
    * \returns Normalized value where 1 is off the screen to the right
    * and 0 is right at the loading area. Negative values are to the left
    * the loading area
    */
    double GetBagPosition();

    /**
    * Select a new bag
    *
    * Call this function to select a new bag. The new bag and name
    * are selected and the bag is set on the end of the conveyer,
    * which is off the screen. You should then set the speed to -1
    * to get the bag to roll into the loading area.
    */
    void NewBag();

    // Button enables
    void EnableButtonNext(bool enable) { mButtonNextEnabled = enable; }
    void EnableButtonNaughty(bool enable) { mButtonNaughtyEnabled = enable; }
    void EnableButtonShip(bool enable) { mButtonShipEnabled = enable; }

    // Platform operations
    CPlatform *GetPlatform() { return &mPlatform; }

	// Claw operations
	CClaw *GetClaw() { return &mClaw; }

	// Coal operations
    CCoal *GetCoal() { return &mCoal; }

	void LoadImage(std::shared_ptr<Gdiplus::Bitmap> &bitmap, const wchar_t *name);

private:
    DECLARE_DYNAMIC(CWorkshopWnd)

protected:

    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
    void LoadNames();
    void LoadGifts();
    void Draw(Gdiplus::Graphics *graphics, std::shared_ptr<Gdiplus::Bitmap> &bitmap, int x, int y);
    void DrawButtons(Gdiplus::Graphics *graphics);
    bool ButtonTest(CPoint point, int x, int y, std::shared_ptr<Gdiplus::Bitmap> &bitmap);
    void TimeManagement();
    void UpdateTimeslice();

    bool mFirstDraw = true;     ///< True for the first time we draw

    // A controller for this workshop
    std::shared_ptr<CWorkshopController> mController;

    /// Manage the simulation timing
    CTimeManagement mTimeManagement;

    double   mTime = 0;             ///< Local time

    std::shared_ptr<Gdiplus::Bitmap>  mBackground;
    std::shared_ptr<Gdiplus::Bitmap>  mBag;
    std::shared_ptr<Gdiplus::Bitmap>  mButtonNext;
    std::shared_ptr<Gdiplus::Bitmap>  mButtonNaughty;
    std::shared_ptr<Gdiplus::Bitmap>  mButtonShip;


    // The gifts
    std::shared_ptr<Gdiplus::Bitmap> mGifts[CWorkshopController::NumRows][CWorkshopController::NumCols];

    bool mButtonNextEnabled = true;
    bool mButtonNaughtyEnabled = false;
    bool mButtonShipEnabled = false;

    int mButtonNextX = 0;
    int mButtonNaughtyX = 0;
    int mButtonShipX = 0;

    double mBagPositionX;
    double mBagPositionY = 0;
    double mBagSpeed = 0;

    /// Names management
    std::vector<std::wstring> mNames;
    unsigned int mName = 0;      ///< Index to current name

    //
    // Platform
    //
    CPlatform mPlatform;

	//
	// Claw
	//
	CClaw mClaw;

    //
    // Coal
    //
    CCoal mCoal;
    
    /// \endcond
};




