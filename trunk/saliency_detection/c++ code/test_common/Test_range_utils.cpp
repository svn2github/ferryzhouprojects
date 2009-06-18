#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/config/SourcePrefix.h>

#include <range_utils.h>

#include <vector>

using namespace std;

class Test_range_utils : public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE(Test_range_utils);
	CPPUNIT_TEST(test_getTotalRangeLength);
	CPPUNIT_TEST(test_getRangeOverlap);
	CPPUNIT_TEST(test_getRangeListOverlap);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void test_getTotalRangeLength();
	void test_getRangeOverlap();
	void test_getRangeListOverlap();

private:
	IndexRangeList ranges1;
	IndexRangeList ranges2;
	IndexRangeList ranges3;
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(Test_range_utils);

void Test_range_utils::setUp() {
	ranges1.addIndexRange(0, 10);
	ranges1.addIndexRange(20, 30);
	ranges1.addIndexRange(40, 50);

	ranges2.addIndexRange(0, 15);
	ranges2.addIndexRange(18, 30);
	ranges2.addIndexRange(42, 44);

	ranges3.addIndexRange(5, 60);
}

void Test_range_utils::tearDown() {
}

void Test_range_utils::test_getTotalRangeLength() {
	CPPUNIT_ASSERT_EQUAL(30, getTotalRangeLength(ranges1));
	CPPUNIT_ASSERT_EQUAL(29, getTotalRangeLength(ranges2));
}

void Test_range_utils::test_getRangeOverlap() {
	CPPUNIT_ASSERT_EQUAL(3, getRangeOverlap(IndexRange(0, 8), IndexRange(2, 5)));
	CPPUNIT_ASSERT_EQUAL(0, getRangeOverlap(IndexRange(0, 8), IndexRange(8, 10)));
	CPPUNIT_ASSERT_EQUAL(0, getRangeOverlap(IndexRange(0, 8), IndexRange(-10, 0)));
	CPPUNIT_ASSERT_EQUAL(7, getRangeOverlap(IndexRange(0, 8), IndexRange(1, 10)));
}

void Test_range_utils::test_getRangeListOverlap() {
	CPPUNIT_ASSERT_EQUAL(22, getRangeListOverlap(ranges1, ranges2));
	CPPUNIT_ASSERT_EQUAL(25, getRangeListOverlap(ranges1, ranges3));
	CPPUNIT_ASSERT_EQUAL(24, getRangeListOverlap(ranges2, ranges3));
	CPPUNIT_ASSERT_EQUAL(24, getRangeListOverlap(ranges3, ranges2));

}