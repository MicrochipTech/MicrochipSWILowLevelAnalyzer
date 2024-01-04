#ifndef SWI_SIMULATION_DATA_GENERATOR
#define SWI_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class SWIAnalyzerSettings;

class SWISimulationDataGenerator
{
public:
	SWISimulationDataGenerator();
	~SWISimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, SWIAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	SWIAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //SWI_SIMULATION_DATA_GENERATOR