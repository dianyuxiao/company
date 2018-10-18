#ifndef __PASSWORD_H
#define __PASSWORD_H

#include "ht32.h"
#include "ht32_board_config.h"
#include "touchkey.h"
KeyType Show_Password(u8 X, u8 Y,u8 KeyValue, u8 LimtLen, u8 Plaintext);
u8 PwdKeyIsExists(KeyType InputPwdKey, u32 StrAddr, u16 CompNum);
u16 NewPwdKey_Record(u32 StrAddr, KeyType NewPwd, u16 ID);
KeyType KeyEnterPwdLoop(u8 X, u8 Y, u8 LimtLen, u8 Plaintext);
u8 CardNumberIsExists(u8* InputCard,u32 StrAddr,u16 CompNum);
u16 NewCardNumber_Record(u32 StrAddr, u8* NewCardNum,u16 ID);
u8 VirtualPassword(KeyType InputPwd, u32 StrAddr);
#endif


