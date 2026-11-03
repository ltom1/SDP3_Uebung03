#include "gtest/gtest.h"

#include "Video.h"
#include "VideoPlayer.h"
#include "VideoPlayerAdapter.h"
#include "Song.h"
#include "MusicPlayer.h"
#include "MusicPlayerAdapter.h"
#include "PlayerClient.h"

#include <iostream>
#include <sstream>
#include <vld.h>

using namespace std;
using namespace testing::internal;

/////////////////////////////////////////////////

TEST(VideoFormat, Test) {
	EXPECT_EQ(to_string(MKV), "MKV-Format");
	EXPECT_EQ(to_string(AVI), "AVI-Format");
	EXPECT_EQ(to_string(WMV), "WMV-Format");
}

/////////////////////////////////////////////////

TEST(Video, Test) {
	Video vid1{ "Me at the zoo", 4, AVI };

	EXPECT_EQ(vid1.GetName(), "Me at the zoo");
	EXPECT_EQ(vid1.GetDuration(), 4);
	EXPECT_EQ(vid1.GetFormat(), AVI);

	EXPECT_EQ(vid1.ToString(), "Me at the zoo [duration -> 4 min], AVI-Format");

	vid1.SetName("My new video");
	EXPECT_EQ(vid1.GetName(), "My new video");

	Video const vid2{ vid1 };
	EXPECT_EQ(vid2.GetName(), vid1.GetName());
	EXPECT_EQ(vid2.GetDuration(), vid1.GetDuration());
	EXPECT_EQ(vid2.GetFormat(), vid1.GetFormat());
	EXPECT_EQ(vid2.ToString(), vid1.ToString());

	Video const vid3{ "YouTube video", 20, MKV };
	vid1 = vid3;
	EXPECT_EQ(vid3.GetName(), vid1.GetName());
	EXPECT_EQ(vid3.GetDuration(), vid1.GetDuration());
	EXPECT_EQ(vid3.GetFormat(), vid1.GetFormat());
	EXPECT_EQ(vid3.ToString(), vid1.ToString());
}

/////////////////////////////////////////////////

TEST(VideoPlayer, Empty) {
	VideoPlayer player;

	std::string output = "";
	
	CaptureStdout();
	player.Play();
	output = GetCapturedStdout();
	cout << output;

	EXPECT_EQ(output, "no video in playlist!\n");

	CaptureStdout();
	player.Stop();
	output = GetCapturedStdout();
	cout << output;
	
	EXPECT_EQ(output, "no video in playlist!\n");

	EXPECT_EQ(player.First(), false);
	EXPECT_EQ(player.Next(), false);
	EXPECT_EQ(player.ActIndex(), 0);
	EXPECT_EQ(player.ActVideo(), "");

	EXPECT_EQ(player.GetVolume(), VideoPlayer::DEFAULT_VOLUME);
	player.SetVolume(0);
	EXPECT_EQ(player.GetVolume(), 0);
	player.SetVolume(50);
	EXPECT_EQ(player.GetVolume(), 50);
	player.SetVolume(VideoPlayer::MAX_VOLUME);
	EXPECT_EQ(player.GetVolume(), VideoPlayer::MAX_VOLUME);
	player.SetVolume(VideoPlayer::MAX_VOLUME + 1);
	EXPECT_EQ(player.GetVolume(), VideoPlayer::MAX_VOLUME);
}

TEST(VideoPlayer, OneVideo) {
	VideoPlayer player;
	std::string output = "";

	player.Add("Test", 123, VideoFormat::AVI);
	EXPECT_EQ(player.ActIndex(), 0);
	EXPECT_EQ(player.ActVideo(), "Test");

	CaptureStdout();
	player.Stop();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "stop video: Test [duration -> 123 min], AVI-Format\n");

	CaptureStdout();
	player.Play();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "playing video number 1: Test [duration -> 123 min], AVI-Format\n");

	player.Next();
	EXPECT_EQ(player.ActIndex(), 0);
	EXPECT_EQ(player.ActVideo(), "Test");

	CaptureStdout();
	player.Play();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "playing video number 1: Test [duration -> 123 min], AVI-Format\n");

	CaptureStdout();
	player.Stop();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "stop video: Test [duration -> 123 min], AVI-Format\n");

	player.First();
	EXPECT_EQ(player.ActIndex(), 0);
	EXPECT_EQ(player.ActVideo(), "Test");

	CaptureStdout();
	player.Play();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "playing video number 1: Test [duration -> 123 min], AVI-Format\n");

	CaptureStdout();
	player.Stop();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "stop video: Test [duration -> 123 min], AVI-Format\n");
}

