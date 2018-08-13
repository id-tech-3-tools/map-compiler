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

#include "table_builder.hpp"
#include <algorithm>
#include <numeric>
#include <sstream>
#include <array>
#include <iostream>
#include <iomanip>
#include <ios>

template<typename Ty>
static Ty clamp(Ty v, Ty min, Ty max)
{
	return v > max ? max : v < min ? min : v;
}

TableBuilder::Row::Row(const std::string& col)
{
	columns.push_back({col, maxColsPerRow});
}

TableBuilder::Row::Row(const std::initializer_list<std::string>& cols)
{
	for (auto& col : cols)
	{
		columns.push_back({col, 1});
	}
}

TableBuilder::Row::Row(const char* col)
{
	columns.push_back({col, maxColsPerRow});
}

TableBuilder::Row::Row(const std::initializer_list<char*>& cols)
{
	for (auto& col : cols)
	{
		columns.push_back({col, 1});
	}
}

void TableBuilder::Row::push(const Column& col)
{
	columns.push_back(col);
}

void TableBuilder::Row::push(const std::string& col, int span)
{
	columns.push_back({col, span});
}

TableBuilder::TableBuilder(int cols, int maxWidth)
{
	this->colsNum = clamp(cols, 1, maxColsPerRow);
	this->rowWidth = clamp(maxWidth, 1, maxRowWidth);
	distributeColsEqually();
}

TableBuilder::TableBuilder(const std::initializer_list<int>& cols)
{
	this->colsNum = clamp(static_cast<int>(cols.size()), 1, maxColsPerRow);
	this->rowWidth = clamp(std::accumulate(cols.begin(), cols.end(), 0), 1, maxRowWidth);
	distributeCols(cols);
}

TableBuilder::~TableBuilder() {}

void TableBuilder::addRow()
{
	rows.push_back(Row());
}

void TableBuilder::addRow(const Row &newRow)
{
	addRow();
	Row &row = rows.back();
	auto colsInRow = clamp(static_cast<int>(newRow.columns.size()), 0, colsNum);
	for (int i = 0; i < colsInRow; i++)
	{
		auto &newCol = newRow.columns[i];
		row.push(newCol.data, clamp(newCol.span, 1, colsNum - i));
	}
}

void TableBuilder::addRows(const std::initializer_list<Row> &newRows)
{
	for (auto &newRow : newRows)
	{
		addRow(newRow);
	}
}

void TableBuilder::addEmptyRow()
{
	addRow(" ");
}

void TableBuilder::addColumn(const std::string& data, int span)
{
	if (rows.size() > 0)
	{
		auto &row = rows.back();
		auto colsInRow = static_cast<int>(row.columns.size());
		if (colsInRow < colsNum)
		{
			row.push(data, clamp(span, 1, colsNum - colsInRow));
		}
	}
}

void TableBuilder::addColumns(const std::initializer_list<std::string>& cols)
{
	for (auto &col : cols)
	{
		addColumn(col);
	}
}

void TableBuilder::setColumnSize(int col, int size)
{
	if (col > 0 && col <= maxColsPerRow)
	{
		colSizes[col - 1].size = size;
		colSizes[col - 1].fixed = true;
		redistributeCols();
	}
}

std::vector<std::string> TableBuilder::build()
{
	std::vector<std::string> rowsOfStr;
	std::array<std::string, maxColsPerRow> buffer;

	for (int r = 0; r < rows.size(); r++) {
		auto &row = rows[r];
		bool wasSplit = false;

		while (true) {
			std::stringstream rowbuf;
			rowbuf << std::left << std::setfill(' ');
			bool useBuffer = wasSplit;
			wasSplit = false;

			for (int c = 0; c < row.columns.size(); c++) {
				auto &col = row.columns[c];
				auto *data = useBuffer ? &buffer[c] : &col.data;
				int colWidth = calculateColSpanWidth(c, col.span);

				rowbuf << std::setw(colWidth);

				// line should be split
				if (data->size() > colWidth) {
					wasSplit = true;
					rowbuf << splitText(*data, colWidth, buffer[c]);
				}
				else {
					rowbuf << *data;
					buffer[c] = "";
				}
			}

			rowsOfStr.push_back(rowbuf.str());

			if (!wasSplit) {
				break;
			}
		}
	}

	return rowsOfStr;
}

void TableBuilder::distributeColsEqually()
{
	for (auto &colSize : colSizes)
	{
		colSize.size = rowWidth / colsNum;
		colSize.fixed = false;
	}
}

void TableBuilder::distributeCols(const std::vector<int>& cols)
{
	auto weights = calculateWeights(cols);
	for (int i = 0; i < colsNum; i++)
	{
		colSizes[i].size = static_cast<int>(weights[i] * rowWidth);
		colSizes[i].fixed = true;
	}
}

void TableBuilder::distributeColsAutomatically()
{
	auto weights = calculateColumnWeights();
	for (int i = 0; i < colsNum; i++)
	{
		colSizes[i].size = clamp(static_cast<int>(weights[i] * rowWidth), 10, rowWidth);
		colSizes[i].fixed = false;
	}
}

TableBuilder::Weights TableBuilder::calculateColumnWeights()
{
	std::vector<int> colLenghts(colsNum);
	for (auto &row : rows)
	{
		for (int c = 0; c < row.columns.size(); c++)
		{
			colLenghts[c] += static_cast<int>(row.columns[c].data.size());
		}
	}

	return calculateWeights(colLenghts);
}

TableBuilder::Weights TableBuilder::calculateWeights(const std::vector<int>& list)
{
	Weights weights;
	float totalLength = static_cast<float>(std::accumulate(list.begin(), list.end(), 0));
	std::transform(list.begin(), list.end(), weights.begin(), [totalLength](int len) {
		return static_cast<float>(len) / totalLength;
	});

	return weights;
}

void TableBuilder::redistributeCols()
{
	int nonFixed = 0;
	int totalWidth = std::accumulate(colSizes.begin(), colSizes.begin() + colsNum, 0, [&](int a, const ColSize& colSize) {
		nonFixed += colSize.fixed ? 0 : 1;
		return a + colSize.size;
	});
	int freeWidth = rowWidth - totalWidth;
	int step = freeWidth / nonFixed;
	std::for_each(colSizes.begin(), colSizes.begin() + colsNum, [&](ColSize &colSize) {
		if (!colSize.fixed)
		{
			colSize.size += step;
		}
	});
}

std::string TableBuilder::splitText(const std::string& text, int maxWidth, std::string& buffer)
{
	std::istringstream is{ text };
	std::string word, fragment;
	int feed = 0;

	while (is >> word) {
		feed += static_cast<int>(word.size()) + 1;
		if (feed > maxWidth) {
			buffer = word;
			int pos = static_cast<int>(is.tellg());
			if (pos >= 0)
			{
				buffer += is.str().substr(pos);
			}
			break;
		}
		fragment += word + " ";
	}

	return fragment;
}

int TableBuilder::calculateColSpanWidth(int colIndex, int span)
{
	int colSize = 0, spanEnd = colIndex + span;
	for (int i = colIndex; i < spanEnd; i++)
	{
		colSize += colSizes[i].size;
	}

	return colSize;
}
