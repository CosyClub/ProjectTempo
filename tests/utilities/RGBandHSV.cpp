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
