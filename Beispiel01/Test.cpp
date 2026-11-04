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
	
	CaptureStdout();
	player.SetVolume(0);
	EXPECT_EQ(player.GetVolume(), 0);
	output = GetCapturedStdout();
	EXPECT_EQ(output, "volume is now -> 0\n");
	cout << output;
	
	CaptureStdout();
	player.SetVolume(50);
	EXPECT_EQ(player.GetVolume(), 50);
	output = GetCapturedStdout();
	EXPECT_EQ(output, "volume is now -> 50\n");
	cout << output;

	CaptureStdout();
	player.SetVolume(VideoPlayer::MAX_VOLUME);
	EXPECT_EQ(player.GetVolume(), VideoPlayer::MAX_VOLUME);
	output = GetCapturedStdout();
	EXPECT_EQ(output, string("volume is now -> ") + to_string(VideoPlayer::MAX_VOLUME) + "\n");
	cout << output;


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

TEST(Song, Test)
{
	Song song1("Hells Bells", 256);

	EXPECT_EQ(song1.GetName(), "Hells Bells");
	EXPECT_EQ(song1.GetDuration(), 256);

	EXPECT_EQ(song1.ToString(), "Hells Bells (256 sec)");

	song1.SetName("Stereo Madness");
	EXPECT_EQ(song1.GetName(), "Stereo Madness");

	Song const song2{ song1 };
	EXPECT_EQ(song1.GetDuration(), song2.GetDuration());
	EXPECT_EQ(song1.GetName(), song2.GetName());
	EXPECT_EQ(song1.ToString(), song2.ToString());

	Song const song3("Deadlocked", 90);
	song1 = song3;

	EXPECT_EQ(song1.GetDuration(), song3.GetDuration());
	EXPECT_EQ(song1.GetName(), song3.GetName());
	EXPECT_EQ(song1.ToString(), song3.ToString());
	EXPECT_EQ(song1.ToString(), song3.ToString());
}

/////////////////////////////////////////////////

TEST(MusicPlayer, Empty)
{
	MusicPlayer player;

	string output = "";

	CaptureStdout();
	player.Start();
	output = GetCapturedStdout();
	cout << output;

	EXPECT_EQ(output, "no song in playlist!\n");

	CaptureStdout();
	player.Stop();
	output = GetCapturedStdout();
	cout << output;

	EXPECT_EQ(output, "no song in playlist!\n");

	CaptureStdout();
	player.SwitchNext();
	output = GetCapturedStdout();
	cout << output;

	EXPECT_EQ(output, "");

	EXPECT_EQ(player.GetActIndex(), 0);

	EXPECT_EQ(player.Find("Hells Bells"), false);
	EXPECT_EQ(player.GetActIndex(), 0);

	EXPECT_EQ(player.GetCount(), 0);

	CaptureStdout();
	player.IncreaseVol(5);
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "volume is now -> " + to_string(MusicPlayer::DEFAULT_VOLUME + 5) + '\n');

	CaptureStdout();
	player.IncreaseVol(0);
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "volume is now -> " + to_string(MusicPlayer::DEFAULT_VOLUME + 5) + '\n');

	CaptureStdout();
	player.DecreaseVol(0);
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "volume is now -> " + to_string(MusicPlayer::DEFAULT_VOLUME + 5) + '\n');

	CaptureStdout();
	player.DecreaseVol(5);
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "volume is now -> " + to_string(MusicPlayer::DEFAULT_VOLUME) + '\n');

	CaptureStdout();
	player.DecreaseVol(50);
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "volume is now -> 0\n");

	CaptureStdout();
	player.IncreaseVol(MusicPlayer::MAX_VOLUME + 100);
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "volume is now -> " + to_string(MusicPlayer::MAX_VOLUME) + '\n');

}

TEST(MusicPlayer, OneSong)
{
	MusicPlayer player;

	string output = "";

	player.Add("Hells Bells", 256);
	EXPECT_EQ(player.GetActIndex(), 0);

	CaptureStdout();
	player.Start();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "playing song number 1: Hells Bells (256 sec)\n");

	CaptureStdout();
	player.Stop();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "stop song: Hells Bells (256 sec)\n");


	player.SwitchNext();
	EXPECT_EQ(player.GetActIndex(), 0);


	CaptureStdout();
	player.Start();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "playing song number 1: Hells Bells (256 sec)\n");

	CaptureStdout();
	player.Stop();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "stop song: Hells Bells (256 sec)\n");


	EXPECT_EQ(player.Find("Hells Bells"), true);
	EXPECT_EQ(player.GetActIndex(), 0);

	EXPECT_EQ(player.Find("StereoMadness"), false);
	EXPECT_EQ(player.GetActIndex(), 0);
}

TEST(MusicPlayer, MultipleSongs)
{
	MusicPlayer player;

	string output = "";

	player.Add("Hells Bells", 256);
	player.Add("Stereomadness", 91);
	player.Add("Clubstep", 92);
	EXPECT_EQ(player.GetActIndex(), 0);

	CaptureStdout();
	player.Start();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "playing song number 1: Hells Bells (256 sec)\n");

	CaptureStdout();
	player.Stop();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "stop song: Hells Bells (256 sec)\n");


	player.SwitchNext();
	EXPECT_EQ(player.GetActIndex(), 1);


	CaptureStdout();
	player.Start();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "playing song number 2: Stereomadness (91 sec)\n");

	CaptureStdout();
	player.Stop();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "stop song: Stereomadness (91 sec)\n");


	player.SwitchNext();
	EXPECT_EQ(player.GetActIndex(), 2);


	CaptureStdout();
	player.Start();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "playing song number 3: Clubstep (92 sec)\n");

	CaptureStdout();
	player.Stop();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "stop song: Clubstep (92 sec)\n");

	player.SwitchNext();
	EXPECT_EQ(player.GetActIndex(), 0);


	EXPECT_EQ(player.Find("Clubstep"), true);
	EXPECT_EQ(player.GetActIndex(), 2);
	EXPECT_EQ(player.Find("Slaughterhouse"), false);
	EXPECT_EQ(player.GetActIndex(), 2);

}

