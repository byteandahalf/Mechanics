#pragma once

enum class BlockEntityType : int 
{
	None,
	Furnace,
	Chest,
	NetherReactor,
	Sign,
	MobSpawner,
	Skull,
	FlowerPot,
	BrewingStand,
	EnchantTable,
	DaylightDetector,
	NoteBlock,

	Barrel = 20,
	Grinder = 21
};

enum BlockEntityRendererId 
{
	DEFAULT,
	SIGN
};
