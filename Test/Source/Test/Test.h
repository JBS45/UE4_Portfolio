// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(Test, Log, All);

#define TESTLOG_CALLINFO (FString(__FUNCTION__)+TEXT("(")+FString::FromInt(__LINE__)+TEXT(")"))
#define TESTLOG_S(Verbosity) UE_LOG(Test,Verbosity,TEXT("%s"),*TESTLOG_CALLINFO)
#define TESTLOG(Verbosity,Format, ...) UE_LOG(Test,Verbosity,TEXT("%s %s"),*TESTLOG_CALLINFO,*FString::Printf(Format,##__VA_ARGS__))