/////////////////////////////////////////////////

TEST(VideoPlayerAdapter, nullptr) {
	try {
		VideoPlayerAdapter const player = VideoPlayerAdapter{ nullptr };
		EXPECT_EQ(1, 0); // Fail.
	}
	catch (std::invalid_argument const& ex) {
		EXPECT_STREQ(ex.what(), "Provided player is nullptr");
	}
}

TEST(VideoPlayerAdapter, Empty) {
	std::string output = "";

	CaptureStdout();
	PlayerClient client{ std::make_unique<VideoPlayerAdapter>(std::make_unique<VideoPlayer>()) };
	client.Run();
	output = GetCapturedStdout();
	cout << output;

	EXPECT_EQ(output, "### Test Stop/Play ###\nno video in playlist!\nno video in playlist!\nIncrease volume 110 times.\nvolume is now -> 9\nvolume is now -> 10\nvolume is now -> 11\nvolume is now -> 12\nvolume is now -> 13\nvolume is now -> 14\nvolume is now -> 15\nvolume is now -> 16\nvolume is now -> 17\nvolume is now -> 18\nvolume is now -> 19\nvolume is now -> 20\nvolume is now -> 21\nvolume is now -> 22\nvolume is now -> 23\nvolume is now -> 24\nvolume is now -> 25\nvolume is now -> 26\nvolume is now -> 27\nvolume is now -> 28\nvolume is now -> 29\nvolume is now -> 30\nvolume is now -> 31\nvolume is now -> 32\nvolume is now -> 33\nvolume is now -> 34\nvolume is now -> 35\nvolume is now -> 36\nvolume is now -> 37\nvolume is now -> 38\nvolume is now -> 39\nvolume is now -> 40\nvolume is now -> 41\nvolume is now -> 42\nvolume is now -> 43\nvolume is now -> 44\nvolume is now -> 45\nvolume is now -> 46\nvolume is now -> 47\nvolume is now -> 48\nvolume is now -> 49\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nDecrease volume 110 times.\nvolume is now -> 49\nvolume is now -> 48\nvolume is now -> 47\nvolume is now -> 46\nvolume is now -> 45\nvolume is now -> 44\nvolume is now -> 43\nvolume is now -> 42\nvolume is now -> 41\nvolume is now -> 40\nvolume is now -> 39\nvolume is now -> 38\nvolume is now -> 37\nvolume is now -> 36\nvolume is now -> 35\nvolume is now -> 34\nvolume is now -> 33\nvolume is now -> 32\nvolume is now -> 31\nvolume is now -> 30\nvolume is now -> 29\nvolume is now -> 28\nvolume is now -> 27\nvolume is now -> 26\nvolume is now -> 25\nvolume is now -> 24\nvolume is now -> 23\nvolume is now -> 22\nvolume is now -> 21\nvolume is now -> 20\nvolume is now -> 19\nvolume is now -> 18\nvolume is now -> 17\nvolume is now -> 16\nvolume is now -> 15\nvolume is now -> 14\nvolume is now -> 13\nvolume is now -> 12\nvolume is now -> 11\nvolume is now -> 10\nvolume is now -> 9\nvolume is now -> 8\nvolume is now -> 7\nvolume is now -> 6\nvolume is now -> 5\nvolume is now -> 4\nvolume is now -> 3\nvolume is now -> 2\nvolume is now -> 1\nvolume is now -> 0\n\n### Test Next/Prev ###\n- Wrap around to last:\nno video in playlist!\n- Wrap around to first:\nno video in playlist!\n- Switch through all entries:\nno video in playlist!\nno video in playlist!\nno video in playlist!\nno video in playlist!\nno video in playlist!\n\n### Test Select ###\n-- Search for \"Does not exist\":\nvideo: Does not exist not found!\nno video in playlist!\nno video in playlist!\n\n-- Search for \"Media 1\":\nvideo: Media 1 not found!\nno video in playlist!\nno video in playlist!\n\n-- Search for \"Media 2\":\nvideo: Media 2 not found!\nno video in playlist!\nno video in playlist!\n\n-- Search for \"Media 2\" - Should find the same one again:\nvideo: Media 2 not found!\nno video in playlist!\nno video in playlist!\n");
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

	EXPECT_EQ(output, "### Test Stop/Play ###\nstop video: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nIncrease volume 110 times.\nvolume is now -> 9\nvolume is now -> 10\nvolume is now -> 11\nvolume is now -> 12\nvolume is now -> 13\nvolume is now -> 14\nvolume is now -> 15\nvolume is now -> 16\nvolume is now -> 17\nvolume is now -> 18\nvolume is now -> 19\nvolume is now -> 20\nvolume is now -> 21\nvolume is now -> 22\nvolume is now -> 23\nvolume is now -> 24\nvolume is now -> 25\nvolume is now -> 26\nvolume is now -> 27\nvolume is now -> 28\nvolume is now -> 29\nvolume is now -> 30\nvolume is now -> 31\nvolume is now -> 32\nvolume is now -> 33\nvolume is now -> 34\nvolume is now -> 35\nvolume is now -> 36\nvolume is now -> 37\nvolume is now -> 38\nvolume is now -> 39\nvolume is now -> 40\nvolume is now -> 41\nvolume is now -> 42\nvolume is now -> 43\nvolume is now -> 44\nvolume is now -> 45\nvolume is now -> 46\nvolume is now -> 47\nvolume is now -> 48\nvolume is now -> 49\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nDecrease volume 110 times.\nvolume is now -> 49\nvolume is now -> 48\nvolume is now -> 47\nvolume is now -> 46\nvolume is now -> 45\nvolume is now -> 44\nvolume is now -> 43\nvolume is now -> 42\nvolume is now -> 41\nvolume is now -> 40\nvolume is now -> 39\nvolume is now -> 38\nvolume is now -> 37\nvolume is now -> 36\nvolume is now -> 35\nvolume is now -> 34\nvolume is now -> 33\nvolume is now -> 32\nvolume is now -> 31\nvolume is now -> 30\nvolume is now -> 29\nvolume is now -> 28\nvolume is now -> 27\nvolume is now -> 26\nvolume is now -> 25\nvolume is now -> 24\nvolume is now -> 23\nvolume is now -> 22\nvolume is now -> 21\nvolume is now -> 20\nvolume is now -> 19\nvolume is now -> 18\nvolume is now -> 17\nvolume is now -> 16\nvolume is now -> 15\nvolume is now -> 14\nvolume is now -> 13\nvolume is now -> 12\nvolume is now -> 11\nvolume is now -> 10\nvolume is now -> 9\nvolume is now -> 8\nvolume is now -> 7\nvolume is now -> 6\nvolume is now -> 5\nvolume is now -> 4\nvolume is now -> 3\nvolume is now -> 2\nvolume is now -> 1\nvolume is now -> 0\n\n### Test Next/Prev ###\n- Wrap around to last:\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\n- Wrap around to first:\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\n- Switch through all entries:\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\n\n### Test Select ###\n-- Search for \"Does not exist\":\nvideo: Does not exist not found!\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nstop video: Media 1 [duration -> 1 min], MKV-Format\n\n-- Search for \"Media 1\":\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nstop video: Media 1 [duration -> 1 min], MKV-Format\n\n-- Search for \"Media 2\":\nvideo: Media 2 not found!\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nstop video: Media 1 [duration -> 1 min], MKV-Format\n\n-- Search for \"Media 2\" - Should find the same one again:\nvideo: Media 2 not found!\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nstop video: Media 1 [duration -> 1 min], MKV-Format\n");
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

	EXPECT_EQ(output, "### Test Stop/Play ###\nstop video: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nIncrease volume 110 times.\nvolume is now -> 9\nvolume is now -> 10\nvolume is now -> 11\nvolume is now -> 12\nvolume is now -> 13\nvolume is now -> 14\nvolume is now -> 15\nvolume is now -> 16\nvolume is now -> 17\nvolume is now -> 18\nvolume is now -> 19\nvolume is now -> 20\nvolume is now -> 21\nvolume is now -> 22\nvolume is now -> 23\nvolume is now -> 24\nvolume is now -> 25\nvolume is now -> 26\nvolume is now -> 27\nvolume is now -> 28\nvolume is now -> 29\nvolume is now -> 30\nvolume is now -> 31\nvolume is now -> 32\nvolume is now -> 33\nvolume is now -> 34\nvolume is now -> 35\nvolume is now -> 36\nvolume is now -> 37\nvolume is now -> 38\nvolume is now -> 39\nvolume is now -> 40\nvolume is now -> 41\nvolume is now -> 42\nvolume is now -> 43\nvolume is now -> 44\nvolume is now -> 45\nvolume is now -> 46\nvolume is now -> 47\nvolume is now -> 48\nvolume is now -> 49\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nDecrease volume 110 times.\nvolume is now -> 49\nvolume is now -> 48\nvolume is now -> 47\nvolume is now -> 46\nvolume is now -> 45\nvolume is now -> 44\nvolume is now -> 43\nvolume is now -> 42\nvolume is now -> 41\nvolume is now -> 40\nvolume is now -> 39\nvolume is now -> 38\nvolume is now -> 37\nvolume is now -> 36\nvolume is now -> 35\nvolume is now -> 34\nvolume is now -> 33\nvolume is now -> 32\nvolume is now -> 31\nvolume is now -> 30\nvolume is now -> 29\nvolume is now -> 28\nvolume is now -> 27\nvolume is now -> 26\nvolume is now -> 25\nvolume is now -> 24\nvolume is now -> 23\nvolume is now -> 22\nvolume is now -> 21\nvolume is now -> 20\nvolume is now -> 19\nvolume is now -> 18\nvolume is now -> 17\nvolume is now -> 16\nvolume is now -> 15\nvolume is now -> 14\nvolume is now -> 13\nvolume is now -> 12\nvolume is now -> 11\nvolume is now -> 10\nvolume is now -> 9\nvolume is now -> 8\nvolume is now -> 7\nvolume is now -> 6\nvolume is now -> 5\nvolume is now -> 4\nvolume is now -> 3\nvolume is now -> 2\nvolume is now -> 1\nvolume is now -> 0\n\n### Test Next/Prev ###\n- Wrap around to last:\nplaying video number 4: Media 2 [duration -> 4 min], MKV-Format\n- Wrap around to first:\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\n- Switch through all entries:\nplaying video number 2: Media 2 [duration -> 2 min], AVI-Format\nplaying video number 3: Media 3 [duration -> 3 min], WMV-Format\nplaying video number 4: Media 2 [duration -> 4 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 2: Media 2 [duration -> 2 min], AVI-Format\n\n### Test Select ###\n-- Search for \"Does not exist\":\nvideo: Does not exist not found!\nplaying video number 2: Media 2 [duration -> 2 min], AVI-Format\nstop video: Media 2 [duration -> 2 min], AVI-Format\n\n-- Search for \"Media 1\":\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nplaying video number 1: Media 1 [duration -> 1 min], MKV-Format\nstop video: Media 1 [duration -> 1 min], MKV-Format\n\n-- Search for \"Media 2\":\nplaying video number 2: Media 2 [duration -> 2 min], AVI-Format\nplaying video number 2: Media 2 [duration -> 2 min], AVI-Format\nstop video: Media 2 [duration -> 2 min], AVI-Format\n\n-- Search for \"Media 2\" - Should find the same one again:\nplaying video number 2: Media 2 [duration -> 2 min], AVI-Format\nplaying video number 2: Media 2 [duration -> 2 min], AVI-Format\nstop video: Media 2 [duration -> 2 min], AVI-Format\n");
}

