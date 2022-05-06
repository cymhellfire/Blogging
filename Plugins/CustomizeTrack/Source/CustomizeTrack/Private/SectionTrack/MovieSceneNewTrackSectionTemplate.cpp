#include "SectionTrack/MovieSceneNewTrackSectionTemplate.h"

struct FNewSectionEvaluationData;

struct FNewSectionExecutionToken : IMovieSceneExecutionToken
{
	virtual void Execute(const FMovieSceneContext& Context, const FMovieSceneEvaluationOperand& Operand, FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) override
	{
		FNewSectionEvaluationData& EvaluationData = PersistentData.GetSectionData<FNewSectionEvaluationData>();
	}
};

struct FNewSectionEvaluationData : IPersistentEvaluationData
{
	
};

FMovieSceneNewTrackSectionTemplate::FMovieSceneNewTrackSectionTemplate(const UMovieSceneNewTrackSection& InSection)
{
	Section = &InSection;
}

void FMovieSceneNewTrackSectionTemplate::Evaluate(const FMovieSceneEvaluationOperand& Operand,
	const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData,
	FMovieSceneExecutionTokens& ExecutionTokens) const
{
	ExecutionTokens.Add(FNewSectionExecutionToken());
}

void FMovieSceneNewTrackSectionTemplate::Setup(FPersistentEvaluationData& PersistentData,
	IMovieScenePlayer& Player) const
{
	PersistentData.AddSectionData<FNewSectionEvaluationData>();
}

void FMovieSceneNewTrackSectionTemplate::TearDown(FPersistentEvaluationData& PersistentData,
	IMovieScenePlayer& Player) const
{
	
}
