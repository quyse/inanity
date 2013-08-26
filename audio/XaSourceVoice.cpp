#include "XASourceVoice.hpp"

XASourceVoice::XASourceVoice(ptr<System> system) : system(system) {}

void XASourceVoice::SetVoice(ComPointer<IXAudio2SourceVoice> voice)
{
	this->voice = voice;
}

void CALLBACK XASourceVoice::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{}

void CALLBACK XASourceVoice::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{}

void CALLBACK XASourceVoice::OnVoiceProcessingPassEnd()
{}

void CALLBACK XASourceVoice::OnStreamEnd()
{}

void CALLBACK XASourceVoice::OnBufferStart(void* pBufferContext)
{}

void CALLBACK XASourceVoice::OnBufferEnd(void* pBufferContext)
{}

void CALLBACK XASourceVoice::OnLoopEnd(void* pBufferContext)
{}

void CALLBACK XASourceVoice::OnVoiceError(void* pBufferContext, HRESULT Error)
{}

