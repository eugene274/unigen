// ------------------------------------------------------------------
// -----                         UTaskCm2Lab                    -----
// -----              Created 2006/07/04 by D. Kresan           -----
// ------------------------------------------------------------------
#include <iostream>
using namespace std;

#include "TMath.h"

#include "UManager.h"
#include "URun.h"
#include "UEvent.h"
#include "UParticle.h"
#include "UTaskCm2Lab.h"


//___________________________________________________________________
//
// UTaskCm2Lab
//
// Task for boosting the particles momenta from CM to LAB frame
//


// ------------------------------------------------------------------
UTaskCm2Lab::UTaskCm2Lab()
{
    // Default constructor
    fEvent = NULL;
}
// ------------------------------------------------------------------


// ------------------------------------------------------------------
UTaskCm2Lab::UTaskCm2Lab(const char *name, Int_t verbose)
:UTask(name, verbose)
{
    // Standard constructor
    fEvent = NULL;
}
// ------------------------------------------------------------------


// ------------------------------------------------------------------
UTaskCm2Lab::~UTaskCm2Lab()
{
    // Destructor
}
// ------------------------------------------------------------------


// ------------------------------------------------------------------
void UTaskCm2Lab::Init()
{
    // Task initialisation
    UManager *mgr = UManager::Instance();
    if(NULL == mgr) {
	cout << "-E- UTaskCm2Lab::Init: "
	    << "UManager is not instantiated!" << endl;
        return;
    }
    // Get Run description
    URun *run = mgr->GetRun();
    fCM = kFALSE;
    if(TMath::Abs(run->GetPTarg()) > 0) fCM = kTRUE;
    fBetaCM = 0.;
    if(fCM) {
        cout << "-I- UTaskCm2Lab::Init: we are in CM frame" << endl;
	Double_t elab = (TMath::Power(run->GetSqrtS(),2)/(run->GetAProj()*run->GetATarg())-
			 2*TMath::Power(0.938271998,2))/(2*0.938271998);
	Double_t plab = TMath::Sqrt(elab*elab - TMath::Power(0.938271998,2));
        cout << "-I- UTaskCm2Lab::Init: Plab = " << plab << " AGeV" << endl;
        fBetaCM = plab / (elab + 0.938271998);
	run->SetPProj(plab);
        run->SetPTarg(0.);
    } else {
        cout << "-I- UTaskCm2Lab::Init: we are in LAB frame" << endl;
    }
    fGammaCM = 1./TMath::Sqrt(1. - fBetaCM*fBetaCM);
    cout << "-I- UTaskCm2Lab::Init: boost parameters:" << endl
	<< "                             betaCM = " << fBetaCM
	<< ",  gammaCM = " << fGammaCM << endl;
    // Get event
    fEvent = mgr->GetEvent();
}
// ------------------------------------------------------------------


// ------------------------------------------------------------------
void UTaskCm2Lab::Exec(Option_t *option)
{
    // Task execution
    if(NULL == fEvent) return;

    // if we are in LAB - no action
    if(! fCM) return;

    // Control output
    if(0 == (fEvents%10)) {
	cout << "-I- UTaskCm2Lab::Exec: event " << fEvents
            << " has " << fEvent->GetNpa() << " tracks" << endl;
    }

    UParticle *particle;
    Double_t pz;
    Double_t e;
    Double_t pz1;
    Double_t ee;

    // Loop over particles
    for(Int_t i = 0; i < fEvent->GetNpa(); i++) {
	// Get particle
	particle = fEvent->GetParticle(i);
	if(NULL == particle) continue;
	// Make boost
	pz = particle->Pz();
        e = particle->E();
	pz1 = fGammaCM*(pz + fBetaCM*e);
	ee = TMath::Sqrt(e*e - pz*pz + pz1*pz1);
	// Set new momentum and energy
	particle->SetPz(pz1);
        particle->SetE(ee);
    }

    fEvents += 1;
}
// ------------------------------------------------------------------


// ------------------------------------------------------------------
void UTaskCm2Lab::Finish()
{
    // Finish of the task execution
    cout << "-I- UTaskCm2Lab::Finish: boost completed" << endl;
}
// ------------------------------------------------------------------


ClassImp(UTaskCm2Lab);



