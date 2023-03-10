// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef GETID_H_
#define GETID_H_


#include "../core/CommonDefs.h"
#include "BluetoothCommand.h"
#include "BluetoothResponse.h"


namespace hermes {
namespace bt {

class GetIdCmd : public BluetoothCommand
{
public:
    GetIdCmd(void);

private:
    virtual bool decode(const uint8_t* const bytes, uint8_t len) override;
};


class GetIdResp : public BluetoothResponse
{
public:
    GetIdResp(bool ack);
    void setIdentifier(Identifier_t partno);

private:
    virtual void encodeData(std::vector<uint8_t>& bytes) override;
    Identifier_t mIdentifier;
};

}
}

#endif // GETINFO_H_