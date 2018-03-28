#include "catch.hpp"
#include "table_builder.hpp"
#include <sstream>
#include <iomanip>
#include <ios>
#include <string>

TEST_CASE("Should create table out of data") {
	const int width = 30;
	const int cols = 2;
	const int cw = width / cols;

	auto table = new TableBuilder(2, width);
	table->addRow();
	table->addColumn("Lorem ipsum");
	table->addColumn("Quam");
	table->addRow();
	table->addColumn("Consectetur adipisicing");
	table->addColumn("Deserunt");
	table->addRow();
	table->addColumn("Eaque");
	table->addColumn("Minima");
	auto rows = table->build();

	REQUIRE(rows.size() == 4);
	
	std::stringstream ss1;
	ss1 << std::left << std::setfill(' ');
	ss1 << std::setw(cw) << "Lorem ipsum" << std::setw(cw) << "Quam";
	REQUIRE(ss1.str() == rows[0]);

	std::stringstream ss2;
	ss2 << std::left << std::setfill(' ');
	ss2 << std::setw(cw) << "Consectetur" << std::setw(cw) << "Deserunt";
	REQUIRE(ss2.str() == rows[1]);

	std::stringstream ss3;
	ss3 << std::left << std::setfill(' ');
	ss3 << std::setw(cw) << "adipisicing" << std::setw(cw) << " ";
	REQUIRE(ss3.str() == rows[2]);
	
	std::stringstream ss4;
	ss4 << std::left << std::setfill(' ');
	ss4 << std::setw(cw) << "Eaque" << std::setw(cw) << "Minima";
	REQUIRE(ss4.str() == rows[3]);
};

TEST_CASE("Should break long text on multiple lines for each column") {
	const int width = 45;
	const int cols = 3;
	const int cw = width / cols;

	auto table = new TableBuilder(cols, width);
	table->addRow();
	table->addColumn("Lorem ipsum dolor sit amet, consectetur adipisicing elit.");
	table->addColumn("Deserunt, suscipit.");
	table->addColumn("Quam pariatur quo accusantium quisquam eaque minima.");
	auto rows = table->build();

	REQUIRE(rows.size() == 6);

	std::stringstream ss1;
	ss1 << std::left << std::setfill(' ');
	ss1 << std::setw(cw) << "Lorem ipsum" << std::setw(cw) << "Deserunt," << std::setw(cw) << "Quam pariatur";
	REQUIRE(ss1.str() == rows[0]);

	std::stringstream ss2;
	ss2 << std::left << std::setfill(' ');
	ss2 << std::setw(cw) << "dolor sit" << std::setw(cw) << "suscipit." << std::setw(cw) << "quo";
	REQUIRE(ss2.str() == rows[1]);

	std::stringstream ss3;
	ss3 << std::left << std::setfill(' ');
	ss3 << std::setw(cw) << "amet," << std::setw(cw) << " " << std::setw(cw) << "accusantium";
	REQUIRE(ss3.str() == rows[2]);

	std::stringstream ss4;
	ss4 << std::left << std::setfill(' ');
	ss4 << std::setw(cw) << "consectetur" << std::setw(cw) << " " << std::setw(cw) << "quisquam eaque";
	REQUIRE(ss4.str() == rows[3]);

	std::stringstream ss5;
	ss5 << std::left << std::setfill(' ');
	ss5 << std::setw(cw) << "adipisicing" << std::setw(cw) << " " << std::setw(cw) << "minima.";
	REQUIRE(ss5.str() == rows[4]);

	std::stringstream ss6;
	ss6 << std::left << std::setfill(' ');
	ss6 << std::setw(cw) << "elit." << std::setw(cw) << " " << std::setw(cw) << " ";
	REQUIRE(ss6.str() == rows[5]);

	delete table;
};

