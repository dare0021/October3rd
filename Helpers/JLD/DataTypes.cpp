#include "DataTypes.h"

JLDDoc::JLDDoc() : JLDData(),
data(std::unordered_map<std::string, JLDData*>())
{
}

JLDDoc::JLDDoc(std::unordered_map<std::string, JLDData*> data) : JLDData(),
data(std::unordered_map<std::string, JLDData*>(data))
{
}

JLDData* JLDDoc::getValue(std::string key)
{
	return data.find(key);
}

/**
 * Evaluates the given string to extract a map value
 * e.g. eval(map, "e.g.2") is equal to map.get("e").getValue("g").getValue(2)
 * 			if the nested object was a list, and 
 *			map.get("e").getValue("g").getValue("2") otherwise
 */
JLDData* JLDDoc::getAfterParse(std::string cmd)
{
	JLDData* out = this;
	std::string next;
	while ((next = getNext(&cmd)) != "")
	{
		switch (out->getType())
		{
			case JLDDataType::Doc:
				out = ((JLDDoc*)out)->getValue(next);
				break;
			case JLDDataType::List:
				out = ((JLDList*)out)->getValue(std::stoi(next));
				break;
			case JLDDataType::String:
				assert(getNext(&cmd) == "", "Extra arguments");
				break;
			case JLDDataType::None:
				assert(0, "Something went wrong");
				break;
			default:
				assert(0, "Not implemented yet");
				break;
		}
	}
	return out;
}

///gets the next keyword in a period separated command string.
///strips the input to after the keyword
///e.g. "key1.key2.key3" -> key1 returned, input changed to key2.key3
///Q: What if the keyword needs to contain the period?
///A: Why would you do that
std::string JLDDoc::getNext(std::string* command)
{
	assert((*command)[0] != '.', "Extra period");
	std::string out = "";
	int i = 0;
	while (i < command->size() && (*command)[i] != '.')
	{
		out += (*command)[i];
		i++;
	}
	command = command->substr(i+1);
	assert((*command)[0] != '.', "Extra period");
	return out;
}

JLDList::JLDList() : JLDData(),
data(std::list<JLDData*>())
{
}

JLDList::JLDList(std::list<JLDData*> data) : JLDData(),
data(std::list<JLDData*>(data))
{
}

JLDData* JLDList::getValue(int index)
{
	return data[index];
}

JLDString::JLDString() : JLDData(),
data("")
{
}

JLDString::JLDString(std::string data) : JLDData(),
data(data)
{
}

std::string JLDString::getValue()
{
	return data;
}