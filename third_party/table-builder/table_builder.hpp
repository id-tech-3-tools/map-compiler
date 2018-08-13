/*
MIT License

Copyright (c) 2018 isRyven

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once
#include <vector>
#include <array>
#include <string>
#include <initializer_list>
#include <ostream>

// TODO: 
// newline break support
// empty space support, for aligning

/*
	TableBuilder builds rows of formatted text out of set of rows and columns
	that wraps inner text.

	Usage: 
		// initialization
		TableBuilder table(numCols, maxWidth); // equal width distribution
		TableBuilder table({colAWidth, colBWidth, colCWidth[, ...]}); // fixed sizes
		// add row and columns I
		table.addRow();
		table.addColumn("name"[, span=1]);
		table.addColumn("age");
		table.addRow();
		table.addColumns({ "name", "age" });
		// add row and columns II
		table.addRow("Column that spans across whole row");
		// add row and columns III
		table.addRow({ "name", "age" });
		// add rows and columns IV
		table.addRows({
			"Column that spans across whole row",
			{ "name", "age" },
			"" // empty row
		});
		// resize column
		TableBuilder table(3, 100); // -> colA = 33, colB = 33, colC = 33
		table.setColumnSize(1, 40); // -> colA = 40, colB = 30, colC = 30 
		table.setColumnSize(2, 20); // -> colA = 40, colB = 20, colC = 40
		// add empty row
		table.addEmptyRow();
		// build table
		std::vector<std::string> rows = table.build(); // just a list of strings
		// build table II
		std::cout << table; // flush directly into the stream
		
*/

class TableBuilder
{
	static const int maxColsPerRow = 5;
	static const int maxRowWidth = 200;
	static const int maxRows = 999;

	int colsNum;
	int rowWidth;
public:

	struct Column
	{
		std::string data;
		int span;
	};
	struct Row
	{
		std::vector<Column> columns;
		explicit Row() {}
		Row(const std::string& col);
		Row(const std::initializer_list<std::string>& cols);
		Row(const char* col);
		Row(const std::initializer_list<char*>& cols);
		void push(const Column& col);
		void push(const std::string& col, int span = 1);
	};

	TableBuilder(int cols, int maxWidth = maxRowWidth);
	TableBuilder(const std::initializer_list<int>& cols);
	~TableBuilder();
	void addRow();
	void addRow(const Row &newRow);
	void addRows(const std::initializer_list<Row>& newRows);
	void addEmptyRow();
	void addColumn(const std::string &data, int span = 1);
	void addColumns(const std::initializer_list<std::string>& cols);
	void setColumnSize(int col, int size);
	std::vector<std::string> build();

private:
	using Weights = std::array<float, maxColsPerRow>;

	struct ColSize
	{
		int size;
		bool fixed; // if set, does not change automatically whilst redistribution
	};

	std::vector<Row> rows;
	std::array<ColSize, maxColsPerRow> colSizes;

	void distributeCols(const std::vector<int>& cols);
	void distributeColsEqually();
	void distributeColsAutomatically();
	void redistributeCols();
	Weights calculateColumnWeights();
	Weights calculateWeights(const std::vector<int>& list);
	std::string splitText(const std::string& text, int maxWidth, std::string& buffer);
	int calculateColSpanWidth(int colIndex, int span);
};

inline std::ostream& operator<<(std::ostream& os, TableBuilder& table) {
	auto rows = table.build();
	for (auto &row : rows)
	{
		os << row << std::endl;
	}
	return os;
}
