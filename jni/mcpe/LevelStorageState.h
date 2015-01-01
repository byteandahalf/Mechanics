#pragma once

enum class LevelStorageState : int
{
	Open,
	Corrupted,
	NotFound,
	IOError,
	NotSupported,
	InvalidArguments,
	Unknown
};
