#pragma once

#include "MovieSceneNewEvent.h"
#include "Channels/MovieSceneChannel.h"
#include "Channels/MovieSceneChannelData.h"
#include "Channels/MovieSceneChannelTraits.h"
#include "MovieSceneClipboard.h"
#include "MovieSceneNewEventChannel.generated.h"

USTRUCT()
struct CUSTOMIZETRACK_API FMovieSceneNewEventChannel : public FMovieSceneChannel
{
	GENERATED_BODY()

	FORCEINLINE TMovieSceneChannelData<FMovieSceneNewEvent> GetData()
	{
		return TMovieSceneChannelData<FMovieSceneNewEvent>(&KeyTimes, &KeyValues, &KeyHandles);
	}

	/**
	 * Constant version of GetData()
	 */
	FORCEINLINE TMovieSceneChannelData<const FMovieSceneNewEvent> GetData() const
	{
		return TMovieSceneChannelData<const FMovieSceneNewEvent>(&KeyTimes, &KeyValues);
	}

	FORCEINLINE TArrayView<const FFrameNumber> GetTimes() const
	{
		return KeyTimes;
	}

	FORCEINLINE TArrayView<const FMovieSceneNewEvent> GetValues() const
	{
		return KeyValues;
	}

	// FMovieSceneChannel interface
	virtual void GetKeys(const TRange<FFrameNumber>&
		WithinRange, TArray<FFrameNumber>* OutKeyTimes, TArray<FKeyHandle>* OutKeyHandles) override;
	virtual void GetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<FFrameNumber> OutKeyTimes) override;
	virtual void SetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<const FFrameNumber> InKeyTimes) override;
	virtual void DuplicateKeys(TArrayView<const FKeyHandle> InHandles, TArrayView<FKeyHandle> OutNewHandles) override;
	virtual void DeleteKeys(TArrayView<const FKeyHandle> InHandles) override;
	virtual void DeleteKeysFrom(FFrameNumber InTime, bool bDeleteKeysBefore) override;
	virtual void ChangeFrameResolution(FFrameRate SourceRate, FFrameRate DestinationRate) override;
	virtual TRange<FFrameNumber> ComputeEffectiveRange() const override;
	virtual int32 GetNumKeys() const override;
	virtual void Reset() override;
	virtual void Offset(FFrameNumber DeltaPosition) override;

private:
	UPROPERTY(meta=(KeyTimes))
	TArray<FFrameNumber> KeyTimes;

	UPROPERTY(meta=(KeyValues))
	TArray<FMovieSceneNewEvent> KeyValues;

	FMovieSceneKeyHandleMap KeyHandles;
};

namespace MovieSceneClipboard
{
	template<> inline FName GetKeyTypeName<FMovieSceneNewEvent>()
	{
		return "MovieSceneNewEvent";
	}
}

template<>
struct TMovieSceneChannelTraits<FMovieSceneNewEventChannel> : TMovieSceneChannelTraitsBase<FMovieSceneNewEventChannel>
{
	enum { SupportsDefaults = false };
};

inline bool EvaluateChannel(const FMovieSceneNewEventChannel* InChannel, FFrameTime InTime, FMovieSceneNewEvent& OutValue)
{
	// Can't evaluate event section data in the typical sense
	return false;
}
