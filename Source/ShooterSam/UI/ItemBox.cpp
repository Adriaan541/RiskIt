// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// void UItemBox::SetItemAndAmount(UTexture2D* ItemIcon, int Amount)
// {
// 	UE_LOG(LogTemp, Display, TEXT("Item Amount = %d"), Amount);
//
// 	if (!ImageIcon)
// 	{
// 		UE_LOG(LogTemp, Display, TEXT("No image icon box"));
// 		return;
// 	}
//
// 	
// 	if (ItemIcon)
// 	{
// 		ImageIcon->SetBrushFromTexture(ItemIcon);
// 		UE_LOG(LogTemp, Display, TEXT("Brush set"));
// 		return;
// 	}
// 	
// 	UE_LOG(LogTemp, Display, TEXT("Brush invalid"));
// 	// TextBox->SetText(FText::FromString(FString::Printf(TEXT("%dx"), Amount)));
// 	UE_LOG(LogTemp, Display, TEXT("Text set"));
// }


void UItemBox::Init(UTexture2D* ItemIcon, int Amount)
{
	if (ImageIcon)
	{
		ImageIcon->SetBrushFromTexture(ItemIcon);
		UE_LOG(LogTemp, Display, TEXT("Brush set"));
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
