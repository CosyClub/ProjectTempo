#include <ostream_operators.hpp>
#include <catch.hpp>

#include <tempo/mask.hpp>

TEST_CASE("2x2 mask at origin", "[utilities][mask]"){
	float values[] = { 1, 2, 3, 4 };
	tempo::Mask mask(glm::ivec2(0, 0), values, glm::ivec2(2, 2));

	REQUIRE(mask.ctr == glm::ivec2(0,0));
	REQUIRE(mask.sz  == glm::ivec2(2,2));

	SECTION("Get Values in Bounds"){
		CHECK(mask.get_value(0, 0) == 1);
		CHECK(mask.get_value(1, 0) == 2);
		CHECK(mask.get_value(0, 1) == 3);
		CHECK(mask.get_value(1, 1) == 4);
	}

	SECTION("Get Values out of Bounds"){
		CHECK(mask.get_value(0, 2) == 0);
		CHECK(mask.get_value(2, 0) == 0);
		CHECK(mask.get_value(1, 2) == 0);
		CHECK(mask.get_value(2, 1) == 0);
		CHECK(mask.get_value(5, 9) == 0);
	}
}

TEST_CASE("4x2 mask at origin", "[utilities][mask]"){
	float values[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	tempo::Mask mask(glm::ivec2(0, 0), values, glm::ivec2(4, 2));

	REQUIRE(mask.ctr == glm::ivec2(0,0));
	REQUIRE(mask.sz  == glm::ivec2(4,2));

	SECTION("Get Values in Bounds"){
		CHECK(mask.get_value(0, 0) == 1);
		CHECK(mask.get_value(1, 0) == 2);
		CHECK(mask.get_value(2, 0) == 3);
		CHECK(mask.get_value(3, 0) == 4);
		CHECK(mask.get_value(0, 1) == 5);
		CHECK(mask.get_value(1, 1) == 6);
		CHECK(mask.get_value(2, 1) == 7);
		CHECK(mask.get_value(3, 1) == 8);
	}

	SECTION("Get Values out of Bound"){
		CHECK(mask.get_value(-1,  0) == 0);
		CHECK(mask.get_value( 0, -1) == 0);
		CHECK(mask.get_value( 4,  0) == 0);
		CHECK(mask.get_value( 0,  2) == 0);
		CHECK(mask.get_value( 9,  9) == 0);
	}
}

TEST_CASE("3x3 mask at (10,20)", "[utilities][mask]"){
	float values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	tempo::Mask mask(glm::ivec2(10, 20), values, glm::ivec2(3, 3));

	REQUIRE(mask.ctr == glm::ivec2(10,20));
	REQUIRE(mask.sz  == glm::ivec2( 3, 3));

	/*
	  :TODO: I have no idea what ctr of mask is meant to be doing...
	  It certainty doesn't seem to define the center point of the mask
	  No unit tests for this for now...

	SECTION("Get Values in Bounds"){
		CHECK(mask.get_value(10, 20) == 1);
		CHECK(mask.get_value(11, 20) == 2);
		CHECK(mask.get_value(12, 20) == 3);
		CHECK(mask.get_value(10, 21) == 4);
		CHECK(mask.get_value(11, 21) == 5);
		CHECK(mask.get_value(12, 21) == 6);
		CHECK(mask.get_value(10, 22) == 7);
		CHECK(mask.get_value(11, 22) == 8);
		CHECK(mask.get_value(12, 22) == 9);
	}
	*/
}
