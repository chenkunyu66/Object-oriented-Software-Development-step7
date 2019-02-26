/**
 * \file Controller.cpp
 *
 * \author
 */

#include "stdafx.h"
#include "Controller.h"
#include "Claw.h"



/**
 * Constructor
 */
CController::CController()
{
}

/**
 * Destructor
 */
CController::~CController()
{
}


/** Initialization function.
*
* This function is called when the system is initialized. This is
* later than when it is constructed, since the controller is not installed on
* a workshop when it is constructed. This function is called when the workshop
* controller is installed and ready to control a workshop.
*/
void CController::Initialize()
{
	EnableButtonNext(true);
	EnableButtonNaughty(false);
	EnableButtonShip(false);
}

/** Set a state
 * \param state The new state to set
 */
void CController::SetState(States state)
{
	mState = state;
	mStateTime = 0;
	// Entry activities for states
	switch (mState)
	{
	case Idle:
		EnableButtonNext(true);
		break;

	case ShowingBag:
		EnableButtonNaughty(false);
		EnableButtonNext(false);
		EnableButtonShip(false);
		SetConveyerSpeed(-1);
		break;

	case DroppCoal:
		EnableButtonNaughty(false);
		EnableButtonNext(false);
		EnableButtonShip(false);
		break;

	case ShipCoal:
		EnableButtonShip(true);
		break;

	case Shipped:
		EnableButtonNaughty(false);
		EnableButtonNext(false);
		EnableButtonShip(false);
		break;

	case ClawMoving:
		EnableButtonNaughty(false);
		EnableButtonNext(false);
		EnableButtonShip(false);
		GetClaw()->SetSpeed(-1, -1);
		break;

	case ClawPinching:
		GetClaw()->SetSpeed(0, 0);
		GetClaw()->SetPinchSpeed(1);
		GetClaw()->Acquire(mCol, mRow);
		break;

	case Dropped:
		GetClaw()->SetSpeed(0, 0);
		GetClaw()->Drop();
		break;

	case ReturnClaw:
		GetClaw()->SetPinchSpeed(0);
		GetClaw()->SetSpeed(0, 1);
		break;

	case ShipClaw:
		GetClaw()->SetSpeed(0, 0);
		EnableButtonShip(true);
		break;

	default:
		break;
	}
}

/** Service the workshop.
*
* This function is called once every millisecond during workshop
* operation. It provides a place to put code the controls the
* workshop. Override in a local function.
*/
void CController::Service()
{
	// Increment state time by 1 millisecond
	mStateTime += 0.001;
	switch (mState)
	{
	case Idle:
		break;

	case ShowingBag:
		if (GetBagPosition() < 0)
		{
			SetState(BagShow);
		}
		break;

	case BagShow:
		EnableButtonNaughty(true);
		SetConveyerSpeed(0);
		break;

	case DroppCoal:
		if (IsCoalLoaded())
		{
			SetState(ShipCoal);
		}
		break;

	case Shipped:
		if (GetBagPosition() >1)
		{
			SetConveyerSpeed(0);
			SetState(Idle);
		}
		break;

	case ClawMoving:
		if (GetClaw()->GetY() < mRow)
		{
			GetClaw()->SetSpeed(-1, 0);
			if (GetClaw()->GetX() < mCol)
			{
				SetState(ClawPinching);
			}
		}
		if (GetClaw()->GetX() < mCol)
		{
			GetClaw()->SetSpeed(0, -1);
			if (GetClaw()->GetY() < mRow)
			{
				SetState(ClawPinching);
			}
		}
		break;

	case ClawPinching:
		if (GetClaw()->GetPinch() == 1)
		{
			SetState(ClawPinch);
		}
		break;

	case ClawPinch:
		GetClaw()->SetPinchSpeed(0);
		if (mRow > 1.5)
		{
			GetClaw()->SetSpeed(1, -1);
			if (GetClaw()->GetY() < GetClaw()->BaggingRow)
			{
				GetClaw()->SetSpeed(1, 0);
				if (GetClaw()->GetX() >= GetClaw()->BaggingColumn)
				{
					SetState(Dropped);
				}
			}
		}
		else
		{
			GetClaw()->SetSpeed(1, 1);
			if (GetClaw()->GetY() > GetClaw()->BaggingRow)
			{
				GetClaw()->SetSpeed(1, 0);
				if (GetClaw()->GetX() >= GetClaw()->BaggingColumn)
				{
					SetState(Dropped);
				}
			}
		}
		break;

	case Dropped:
		if (GetClaw()->DropComplete())
		{
			SetState(ReturnClaw);
		}
		break;

	case ReturnClaw:
		if (GetClaw()->GetY() > 4)
		{
			SetState(ShipClaw);
		}
		break;

	default:
		break;
	}
}


/**
* Indication that the Next On List button has been pressed
*
* This function is called whenever the Next On List button is pressed.
* It is only called if the button is enabled.
*/
void CController::OnNextPressed() 
{
	if (mState == Idle)
	{
		NewBag();
		SetState(ShowingBag);
	}
}

/**
* Indication that the Naughty button has been pressed
*
* This function is called whenever the Naughty button is pressed.
* It is only called if the button is enabled.
*/
void CController::OnNaughtyPressed() 
{
	switch (mState)
	{
	case BagShow:
		SetState(DroppCoal);
		LoadCoal();
		break;

	default:
		break;
	}
}

/**
* Indication that the Ship button has been pressed
*
* This function is called whenever the Ship button is pressed.
* It is only called if the button is enabled.
*/
void CController::OnShipPressed() 
{
	switch (mState)
	{
	case ShipClaw:
		SetConveyerSpeed(1);
		SetState(Shipped);
		break;

	case ShipCoal:
		SetConveyerSpeed(1);
		SetState(Shipped);
		break;

	default:
		break;
	}
}

/**
* Indication that a Gift has been clicked on
*
* This functin is called whenever the user clicks on
* any of the gifts.
* \param col Column clicked on, left is 0, right is 3
* \param row Row clicked on. Bottom row is 0, top is 3
*/
void CController::OnGiftSelected(int col, int row)
{
	switch (mState)
	{
	case BagShow:
		mCol = col;
		mRow = row;
		SetState(ClawMoving);
		break;

	case ShipClaw:
		mCol = col;
		mRow = row;
		SetState(ClawMoving);
		break;

	default:
		break;
	}
}