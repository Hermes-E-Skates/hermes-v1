// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef BLUETOOTHRESPONSE_H_
#define BLUETOOTHRESPONSE_H_

#include <vector>
#include "BluetoothCommand.h"
#include "../core/CommonDefs.h"


namespace eskates {
namespace bt {

class BluetoothResponse
{
public:
    BluetoothResponse(CmdId_t id, bool ack);
	
    virtual void encodeData(std::vector<uint8_t>& bytes) = 0;
    uint8_t getSize(void) const;
	
    const uint8_t* encode(void);
    CmdId_t getCmdId(void) const;
    
private:
    CmdId_t mCmdId;
    bool mAck;
    std::vector<uint8_t> mEncodedResponse;
};

}
}

#endif // BLUETOOTHRESPONSE_H_