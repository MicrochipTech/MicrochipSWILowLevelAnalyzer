#include "SWIAnalyzer.h"
#include "SWIAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

SWIAnalyzer::SWIAnalyzer() : Analyzer2(), mSettings( new SWIAnalyzerSettings() ), mSimulationInitilized( false )
{
    SetAnalyzerSettings( mSettings.get() );
}

SWIAnalyzer::~SWIAnalyzer()
{
    KillThread();
}

void SWIAnalyzer::SetupResults()
{
    mResults.reset( new SWIAnalyzerResults( this, mSettings.get() ) );
    SetAnalyzerResults( mResults.get() );
    mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannel );
}

void SWIAnalyzer::AddFrame( U64 SampleBegin, U64 SampleEnd, SWIFrameType FrameType, U64 Data1, U8 Flag )
{
    Frame frm;

    frm.mFlags = Flag;
    frm.mStartingSampleInclusive = SampleBegin;
    frm.mEndingSampleInclusive = SampleEnd - 1;
    frm.mData1 = Data1;
    frm.mType = FrameType;

    mResults->AddFrame( frm );
    mResults->CommitResults();
}

U8 SWIAnalyzer::GetBit( bool HSPD, U64 LowTime )
{
    if( HSPD )
    {
        if( LowTime <= Get_us_Val(mSettings->mTHSDPLowMax0) && LowTime >= Get_us_Val( mSettings->mTHSDPLowMax1 ) )
        {
            // HSPD 0
            return SWI_FLAG_ACK;
        }
        if( LowTime < Get_us_Val( mSettings->mTHSDPLowMax1 ) )
        {
            // HSPD 1
            return !SWI_FLAG_ACK;
        }
    }
    else
    {
        if( LowTime <= Get_us_Val(mSettings->mTSTDPLowMax0 ) && LowTime >= Get_us_Val( mSettings->mTSTDPLowMax1 ) )
        {
            // STPD 0
            return SWI_FLAG_ACK;
        }
        if( LowTime < Get_us_Val( mSettings->mTSTDPLowMax1 ) )
        {
            // STPD 1
            return !SWI_FLAG_ACK;
        }
    }
    return SWI_MISSING_FLAG_ACK;
}

void SWIAnalyzer::WorkerThread()
{
    mSampleRateHz = GetSampleRate();

    mSerial = GetAnalyzerChannelData( mSettings->mInputChannel );
    if( mSerial->GetBitState() == BIT_LOW )
        mSerial->AdvanceToNextEdge();

    for( ;; )
    {
        U64 dataholder = 0xFF;
        U8 flagholder = SWI_MISSING_FLAG_ACK;
        if( mSerial->GetSampleOfNextEdge() - mSerial->GetSampleNumber() >= 150 * GetSampleRate() / 1e6 * 4 ) // Startstop
        {
            mResults->AddMarker( mSerial->GetSampleOfNextEdge(), AnalyzerResults::Start, mSettings->mInputChannel );
        }
        mSerial->AdvanceToNextEdge(); // falling edge -- beginning of the start bit

        U64 starting_sample = mSerial->GetSampleNumber();
        if( ( mSerial->GetSampleOfNextEdge() - starting_sample ) >= Get_us_Val( 150 ) ) // Discharge Check
        {
            // is dicharge
            mResults->CommitResults();
            mSerial->AdvanceToNextEdge(); // rising edge
            U64 Uptime = mSerial->GetSampleOfNextEdge() - mSerial->GetSampleNumber();
            if( Uptime <= Get_us_Val(mSettings->mTRRMax) && Uptime >= Get_us_Val(mSettings->mTRRMin ) )
            {
                mSerial->AdvanceToNextEdge(); // falling edge
                U64 Downtime = mSerial->GetSampleOfNextEdge() - mSerial->GetSampleNumber();
                if( Downtime <= Get_us_Val( mSettings->mTDackMax ) && Downtime >= Get_us_Val( mSettings->mTDackMin ) )
                {
                    flagholder = SWI_FLAG_ACK;
                }
                mSerial->AdvanceToNextEdge(); // rising edge
            }
            AddFrame( starting_sample, mSerial->GetSampleNumber(), SWIDischarge, dataholder, flagholder );
        }
        else
        {
            // assuming is a byte
            dataholder = 0x00;
            bool isHSPD = true;
            for( int i = 0; i < 9; i++ )
            {
                U64 starttime = mSerial->GetSampleNumber();
                U64 lowtime = mSerial->GetSampleOfNextEdge() - mSerial->GetSampleNumber();
                mSerial->AdvanceToNextEdge(); // rising
                U64 hightime;
                U64 endtime;
                if( mSerial->DoMoreTransitionsExistInCurrentData() == true )
                {
                    hightime = mSerial->GetSampleOfNextEdge() - mSerial->GetSampleNumber();
                    endtime = mSerial->GetSampleOfNextEdge();
                }
                else
                {
                    hightime = Get_us_Val( mSettings->mTHTSS );
                    endtime = 0;
                }
                if( hightime >= Get_us_Val( mSettings->mTHTSS ) )
                {
                    // start stop
                    endtime = starttime + 200;
                    mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Stop, mSettings->mInputChannel );
                    if( i == 8 )
                    {
                        flagholder = GetBit( isHSPD, lowtime );
                        AddFrame( starting_sample, endtime, SWIData, dataholder, flagholder );
                        break;
                    }
                    mResults->AddMarker( mSerial->GetSampleOfNextEdge(), AnalyzerResults::Start, mSettings->mInputChannel );
                    if( i < 8 )
                    {
                        i = 9;
                        break;
                    }
                }
                if( lowtime >= Get_us_Val( mSettings->mTHTSS ) )
                {
                    // discharge
                    break;
                }
                if( ( i == 0 ) && ( hightime + lowtime >= Get_us_Val( 25 ) ) )
                {
                    isHSPD = false;
                }

                if( i < 8 )
                {
					if(GetBit(isHSPD, lowtime) == !SWI_FLAG_ACK){
						dataholder += (U64)1 << ( 7 - i );
					}
                    mSerial->AdvanceToNextEdge(); // falling
                }
                if( i == 8 )
                {
                    flagholder = GetBit( isHSPD, lowtime );
                    AddFrame( starting_sample, endtime, SWIData, dataholder, flagholder );
                }
            }
        }

        if( mSerial->GetSampleOfNextEdge() - mSerial->GetSampleNumber() >= mSettings->mTHTSS * GetSampleRate() / 1e6 * 4 ) // Startstop
        {
            mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Stop, mSettings->mInputChannel );
        }
        CheckIfThreadShouldExit();
    }
}

U64 SWIAnalyzer::Get_us_Val( U64 us_value )
{
    return U64( us_value * GetSampleRate() / 1e6 );
}

bool SWIAnalyzer::NeedsRerun()
{
    return false;
}

U32 SWIAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate,
                                         SimulationChannelDescriptor** simulation_channels )
{
    if( mSimulationInitilized == false )
    {
        mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
        mSimulationInitilized = true;
    }

    return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 SWIAnalyzer::GetMinimumSampleRateHz()
{
    return mSettings->mBitRate * 4;
}

const char* SWIAnalyzer::GetAnalyzerName() const
{
    return "Microchip SWI";
}

const char* GetAnalyzerName()
{
    return "Microchip SWI";
}

Analyzer* CreateAnalyzer()
{
    return new SWIAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
    delete analyzer;
}