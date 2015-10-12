#include "JLDIO.h"

JLDIO::JLDIO(std::string rawText) :
macros(std::unordered_map<std::string, std::string>())
{
}

JLDDoc* JLDIO::parseText(std::string rawText)
{
	rawText = findMacros(stripComments(rawText));
}

std::string JLDIO::stripComments(std::string raw)
{
	std::string out = "";
	bool wasSlash = false;
	bool lineComment = false;
	bool blockComment = false;
	bool foundSecondStar = false;
	for (char c : raw)
	{
		if(lineComment)
		{
			if(c == '\n')
			{
				lineComment = false;
				out += '\n';
			}
			continue;
		}
		if(blockComment)
		{
			if(foundSecondStar && c == '/')
				blockComment = false;
			else if(c == '*')
				foundSecondStar = true;
			else
				foundSecondStar = false;
			continue;
		}
		if(!wasSlash && c == '/')
		{
			wasSlash = true;
			continue;
		}
		if(wasSlash)
		{
			wasSlash = false;
			if(c == '/')
			{
				lineComment = true;
				continue;
			}
			else if(c == '*')
			{
				blockComment = true;
				continue;
			}
			else
			{
				out += '/';
			}
		}
		out += c;
	}
}

std::string JLDIO::findMacros(std::string raw)
{
	std::unordered_map<std::string, std::string> macros;
	std::string line = "";
	std::string out = "";
	int cutAfter = 0;
	int thisLine = 0;
	for (char c : raw)
	{
		if(c != '\n')
			line += c;
		else
		{
			thisLine = line.size() + 1;
			if(line.substr(0, 8) == "#define ")
			{
				int i = 9;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				line = line.substr(i-1);
				i = 0;
				while (line[i] != ' ' && line[i] != '\t')
					i++;
				macros.insert({line.substr(0, i), line.substr(i)});
			}
			else
				break;
			cutAfter += thisLine;
			line = "";
		}
	}
	out = raw.substr(cutAfter);
	for (auto kvp : macros)
		out = findAndReplace(out, kvp->first, kvp->second);
	return out;
}

std::string JLDIO::findAndReplace(std::string raw, std::string from, std::string to)
{
	size_t loc;
	while((loc=raw.find(from)) != std::string::npos)
		raw.replace(loc, from.size(), to);
}