/**
 * \file Controller.h
 *
 * \author 
 *
 * 
 */

#include "WorkshopController.h"

#pragma once
/**
 *controller class
 */
class CController :
    public CWorkshopController
{
public:
    CController();
    virtual ~CController();
	/// The state Workshop states
	enum States {
		Idle, ShowingBag, BagShow, DroppCoal, ShipCoal, ShipClaw,
		Shipped, ClawMoving, ClawPinching, ClawPinch, Dropped, ReturnClaw };

    virtual void Initialize() override;

    virtual void OnNextPressed() override;
    virtual void OnNaughtyPressed() override;
    virtual void OnShipPressed() override;
    virtual void Service() override;
    virtual void OnGiftSelected(int col, int row) override;

private:

	void SetState(States state);
	
	States mState = Idle; ///<State
	
	double mStateTime = 0;  ///< The time in a current state

	///Row of the gift
	int mRow;
	///column of the gift
	int mCol;
};

