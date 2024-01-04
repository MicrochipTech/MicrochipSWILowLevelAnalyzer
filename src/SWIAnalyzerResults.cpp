#include "SWIAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "SWIAnalyzer.h"
#include "SWIAnalyzerSettings.h"
#include <iostream>
#include <sstream>
#include <fstream>

SWIAnalyzerResults::SWIAnalyzerResults( SWIAnalyzer* analyzer, SWIAnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

SWIAnalyzerResults::~SWIAnalyzerResults()
{
}

void SWIAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	ClearResultStrings();
	Frame frame = GetFrame( frame_index );

	char ack[ 32 ];
            if( frame.mFlags == SWI_FLAG_ACK )
                snprintf( ack, sizeof( ack ), "ACK" );
            else if( frame.mFlags == SWI_MISSING_FLAG_ACK )
                snprintf( ack, sizeof( ack ), "Missing ACK/NAK" );
            else
                snprintf( ack, sizeof( ack ), "NAK" );

	char number_str[128];
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );

	std::stringstream ss;
	if(frame.mType == SWIDischarge){
		ss << "Discovery + " << ack;
	}
	else{
		ss << number_str << " + " << ack;
	}

	AddResultString(  ss.str().c_str() );
}

void SWIAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	std::ofstream file_stream( file, std::ios::out );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],Value" << std::endl;

	U64 num_frames = GetNumFrames();
	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );
		
		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		char number_str[128];
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );

		char ack[ 32 ];
            if( frame.mFlags == SWI_FLAG_ACK )
                snprintf( ack, sizeof( ack ), "ACK" );
            else if( frame.mFlags == SWI_MISSING_FLAG_ACK )
                snprintf( ack, sizeof( ack ), "Missing ACK/NAK" );
            else
                snprintf( ack, sizeof( ack ), "NAK" );

		file_stream << time_str << "," << number_str << "," << ack << std::endl;

		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
}

void SWIAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
#ifdef SUPPORTS_PROTOCOL_SEARCH
	Frame frame = GetFrame( frame_index );
	ClearTabularText();

	char number_str[128];
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
	AddTabularText( number_str );
#endif
}

void SWIAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	//not supported

}

void SWIAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	//not supported
}