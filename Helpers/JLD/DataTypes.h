#ifndef __JLD_DATATYPES_H__
#define __JLD_DATATYPES_H__

#include <unordered_map>
#include <list>
#include <string>

enum class JLDDataType
{
    None,
    Doc,
    List,
    String
};

class JLDData
{
public:
	JLDData(){}
	JLDDataType getType(){return JLDDataType::None;}
};

class JLDDoc : JLDData
{
public:
    JLDDoc();
    JLDDoc(std::unordered_map<std::string, JLDData*>);
    JLDDataType getType(){return JLDDataType::Doc;}
    JLDData* getValue(std::string key);
    JLDData* getAfterParse(std::string command);

private:
    std::string getNext(std::string* command);
    std::unordered_map<std::string, JLDData*> data;
};

class JLDList : JLDData
{
public:
    JLDList();
    JLDList(std::list<JLDData*>);
    JLDDataType getType(){return JLDDataType::List;}
    JLDData* getValue(int index);

private:
    std::list<JLDData*> data;
};

class JLDString : JLDData
{
public:
    JLDString();
    JLDSTring(std::string*);
    JLDDataType getType(){return JLDDataType::String;}
    std::string getValue();

private:
    std::string data;
};

#endif // __JLD_DATATYPES_H__