/////////////////////////////////////////////////

TEST(MusicPlayerAdapter, nullptr) {
	try {
		MusicPlayerAdapter const player = MusicPlayerAdapter{ nullptr };
		EXPECT_EQ(1, 0); // Fail.
	}
	catch (std::invalid_argument const& ex) {
		EXPECT_STREQ(ex.what(), "Provided player is nullptr");
	}
}

TEST(MusicPlayerAdapter, Empty) {
	std::string output = "";

	CaptureStdout();
	PlayerClient client{ std::make_unique<MusicPlayerAdapter>(std::make_unique<MusicPlayer>()) };
	client.Run();
	output = GetCapturedStdout();
	cout << output;

	EXPECT_EQ(output, "### Test Stop/Play ###\nno song in playlist!\nno song in playlist!\nIncrease volume 110 times.\nvolume is now -> 16\nvolume is now -> 17\nvolume is now -> 18\nvolume is now -> 19\nvolume is now -> 20\nvolume is now -> 21\nvolume is now -> 22\nvolume is now -> 23\nvolume is now -> 24\nvolume is now -> 25\nvolume is now -> 26\nvolume is now -> 27\nvolume is now -> 28\nvolume is now -> 29\nvolume is now -> 30\nvolume is now -> 31\nvolume is now -> 32\nvolume is now -> 33\nvolume is now -> 34\nvolume is now -> 35\nvolume is now -> 36\nvolume is now -> 37\nvolume is now -> 38\nvolume is now -> 39\nvolume is now -> 40\nvolume is now -> 41\nvolume is now -> 42\nvolume is now -> 43\nvolume is now -> 44\nvolume is now -> 45\nvolume is now -> 46\nvolume is now -> 47\nvolume is now -> 48\nvolume is now -> 49\nvolume is now -> 50\nvolume is now -> 51\nvolume is now -> 52\nvolume is now -> 53\nvolume is now -> 54\nvolume is now -> 55\nvolume is now -> 56\nvolume is now -> 57\nvolume is now -> 58\nvolume is now -> 59\nvolume is now -> 60\nvolume is now -> 61\nvolume is now -> 62\nvolume is now -> 63\nvolume is now -> 64\nvolume is now -> 65\nvolume is now -> 66\nvolume is now -> 67\nvolume is now -> 68\nvolume is now -> 69\nvolume is now -> 70\nvolume is now -> 71\nvolume is now -> 72\nvolume is now -> 73\nvolume is now -> 74\nvolume is now -> 75\nvolume is now -> 76\nvolume is now -> 77\nvolume is now -> 78\nvolume is now -> 79\nvolume is now -> 80\nvolume is now -> 81\nvolume is now -> 82\nvolume is now -> 83\nvolume is now -> 84\nvolume is now -> 85\nvolume is now -> 86\nvolume is now -> 87\nvolume is now -> 88\nvolume is now -> 89\nvolume is now -> 90\nvolume is now -> 91\nvolume is now -> 92\nvolume is now -> 93\nvolume is now -> 94\nvolume is now -> 95\nvolume is now -> 96\nvolume is now -> 97\nvolume is now -> 98\nvolume is now -> 99\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nDecrease volume 110 times.\nvolume is now -> 99\nvolume is now -> 98\nvolume is now -> 97\nvolume is now -> 96\nvolume is now -> 95\nvolume is now -> 94\nvolume is now -> 93\nvolume is now -> 92\nvolume is now -> 91\nvolume is now -> 90\nvolume is now -> 89\nvolume is now -> 88\nvolume is now -> 87\nvolume is now -> 86\nvolume is now -> 85\nvolume is now -> 84\nvolume is now -> 83\nvolume is now -> 82\nvolume is now -> 81\nvolume is now -> 80\nvolume is now -> 79\nvolume is now -> 78\nvolume is now -> 77\nvolume is now -> 76\nvolume is now -> 75\nvolume is now -> 74\nvolume is now -> 73\nvolume is now -> 72\nvolume is now -> 71\nvolume is now -> 70\nvolume is now -> 69\nvolume is now -> 68\nvolume is now -> 67\nvolume is now -> 66\nvolume is now -> 65\nvolume is now -> 64\nvolume is now -> 63\nvolume is now -> 62\nvolume is now -> 61\nvolume is now -> 60\nvolume is now -> 59\nvolume is now -> 58\nvolume is now -> 57\nvolume is now -> 56\nvolume is now -> 55\nvolume is now -> 54\nvolume is now -> 53\nvolume is now -> 52\nvolume is now -> 51\nvolume is now -> 50\nvolume is now -> 49\nvolume is now -> 48\nvolume is now -> 47\nvolume is now -> 46\nvolume is now -> 45\nvolume is now -> 44\nvolume is now -> 43\nvolume is now -> 42\nvolume is now -> 41\nvolume is now -> 40\nvolume is now -> 39\nvolume is now -> 38\nvolume is now -> 37\nvolume is now -> 36\nvolume is now -> 35\nvolume is now -> 34\nvolume is now -> 33\nvolume is now -> 32\nvolume is now -> 31\nvolume is now -> 30\nvolume is now -> 29\nvolume is now -> 28\nvolume is now -> 27\nvolume is now -> 26\nvolume is now -> 25\nvolume is now -> 24\nvolume is now -> 23\nvolume is now -> 22\nvolume is now -> 21\nvolume is now -> 20\nvolume is now -> 19\nvolume is now -> 18\nvolume is now -> 17\nvolume is now -> 16\nvolume is now -> 15\nvolume is now -> 14\nvolume is now -> 13\nvolume is now -> 12\nvolume is now -> 11\nvolume is now -> 10\nvolume is now -> 9\nvolume is now -> 8\nvolume is now -> 7\nvolume is now -> 6\nvolume is now -> 5\nvolume is now -> 4\nvolume is now -> 3\nvolume is now -> 2\nvolume is now -> 1\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\n\n### Test Next/Prev ###\n- Wrap around to last:\nno song in playlist!\n- Wrap around to first:\nno song in playlist!\n- Switch through all entries:\nno song in playlist!\nno song in playlist!\nno song in playlist!\nno song in playlist!\nno song in playlist!\n\n### Test Select ###\n-- Search for \"Does not exist\":\nsong: Does not exist not found!\nno song in playlist!\nno song in playlist!\n\n-- Search for \"Media 1\":\nsong: Media 1 not found!\nno song in playlist!\nno song in playlist!\n\n-- Search for \"Media 2\":\nsong: Media 2 not found!\nno song in playlist!\nno song in playlist!\n\n-- Search for \"Media 2\" - Should find the same one again:\nsong: Media 2 not found!\nno song in playlist!\nno song in playlist!\n");
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

	EXPECT_EQ(output, "### Test Stop/Play ###\nstop song: Media 1 (1 sec)\nplaying song number 1: Media 1 (1 sec)\nIncrease volume 110 times.\nvolume is now -> 16\nvolume is now -> 17\nvolume is now -> 18\nvolume is now -> 19\nvolume is now -> 20\nvolume is now -> 21\nvolume is now -> 22\nvolume is now -> 23\nvolume is now -> 24\nvolume is now -> 25\nvolume is now -> 26\nvolume is now -> 27\nvolume is now -> 28\nvolume is now -> 29\nvolume is now -> 30\nvolume is now -> 31\nvolume is now -> 32\nvolume is now -> 33\nvolume is now -> 34\nvolume is now -> 35\nvolume is now -> 36\nvolume is now -> 37\nvolume is now -> 38\nvolume is now -> 39\nvolume is now -> 40\nvolume is now -> 41\nvolume is now -> 42\nvolume is now -> 43\nvolume is now -> 44\nvolume is now -> 45\nvolume is now -> 46\nvolume is now -> 47\nvolume is now -> 48\nvolume is now -> 49\nvolume is now -> 50\nvolume is now -> 51\nvolume is now -> 52\nvolume is now -> 53\nvolume is now -> 54\nvolume is now -> 55\nvolume is now -> 56\nvolume is now -> 57\nvolume is now -> 58\nvolume is now -> 59\nvolume is now -> 60\nvolume is now -> 61\nvolume is now -> 62\nvolume is now -> 63\nvolume is now -> 64\nvolume is now -> 65\nvolume is now -> 66\nvolume is now -> 67\nvolume is now -> 68\nvolume is now -> 69\nvolume is now -> 70\nvolume is now -> 71\nvolume is now -> 72\nvolume is now -> 73\nvolume is now -> 74\nvolume is now -> 75\nvolume is now -> 76\nvolume is now -> 77\nvolume is now -> 78\nvolume is now -> 79\nvolume is now -> 80\nvolume is now -> 81\nvolume is now -> 82\nvolume is now -> 83\nvolume is now -> 84\nvolume is now -> 85\nvolume is now -> 86\nvolume is now -> 87\nvolume is now -> 88\nvolume is now -> 89\nvolume is now -> 90\nvolume is now -> 91\nvolume is now -> 92\nvolume is now -> 93\nvolume is now -> 94\nvolume is now -> 95\nvolume is now -> 96\nvolume is now -> 97\nvolume is now -> 98\nvolume is now -> 99\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nDecrease volume 110 times.\nvolume is now -> 99\nvolume is now -> 98\nvolume is now -> 97\nvolume is now -> 96\nvolume is now -> 95\nvolume is now -> 94\nvolume is now -> 93\nvolume is now -> 92\nvolume is now -> 91\nvolume is now -> 90\nvolume is now -> 89\nvolume is now -> 88\nvolume is now -> 87\nvolume is now -> 86\nvolume is now -> 85\nvolume is now -> 84\nvolume is now -> 83\nvolume is now -> 82\nvolume is now -> 81\nvolume is now -> 80\nvolume is now -> 79\nvolume is now -> 78\nvolume is now -> 77\nvolume is now -> 76\nvolume is now -> 75\nvolume is now -> 74\nvolume is now -> 73\nvolume is now -> 72\nvolume is now -> 71\nvolume is now -> 70\nvolume is now -> 69\nvolume is now -> 68\nvolume is now -> 67\nvolume is now -> 66\nvolume is now -> 65\nvolume is now -> 64\nvolume is now -> 63\nvolume is now -> 62\nvolume is now -> 61\nvolume is now -> 60\nvolume is now -> 59\nvolume is now -> 58\nvolume is now -> 57\nvolume is now -> 56\nvolume is now -> 55\nvolume is now -> 54\nvolume is now -> 53\nvolume is now -> 52\nvolume is now -> 51\nvolume is now -> 50\nvolume is now -> 49\nvolume is now -> 48\nvolume is now -> 47\nvolume is now -> 46\nvolume is now -> 45\nvolume is now -> 44\nvolume is now -> 43\nvolume is now -> 42\nvolume is now -> 41\nvolume is now -> 40\nvolume is now -> 39\nvolume is now -> 38\nvolume is now -> 37\nvolume is now -> 36\nvolume is now -> 35\nvolume is now -> 34\nvolume is now -> 33\nvolume is now -> 32\nvolume is now -> 31\nvolume is now -> 30\nvolume is now -> 29\nvolume is now -> 28\nvolume is now -> 27\nvolume is now -> 26\nvolume is now -> 25\nvolume is now -> 24\nvolume is now -> 23\nvolume is now -> 22\nvolume is now -> 21\nvolume is now -> 20\nvolume is now -> 19\nvolume is now -> 18\nvolume is now -> 17\nvolume is now -> 16\nvolume is now -> 15\nvolume is now -> 14\nvolume is now -> 13\nvolume is now -> 12\nvolume is now -> 11\nvolume is now -> 10\nvolume is now -> 9\nvolume is now -> 8\nvolume is now -> 7\nvolume is now -> 6\nvolume is now -> 5\nvolume is now -> 4\nvolume is now -> 3\nvolume is now -> 2\nvolume is now -> 1\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\n\n### Test Next/Prev ###\n- Wrap around to last:\nplaying song number 1: Media 1 (1 sec)\n- Wrap around to first:\nplaying song number 1: Media 1 (1 sec)\n- Switch through all entries:\nplaying song number 1: Media 1 (1 sec)\nplaying song number 1: Media 1 (1 sec)\nplaying song number 1: Media 1 (1 sec)\nplaying song number 1: Media 1 (1 sec)\nplaying song number 1: Media 1 (1 sec)\n\n### Test Select ###\n-- Search for \"Does not exist\":\nsong: Does not exist not found!\nplaying song number 1: Media 1 (1 sec)\nstop song: Media 1 (1 sec)\n\n-- Search for \"Media 1\":\nplaying song number 1: Media 1 (1 sec)\nplaying song number 1: Media 1 (1 sec)\nstop song: Media 1 (1 sec)\n\n-- Search for \"Media 2\":\nsong: Media 2 not found!\nplaying song number 1: Media 1 (1 sec)\nstop song: Media 1 (1 sec)\n\n-- Search for \"Media 2\" - Should find the same one again:\nsong: Media 2 not found!\nplaying song number 1: Media 1 (1 sec)\nstop song: Media 1 (1 sec)\n");
}

