#include "KeyTrack/MovieSceneNewEventChannel.h"

void FMovieSceneNewEventChannel::GetKeys(const TRange<FFrameNumber>& WithinRange, TArray<FFrameNumber>* OutKeyTimes,
	TArray<FKeyHandle>* OutKeyHandles)
{
	GetData().GetKeys(WithinRange, OutKeyTimes, OutKeyHandles);
}

void FMovieSceneNewEventChannel::GetKeyTimes(TArrayView<const FKeyHandle> InHandles,
	TArrayView<FFrameNumber> OutKeyTimes)
{
	GetData().GetKeyTimes(InHandles, OutKeyTimes);
}

void FMovieSceneNewEventChannel::SetKeyTimes(TArrayView<const FKeyHandle> InHandles,
	TArrayView<const FFrameNumber> InKeyTimes)
{
	GetData().SetKeyTimes(InHandles, InKeyTimes);
}

void FMovieSceneNewEventChannel::DuplicateKeys(TArrayView<const FKeyHandle> InHandles,
	TArrayView<FKeyHandle> OutNewHandles)
{
	GetData().DuplicateKeys(InHandles, OutNewHandles);
}

void FMovieSceneNewEventChannel::DeleteKeys(TArrayView<const FKeyHandle> InHandles)
{
	GetData().DeleteKeys(InHandles);
}

void FMovieSceneNewEventChannel::DeleteKeysFrom(FFrameNumber InTime, bool bDeleteKeysBefore)
{
	GetData().DeleteKeysFrom(InTime, bDeleteKeysBefore);
}

void FMovieSceneNewEventChannel::ChangeFrameResolution(FFrameRate SourceRate, FFrameRate DestinationRate)
{
	GetData().ChangeFrameResolution(SourceRate, DestinationRate);
}

TRange<FFrameNumber> FMovieSceneNewEventChannel::ComputeEffectiveRange() const
{
	return GetData().GetTotalRange();
}

int32 FMovieSceneNewEventChannel::GetNumKeys() const
{
	return KeyTimes.Num();
}

void FMovieSceneNewEventChannel::Reset()
{
	KeyTimes.Reset();
	KeyValues.Reset();
	KeyHandles.Reset();
}

void FMovieSceneNewEventChannel::Offset(FFrameNumber DeltaPosition)
{
	GetData().Offset(DeltaPosition);
}
