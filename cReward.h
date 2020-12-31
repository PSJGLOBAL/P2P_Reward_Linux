#pragma once

#include "cJSON.h"
#include "cURL.h"
#include "Common.h"

class cReward
{
public:
	cReward();
	~cReward();
	
	std::string convertDouble(double value);

	std::string GetWalletInfoPath();
	std::string GetResultInfoPath();
	std::string GetRewardResultInfoPath();

	bool WalletInfoRead(const std::string _WalletInfoJson, std::string &_MasterWalletAddr, std::string &_MasterWalletPrivateKey);
	bool ResultInfoRead(const std::string _ResultInfoJson, const std::string _Master_Wallet_Addr, const std::string _Master_Wallet_PrivateKey, long double& _nTotalRewardCoin);
	bool RewardResultInfoRead(const std::string _RewardResultInfoJson);

	bool GetRewardInfo();
	bool GetReRewardInfo();

	bool SetContentHeader();
	bool SetUrl();

	long double TokenBalance();
	__int64 GetGasFee();
	__int64 GetGasLimit();

	bool Token_Transfer();

	bool Token_ReTransfer();

public:
	cJSON* m_pJson;
	
	std::map<int, StReward_Result>									m_Reward_Result;
	std::map<std::string, StReward_Info>							m_SumReward;
	std::multimap<std::string, StReward_Info>::iterator				m_it;
	std::multimap<std::string, StRewardList>::iterator				m_it2;

	std::pair<std::map<std::string, StReward_Info>::iterator, bool> m_Ret;

	StReward* m_pRewardInfo;
	StReReward* m_pReRewardInfo;

	cURL* m_pUrl;

	std::string m_strContentType;
	std::string m_strContentHeader;
	std::string m_strURL;

	std::string m_strParam, m_strParam2, m_strParam3, m_strParam4, m_strParam5, m_strParam6, m_strParam7, m_strParam8, m_strParam9, m_strParam10;

	__int64 nGas_Price, nGas_Limit;

	bool bTransfer_State;
	long double m_nTotalRewardCoin;
	int m_nReCount;
};