TEST(MusicPlayerAdapter, Multiple) {
	std::string output = "";

	MusicPlayer::UPtr player = std::make_unique<MusicPlayer>();
	player->Add("Media 1", 1);
	player->Add("Media 2", 2);
	player->Add("Media 3", 3);
	player->Add("Media 2", 4);

	CaptureStdout();
	PlayerClient client{ std::make_unique<MusicPlayerAdapter>(std::move(player)) };
	client.Run();
	output = GetCapturedStdout();
	cout << output;

	EXPECT_EQ(output, "### Test Stop/Play ###\nstop song: Media 1 (1 sec)\nplaying song number 1: Media 1 (1 sec)\nIncrease volume 110 times.\nvolume is now -> 16\nvolume is now -> 17\nvolume is now -> 18\nvolume is now -> 19\nvolume is now -> 20\nvolume is now -> 21\nvolume is now -> 22\nvolume is now -> 23\nvolume is now -> 24\nvolume is now -> 25\nvolume is now -> 26\nvolume is now -> 27\nvolume is now -> 28\nvolume is now -> 29\nvolume is now -> 30\nvolume is now -> 31\nvolume is now -> 32\nvolume is now -> 33\nvolume is now -> 34\nvolume is now -> 35\nvolume is now -> 36\nvolume is now -> 37\nvolume is now -> 38\nvolume is now -> 39\nvolume is now -> 40\nvolume is now -> 41\nvolume is now -> 42\nvolume is now -> 43\nvolume is now -> 44\nvolume is now -> 45\nvolume is now -> 46\nvolume is now -> 47\nvolume is now -> 48\nvolume is now -> 49\nvolume is now -> 50\nvolume is now -> 51\nvolume is now -> 52\nvolume is now -> 53\nvolume is now -> 54\nvolume is now -> 55\nvolume is now -> 56\nvolume is now -> 57\nvolume is now -> 58\nvolume is now -> 59\nvolume is now -> 60\nvolume is now -> 61\nvolume is now -> 62\nvolume is now -> 63\nvolume is now -> 64\nvolume is now -> 65\nvolume is now -> 66\nvolume is now -> 67\nvolume is now -> 68\nvolume is now -> 69\nvolume is now -> 70\nvolume is now -> 71\nvolume is now -> 72\nvolume is now -> 73\nvolume is now -> 74\nvolume is now -> 75\nvolume is now -> 76\nvolume is now -> 77\nvolume is now -> 78\nvolume is now -> 79\nvolume is now -> 80\nvolume is now -> 81\nvolume is now -> 82\nvolume is now -> 83\nvolume is now -> 84\nvolume is now -> 85\nvolume is now -> 86\nvolume is now -> 87\nvolume is now -> 88\nvolume is now -> 89\nvolume is now -> 90\nvolume is now -> 91\nvolume is now -> 92\nvolume is now -> 93\nvolume is now -> 94\nvolume is now -> 95\nvolume is now -> 96\nvolume is now -> 97\nvolume is now -> 98\nvolume is now -> 99\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nDecrease volume 110 times.\nvolume is now -> 99\nvolume is now -> 98\nvolume is now -> 97\nvolume is now -> 96\nvolume is now -> 95\nvolume is now -> 94\nvolume is now -> 93\nvolume is now -> 92\nvolume is now -> 91\nvolume is now -> 90\nvolume is now -> 89\nvolume is now -> 88\nvolume is now -> 87\nvolume is now -> 86\nvolume is now -> 85\nvolume is now -> 84\nvolume is now -> 83\nvolume is now -> 82\nvolume is now -> 81\nvolume is now -> 80\nvolume is now -> 79\nvolume is now -> 78\nvolume is now -> 77\nvolume is now -> 76\nvolume is now -> 75\nvolume is now -> 74\nvolume is now -> 73\nvolume is now -> 72\nvolume is now -> 71\nvolume is now -> 70\nvolume is now -> 69\nvolume is now -> 68\nvolume is now -> 67\nvolume is now -> 66\nvolume is now -> 65\nvolume is now -> 64\nvolume is now -> 63\nvolume is now -> 62\nvolume is now -> 61\nvolume is now -> 60\nvolume is now -> 59\nvolume is now -> 58\nvolume is now -> 57\nvolume is now -> 56\nvolume is now -> 55\nvolume is now -> 54\nvolume is now -> 53\nvolume is now -> 52\nvolume is now -> 51\nvolume is now -> 50\nvolume is now -> 49\nvolume is now -> 48\nvolume is now -> 47\nvolume is now -> 46\nvolume is now -> 45\nvolume is now -> 44\nvolume is now -> 43\nvolume is now -> 42\nvolume is now -> 41\nvolume is now -> 40\nvolume is now -> 39\nvolume is now -> 38\nvolume is now -> 37\nvolume is now -> 36\nvolume is now -> 35\nvolume is now -> 34\nvolume is now -> 33\nvolume is now -> 32\nvolume is now -> 31\nvolume is now -> 30\nvolume is now -> 29\nvolume is now -> 28\nvolume is now -> 27\nvolume is now -> 26\nvolume is now -> 25\nvolume is now -> 24\nvolume is now -> 23\nvolume is now -> 22\nvolume is now -> 21\nvolume is now -> 20\nvolume is now -> 19\nvolume is now -> 18\nvolume is now -> 17\nvolume is now -> 16\nvolume is now -> 15\nvolume is now -> 14\nvolume is now -> 13\nvolume is now -> 12\nvolume is now -> 11\nvolume is now -> 10\nvolume is now -> 9\nvolume is now -> 8\nvolume is now -> 7\nvolume is now -> 6\nvolume is now -> 5\nvolume is now -> 4\nvolume is now -> 3\nvolume is now -> 2\nvolume is now -> 1\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\n\n### Test Next/Prev ###\n- Wrap around to last:\nplaying song number 4: Media 2 (4 sec)\n- Wrap around to first:\nplaying song number 1: Media 1 (1 sec)\n- Switch through all entries:\nplaying song number 2: Media 2 (2 sec)\nplaying song number 3: Media 3 (3 sec)\nplaying song number 4: Media 2 (4 sec)\nplaying song number 1: Media 1 (1 sec)\nplaying song number 2: Media 2 (2 sec)\n\n### Test Select ###\n-- Search for \"Does not exist\":\nsong: Does not exist not found!\nplaying song number 2: Media 2 (2 sec)\nstop song: Media 2 (2 sec)\n\n-- Search for \"Media 1\":\nplaying song number 1: Media 1 (1 sec)\nplaying song number 1: Media 1 (1 sec)\nstop song: Media 1 (1 sec)\n\n-- Search for \"Media 2\":\nplaying song number 2: Media 2 (2 sec)\nplaying song number 2: Media 2 (2 sec)\nstop song: Media 2 (2 sec)\n\n-- Search for \"Media 2\" - Should find the same one again:\nplaying song number 2: Media 2 (2 sec)\nplaying song number 2: Media 2 (2 sec)\nstop song: Media 2 (2 sec)\n");
}

