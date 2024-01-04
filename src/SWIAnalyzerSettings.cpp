#include "SWIAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


SWIAnalyzerSettings::SWIAnalyzerSettings()
    : mInputChannel( UNDEFINED_CHANNEL ),
      mTBit( 25 ),
      mTHSDPLowMax1( 3 ),
      mTHSDPLowMax0( 16 ),
      mTSTDPLowMax1( 13 ),
      mTSTDPLowMax0( 64 ),
      mTHTSS( 150 ),
      mTDackMin( 8 ),
      mTDackMax( 24 ),
      mTRRMin( 7 ),
      mTRRMax( 100 ),
      mBitRate( 9600 )
{
    mInputChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
    mInputChannelInterface->SetTitleAndTooltip( "SIO", "Single Wire IO" );
    mInputChannelInterface->SetChannel( mInputChannel );


    mTBitInterface.reset( new AnalyzerSettingInterfaceInteger() );
    mTBitInterface->SetTitleAndTooltip( "Max T-Bit, HSPD", "Specify the max bit frame for HSPD." );
    mTBitInterface->SetMax( 100000000 );
    mTBitInterface->SetMin( 8 );
    mTBitInterface->SetInteger( mTBit );

    mTHSDPLowMax1Interface.reset( new AnalyzerSettingInterfaceInteger() );
    mTHSDPLowMax1Interface->SetTitleAndTooltip( "Max T-Low1, HSPD", "Specify the max bit frame for HSPD." );
    mTHSDPLowMax1Interface->SetMax( 100000000 );
    mTHSDPLowMax1Interface->SetMin( 1 );
    mTHSDPLowMax1Interface->SetInteger( mTHSDPLowMax1 );

    mTHSDPLowMax0Interface.reset( new AnalyzerSettingInterfaceInteger() );
    mTHSDPLowMax0Interface->SetTitleAndTooltip( "Max T-Low0, HSPD", "Specify the max bit frame for HSPD." );
    mTHSDPLowMax0Interface->SetMax( 100000000 );
    mTHSDPLowMax0Interface->SetMin( 1 );
    mTHSDPLowMax0Interface->SetInteger( mTHSDPLowMax0 );

    mTSTDPLowMax1Interface.reset( new AnalyzerSettingInterfaceInteger() );
    mTSTDPLowMax1Interface->SetTitleAndTooltip( "Max T-Low1, STPD", "Specify the max bit frame for HSPD." );
    mTSTDPLowMax1Interface->SetMax( 100000000 );
    mTSTDPLowMax1Interface->SetMin( 1 );
    mTSTDPLowMax1Interface->SetInteger( mTSTDPLowMax1 );

    mTSTDPLowMax0Interface.reset( new AnalyzerSettingInterfaceInteger() );
    mTSTDPLowMax0Interface->SetTitleAndTooltip( "Max T-Low0, STPD", "Specify the max bit frame for HSPD." );
    mTSTDPLowMax0Interface->SetMax( 100000000 );
    mTSTDPLowMax0Interface->SetMin( 1 );
    mTSTDPLowMax0Interface->SetInteger( mTSTDPLowMax0 );

    mTHTSSInterface.reset( new AnalyzerSettingInterfaceInteger() );
    mTHTSSInterface->SetTitleAndTooltip( "T-HTSS/DSCHG", "Specify the max bit frame for HSPD." );
    mTHTSSInterface->SetMax( 100000000 );
    mTHTSSInterface->SetMin( 1 );
    mTHTSSInterface->SetInteger( mTHTSS );

    mTDackMinInterface.reset( new AnalyzerSettingInterfaceInteger() );
    mTDackMinInterface->SetTitleAndTooltip( "T-Dack Min", "Specify the max bit frame for HSPD." );
    mTDackMinInterface->SetMax( 100000000 );
    mTDackMinInterface->SetMin( 1 );
    mTDackMinInterface->SetInteger( mTDackMin );

    mTDackMaxInterface.reset( new AnalyzerSettingInterfaceInteger() );
    mTDackMaxInterface->SetTitleAndTooltip( "T-Dack Max", "Specify the max bit frame for HSPD." );
    mTDackMaxInterface->SetMax( 100000000 );
    mTDackMaxInterface->SetMin( 1 );
    mTDackMaxInterface->SetInteger( mTDackMax );

    mTRRMinInterface.reset( new AnalyzerSettingInterfaceInteger() );
    mTRRMinInterface->SetTitleAndTooltip( "T-Reset Recover Min", "Specify the max bit frame for HSPD." );
    mTRRMinInterface->SetMax( 100000000 );
    mTRRMinInterface->SetMin( 1 );
    mTRRMinInterface->SetInteger( mTRRMin );

    mTRRMaxInterface.reset( new AnalyzerSettingInterfaceInteger() );
    mTRRMaxInterface->SetTitleAndTooltip( "T-Reset Recover Max", "Specify the max bit frame for HSPD." );
    mTRRMaxInterface->SetMax( 100000000 );
    mTRRMaxInterface->SetMin( 1 );
    mTRRMaxInterface->SetInteger( mTRRMax );


    AddInterface( mInputChannelInterface.get() );
    AddInterface( mTBitInterface.get() );
    AddInterface( mTHSDPLowMax1Interface.get() );
    AddInterface( mTHSDPLowMax0Interface.get() );
    AddInterface( mTSTDPLowMax1Interface.get() );
    AddInterface( mTSTDPLowMax0Interface.get() );
    AddInterface( mTHTSSInterface.get() );
    AddInterface( mTDackMinInterface.get() );
    AddInterface( mTDackMaxInterface.get() );
    AddInterface( mTRRMinInterface.get() );
    AddInterface( mTRRMaxInterface.get() );

    AddExportOption( 0, "Export as text/csv file" );
    AddExportExtension( 0, "text", "txt" );
    AddExportExtension( 0, "csv", "csv" );

    ClearChannels();
    AddChannel( mInputChannel, "Serial", false );
}

