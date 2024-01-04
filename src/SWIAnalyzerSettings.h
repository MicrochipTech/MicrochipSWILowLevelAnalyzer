#ifndef SWI_ANALYZER_SETTINGS
#define SWI_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class SWIAnalyzerSettings : public AnalyzerSettings
{
public:
	SWIAnalyzerSettings();
	virtual ~SWIAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mInputChannel;
	int mTBit;
	int mTHSDPLowMax1;
	int mTHSDPLowMax0;
	int mTSTDPLowMax1;
    int mTSTDPLowMax0;
    int mTHTSS;
    int mTDackMin;
    int mTDackMax;
    int mTRRMin;
    int mTRRMax;
	U32 mBitRate;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mTBitInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mTHSDPLowMax1Interface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mTHSDPLowMax0Interface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mTSTDPLowMax1Interface;
    std::auto_ptr< AnalyzerSettingInterfaceInteger >	mTSTDPLowMax0Interface;
    std::auto_ptr< AnalyzerSettingInterfaceInteger >	mTHTSSInterface;
    std::auto_ptr< AnalyzerSettingInterfaceInteger >	mTDackMinInterface;
    std::auto_ptr< AnalyzerSettingInterfaceInteger >	mTDackMaxInterface;
    std::auto_ptr< AnalyzerSettingInterfaceInteger >	mTRRMinInterface;
    std::auto_ptr< AnalyzerSettingInterfaceInteger >	mTRRMaxInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mBitRateInterface;
};

#endif //SWI_ANALYZER_SETTINGS
