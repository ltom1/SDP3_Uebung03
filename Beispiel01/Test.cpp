#include "gtest/gtest.h"

#include "Video.h"
#include "VideoPlayer.h"

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

TEST(VideoPlayer, Test) {
	VideoPlayer player;
	player.Play();
}

/////////////////////////////////////////////////