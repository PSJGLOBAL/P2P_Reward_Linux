#pragma once
#include "Common.h"

using namespace rapidjson;

class cJSON
{
public:
    cJSON();
    ~cJSON();

    bool bRead_WalletInfo(const std::string _WalletInfo_Json, std::string& _MasterWalletAddr, std::string& _MasterWalletPrivateKey);
    bool bRead_ResultInfo(const std::string _ResultInfo_Json, const std::string _Master_Wallet_Addr, const std::string _Master_Wallet_PrivateKey, long double& _nTotalRewardCount);
    bool bRead_RewardResultInfo(const std::string _RewardResultInfoJson);

    bool bWrite_RewardResult(std::map<int, StReward_Result> _ResultMap, std::string FaileName, long double nTotalRewardCoin);

    struct stBalanceInfo Get_BalanceInfo(const std::string _String_Json);
    __int64   Get_GasPrice(const std::string _String_Json);
    __int64   Get_GasLimit(const std::string _String_Json);
    StErrorCode Get_TokenSendTransactionHash(const std::string _String_Json);
    inline StReward* GetRewardInfo() { return &m_Reward; }
    inline StReReward* GetReRewardInfo() { return &m_ReReward; }

private:
    char* m_WalletJson, * m_DataJson;
    __int64 nFileSize;
    FILE* fp[3];
    StReward	m_Reward;
    StReReward	m_ReReward;

    Document	m_DocWallet, m_DocData, m_DocStringData;
    std::pair<std::map<std::string, StReward_Info>::iterator, bool> ret;
    std::pair<std::map<std::string, StRewardList>::iterator, bool> ret2;
    std::map<std::string, StReward_Info>::iterator it;
    std::map<std::string, StRewardList>::iterator it2;
};

