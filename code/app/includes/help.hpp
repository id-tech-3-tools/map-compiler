#pragma once
#include <vector>
#include <string>

class HelpPrinter
{
public:
	virtual ~HelpPrinter() {};
	void virtual print(const std::string &argument) = 0;
};


class CompilerHelpPrinter : public HelpPrinter
{
	const int optMaxWidth = 100;
	const int optIndentLevel = 2;
	const int optMaxNameWidth = 38;
	const int optMaxDescWidth = optMaxWidth - optMaxNameWidth - optIndentLevel;

	void printCommon() const;
	void printStages() const;
	void printBspCompile() const;
	void printVisCompile() const;
	void printLightCompile() const;
	void printBspAnalyze() const;
	void printBspScale() const;
	void printConverter() const;
	void printExportLightmaps() const;
	void printExportEnts() const;
	void printFixAas() const;
	void printBspInfo() const;
	void printImportLightmaps() const;
	void printMinimap() const;

public:
	CompilerHelpPrinter();
	~CompilerHelpPrinter();
	void print(const std::string &argument) override;
};