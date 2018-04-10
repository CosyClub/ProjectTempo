#include <ostream_operators.hpp>
#include <catch.hpp>

#include <client/misc/RGBtoHSV.hpp>

#include <random>

TEST_CASE("RGBftoHSV", "[client][utilities][rgbhsv]"){
	// Reference answers obtained with: http://www.javascripter.net/faq/rgb2hsv.htm
	// Cross referenced with color picker in gimp

	CHECK(client::RGBftoHSV(irr::video::SColorf(1.0, 0.0, 0.0)) ==
	      irr::core::vector3df(0.0, 1.0, 1.0));

	CHECK(client::RGBftoHSV(irr::video::SColorf(0.0, 1.0, 0.0)) ==
	      irr::core::vector3df(120.0, 1.0, 1.0));

	CHECK(client::RGBftoHSV(irr::video::SColorf(0.0, 0.0, 1.0)) ==
	      irr::core::vector3df(240.0, 1.0, 1.0));

	CHECK(client::RGBftoHSV(irr::video::SColorf(0.0, 0.0, 0.0)) ==
	      irr::core::vector3df(0.0, 0.0, 0.0));

	CHECK(client::RGBftoHSV(irr::video::SColorf(0.5, 0.5, 0.5)) ==
	      irr::core::vector3df(0.0, 0.0, 0.5));

	CHECK(client::RGBftoHSV(irr::video::SColorf(1.0, 1.0, 1.0)) ==
	      irr::core::vector3df(0.0, 0.0, 1.0));
}

TEST_CASE("HSVtoRGB", "[client][utilities][rgbhsv]"){
	// Reference answers obtained with color picker in gimp

	CHECK(client::HSVtoRGB(irr::core::vector3df(  0.0, 0.0, 0.0)) ==
	      irr::video::SColor(255, 0,0,0));

	CHECK(client::HSVtoRGB(irr::core::vector3df(100.0, 0.0, 0.0)) ==
	      irr::video::SColor(255, 0,0,0));

	CHECK(client::HSVtoRGB(irr::core::vector3df(100.0, 0.5, 0.0)) ==
	      irr::video::SColor(255, 0,0,0));

	CHECK(client::HSVtoRGB(irr::core::vector3df(100.0, 0.5, 0.3)) ==
	      irr::video::SColor(255, 51,77,38));

	CHECK(client::HSVtoRGB(irr::core::vector3df(200.0, 0.7, 0.2)) ==
	      irr::video::SColor(255, 15,39,51));

	CHECK(client::HSVtoRGB(irr::core::vector3df(360.0, 0.4, 0.9)) ==
	      irr::video::SColor(255, 230,138,138));
}

TEST_CASE("Round trip from RGB to HSV to RGB is identity", "[client][utilities][rgbhsv]"){
	srand(10); // make tests deterministic
	for(int i = 0; i < 500; ++i){

		irr::video::SColor pre(255,
		                       ((float)rand() / (float)RAND_MAX) * 255,
		                       ((float)rand() / (float)RAND_MAX) * 255,
		                       ((float)rand() / (float)RAND_MAX) * 255
		                       );

		irr::video::SColor post = client::HSVtoRGB(client::RGBtoHSV(pre));

		CHECK(pre == post);
	}
}

TEST_CASE("Round trip from HSV to RGB to HSV is identity", "[client][utilities][rgbhsv]"){
	srand(42); // make tests deterministic
	for(int i = 0; i < 500; ++i){

		irr::core::vector3df pre(((float)rand() / (float)RAND_MAX) * 360.0,
		                         ((float)rand() / (float)RAND_MAX),
		                         ((float)rand() / (float)RAND_MAX)
		                        );

		irr::core::vector3df post = client::RGBftoHSV(client::HSVtoRGBf(pre));

		CHECK(pre.X == Approx(post.X).margin(0.001));
		CHECK(pre.Y == Approx(post.Y).margin(0.001));
		CHECK(pre.Z == Approx(post.Z).margin(0.001));
	}
}