TEST_CASE("Should resize column and redistribute width")
{
	const int width = 50;
	const int cols = 2;
	const int colA = 15;
	const int colB = width - colA;

	auto table = new TableBuilder(cols, width);
	table->setColumnSize(1, colA);
	table->addRow();
	table->addColumn("Lorem ipsum dolor sit amet, consectetur adipisicing elit.");
	table->addColumn("Deserunt, suscipit.");
	auto rows = table->build();

	std::stringstream ss1;
	ss1 << std::left << std::setfill(' ');
	ss1 << std::setw(colA) << "Lorem ipsum" << std::setw(colB) << "Deserunt, suscipit.";
	REQUIRE(ss1.str() == rows[0]);
	
	delete table;
}

TEST_CASE("Should initialize table with fixed columns")
{
	const int colA = 20;
	const int colB = 30;

	auto table = new TableBuilder {colA, colB};
	table->addRow();
	table->addColumn("Lorem ipsum dolor sit amet, consectetur adipisicing elit.");
	table->addColumn("Deserunt, suscipit.");
	auto rows = table->build();

	std::stringstream ss1;
	ss1 << std::left << std::setfill(' ');
	ss1 << std::setw(colA) << "Lorem ipsum dolor" << std::setw(colB) << "Deserunt, suscipit.";
	REQUIRE(ss1.str() == rows[0]);

	delete table;
}


TEST_CASE("Should allow to span across multiple columns")
{
	const int colA = 20;
	const int colB = 30;
	const int width = colA + colB;

	auto table = new TableBuilder{ colA, colB };
	table->addRow();
	table->addColumn("Lorem ipsum dolor sit amet, consectetur adipisicing elit.", 2);
	auto rows = table->build();

	std::stringstream ss1;
	ss1 << std::left << std::setfill(' ');
	ss1 << std::setw(width) << "Lorem ipsum dolor sit amet, consectetur";
	REQUIRE(ss1.str() == rows[0]);

	std::stringstream ss2;
	ss2 << std::left << std::setfill(' ');
	ss2 << std::setw(width) << "adipisicing elit.";
	REQUIRE(ss2.str() == rows[1]);

	delete table;
}

TEST_CASE("Should allow to set empty rows")
{
	const int colA = 20;
	const int colB = 30;
	const int width = 50;

	auto table = new TableBuilder{ colA, colB };
	table->addRow();
	table->addColumn("Lorem ipsum dolor sit amet, consectetur.", 2);
	table->addEmptyRow();
	auto rows = table->build();
	
	std::stringstream ss1;
	ss1 << std::left << std::setfill(' ');
	ss1 << std::setw(width) << " ";
	REQUIRE(ss1.str() == rows[1]);

	delete table;
}

TEST_CASE("Should allow adding multiple columns using addColumns")
{
	const int colA = 20;
	const int colB = 30;

	auto table = new TableBuilder{ colA, colB };
	table->addRow();
	table->addColumns({ "Deserunt, suscipit." , "Quam pariatur quo." });
	auto rows = table->build();

	std::stringstream ss1;
	ss1 << std::left << std::setfill(' ');
	ss1 << std::setw(colA) << "Deserunt, suscipit." << std::setw(colB) << "Quam pariatur quo.";
	REQUIRE(ss1.str() == rows[0]);

	delete table;
}

TEST_CASE("Should allow adding single column that spans across row using addRow")
{
	const int colA = 20;
	const int colB = 30;
	const int width = colA + colB;

	auto table = new TableBuilder{ colA, colB };

	table->addRow("Deserunt, suscipit. Quam pariatur quo.");

	auto rows = table->build();

	std::stringstream ss1;
	ss1 << std::left << std::setfill(' ');
	ss1 << std::setw(width) << "Deserunt, suscipit. Quam pariatur quo.";
	REQUIRE(ss1.str() == rows[0]);

	delete table;
}

TEST_CASE("Should allow adding multiple columns using addRow")
{
	const int colA = 20;
	const int colB = 30;

	auto table = new TableBuilder{ colA, colB };
	table->addRow({ "Deserunt, suscipit." , "Quam pariatur quo." });
	auto rows = table->build();

	std::stringstream ss1;
	ss1 << std::left << std::setfill(' ');
	ss1 << std::setw(colA) << "Deserunt, suscipit." << std::setw(colB) << "Quam pariatur quo.";
	REQUIRE(ss1.str() == rows[0]);

	delete table;
}