TEST(VideoPlayer, MultipleVideos) {
	VideoPlayer player;
	std::string output = "";

	player.Add("Video 1", 1, VideoFormat::AVI);
	player.Add("Video 2", 2, VideoFormat::MKV);
	player.Add("Video 3", 3, VideoFormat::WMV);
	EXPECT_EQ(player.ActIndex(), 0);
	EXPECT_EQ(player.ActVideo(), "Video 1");

	CaptureStdout();
	player.Stop();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "stop video: Video 1 [duration -> 1 min], AVI-Format\n");

	CaptureStdout();
	player.Play();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "playing video number 1: Video 1 [duration -> 1 min], AVI-Format\n");

	player.Next();
	EXPECT_EQ(player.ActIndex(), 1);
	EXPECT_EQ(player.ActVideo(), "Video 2");

	CaptureStdout();
	player.Play();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "playing video number 2: Video 2 [duration -> 2 min], MKV-Format\n");

	CaptureStdout();
	player.Stop();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "stop video: Video 2 [duration -> 2 min], MKV-Format\n");

	player.Next();
	EXPECT_EQ(player.ActIndex(), 2);
	EXPECT_EQ(player.ActVideo(), "Video 3");

	CaptureStdout();
	player.Play();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "playing video number 3: Video 3 [duration -> 3 min], WMV-Format\n");

	CaptureStdout();
	player.Stop();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "stop video: Video 3 [duration -> 3 min], WMV-Format\n");

	player.Next();
	EXPECT_EQ(player.ActIndex(), 2);
	EXPECT_EQ(player.ActVideo(), "Video 3");

	CaptureStdout();
	player.Play();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "playing video number 3: Video 3 [duration -> 3 min], WMV-Format\n");

	CaptureStdout();
	player.Stop();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "stop video: Video 3 [duration -> 3 min], WMV-Format\n");

	player.First();
	EXPECT_EQ(player.ActIndex(), 0);
	EXPECT_EQ(player.ActVideo(), "Video 1");

	CaptureStdout();
	player.Stop();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "stop video: Video 1 [duration -> 1 min], AVI-Format\n");

	CaptureStdout();
	player.Play();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "playing video number 1: Video 1 [duration -> 1 min], AVI-Format\n");

}

/////////////////////////////////////////////////

TEST(VideoPlayerAdapter, Empty) {
	std::string output = "";
	
	CaptureStdout();
	PlayerClient client{ std::make_unique<VideoPlayerAdapter>(std::make_unique<VideoPlayer>()) };
	client.Run();
	output = GetCapturedStdout();
	cout << output;

	EXPECT_EQ(output, "### Test Stop/Play ###\nno video in playlist!\nno video in playlist!\nIncrease volume 110 times.\nDecrease volume 110 times.\n\n### Test Next/Prev ###\n- Wrap around to last:\nno video in playlist!\n- Wrap around to first:\nno video in playlist!\n- Switch through all entries:\nno video in playlist!\nno video in playlist!\nno video in playlist!\nno video in playlist!\nno video in playlist!\n\n### Test Select ###\n-- Search for \"Does not exist\":\nvideo: Does not exist not found!\nno video in playlist!\nno video in playlist!\n\n-- Search for \"Media 1\":\nvideo: Media 1 not found!\nno video in playlist!\nno video in playlist!\n\n-- Search for \"Media 2\":\nvideo: Media 2 not found!\nno video in playlist!\nno video in playlist!\n\n-- Search for \"Media 2\" - Should find the same one again:\nvideo: Media 2 not found!\nno video in playlist!\nno video in playlist!\n");
}

