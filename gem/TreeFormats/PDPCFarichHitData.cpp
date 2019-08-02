
#include "PDPCFarichHitData.h"
#include "TString.h"
#include <iostream>
#include <iomanip>

using namespace std;

ClassImp(PDPCFarichHitData)

PDPCFarichHitData::PDPCFarichHitData() : TObject()
{
    Init();
}

void PDPCFarichHitData::Init()
{
    firstHit = kFALSE;
    dieSID = 0;
    die = 0;
    tile = 0;
    module = 0;
    busboard = 0;
    target = 0;
    tileUID = 0;
    timestamp = 0;
    frame = 0;
    frameSequence = 0;
    sync = -1;
    cellCount[0] = 0;
    cellCount[1] = 0;
    cellCount[2] = 0;
    cellCount[3] = 0;
}

void PDPCFarichHitData::Print(Option_t *opt)
{
    TString sopt(opt);
    if( sopt.Contains("h",TString::kExact) && firstHit || 
        sopt.Contains("H",TString::kExact) ) { //print header before first hit data if 'h' option given
        cout << "first\tsync\tseq\tframe\ttarget\tdie\tTS\t ph1\t ph2\t ph3\t ph4" << endl;
    }
    //    streamsize w=cout.width(8);
    cout.setf(ios::left);
    cout << setw(8) << firstHit << setw(8) << sync << setw(8) << frameSequence
        << setw(8) << frame << setw(8) << target << setw(8) << die << setw(8) << timestamp << " "
        << setw(8) << cellCount[0] << setw(8) << cellCount[1] << setw(8) << cellCount[2]
        << setw(8) << cellCount[3] << endl;
}

Int_t PDPCFarichHitData::Compare(const TObject* right) const
{
    PDPCFarichHitData* r = (PDPCFarichHitData*)right;
    if( frameSequence<r->GetFrameSequence() )
        return -1;
    else if( frameSequence>r->GetFrameSequence() )
        return 1;
    
    if( timestamp<r->GetTimestamp() )
        return -1;
    else
        return 1;
    
    if( dieSID<r->GetDieSerialID() )
        return -1;
    else if( dieSID>r->GetDieSerialID() )
        return 1;
        
    return 0;
}