TEST_CASE("should allow adding multiple column rows using addRows")
{
	const int colA = 20;
	const int colB = 30;

	auto table = new TableBuilder{ colA, colB };
	table->addRows({ { "Deserunt, suscipit." , "Quam pariatur quo." } });
	auto rows = table->build();

	std::stringstream ss1;
	ss1 << std::left << std::setfill(' ');
	ss1 << std::setw(colA) << "Deserunt, suscipit." << std::setw(colB) << "Quam pariatur quo.";
	REQUIRE(ss1.str() == rows[0]);

	delete table;
}

TEST_CASE("Should allow adding single column rows using addRows")
{
	const int colA = 20;
	const int colB = 30;
	const int width = colA + colB;

	auto table = new TableBuilder{ colA, colB };
	table->addRows({
		"Deserunt, suscipit. Quam pariatur quo.",
		"Deserunt, suscipit.",
		"",
		"Quam pariatur quo."
	});
	auto rows = table->build();

	std::stringstream ss1;
	ss1 << std::left << std::setfill(' ');
	ss1 << std::setw(width) << "Deserunt, suscipit. Quam pariatur quo.";
	REQUIRE(ss1.str() == rows[0]);

	std::stringstream ss2;
	ss2 << std::left << std::setfill(' ');
	ss2 << std::setw(width) << "Deserunt, suscipit.";
	REQUIRE(ss2.str() == rows[1]);

	std::stringstream ss3;
	ss3 << std::left << std::setfill(' ');
	ss3 << std::setw(width) << "";
	REQUIRE(ss3.str() == rows[2]);

	std::stringstream ss4;
	ss4 << std::left << std::setfill(' ');
	ss4 << std::setw(width) << "Quam pariatur quo.";
	REQUIRE(ss4.str() == rows[3]);

	delete table;
}

TEST_CASE("test should allow adding multiple type rows using addRows")
{
	const int colA = 20;
	const int colB = 30;
	const int width = colA + colB;

	auto table = new TableBuilder{ colA, colB };
	table->addRows({
		{"Deserunt, suscipit.", "Quam pariatur quo."},
		"Deserunt, suscipit.",
		"",
		"Quam pariatur quo."
	});
	auto rows = table->build();

	std::stringstream ss1;
	ss1 << std::left << std::setfill(' ');
	ss1 << std::setw(colA) << "Deserunt, suscipit." << std::setw(colB) << "Quam pariatur quo.";
	REQUIRE(ss1.str() == rows[0]);

	std::stringstream ss2;
	ss2 << std::left << std::setfill(' ');
	ss2 << std::setw(width) << "Deserunt, suscipit.";
	REQUIRE(ss2.str() == rows[1]);

	std::stringstream ss3;
	ss3 << std::left << std::setfill(' ');
	ss3 << std::setw(width) << "";
	REQUIRE(ss3.str() == rows[2]);

	std::stringstream ss4;
	ss4 << std::left << std::setfill(' ');
	ss4 << std::setw(width) << "Quam pariatur quo.";
	REQUIRE(ss4.str() == rows[3]);

	delete table;
}

TEST_CASE("test should allow adding strings in the row")
{
	const int colA = 20;
	const int colB = 30;
	const int width = colA + colB;

	auto table = new TableBuilder{ colA, colB };
	std::string val1{ "Deserunt, suscipit." };
	std::string val2{ "Quam pariatur quo." };

	table->addRow({val1, val2});
	table->addRow(val1.c_str());
	auto rows = table->build();

	std::stringstream ss1;
	ss1 << std::left << std::setfill(' ');
	ss1 << std::setw(colA) << "Deserunt, suscipit." << std::setw(colB) << "Quam pariatur quo.";
	REQUIRE(ss1.str() == rows[0]);

	std::stringstream ss2;
	ss2 << std::left << std::setfill(' ');
	ss2 << std::setw(width) << "Deserunt, suscipit.";
	REQUIRE(ss2.str() == rows[1]);

	delete table;
}