TEST(VideoPlayerAdapter, One) {
	std::string output = "";

	VideoPlayer::UPtr player = std::make_unique<VideoPlayer>();
	player->Add("Media 1", 1, VideoFormat::MKV);

	CaptureStdout();
	PlayerClient client{ std::make_unique<VideoPlayerAdapter>(std::move(player)) };
	client.Run();
	output = GetCapturedStdout();
	cout << output;

	EXPECT_EQ(output, "### Test Stop/Play ###\nstop video: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nIncrease volume 110 times.\nDecrease volume 110 times.\n\n### Test Next/Prev ###\n- Wrap around to last:\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\n- Wrap around to first:\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\n- Switch through all entries:\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\n\n### Test Select ###\n-- Search for \"Does not exist\":\nvideo: Does not exist not found!\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nstop video: Media 1 [duration -> 1 min], MKV-Format\n\n-- Search for \"Media 1\":\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nstop video: Media 1 [duration -> 1 min], MKV-Format\n\n-- Search for \"Media 2\":\nvideo: Media 2 not found!\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nstop video: Media 1 [duration -> 1 min], MKV-Format\n\n-- Search for \"Media 2\" - Should find the same one again:\nvideo: Media 2 not found!\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nstop video: Media 1 [duration -> 1 min], MKV-Format\n");
}

TEST(VideoPlayerAdapter, Multiple) {
	std::string output = "";

	VideoPlayer::UPtr player = std::make_unique<VideoPlayer>();
	player->Add("Media 1", 1, VideoFormat::MKV);
	player->Add("Media 2", 2, VideoFormat::AVI);
	player->Add("Media 3", 3, VideoFormat::WMV);
	player->Add("Media 2", 4, VideoFormat::MKV);

	CaptureStdout();
	PlayerClient client{ std::make_unique<VideoPlayerAdapter>(std::move(player)) };
	client.Run();
	output = GetCapturedStdout();
	cout << output;

	EXPECT_EQ(output, "### Test Stop/Play ###\nstop video: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nIncrease volume 110 times.\nDecrease volume 110 times.\n\n### Test Next/Prev ###\n- Wrap around to last:\nplaying video number 4: Media 2 [duration -> 4 min], MKV-Format\n- Wrap around to first:\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\n- Switch through all entries:\nplaying video number 2: Media 2 [duration -> 2 min], AVI-Format\nplaying video number 3: Media 3 [duration -> 3 min], WMV-Format\nplaying video number 4: Media 2 [duration -> 4 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 2: Media 2 [duration -> 2 min], AVI-Format\n\n### Test Select ###\n-- Search for \"Does not exist\":\nvideo: Does not exist not found!\nplaying video number 2: Media 2 [duration -> 2 min], AVI-Format\nstop video: Media 2 [duration -> 2 min], AVI-Format\n\n-- Search for \"Media 1\":\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nstop video: Media 1 [duration -> 1 min], MKV-Format\n\n-- Search for \"Media 2\":\nplaying video number 2: Media 2 [duration -> 2 min], AVI-Format\nstop video: Media 2 [duration -> 2 min], AVI-Format\n\n-- Search for \"Media 2\" - Should find the same one again:\nplaying video number 2: Media 2 [duration -> 2 min], AVI-Format\nstop video: Media 2 [duration -> 2 min], AVI-Format\n");
}

/////////////////////////////////////////////////

TEST(MusicPlayerAdapter, Empty) {
	std::string output = "";

	CaptureStdout();
	PlayerClient client{ std::make_unique<MusicPlayerAdapter>(std::make_unique<MusicPlayer>()) };
	client.Run();
	output = GetCapturedStdout();
	cout << output;

	EXPECT_EQ(output, "### Test Stop/Play ###\nno song in playlist!\nno song in playlist!\nIncrease volume 110 times.\nDecrease volume 110 times.\n\n### Test Next/Prev ###\n- Wrap around to last:\nno song in playlist!\n- Wrap around to first:\nno song in playlist!\n- Switch through all entries:\nno song in playlist!\nno song in playlist!\nno song in playlist!\nno song in playlist!\nno song in playlist!\n\n### Test Select ###\n-- Search for \"Does not exist\":\nsong: Does not exist not found!\nno song in playlist!\nno song in playlist!\n\n-- Search for \"Media 1\":\nsong: Media 1 not found!\nno song in playlist!\nno song in playlist!\n\n-- Search for \"Media 2\":\nsong: Media 2 not found!\nno song in playlist!\nno song in playlist!\n\n-- Search for \"Media 2\" - Should find the same one again:\nsong: Media 2 not found!\nno song in playlist!\nno song in playlist!\n");
}