SWIAnalyzerSettings::~SWIAnalyzerSettings()
{
}

bool SWIAnalyzerSettings::SetSettingsFromInterfaces()
{
    mInputChannel = mInputChannelInterface->GetChannel();
    mTBit = mTBitInterface->GetInteger();
    mTHSDPLowMax1 = mTHSDPLowMax1Interface->GetInteger();
    mTHSDPLowMax0 = mTHSDPLowMax0Interface->GetInteger();

    mTSTDPLowMax1 = mTSTDPLowMax1Interface->GetInteger();
    mTSTDPLowMax0 = mTSTDPLowMax0Interface->GetInteger();
    mTHTSS = mTHTSSInterface->GetInteger();
    mTDackMin = mTDackMinInterface->GetInteger();
    mTDackMax = mTDackMaxInterface->GetInteger();
    mTRRMin = mTRRMinInterface->GetInteger();
    mTRRMax = mTRRMaxInterface->GetInteger();

    ClearChannels();
    AddChannel( mInputChannel, "Microchip SWI", true );

    return true;
}

void SWIAnalyzerSettings::UpdateInterfacesFromSettings()
{
    mInputChannelInterface->SetChannel( mInputChannel );
    mTBitInterface->SetInteger( mTBit );
    mTHSDPLowMax1Interface->SetInteger( mTHSDPLowMax1 );
    mTHSDPLowMax0Interface->SetInteger( mTHSDPLowMax0 );
    mTSTDPLowMax1Interface->SetInteger( mTSTDPLowMax1 );
    mTSTDPLowMax0Interface->SetInteger( mTSTDPLowMax0 );
    mTHTSSInterface->SetInteger( mTHTSS );
    mTDackMinInterface->SetInteger( mTDackMin );
    mTDackMaxInterface->SetInteger( mTDackMax );
    mTRRMinInterface->SetInteger( mTRRMin );
    mTRRMaxInterface->SetInteger( mTRRMax );
}

void SWIAnalyzerSettings::LoadSettings( const char* settings )
{
    SimpleArchive text_archive;
    text_archive.SetString( settings );

    text_archive >> mInputChannel;
    text_archive >> mTBit;
    text_archive >> mTHSDPLowMax1;
    text_archive >> mTHSDPLowMax0;
    text_archive >> mTSTDPLowMax1;
    text_archive >> mTSTDPLowMax0;
    text_archive >> mTHTSS;
    text_archive >> mTDackMin;
    text_archive >> mTDackMax;
    text_archive >> mTRRMin;
    text_archive >> mTRRMax;

    ClearChannels();
    AddChannel( mInputChannel, "Microchip SWI", true );

    UpdateInterfacesFromSettings();
}

const char* SWIAnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;

    text_archive << mInputChannel;
    text_archive << mTBit;
    text_archive << mTHSDPLowMax1;
    text_archive << mTHSDPLowMax0;
    text_archive << mTSTDPLowMax1;
    text_archive << mTSTDPLowMax0;
    text_archive << mTHTSS;
    text_archive << mTDackMin;
    text_archive << mTDackMax;
    text_archive << mTRRMin;
    text_archive << mTRRMax;

    return SetReturnString( text_archive.GetString() );
}
