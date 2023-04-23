#include "MinipairDescriptorFactory.h"

#include "SpeakerWaveProperties.h"
#include "SpeakerTopologyProperties.h"


#include "MiniportTopology.h"
#include "MiniportWaveRT.h"

MinipairDescriptorFactory::MinipairDescriptorFactory()
{
}
MinipairDescriptorFactory::~MinipairDescriptorFactory()
{
}

void MinipairDescriptorFactory::SetLastCharacterOfString(PWSTR string, int valueToSet)
{
	UNREFERENCED_PARAMETER(string);
	UNREFERENCED_PARAMETER(valueToSet);
	//TODO: implement
	//string[wcslen(string) - 1] = ('0' + (valueToSet % 36));
}

int MinipairDescriptorFactory::m_CurrentIndex = 0;

ENDPOINT_MINIPAIR MinipairDescriptorFactory::m_SpeakerTemplate =
{
	DeviceType::RenderDevice,
	L"TopologyRender-0",            // make sure this or the template name matches with KSNAME_TopologySpeakerHeadphone in the inf's [Strings] section 
	L"TopologyRenderTemplate",                                   // optional template name
	MiniportTopology::Create,
	&SpeakerTopoMiniportFilterDescriptor,
	0, NULL,                                // Interface properties
	L"WaveRender-0",                // make sure this or the template name matches with KSNAME_WaveSpeakerHeadphone in the inf's [Strings] section
	L"WaveRenderTemplate",                                   // optional template name
	MiniportWaveRT::Create,
	&SpeakerWaveMiniportFilterDescriptor,
	0, NULL,                                // Interface properties
	SPEAKER_DEVICE_MAX_CHANNELS,
	SpeakerPinDeviceFormatsAndModes,
	SIZEOF_ARRAY(SpeakerPinDeviceFormatsAndModes),
	SpeakerTopologyPhysicalConnections,
	SIZEOF_ARRAY(SpeakerTopologyPhysicalConnections),
	ENDPOINT_FLAG_NONE,
	NULL, 0, NULL,
};

NTSTATUS MinipairDescriptorFactory::CreateSpeaker(_Outptr_ ENDPOINT_MINIPAIR** pMinipair)
{
	ENDPOINT_MINIPAIR* pNewMinipair = new(NonPagedPoolNx, MINIADAPTER_POOLTAG) ENDPOINT_MINIPAIR;
	if (!pNewMinipair)
	{
		*pMinipair = NULL;
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	*pNewMinipair = m_SpeakerTemplate;

	MinipairDescriptorFactory::SetLastCharacterOfString(pNewMinipair->TopoName, m_CurrentIndex);
	MinipairDescriptorFactory::SetLastCharacterOfString(pNewMinipair->WaveName, m_CurrentIndex);
	//TODO: check if correct

	*pMinipair = pNewMinipair;
	m_CurrentIndex = m_CurrentIndex + 1;

	return STATUS_SUCCESS;
	/*
	ENDPOINT_MINIPAIR* pNewMinipair = (ENDPOINT_MINIPAIR*)ExAllocatePoolWithTag(NonPagedPoolNx, sizeof(ENDPOINT_MINIPAIR), MINIADAPTER_POOLTAG);
	RtlCopyMemory(pNewMinipair, &m_MicrophoneTemplate, sizeof(ENDPOINT_MINIPAIR));
	*/
}
