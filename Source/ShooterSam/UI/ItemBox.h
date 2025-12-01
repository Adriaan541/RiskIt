// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "ItemBox.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SHOOTERSAM_API UItemBox : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UVerticalBox* VerticalBox;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* ImageIcon;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UTextBlock* TextBox;

	FName ItemName;

	void Init(UTexture2D* ItemIcon, int Amount);

	void SetAmount(int Amount);
};
