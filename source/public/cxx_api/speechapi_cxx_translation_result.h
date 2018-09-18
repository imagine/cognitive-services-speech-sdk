//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//
// speechapi_cxx_translation_result.h: Public API declarations for TranslationResult C++ class
//

#pragma once
#include <string>
#include <vector>
#include <map>
#include <new>
#include <speechapi_cxx_common.h>
#include <speechapi_cxx_enums.h>
#include <speechapi_c.h>
#include <speechapi_cxx_speech_recognition_result.h>

namespace Microsoft {
namespace CognitiveServices {
namespace Speech {
namespace Translation {

/// <summary>
/// Defines the translation text result.
/// </summary>

class TranslationTextResult final : public RecognitionResult
{
private:

    std::map<std::string, std::string> m_translations;

public:
    /// <summary>
    /// It is intended for internal use only. It creates an instance of <see cref="TranslationTextResult"/>.
    /// </summary>
    /// <param name="resultHandle">The handle of the result returned by recognizer in C-API.</param>
    explicit TranslationTextResult(SPXRESULTHANDLE resultHandle) :
        RecognitionResult(resultHandle),
        Translations(m_translations)
    {
        PopulateResultFields(resultHandle);
        SPX_DBG_TRACE_VERBOSE("%s (this=0x%x, handle=0x%x) -- resultid=%s.", __FUNCTION__, this, Handle, ResultId.c_str());
    };

    /// <summary>
    /// Destructs the instance.
    /// </summary>
    virtual ~TranslationTextResult()
    {
        SPX_DBG_TRACE_VERBOSE("%s (this-0x%x, handle=0x%x)", __FUNCTION__, this, Handle);
    }

    /// <summary>
    /// Presents the translation results. Each item in the map is a key value pair, where key is the language tag of the translated text,
    /// and value is the translation text in that language.
    /// </summary>
    const std::map<std::string, std::string>& Translations;

private:
    void PopulateResultFields(SPXRESULTHANDLE resultHandle)
    {
        SPX_INIT_HR(hr);

        size_t bufLen = 0;
        std::shared_ptr<Result_TranslationTextBufferHeader> phraseBuffer;
        // retrieve the required buffer size first.
        hr = translation_text_result_get_translation_text_buffer_header(resultHandle, nullptr, &bufLen);
        if (hr == SPXERR_BUFFER_TOO_SMALL)
        {
            char *ptr = new char[bufLen];
            phraseBuffer = std::shared_ptr<Result_TranslationTextBufferHeader>((Result_TranslationTextBufferHeader*)ptr,
                [](void *to_delete) { delete[] ((char*)to_delete); });
            hr = translation_text_result_get_translation_text_buffer_header(resultHandle, phraseBuffer.get(), &bufLen);
        }
        SPX_THROW_ON_FAIL(hr);

        if (phraseBuffer->bufferSize > bufLen)
        {
            SPX_THROW_HR(SPXERR_RUNTIME_ERROR);
        }

        for (size_t i = 0; i < phraseBuffer->numberEntries; i++)
        {
            m_translations[std::string(phraseBuffer->targetLanguages[i])] = std::string(phraseBuffer->translationTexts[i]);
        }

        SPX_DBG_TRACE_VERBOSE("Translation phrases: numberentries: %d", (int)m_translations.size());
#ifdef _DEBUG
        for (const auto& cf : m_translations)
        {
            (void)(cf); // prevent warning for cf when compiling release builds
            SPX_DBG_TRACE_VERBOSE(" phrase for %s: %s", cf.first.c_str(), cf.second.c_str());
        }
#endif
    };

    DISABLE_DEFAULT_CTORS(TranslationTextResult);
};


/// <summary>
/// Defines the translation synthesis result, i.e. the voice output of the translated text in the target language.
/// </summary>
class TranslationSynthesisResult
{
private:

    ResultReason m_reason;
    std::vector<uint8_t> m_audioData;

public:
    /// <summary>
    /// It is intended for internal use only. It creates an instance of <see cref="TranslationSynthesisResult"/>
    /// </summary>
    /// <param name="resultHandle">The handle of the result returned by recognizer in C-API.</param>
    explicit TranslationSynthesisResult(SPXRESULTHANDLE resultHandle) :
        Reason(m_reason),
        Audio(m_audioData)
    {
        PopulateResultFields(resultHandle);
        SPX_DBG_TRACE_VERBOSE("%s (this=0x%x, handle=0x%x) reason=0x%x", __FUNCTION__, this, resultHandle, Reason);
    };

    /// <summary>
    /// Destructs the instance.
    /// </summary>
    virtual ~TranslationSynthesisResult()
    {
        SPX_DBG_TRACE_VERBOSE("%s (this-0x%x)", __FUNCTION__, this);
    };

    /// <summary>
    /// Recognition reason.
    /// </summary>
    const ResultReason& Reason;

    /// <summary>
    /// The voice output of the translated text in the target language.
    /// </summary>
    const std::vector<uint8_t>& Audio;


private:

    DISABLE_DEFAULT_CTORS(TranslationSynthesisResult);

    void PopulateResultFields(SPXRESULTHANDLE resultHandle)
    {
        SPX_INIT_HR(hr);

        Result_Reason resultReason;
        SPX_THROW_ON_FAIL(hr = result_get_reason(resultHandle, &resultReason));
        m_reason = (ResultReason)resultReason;

        size_t bufLen = 0;
        hr = translation_synthesis_result_get_audio_data(resultHandle, nullptr, &bufLen);
        if (hr == SPXERR_BUFFER_TOO_SMALL)
        {
            m_audioData.resize(bufLen);
            hr = translation_synthesis_result_get_audio_data(resultHandle, m_audioData.data(), &bufLen);
        }
        SPX_THROW_ON_FAIL(hr);

        SPX_DBG_TRACE_VERBOSE("Translation synthesis: audio length: %d, verctor size:", bufLen, m_audioData.size());
    };
};


} } } } // Microsoft::CognitiveServices::Speech::Translation
