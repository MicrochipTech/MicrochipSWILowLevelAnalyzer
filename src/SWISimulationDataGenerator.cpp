#include "SWISimulationDataGenerator.h"
#include "SWIAnalyzerSettings.h"
#include <AnalyzerHelpers.h>

SWISimulationDataGenerator::SWISimulationDataGenerator() : mSerialText( "SWI Simulation Data" ), mStringIndex( 0 )
{
}

SWISimulationDataGenerator::~SWISimulationDataGenerator()
{
}

void SWISimulationDataGenerator::Initialize( U32 simulation_sample_rate, SWIAnalyzerSettings* settings )
{
    mSimulationSampleRateHz = simulation_sample_rate;
    mSettings = settings;

    mSerialSimulationData.SetChannel( mSettings->mInputChannel );
    mSerialSimulationData.SetSampleRate( simulation_sample_rate );
    mSerialSimulationData.SetInitialBitState( BIT_HIGH );
}

U32 SWISimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate,
                                                        SimulationChannelDescriptor** simulation_channel )
{
    U64 adjusted_largest_sample_requested =
        AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

    while( mSerialSimulationData.GetCurrentSampleNumber() < adjusted_largest_sample_requested )
    {
        CreateSerialByte();
    }

    *simulation_channel = &mSerialSimulationData;
    return 1;
}

void SWISimulationDataGenerator::CreateSerialByte()
{
    U32 samples_per_bit = mSimulationSampleRateHz / mSettings->mBitRate;

    U8 byte = mSerialText[ mStringIndex ];
    mStringIndex++;
    if( mStringIndex == mSerialText.size() )
        mStringIndex = 0;

    // we're currenty high
    // let's move forward a little
    mSerialSimulationData.Advance( samples_per_bit / 10 );

    U8 mask = 0x1 << 7;
    for( U32 i = 0; i < 9; i++ )
    {
        if( ( byte & mask ) != 0 )
        {
            mSerialSimulationData.TransitionIfNeeded( BIT_LOW );
            mSerialSimulationData.Advance( 1 * samples_per_bit / 100 );
            mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
            mSerialSimulationData.Advance( 19 * samples_per_bit / 100 );
        }
        else
        {
            mSerialSimulationData.TransitionIfNeeded( BIT_LOW );
            mSerialSimulationData.Advance( 10 * samples_per_bit / 100 );
            mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
            mSerialSimulationData.Advance( 10 * samples_per_bit / 100 );
        }
        mask = mask >> 1;
    }
    // lets pad the end a bit for the stop bit:
    mSerialSimulationData.Advance( samples_per_bit );
}
