// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ItemTradeData.h"

TObjectPtr<UDataTable> UItemTradeConfig::GetRandomTable() const
{
	float Total = 0.0f;
	for (const FTradeTable& Table : TradeTables)
		Total += Table.Weight;

	float Sum = 0.0f;
	const float RAND = FMath::RandRange(0.0f, Total);

	for (const FTradeTable& Table : TradeTables)
	{
		Sum += Table.Weight;
		if (Sum >= RAND)
			return Table.Table;
	}

	return (*TradeTables.begin()).Table;
}
