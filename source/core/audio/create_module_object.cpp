//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//
// create_module_object.cpp: Implementation definitions for *CreateModuleObject* methods
//

#include "stdafx.h"

#include "audio_pump.h"
#include "audio_config.h"
#include "pull_audio_input_stream.h"
#include "push_audio_input_stream.h"
#include "push_audio_output_stream.h"
#include "pull_audio_output_stream.h"
#include "interactive_microphone.h"
#include "factory_helpers.h"
#include "wav_file_reader.h"
#include "wav_file_pump.h"
#include "wav_file_writer.h"
#include "default_speaker.h"
#include "null_audio_output.h"
#include "audio_data_stream.h"
#include "microphone_pump.h"


namespace Microsoft {
namespace CognitiveServices {
namespace Speech {
namespace Impl {


SPX_EXTERN_C void* AudioLib_CreateModuleObject(const char* className, const char* interfaceName)
{
    SPX_FACTORY_MAP_BEGIN();
    SPX_FACTORY_MAP_ENTRY(CSpxAudioPump, ISpxAudioPumpInit);
    SPX_FACTORY_MAP_ENTRY(CSpxMicrophonePump, ISpxAudioPump);
    SPX_FACTORY_MAP_ENTRY(CSpxAudioConfig, ISpxAudioConfig);
    SPX_FACTORY_MAP_ENTRY(CSpxPushAudioInputStream, ISpxAudioStreamInitFormat);
    SPX_FACTORY_MAP_ENTRY(CSpxPullAudioInputStream, ISpxAudioStreamInitFormat);
    SPX_FACTORY_MAP_ENTRY(CSpxPushAudioOutputStream, ISpxAudioStreamInitFormat);
    SPX_FACTORY_MAP_ENTRY(CSpxPullAudioOutputStream, ISpxAudioStreamInitFormat);
    SPX_FACTORY_MAP_ENTRY(CSpxPullAudioOutputStream, ISpxAudioOutput);
    SPX_FACTORY_MAP_ENTRY(CSpxInteractiveMicrophone, ISpxAudioPump);
    SPX_FACTORY_MAP_ENTRY(CSpxWavFileReader, ISpxAudioFile);
    SPX_FACTORY_MAP_ENTRY(CSpxWavFilePump, ISpxAudioFile);
    SPX_FACTORY_MAP_ENTRY(CSpxWavFileWriter, ISpxAudioFile);
    SPX_FACTORY_MAP_ENTRY(CSpxDefaultSpeaker, ISpxAudioOutput);
    SPX_FACTORY_MAP_ENTRY(CSpxNullAudioOutput, ISpxAudioOutput);
    SPX_FACTORY_MAP_ENTRY(CSpxAudioDataStream, ISpxAudioDataStream);
    SPX_FACTORY_MAP_END();
}


} } } } // Microsoft::CognitiveServices::Speech::Impl
