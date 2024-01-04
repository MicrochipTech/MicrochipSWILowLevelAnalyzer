#ifndef SWI_ANALYZER_H
#define SWI_ANALYZER_H

#include <Analyzer.h>
#include "SWIAnalyzerResults.h"
#include "SWISimulationDataGenerator.h"

enum SWIFrameType
{
    SWIAddress,
    SWIData,
	SWIDischarge
};

class SWIAnalyzerSettings;
class ANALYZER_EXPORT SWIAnalyzer : public Analyzer2
{
public:
	SWIAnalyzer();
	virtual ~SWIAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	U64 SWIAnalyzer::Get_us_Val(U64 us_value);
	void SWIAnalyzer::AddFrame( U64 SampleBegin, U64 SampleEnd, SWIFrameType FrameType, U64 Data1, U8 Flag );
	U8 SWIAnalyzer::GetBit(bool HSPD, U64 LowTime);

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

#pragma warning(disable : 4251 ) // Disables DLL interface warning
protected: //vars
	std::auto_ptr< SWIAnalyzerSettings > mSettings;
	std::auto_ptr< SWIAnalyzerResults > mResults;
	AnalyzerChannelData* mSerial;

	SWISimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //SWI_ANALYZER_H
