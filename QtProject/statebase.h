#ifndef STATEBASE_H
#define STATEBASE_H
#include <QString>
#include <Data.h>

class StateBase
{
public:
    StateBase();

    virtual void create();
    virtual void edit(QString key);
    virtual void build();
    virtual void del(QString key);
    virtual void save();

private:
//    std::string key;
};

#endif // STATEBASE_H
