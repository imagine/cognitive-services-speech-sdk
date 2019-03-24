//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//
// event_helpers.h: Private implementation declarations for EventSignal related C methods
//

#pragma once
#include <speechapi_c_common.h>


namespace Microsoft {
namespace CognitiveServices {
namespace Speech {
namespace Impl {
    

SPXAPI_PRIVATE recognizer_session_set_event_callback(ISpxRecognizerEvents::SessionEvent_Type ISpxRecognizerEvents::*psessionEvent, SPXRECOHANDLE hreco, PSESSION_CALLBACK_FUNC pCallback, void* pvContext);
SPXAPI_PRIVATE recognizer_recognition_set_event_callback(ISpxRecognizerEvents::RecoEvent_Type ISpxRecognizerEvents::*precoEvent, SPXRECOHANDLE hreco, PRECOGNITION_CALLBACK_FUNC pCallback, void* pvContext);

SPXAPI_PRIVATE connection_set_event_callback(ISpxRecognizerEvents::ConnectionEvent_Type ISpxRecognizerEvents::*connectionEvent, SPXCONNECTIONHANDLE connection, CONNECTION_CALLBACK_FUNC callback, void* context);

SPXAPI_PRIVATE synthesizer_set_event_callback(std::list<std::pair<void*, std::shared_ptr<ISpxSynthesizerEvents::SynthEvent_Type>>> ISpxSynthesizerEvents::*psynthEvents, SPXSYNTHHANDLE hsynth, PSYNTHESIS_CALLBACK_FUNC pCallback, void* pvContext);

} } } } // Microsoft::CognitiveServices::Speech::Impl
