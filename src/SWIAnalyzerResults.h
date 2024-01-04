#ifndef SWI_ANALYZER_RESULTS
#define SWI_ANALYZER_RESULTS

#include <AnalyzerResults.h>

#define SWI_FLAG_ACK ( 1 << 0 )
#define SWI_MISSING_FLAG_ACK ( 1 << 1 )

class SWIAnalyzer;
class SWIAnalyzerSettings;

class SWIAnalyzerResults : public AnalyzerResults
{
public:
	SWIAnalyzerResults( SWIAnalyzer* analyzer, SWIAnalyzerSettings* settings );
	virtual ~SWIAnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions

protected:  //vars
	SWIAnalyzerSettings* mSettings;
	SWIAnalyzer* mAnalyzer;
};

#endif //SWI_ANALYZER_RESULTS
