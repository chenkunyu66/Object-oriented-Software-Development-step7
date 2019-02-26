#include "stdafx.h"
#include "WorkshopController.h"
#include "WorkshopWnd.h"

#include <cassert>


/** \cond */
CWorkshopController::CWorkshopController()
{
}


CWorkshopController::~CWorkshopController()
{
}


void CWorkshopController::SetWorkshop(CWorkshopWnd *workshop)
{
    mWorkshop = workshop;
}
/** \endcond */


void CWorkshopController::EnableButtonNext(bool enable)
{
    assert(mWorkshop != NULL);
    mWorkshop->EnableButtonNext(enable);
}

void CWorkshopController::EnableButtonNaughty(bool enable)
{
    assert(mWorkshop != NULL);
    mWorkshop->EnableButtonNaughty(enable);

}

void CWorkshopController::EnableButtonShip(bool enable)
{
    assert(mWorkshop != NULL);
    mWorkshop->EnableButtonShip(enable);
}

void CWorkshopController::SetConveyerSpeed(double speed)
{
    assert(mWorkshop != NULL);
    mWorkshop->SetConveyerSpeed(speed);
}


double CWorkshopController::GetBagPosition()
{
    assert(mWorkshop != NULL);
    return mWorkshop->GetBagPosition();
}


void CWorkshopController::NewBag()
{
    assert(mWorkshop != NULL);
    mWorkshop->NewBag();
}


CClaw *CWorkshopController::GetClaw()
{
	assert(mWorkshop != NULL);

	return mWorkshop->GetClaw();
}


void CWorkshopController::LoadCoal()
{
    assert(mWorkshop != NULL);
    mWorkshop->GetCoal()->LoadCoal();
}


bool CWorkshopController::IsCoalLoaded()
{
    assert(mWorkshop != NULL);
    return mWorkshop->GetCoal()->IsCoalLoaded();
}
