// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemBox::Init(UTexture2D* ItemIcon, int Amount)
{
	if (ImageIcon)
	{
		ImageIcon->SetBrushFromTexture(ItemIcon);
	}

	if (TextBox)
	{
		TextBox->SetText(FText::FromString(FString::Printf(TEXT("%dx"), Amount)));
	}
}

void UItemBox::SetAmount(int Amount)
{
	TextBox->SetText(FText::FromString(FString::Printf(TEXT("%d"), Amount)));
}
