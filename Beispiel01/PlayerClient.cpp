#include "PlayerClient.h"

#include <stdexcept>
#include <iostream>

static std::string const cErrNullptr = "Provided player was nullptr";

PlayerClient::PlayerClient(IMediaPlayer::UPtr player) :
	mPlayer{ std::move(player) }
{
}

void PlayerClient::SetPlayer(IMediaPlayer::UPtr player)
{
	if (player == nullptr) throw std::invalid_argument(cErrNullptr);

	mPlayer = std::move(player);
}

void PlayerClient::Run()
{
	if (mPlayer == nullptr) throw std::invalid_argument(cErrNullptr);

	std::cout << "### Test Stop/Play ###\n";

	mPlayer->Stop();
	mPlayer->Play();

	size_t const volChgAmount = 110;

	std::cout << "Increase volume " << volChgAmount << " times.\n";
	for (size_t i = 0; i < volChgAmount; ++i)
	{
		mPlayer->VolInc();
	}

	std::cout << "Decrease volume " << volChgAmount << " times.\n";
	for (size_t i = 0; i < volChgAmount; ++i)
	{
		mPlayer->VolDec();
	}

	std::cout << "\n### Test Next/Prev ###\n";

	std::cout << "- Wrap around to last:\n";
	mPlayer->Prev();
	mPlayer->Play();

	std::cout << "- Wrap around to first:\n";
	mPlayer->Next();
	mPlayer->Play();

	std::cout << "- Switch through all entries:\n";
	for (size_t i = 0; i < 5; ++i)
	{
		mPlayer->Next();
		mPlayer->Play();
	}

	std::cout << "\n### Test Select ###\n";

	std::cout << "-- Search for \"Does not exist\":\n";
	mPlayer->Select("Does not exist");
	mPlayer->Play();
	mPlayer->Stop();

	std::cout << "\n-- Search for \"Media 1\":\n";
	mPlayer->Select("Media 1");
	mPlayer->Play();
	mPlayer->Stop();

	std::cout << "\n-- Search for \"Media 2\":\n";

	mPlayer->Select("Media 2");
	mPlayer->Play();
	mPlayer->Stop();

	std::cout << "\n-- Search for \"Media 2\" - Should find the same one again:\n";

	mPlayer->Select("Media 2");
	mPlayer->Play();
	mPlayer->Stop();
}