#pragma once

#if defined(_WIN32) && defined(_WIN64)
    #include <io.h>
#else
    #include <stdint.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include <error.h>
#endif

// Common
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <assert.h>
#include <string>
#include <vector>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/reader.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>

#include <curl/curl.h>

#ifndef WIN32
    #define __int64 long long
    #define MAX_PATH 260
#endif

extern const char* SmartContract;
extern const char* APIKey;
extern const char* Network_Type;
extern const char* PrivateKey;
extern const int RESEND_COUNT;

typedef enum eArgType
{
    ARGUMENT_TYPE = 1,
    FILE_TYPE,
}ARG_TYPE;

typedef struct StErrorCode
{
    std::string Code;
    std::string Msg;
    std::string Hex;

    StErrorCode()
    {
        Code.clear();
        Msg.clear();
        Hex.clear();
    }
}ErrorCode;

typedef struct StReward_Result
{
    std::string Wallet_Address;
    std::string TransactionHash;
    std::string ErrorCode;
    std::string ErrorMsg;

    long double	Total_Reward_Coin;

    long double	Price;
    __int64		Work_Time;

    bool		bState;

    StReward_Result()
    {
        Total_Reward_Coin = 0.0f;
        Price = 0.0f;
        Work_Time = 0;
        Wallet_Address.clear();
        TransactionHash.clear();
        ErrorCode.clear();
        ErrorMsg.clear();
        bState = false;
    }
}Reward_Result;

typedef struct StReward_Info
{
    long double Price;
    __int64 Work_Time;

    std::string	 Wallet_addr;
    std::string  Time_Unit;

    StReward_Info()
    {
        Price = 0.0f;
        Work_Time = 0;

        Time_Unit.clear();
        Wallet_addr.clear();
    }
}Reward_Info;

typedef struct StRewardList
{
    std::string Wallet_Address;
    std::string TxHash;			
    std::string Price;			
    std::string ErrorCode;		
    std::string ErrorMsg;		

    StRewardList()
    {
        Wallet_Address.clear();
        TxHash.clear();
        Price.clear();
        ErrorCode.clear();
        ErrorMsg.clear();
    }
}RewardList;

typedef struct StSumReward_Info
{
    long double Price;
    std::string	 Wallet_addr;

    StSumReward_Info()
    {
        Price = 0.0f;
        Wallet_addr.clear();
    }
}SumReward_Info;

typedef struct StReward
{
    std::string  Project_Flag;	// RUNNING or DONE 	
    std::string  Master_Wallet_Addr;
    std::string  Master_Wallet_Privatekey;

    std::map<std::string, StReward_Info> Reward_List;

    StReward()
    {
        Project_Flag.clear();
        Master_Wallet_Addr.clear();
        Reward_List.clear();
    }
}Reward;

//토큰 재전송을 위해 사용하는 구조체
typedef struct StReReward
{
    std::string  Master_Wallet_Addr;
    std::string  Master_Wallet_Privatekey;

    std::map<std::string, StRewardList> ReReward_List;

    StReReward()
    {
        Master_Wallet_Addr.clear();
        Master_Wallet_Privatekey.clear();
        ReReward_List.clear();
    }
}ReReward;

typedef struct stBalanceInfo
{
    long double Balance;

    stBalanceInfo()
    {
        Balance = 0.0f;
    }
}BalanceInfo;