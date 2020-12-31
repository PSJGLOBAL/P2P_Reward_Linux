#include "Common.h"
#include "cJSON.h"

cJSON::cJSON()
{	
	for (int nLoop = 0; nLoop < 3; nLoop++)
		fp[nLoop] = NULL;

	m_WalletJson = nullptr;
	m_DataJson = nullptr;
	
	nFileSize = 0;
}

cJSON::~cJSON()
{

}

bool cJSON::bRead_WalletInfo(const std::string _WalletInfo_Json, std::string &_MasterWalletAddr, std::string &_MasterWalletPrivateKey)
{
	bool bRet = true;

	if (-1 != access(_WalletInfo_Json.c_str(), 0))
	{
		fp[0] = fopen(_WalletInfo_Json.c_str(), "rb");

		if (NULL == fp[0])
		{
			bRet = false;
		}

		int Err = fseek(fp[0], 0, SEEK_END);

		if (0 != Err)
		{
			bRet = false;
		}

		nFileSize = ftell(fp[0]);

		if (0 == nFileSize)
		{
			bRet = false;
		}

		Err = fseek(fp[0], 0, SEEK_SET);
	
		if (0 != Err)
		{
			bRet = false;
		}

		if (0 != nFileSize)
		{
			try
			{
				m_WalletJson = (char*)malloc(sizeof(char)* static_cast<size_t>(nFileSize));

				if (NULL != m_WalletJson)
				{
					uint64_t	tot_num = 0;
					uint64_t	cur_num = 0;

					while ((cur_num = fread(&m_WalletJson[tot_num], sizeof(char), static_cast<size_t>(nFileSize - tot_num), fp[0])) > 0)
					{
						tot_num += cur_num;
					}

					if (tot_num != static_cast<uint64_t>(nFileSize))
					{
						std::cout << "Not Read WalletInfo JSON File All Contents" << std::endl;
					}
					else
					{
						Err = fclose(fp[0]);
						if (0 != Err)
						{
							bRet = false;
						}
					}

				}
			}
			catch (std::bad_alloc)
			{
				std::cout << "Memory Allocation Fail" << std::endl;
				bRet = false;
			}

			m_DocWallet.Parse(m_WalletJson, static_cast<size_t>(nFileSize));
		
			m_Reward.Master_Wallet_Addr.assign( m_DocWallet["Master_Wallet_Addr"].GetString());
			m_Reward.Master_Wallet_Privatekey.assign(m_DocWallet["Master_Wallet_Privatekey"].GetString());
			
			_MasterWalletAddr.assign(m_DocWallet["Master_Wallet_Addr"].GetString());
			_MasterWalletPrivateKey.assign(m_DocWallet["Master_Wallet_Privatekey"].GetString());

			std::cout << "Master Wallet Address = [ " << m_Reward.Master_Wallet_Addr.c_str() << " ]\n\n"
					  << "Master Wallet PrivateKey = [ " << m_Reward.Master_Wallet_Privatekey.c_str() << " ]" << std::endl;
		}
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool cJSON::bRead_ResultInfo(const std::string _ResultInfo_Json, const std::string _Master_Wallet_Addr, const std::string _Master_Wallet_PrivateKey, long double&_nTotalRewardCount)
{
	bool bRet = true;
	
	if (-1 != access(_ResultInfo_Json.c_str(), 0))
	{
		fp[1] = fopen(_ResultInfo_Json.c_str(), "rb");

		if (NULL == fp[1])
		{
			bRet = false;
		}

		int Err = fseek(fp[1], 0, SEEK_END);
		if (0 != Err)
		{
			bRet = false;
		}

		nFileSize = ftell(fp[1]);
		if (0 == nFileSize)
		{
			bRet = false;
		}

		Err = fseek(fp[1], 0, SEEK_SET);

		if (0 != Err)
		{
			bRet = false;
		}

		if (0 != nFileSize)
		{
			try
			{
				m_DataJson = (char*)malloc(sizeof(char) * static_cast<size_t>(nFileSize));

				if (NULL != m_DataJson)
				{
					uint64_t	tot_num = 0;
					uint64_t	cur_num = 0;

					while ((cur_num = fread(&m_DataJson[tot_num], sizeof(char), static_cast<size_t>(nFileSize - tot_num), fp[1])) > 0)
					{
						tot_num += cur_num;
					}

					if (tot_num != static_cast<uint64_t>(nFileSize))
					{
						std::cout << "not Read JSON File All Contents" << std::endl;
					}
					else
					{
						Err = fclose(fp[1]);
						if (0 != Err)
						{
							bRet = false;
						}
					}

				}
			}
			catch (std::bad_alloc)
			{
				std::cout << "Memory Allocation Fail" << std::endl;
				bRet = false;
			}

			m_DocData.Parse(m_DataJson, static_cast<size_t>(nFileSize));

			std::string Project_State = m_DocData["proj_flag"].GetString();

			if ("DONE" == Project_State)
			{
				m_Reward.Master_Wallet_Addr.assign(_Master_Wallet_Addr);
				m_Reward.Master_Wallet_Privatekey.assign(_Master_Wallet_PrivateKey);

				m_Reward.Project_Flag.assign(m_DocData["proj_flag"].GetString());

				const Value& Task_Info = m_DocData["task_info"];
				const Value& NameList = Task_Info["tasknamelist"];

				if (NameList.IsArray())
				{
					StReward_Info _reward_Info;

					for (SizeType i = 0; i < NameList.Size(); ++i)
					{						
						if (true == NameList[i]["value"].IsString())
						{
							// 해당 금액이 스트링이라면
							std::string TempPrice = NameList[i]["value"].GetString();
							_reward_Info.Price = static_cast<float>(atof(TempPrice.c_str()));
						}
						else if (true == NameList[i]["value"].IsFloat() || true == NameList[i]["value"].IsLosslessFloat())
						{
							_reward_Info.Price = NameList[i]["value"].GetFloat();
						}
						else if (true == NameList[i]["value"].IsDouble() || true == NameList[i]["value"].IsLosslessDouble())
						{
							_reward_Info.Price = NameList[i]["value"].GetDouble();
						}
						else if (true == NameList[i]["value"].IsInt64() || true == NameList[i]["value"].IsUint64())
						{
							_reward_Info.Price = static_cast<float>(NameList[i]["value"].GetInt64());
						}
						else if (true == NameList[i]["value"].IsInt() || true == NameList[i]["value"].IsUint())
						{
							_reward_Info.Price = static_cast<float>(NameList[i]["value"].GetInt());
						}

						_reward_Info.Work_Time = NameList[i]["work_time"].GetInt64();

						_reward_Info.Time_Unit.assign(NameList[i]["time_unit"].GetString());

						_nTotalRewardCount += _reward_Info.Price;

						std::string TempWallet = NameList[i]["wallet_addr"].GetString();
						
						if (-1 == reinterpret_cast<size_t>(TempWallet.find("0x")))
						{
							_reward_Info.Wallet_addr.assign("0x");
							_reward_Info.Wallet_addr.append(NameList[i]["wallet_addr"].GetString());
						}
						else
						{
							_reward_Info.Wallet_addr.assign(NameList[i]["wallet_addr"].GetString());
						}

						ret = m_Reward.Reward_List.insert(std::make_pair(_reward_Info.Wallet_addr, _reward_Info));

						if (false == ret.second)
						{
							it = m_Reward.Reward_List.begin();

							for (it = m_Reward.Reward_List.begin(); it != m_Reward.Reward_List.end(); ++it)
							{
								if (it->second.Wallet_addr == _reward_Info.Wallet_addr)
								{
									m_Reward.Reward_List[it->second.Wallet_addr].Price += _reward_Info.Price;
									m_Reward.Reward_List[it->second.Wallet_addr].Work_Time += _reward_Info.Work_Time;
								}
							}
						}
					}
				}
			}
			else
			{
				std::cout << "resultInfo.json File is Not Found DONE State" << std::endl;
				bRet = false;
			}
		}
	}
	else
	{
		bRet = false;
	}
	return bRet;
}

bool cJSON::bRead_RewardResultInfo(const std::string _RewardResultInfoJson)
{
	bool bRet = true;

	if (-1 != access(_RewardResultInfoJson.c_str(), 0))
	{
		fp[2] =fopen(_RewardResultInfoJson.c_str(), "rb");

		if (NULL == fp[2])
		{
			bRet = false;
		}

		int nErr = fseek(fp[1], 0, SEEK_END);
		if (0 != nErr)
		{
			bRet = false;
		}

		nFileSize = ftell(fp[1]);
		if (0 == nFileSize)
		{
			bRet = false;
		}

		nErr = fseek(fp[1], 0, SEEK_SET);

		if (0 != nErr)
		{
			bRet = false;
		}

		if (0 != nFileSize)
		{
			try
			{
				m_DataJson = (char*)malloc(sizeof(char) * static_cast<size_t>(nFileSize));

				if (NULL != m_DataJson)
				{
					uint64_t	tot_num = 0;
					uint64_t	cur_num = 0;

					while ((cur_num = fread(&m_DataJson[tot_num], sizeof(char), static_cast<size_t>(nFileSize - tot_num), fp[1])) > 0)
					{
						tot_num += cur_num;
					}

					if (tot_num != static_cast<uint64_t>(nFileSize))
					{
						std::cout << "not Read JSON File All Contents" << std::endl;
					}
					else
					{
						nErr = fclose(fp[2]);
						if (0 != nErr)
						{
							bRet = false;
						}
					}

				}
			}
			catch (std::bad_alloc)
			{
				std::cout << "Memory Allocation Fail" << std::endl;
				bRet = false;
			}

			m_DocData.Parse(m_DataJson, static_cast<size_t>(nFileSize));

			const Value& Reward_Result = m_DocData["Reward_Result"];

			if (Reward_Result.IsArray())
			{
				StRewardList RewardInfo;

				for (SizeType i = 0; i < Reward_Result.Size(); ++i)
				{
					RewardInfo.Wallet_Address.assign(Reward_Result[i]["Wallet_Address"].GetString());
					RewardInfo.TxHash.assign(Reward_Result[i]["TxHash"].GetString());
					
					if (true == Reward_Result[i]["Price"].IsString())
					{
						RewardInfo.Price.assign(Reward_Result[i]["Price"].GetString());
					}
					else if (true == Reward_Result[i]["Price"].IsFloat())
					{
						RewardInfo.Price.assign(std::to_string(Reward_Result[i]["Price"].GetFloat()));
					}
					else if (true == Reward_Result[i]["Price"].IsInt64())
					{
						RewardInfo.Price.assign(std::to_string(Reward_Result[i]["Price"].GetInt64()));
					}

					RewardInfo.ErrorCode.assign(Reward_Result[i]["ErrorCode"].GetString());
					RewardInfo.ErrorMsg.assign(Reward_Result[i]["ErrorMsg"].GetString());

					m_ReReward.ReReward_List.insert(std::make_pair(RewardInfo.Wallet_Address, RewardInfo));
				}
			}
			m_ReReward.Master_Wallet_Addr.assign(m_Reward.Master_Wallet_Addr);
			m_ReReward.Master_Wallet_Privatekey.assign(m_Reward.Master_Wallet_Privatekey);
		}
	}
	else
	{
		bRet = false;
	}
	return bRet;
}

bool cJSON::bWrite_RewardResult(std::map<int, StReward_Result> _ResultMap, std::string FaileName, long double nTotalRewardCoin)
{
	Document doc;
	std::string TempJson;
	std::map<int, StReward_Result>::iterator Tempit;

	StringBuffer s;
	Writer<StringBuffer> writer(s);

	for (Tempit = _ResultMap.begin(); Tempit != _ResultMap.end(); ++Tempit)
	{
	    if (Tempit == _ResultMap.begin())
	    {
			writer.StartObject();
			std::string TotalRewardCoin("Total_Reward_Coin");
			writer.String(TotalRewardCoin.c_str(), static_cast<SizeType>(TotalRewardCoin.length()));
			std::string RewardCoin(std::to_string(Tempit->second.Total_Reward_Coin));
			writer.String(RewardCoin.c_str(), static_cast<SizeType>(RewardCoin.length()));

			std::string Reward_Result("Reward_Result");
			writer.String(Reward_Result.c_str(), static_cast<SizeType>(Reward_Result.length()));
			writer.StartArray();
	    }

	    writer.StartObject();
	    std::string Wallet_Address("Wallet_Address");
	    writer.String(Wallet_Address.c_str(), static_cast<SizeType>(Wallet_Address.length()));
	    std::string Wallet(Tempit->second.Wallet_Address.c_str());
	    writer.String(Wallet.c_str(), static_cast<SizeType>(Wallet.length()));

	    std::string TxHash("TxHash");
	    writer.String(TxHash.c_str(), static_cast<SizeType>(TxHash.length()));
	    std::string HashInfo(Tempit->second.TransactionHash);
	    writer.String(HashInfo.c_str(), static_cast<SizeType>(HashInfo.length()));

	    std::string Price("Price");
	    writer.String(Price.c_str(), static_cast<SizeType>(Price.length()));
	    std::string Count(std::to_string(Tempit->second.Price));
	    writer.String(Count.c_str(), static_cast<SizeType>(Count.length()));

	    std::string ErrorCode("ErrorCode");
	    writer.String(ErrorCode.c_str(), static_cast<SizeType>(ErrorCode.length()));
	    std::string Code(Tempit->second.ErrorCode);
	    writer.String(Code.c_str(), static_cast<SizeType>(Code.length()));

	    std::string ErrorMsg("ErrorMsg");
	    writer.String(ErrorMsg.c_str(), static_cast<SizeType>(ErrorMsg.length()));
	    std::string Msg(Tempit->second.ErrorMsg);
	    writer.String(Msg.c_str(), static_cast<SizeType>(Msg.length()));
	    writer.EndObject();
	}	

	writer.EndArray();
	writer.EndObject();

	FILE *fp = NULL;
	fp = fopen(FaileName.c_str(), "wb");
	fwrite(s.GetString(), s.GetSize(), 1, fp);
	fclose(fp);

	return true;
}

struct stBalanceInfo cJSON::Get_BalanceInfo(const std::string _String_Json)
{
	stBalanceInfo BalanceInfo;

	m_DocStringData.Parse(_String_Json.c_str());	
	
	if(false == m_DocStringData["meta"]["error"].IsString())
	{
	    BalanceInfo.Balance = std::stold(m_DocStringData["payload"]["token"].GetString());
	}
	
	return BalanceInfo;
}

__int64 cJSON::Get_GasPrice(const std::string _String_Json)
{
    __int64 Data = 0;

    m_DocStringData.Parse(_String_Json.c_str());
    
    if (false == m_DocStringData["meta"]["error"].IsString())
    {
	Data = static_cast<__int64>(strtoll(m_DocStringData["payload"]["average"].GetString(), NULL, 10) * 1000000000);
    }
    return Data;
}

__int64 cJSON::Get_GasLimit(const std::string _String_Json)
{
    __int64 Data;
    m_DocStringData.Parse(_String_Json.c_str());

    if (false == m_DocStringData["meta"]["error"].IsString())
    {
		Data = static_cast<__int64>(strtoll(m_DocStringData["payload"]["gasLimit"].GetString(), NULL, 10 ) * 1.40);
    }
    return Data;
}

StErrorCode cJSON::Get_TokenSendTransactionHash(const std::string _String_Json)
{	
    StErrorCode StCode;

    m_DocStringData.Parse(_String_Json.c_str());

    if (true == m_DocStringData["meta"]["error"]["message"].IsString())
    {
        StCode.Msg.assign(m_DocStringData["meta"]["error"]["message"].GetString());
        StCode.Code.assign(std::to_string(m_DocStringData["meta"]["error"]["code"].GetInt()));
        StCode.Hex.assign("None");
    }	
    else
    {
        StCode.Hex.assign(m_DocStringData["payload"]["hex"].GetString());
        StCode.Code.assign("0");
        StCode.Msg.assign("None");	
    }
    return StCode;
}