TEST(MusicPlayerAdapter, One) {
	std::string output = "";

	MusicPlayer::UPtr player = std::make_unique<MusicPlayer>();
	player->Add("Media 1", 1);

	CaptureStdout();
	PlayerClient client{ std::make_unique<MusicPlayerAdapter>(std::move(player)) };
	client.Run();
	output = GetCapturedStdout();
	cout << output;

	EXPECT_EQ(output, "### Test Stop/Play ###\nstop song: Media 1 (1 sec)\nplaying song number 1: Media 1 (1 sec)\nIncrease volume 110 times.\nDecrease volume 110 times.\n\n### Test Next/Prev ###\n- Wrap around to last:\nplaying song number 1: Media 1 (1 sec)\n- Wrap around to first:\nplaying song number 1: Media 1 (1 sec)\n- Switch through all entries:\nplaying song number 1: Media 1 (1 sec)\nplaying song number 1: Media 1 (1 sec)\nplaying song number 1: Media 1 (1 sec)\nplaying song number 1: Media 1 (1 sec)\nplaying song number 1: Media 1 (1 sec)\n\n### Test Select ###\n-- Search for \"Does not exist\":\nsong: Does not exist not found!\nplaying song number 1: Media 1 (1 sec)\nstop song: Media 1 (1 sec)\n\n-- Search for \"Media 1\":\nplaying song number 1: Media 1 (1 sec)\nstop song: Media 1 (1 sec)\n\n-- Search for \"Media 2\":\nsong: Media 2 not found!\nplaying song number 1: Media 1 (1 sec)\nstop song: Media 1 (1 sec)\n\n-- Search for \"Media 2\" - Should find the same one again:\nsong: Media 2 not found!\nplaying song number 1: Media 1 (1 sec)\nstop song: Media 1 (1 sec)\n");
}

TEST(MusicPlayerAdapter, Multiple) {
	std::string output = "";
	int *x = new int;
	*x = 1;

	MusicPlayer::UPtr player = std::make_unique<MusicPlayer>();
	player->Add("Media 1", *x);
	player->Add("Media 2", 2);
	player->Add("Media 3", 3);
	player->Add("Media 2", 4);

	CaptureStdout();
	PlayerClient client{ std::make_unique<MusicPlayerAdapter>(std::move(player)) };
	client.Run();
	output = GetCapturedStdout();
	cout << output;

	EXPECT_EQ(output, "### Test Stop/Play ###\nstop song: Media 1 (1 sec)\nplaying song number 1: Media 1 (1 sec)\nIncrease volume 110 times.\nDecrease volume 110 times.\n\n### Test Next/Prev ###\n- Wrap around to last:\nplaying song number 4: Media 2 (4 sec)\n- Wrap around to first:\nplaying song number 1: Media 1 (1 sec)\n- Switch through all entries:\nplaying song number 2: Media 2 (2 sec)\nplaying song number 3: Media 3 (3 sec)\nplaying song number 4: Media 2 (4 sec)\nplaying song number 1: Media 1 (1 sec)\nplaying song number 2: Media 2 (2 sec)\n\n### Test Select ###\n-- Search for \"Does not exist\":\nsong: Does not exist not found!\nplaying song number 2: Media 2 (2 sec)\nstop song: Media 2 (2 sec)\n\n-- Search for \"Media 1\":\nplaying song number 1: Media 1 (1 sec)\nstop song: Media 1 (1 sec)\n\n-- Search for \"Media 2\":\nplaying song number 2: Media 2 (2 sec)\nstop song: Media 2 (2 sec)\n\n-- Search for \"Media 2\" - Should find the same one again:\nplaying song number 2: Media 2 (2 sec)\nstop song: Media 2 (2 sec)\n");
}

/////////////////////////////////////////////////