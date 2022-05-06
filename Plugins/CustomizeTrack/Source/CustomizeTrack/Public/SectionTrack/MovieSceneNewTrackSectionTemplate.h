#pragma once

#include "CoreMinimal.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "MovieSceneNewTrackSectionTemplate.generated.h"

class UMovieSceneNewTrackSection;

USTRUCT()
struct FMovieSceneNewTrackSectionTemplate : public FMovieSceneEvalTemplate
{
	GENERATED_BODY()

	FMovieSceneNewTrackSectionTemplate()
		: Section(nullptr)
	{}
	FMovieSceneNewTrackSectionTemplate(const UMovieSceneNewTrackSection& InSection);

	virtual void SetupOverrides() override
	{
		EnableOverrides(RequiresSetupFlag | RequiresTearDownFlag);
	}

	virtual void Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context,
		const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override;
protected:
	virtual void Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	virtual void TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;

	virtual UScriptStruct& GetScriptStructImpl() const override { return *StaticStruct(); }

	UPROPERTY()
	const UMovieSceneNewTrackSection* Section;
};
