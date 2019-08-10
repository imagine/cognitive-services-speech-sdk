#!/usr/bin/env bash
T="$(basename "$0" .sh)"
BUILD_DIR="$1"
PLATFORM="$2"
BINARY_DIR="$3"
TESTSET="${4:-dev}"

SCRIPT_DIR="$(dirname "${BASH_SOURCE[0]}")"

. "$SCRIPT_DIR/../functions.sh" || exit 1

isOneOf "$PLATFORM" {{Windows,Linux}-{x86,x64},Linux-{arm32,arm64},OSX-x64}-{Debug,Release} ||
  exitWithSuccess "Test %s: skip on this platform\n" "$T"

TEST_CODE="$BINARY_DIR/cxx_api_tests"
[[ -x $TEST_CODE ]] ||
  exitWithSuccess "Test %s: skip, no test code\n" "$T"

. "$SCRIPT_DIR/../test-harness.sh" || exit 1

PATTERN=
case $TESTSET in
  prod|int)
    # All default (non-hidden) + the specified tag
    PATTERN="~[.],[$TESTSET]"
    ;;
esac

RUN_OFFLINE_UNIDEC_TESTS=false
case $PLATFORM in
  Windows-x64*)
    UNIDEC_RUNTIME_PATH="$SCRIPT_DIR/../external/unidec/Richland.Speech.UnidecRuntime/native"
    if [[ -d "$UNIDEC_RUNTIME_PATH" ]]; then
      PATH="${UNIDEC_RUNTIME_PATH}:$PATH"
      RUN_OFFLINE_UNIDEC_TESTS=true
    fi
    ;;
  Linux-x64*)
    UNIDEC_RUNTIME_PATH="$SCRIPT_DIR/../external/unidec/Richland.Speech.UnidecRuntime.linux/native"
    if [[ -d "$UNIDEC_RUNTIME_PATH" ]]; then
      LD_LIBRARY_PATH="${UNIDEC_RUNTIME_PATH}:$LD_LIBRARY_PATH"
      RUN_OFFLINE_UNIDEC_TESTS=true
    fi
    ;;
esac

if [[ $RUN_OFFLINE_UNIDEC_TESTS = true ]]; then
  OFFLINE_MODEL_PATH_ROOT="external/unidec/Unidec.Model/model"
  OFFLINE_MODEL_LANGUAGE="en-US"
else
  if [[ $PATTERN ]]; then
    PATTERN="~[unidec]$PATTERN"
  else
    PATTERN="~[unidec]~[.]"
  fi
  OFFLINE_MODEL_PATH_ROOT=""
  OFFLINE_MODEL_LANGUAGE=""
fi

runCatchSuite \
  TESTRUNNER \
  "test-$T-$PLATFORM-$RANDOM" \
  "$PLATFORM" \
  "$SPEECHSDK_SPEECH_KEY $SPEECHSDK_LUIS_KEY $SPEECHSDK_BOT_SUBSCRIPTION $SPEECHSDK_PRINCETON_CONVERSATIONTRANSCRIBER_PPE_KEY $SPEECHSDK_PRINCETON_CONVERSATIONTRANSCRIBER_PROD_KEY" \
  "$T" \
  240 \
  "$PATTERN" \
  "$TEST_CODE" \
    --keySpeech "$SPEECHSDK_SPEECH_KEY" \
    --region "$SPEECHSDK_SPEECH_REGION" \
    --keyLUIS "$SPEECHSDK_LUIS_KEY" \
    --regionIdLUIS "$SPEECHSDK_LUIS_REGION" \
    --luisAppId "$SPEECHSDK_LUIS_HOMEAUTOMATION_APPID" \
    --InroomEndpoint "$SPEECHSDK_PRINCETON_INROOM_ENDPOINT" \
    --OnlineEndpoint "$SPEECHSDK_PRINCETON_ONLINE_ENDPOINT" \
    --inputDir "$SPEECHSDK_INPUTDIR" \
    --keyDialog "$SPEECHSDK_BOT_SUBSCRIPTION" \
    --dialogRegionId "$SPEECHSDK_BOT_REGION" \
    --dialogBotSecret "$SPEECHSDK_BOT_FUNCTIONALTESTBOT" \
    --keyConversationTranscriberPPE "$SPEECHSDK_PRINCETON_CONVERSATIONTRANSCRIBER_PPE_KEY" \
    --offlineModelPathRoot="$OFFLINE_MODEL_PATH_ROOT" \
    --offlineModelLanguage="$OFFLINE_MODEL_LANGUAGE" \
