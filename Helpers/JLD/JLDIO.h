#ifndef __JLD_IO_H__
#define __JLD_IO_H__

#include "DataTypes.h"

class JLDIO
{
public:
	JLDIO();
	JLDDoc* parseText(std::string rawText);

private:
	std::string stripComments(std::string);
	std::string findMacros(std::string);
	std::string findAndReplace(std::string text, std::string from, std::string to);
};

#endif // __JLD_IO_H__