/////////////////////////////////////////////////

TEST(PlayerClient, nullptr) {
	try {
		PlayerClient const client = PlayerClient{ nullptr };
		EXPECT_EQ(1, 0); // Fail.
	}
	catch (std::invalid_argument const& ex) {
		EXPECT_STREQ(ex.what(), "Provided player is nullptr");
	}

	PlayerClient client = PlayerClient{ std::make_unique<MusicPlayerAdapter>(std::move(std::make_unique<MusicPlayer>())) };
	try {
		client.SetPlayer(nullptr);
		EXPECT_EQ(1, 0); // Fail.
	}
	catch (std::invalid_argument const& ex) {
		EXPECT_STREQ(ex.what(), "Provided player is nullptr");
	}
}

TEST(PlayerClient, SetPlayer) {
	std::string output = "";

	PlayerClient client = PlayerClient{ std::make_unique<MusicPlayerAdapter>(std::move(std::make_unique<MusicPlayer>())) };

	CaptureStdout();
	client.Run();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "### Test Stop/Play ###\nno song in playlist!\nno song in playlist!\nIncrease volume 110 times.\nvolume is now -> 16\nvolume is now -> 17\nvolume is now -> 18\nvolume is now -> 19\nvolume is now -> 20\nvolume is now -> 21\nvolume is now -> 22\nvolume is now -> 23\nvolume is now -> 24\nvolume is now -> 25\nvolume is now -> 26\nvolume is now -> 27\nvolume is now -> 28\nvolume is now -> 29\nvolume is now -> 30\nvolume is now -> 31\nvolume is now -> 32\nvolume is now -> 33\nvolume is now -> 34\nvolume is now -> 35\nvolume is now -> 36\nvolume is now -> 37\nvolume is now -> 38\nvolume is now -> 39\nvolume is now -> 40\nvolume is now -> 41\nvolume is now -> 42\nvolume is now -> 43\nvolume is now -> 44\nvolume is now -> 45\nvolume is now -> 46\nvolume is now -> 47\nvolume is now -> 48\nvolume is now -> 49\nvolume is now -> 50\nvolume is now -> 51\nvolume is now -> 52\nvolume is now -> 53\nvolume is now -> 54\nvolume is now -> 55\nvolume is now -> 56\nvolume is now -> 57\nvolume is now -> 58\nvolume is now -> 59\nvolume is now -> 60\nvolume is now -> 61\nvolume is now -> 62\nvolume is now -> 63\nvolume is now -> 64\nvolume is now -> 65\nvolume is now -> 66\nvolume is now -> 67\nvolume is now -> 68\nvolume is now -> 69\nvolume is now -> 70\nvolume is now -> 71\nvolume is now -> 72\nvolume is now -> 73\nvolume is now -> 74\nvolume is now -> 75\nvolume is now -> 76\nvolume is now -> 77\nvolume is now -> 78\nvolume is now -> 79\nvolume is now -> 80\nvolume is now -> 81\nvolume is now -> 82\nvolume is now -> 83\nvolume is now -> 84\nvolume is now -> 85\nvolume is now -> 86\nvolume is now -> 87\nvolume is now -> 88\nvolume is now -> 89\nvolume is now -> 90\nvolume is now -> 91\nvolume is now -> 92\nvolume is now -> 93\nvolume is now -> 94\nvolume is now -> 95\nvolume is now -> 96\nvolume is now -> 97\nvolume is now -> 98\nvolume is now -> 99\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nvolume is now -> 100\nDecrease volume 110 times.\nvolume is now -> 99\nvolume is now -> 98\nvolume is now -> 97\nvolume is now -> 96\nvolume is now -> 95\nvolume is now -> 94\nvolume is now -> 93\nvolume is now -> 92\nvolume is now -> 91\nvolume is now -> 90\nvolume is now -> 89\nvolume is now -> 88\nvolume is now -> 87\nvolume is now -> 86\nvolume is now -> 85\nvolume is now -> 84\nvolume is now -> 83\nvolume is now -> 82\nvolume is now -> 81\nvolume is now -> 80\nvolume is now -> 79\nvolume is now -> 78\nvolume is now -> 77\nvolume is now -> 76\nvolume is now -> 75\nvolume is now -> 74\nvolume is now -> 73\nvolume is now -> 72\nvolume is now -> 71\nvolume is now -> 70\nvolume is now -> 69\nvolume is now -> 68\nvolume is now -> 67\nvolume is now -> 66\nvolume is now -> 65\nvolume is now -> 64\nvolume is now -> 63\nvolume is now -> 62\nvolume is now -> 61\nvolume is now -> 60\nvolume is now -> 59\nvolume is now -> 58\nvolume is now -> 57\nvolume is now -> 56\nvolume is now -> 55\nvolume is now -> 54\nvolume is now -> 53\nvolume is now -> 52\nvolume is now -> 51\nvolume is now -> 50\nvolume is now -> 49\nvolume is now -> 48\nvolume is now -> 47\nvolume is now -> 46\nvolume is now -> 45\nvolume is now -> 44\nvolume is now -> 43\nvolume is now -> 42\nvolume is now -> 41\nvolume is now -> 40\nvolume is now -> 39\nvolume is now -> 38\nvolume is now -> 37\nvolume is now -> 36\nvolume is now -> 35\nvolume is now -> 34\nvolume is now -> 33\nvolume is now -> 32\nvolume is now -> 31\nvolume is now -> 30\nvolume is now -> 29\nvolume is now -> 28\nvolume is now -> 27\nvolume is now -> 26\nvolume is now -> 25\nvolume is now -> 24\nvolume is now -> 23\nvolume is now -> 22\nvolume is now -> 21\nvolume is now -> 20\nvolume is now -> 19\nvolume is now -> 18\nvolume is now -> 17\nvolume is now -> 16\nvolume is now -> 15\nvolume is now -> 14\nvolume is now -> 13\nvolume is now -> 12\nvolume is now -> 11\nvolume is now -> 10\nvolume is now -> 9\nvolume is now -> 8\nvolume is now -> 7\nvolume is now -> 6\nvolume is now -> 5\nvolume is now -> 4\nvolume is now -> 3\nvolume is now -> 2\nvolume is now -> 1\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\nvolume is now -> 0\n\n### Test Next/Prev ###\n- Wrap around to last:\nno song in playlist!\n- Wrap around to first:\nno song in playlist!\n- Switch through all entries:\nno song in playlist!\nno song in playlist!\nno song in playlist!\nno song in playlist!\nno song in playlist!\n\n### Test Select ###\n-- Search for \"Does not exist\":\nsong: Does not exist not found!\nno song in playlist!\nno song in playlist!\n\n-- Search for \"Media 1\":\nsong: Media 1 not found!\nno song in playlist!\nno song in playlist!\n\n-- Search for \"Media 2\":\nsong: Media 2 not found!\nno song in playlist!\nno song in playlist!\n\n-- Search for \"Media 2\" - Should find the same one again:\nsong: Media 2 not found!\nno song in playlist!\nno song in playlist!\n");

	client.SetPlayer(std::make_unique<VideoPlayerAdapter>(std::move(std::make_unique<VideoPlayer>())));

	CaptureStdout();
	client.Run();
	output = GetCapturedStdout();
	cout << output;
	EXPECT_EQ(output, "### Test Stop/Play ###\nno video in playlist!\nno video in playlist!\nIncrease volume 110 times.\nvolume is now -> 9\nvolume is now -> 10\nvolume is now -> 11\nvolume is now -> 12\nvolume is now -> 13\nvolume is now -> 14\nvolume is now -> 15\nvolume is now -> 16\nvolume is now -> 17\nvolume is now -> 18\nvolume is now -> 19\nvolume is now -> 20\nvolume is now -> 21\nvolume is now -> 22\nvolume is now -> 23\nvolume is now -> 24\nvolume is now -> 25\nvolume is now -> 26\nvolume is now -> 27\nvolume is now -> 28\nvolume is now -> 29\nvolume is now -> 30\nvolume is now -> 31\nvolume is now -> 32\nvolume is now -> 33\nvolume is now -> 34\nvolume is now -> 35\nvolume is now -> 36\nvolume is now -> 37\nvolume is now -> 38\nvolume is now -> 39\nvolume is now -> 40\nvolume is now -> 41\nvolume is now -> 42\nvolume is now -> 43\nvolume is now -> 44\nvolume is now -> 45\nvolume is now -> 46\nvolume is now -> 47\nvolume is now -> 48\nvolume is now -> 49\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nvolume is now -> 50\nDecrease volume 110 times.\nvolume is now -> 49\nvolume is now -> 48\nvolume is now -> 47\nvolume is now -> 46\nvolume is now -> 45\nvolume is now -> 44\nvolume is now -> 43\nvolume is now -> 42\nvolume is now -> 41\nvolume is now -> 40\nvolume is now -> 39\nvolume is now -> 38\nvolume is now -> 37\nvolume is now -> 36\nvolume is now -> 35\nvolume is now -> 34\nvolume is now -> 33\nvolume is now -> 32\nvolume is now -> 31\nvolume is now -> 30\nvolume is now -> 29\nvolume is now -> 28\nvolume is now -> 27\nvolume is now -> 26\nvolume is now -> 25\nvolume is now -> 24\nvolume is now -> 23\nvolume is now -> 22\nvolume is now -> 21\nvolume is now -> 20\nvolume is now -> 19\nvolume is now -> 18\nvolume is now -> 17\nvolume is now -> 16\nvolume is now -> 15\nvolume is now -> 14\nvolume is now -> 13\nvolume is now -> 12\nvolume is now -> 11\nvolume is now -> 10\nvolume is now -> 9\nvolume is now -> 8\nvolume is now -> 7\nvolume is now -> 6\nvolume is now -> 5\nvolume is now -> 4\nvolume is now -> 3\nvolume is now -> 2\nvolume is now -> 1\nvolume is now -> 0\n\n### Test Next/Prev ###\n- Wrap around to last:\nno video in playlist!\n- Wrap around to first:\nno video in playlist!\n- Switch through all entries:\nno video in playlist!\nno video in playlist!\nno video in playlist!\nno video in playlist!\nno video in playlist!\n\n### Test Select ###\n-- Search for \"Does not exist\":\nvideo: Does not exist not found!\nno video in playlist!\nno video in playlist!\n\n-- Search for \"Media 1\":\nvideo: Media 1 not found!\nno video in playlist!\nno video in playlist!\n\n-- Search for \"Media 2\":\nvideo: Media 2 not found!\nno video in playlist!\nno video in playlist!\n\n-- Search for \"Media 2\" - Should find the same one again:\nvideo: Media 2 not found!\nno video in playlist!\nno video in playlist!\n");
}