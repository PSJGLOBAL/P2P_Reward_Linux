#include "cReward.h"

cReward::cReward()
{
    m_pJson = new cJSON();
    m_pRewardInfo = new StReward();
    m_pReRewardInfo = new StReReward();
    m_pUrl = new cURL();

    m_strParam.clear();
    m_strParam2.clear();
    m_strParam3.clear();
    m_strParam4.clear();
    m_strParam5.clear();
    m_strParam6.clear();
    m_strParam7.clear();
    m_strParam8.clear();
    m_strParam9.clear();
    m_strParam10.clear();

    nGas_Price = nGas_Limit = 0;

    bTransfer_State = false;

    m_nReCount = 0;

    m_nTotalRewardCoin = 0;
}

cReward::~cReward()
{
    delete m_pJson;
    delete m_pRewardInfo;
    delete m_pUrl;

    m_pJson = NULL;
    m_pRewardInfo = NULL;
    m_pUrl = NULL;
}

std::string cReward::convertDouble(double value)
{
    std::ostringstream o;
    if (!(o << value))
        return "";
    return o.str();
}

std::string cReward::GetWalletInfoPath()
{
    char* sPath;// [MAX_PATH] = { 0, };
    sPath = getcwd(NULL, MAX_PATH);
    std::string WalletInfo;

    WalletInfo.assign(sPath);
    WalletInfo.append("/");
    WalletInfo.append("WalletInfo.json");

    std::cout << "WalletInfo Json File Location : [ " << WalletInfo.c_str() << " ] " << std::endl;

    return WalletInfo;
}

std::string cReward::GetResultInfoPath()
{
    char* sPath;// [MAX_PATH] = { 0, };
    sPath = getcwd(NULL, MAX_PATH);
    std::string ResultInfo;

    ResultInfo.assign(sPath);
    ResultInfo.append("/");
    ResultInfo.append("resultinfo.json");

    std::cout << "ResultInfo Json File Location : [ " << ResultInfo.c_str() << " ] " << std::endl;

    return ResultInfo;
}

std::string cReward::GetRewardResultInfoPath()
{
    char* sPath;// [MAX_PATH] = { 0, };
    sPath = getcwd(NULL, MAX_PATH);
    std::string RewardResultInfo;

    RewardResultInfo.assign(sPath);
    RewardResultInfo.append("/");
    RewardResultInfo.append("Reward_Result.json");

    std::cout << "RewardResultInfo Json File Location : [ " << RewardResultInfo.c_str() << " ] " << std::endl;

    return RewardResultInfo;
}

bool cReward::WalletInfoRead(const std::string _WalletInfoJson, std::string& _MasterWalletAddr, std::string& _MasterWalletPrivateKey)
{
    bool bRet = true;
    bRet = m_pJson->bRead_WalletInfo(_WalletInfoJson, _MasterWalletAddr, _MasterWalletPrivateKey);
    m_pRewardInfo->Master_Wallet_Addr = _MasterWalletAddr;
    m_pRewardInfo->Master_Wallet_Privatekey = _MasterWalletPrivateKey;
    return bRet;
}

bool cReward::ResultInfoRead(const std::string _ResultInfoJson, const std::string _Master_Wallet_Addr, const std::string _Master_Wallet_PrivateKey, long double& _nTotalRewardCoin)
{
    bool bRet = true;

    bRet = m_pJson->bRead_ResultInfo(_ResultInfoJson, _Master_Wallet_Addr, _Master_Wallet_PrivateKey, _nTotalRewardCoin);

    return bRet;
}

bool cReward::RewardResultInfoRead(const std::string _RewardResultInfoJson)
{
    bool bRet = true;

    bRet = m_pJson->bRead_RewardResultInfo(_RewardResultInfoJson);

    return bRet;
}

bool cReward::GetRewardInfo()
{
    bool bRet = true;

    m_pRewardInfo = m_pJson->GetRewardInfo();

    if (true == m_pRewardInfo->Master_Wallet_Addr.empty() || true == m_pRewardInfo->Master_Wallet_Privatekey.empty() || 0 == m_pRewardInfo->Reward_List.size())
    {
        bRet = false;
    }

    std::cout << "Master Wallet Address : [ " << m_pRewardInfo->Master_Wallet_Addr.c_str() << " ]" << std::endl;
    std::cout << "Project_Status : [ " << m_pRewardInfo->Project_Flag.c_str() << " ]" << std::endl;
    std::cout << "Total Reward Count : [ " << m_pRewardInfo->Reward_List.size() << " ]" << std::endl;

    remove(GetResultInfoPath().c_str());
    return bRet;
}

bool cReward::GetReRewardInfo()
{
    bool bRet = true;

    m_pReRewardInfo = m_pJson->GetReRewardInfo();

    if (true == m_pReRewardInfo->Master_Wallet_Addr.empty() || true == m_pReRewardInfo->Master_Wallet_Privatekey.empty() || 0 == m_pReRewardInfo->ReReward_List.size())
    {
        bRet = false;
    }

    std::cout << "ReSend Master Wallet Address : [ " << m_pRewardInfo->Master_Wallet_Addr.c_str() << " ]" << std::endl;

    remove(GetRewardResultInfoPath().c_str());

    return bRet;
}

bool cReward::SetContentHeader()
{
    bool bRet = false;

    // Content Type 헤더 설정	
    m_strContentType.assign("Content-Type: application/json");
    m_strContentHeader.assign("X-API-Key: ");
    m_strContentHeader.append(APIKey);

    if (false == m_strContentHeader.empty() && false == m_strContentType.empty())
    {
        bRet = true;
    }

    return bRet;
}

bool cReward::SetUrl()
{
    bool bRet = false;

    m_strURL.assign("https://api.cryptoapis.io/v1/bc/eth/");

    if (true != m_strURL.empty())
    {
        bRet = true;
    }

    return bRet;
}

long double cReward::TokenBalance()
{
    long double Total_Token = 0;

    m_pUrl->InitCURL();
    if (true == SetContentHeader())
    {
        m_pUrl->SetCurlHeaderList(m_strContentType);
        m_pUrl->SetCurlHeaderList(m_strContentHeader);
    }
    if (true == SetUrl())
    {
        m_pUrl->SetUrl(m_strURL);
    }

    m_strParam.assign(Network_Type);
    m_strParam2.assign(m_pRewardInfo->Master_Wallet_Addr.c_str());
    m_strParam3.assign(SmartContract);

    Total_Token = m_pUrl->Get_TokenBalance(m_strParam, m_strParam2, m_strParam3, m_pJson);

    m_pUrl->CleanCURL();

    return Total_Token;
}

__int64 cReward::GetGasFee()
{
    m_pUrl->InitCURL();
    m_pUrl->SetCurlHeaderList(m_strContentType);
    m_pUrl->SetCurlHeaderList(m_strContentHeader);
    m_pUrl->SetUrl(m_strURL);

    m_strParam.assign(Network_Type);

    nGas_Price = m_pUrl->Get_GasFee(m_strParam, m_pJson);

    m_pUrl->CleanCURL();

    return nGas_Price;
}

__int64 cReward::GetGasLimit()
{
    for (m_it = m_pRewardInfo->Reward_List.begin(); m_it != m_pRewardInfo->Reward_List.end(); ++m_it)
    {
        m_pUrl->InitCURL();
        m_pUrl->SetCurlHeaderList(m_strContentType);
        m_pUrl->SetCurlHeaderList(m_strContentHeader);
        m_pUrl->SetUrl(m_strURL);

        m_strParam.assign(Network_Type);
        m_strParam2.assign(m_pRewardInfo->Master_Wallet_Addr); //From Wallet Address
        m_strParam3.assign(m_it->second.Wallet_addr.c_str());
        nGas_Limit = m_pUrl->Get_GasLimit(m_strParam, m_strParam2, m_strParam3, m_pJson);
        m_pUrl->CleanCURL();
    }

    return nGas_Limit;
}

bool cReward::Token_Transfer()
{
    bool bRet = true;

    int nLoop = 0;

    for (m_it = m_pRewardInfo->Reward_List.begin(); m_it != m_pRewardInfo->Reward_List.end(); ++m_it)
    {
        m_pUrl->InitCURL();
        m_pUrl->SetCurlHeaderList(m_strContentType);
        m_pUrl->SetCurlHeaderList(m_strContentHeader);
        m_pUrl->SetUrl(m_strURL);

        m_strParam.assign(Network_Type);
        m_strParam2.assign(m_pRewardInfo->Master_Wallet_Addr.c_str());
        m_strParam3.assign(m_it->second.Wallet_addr.c_str());
        m_strParam4.assign(SmartContract);
        m_strParam5.assign(m_pRewardInfo->Master_Wallet_Privatekey);
        m_strParam6.assign(std::to_string(nGas_Price));

        nGas_Limit = 100000;
        m_strParam7.assign(std::to_string(nGas_Limit));
        m_strParam8.assign(std::to_string(m_it->second.Price));

        StErrorCode StCode;
        StReward_Result Result;

        StCode = m_pUrl->Transfer_Reward(m_strParam, m_strParam2, m_strParam3, m_strParam4, m_strParam5, m_strParam6, m_strParam7, m_strParam8, nLoop, m_pJson);

        m_pUrl->CleanCURL();

        if (0 == StCode.Hex.find("0x"))
        {
            //전송 성공
            Result.Wallet_Address.assign(m_it->second.Wallet_addr.c_str());
            Result.Price = m_it->second.Price;
            Result.Work_Time = m_it->second.Work_Time;
            Result.TransactionHash.assign(StCode.Hex.c_str());
            Result.bState = true;
            Result.ErrorCode.assign(StCode.Code.c_str());
            Result.ErrorMsg.assign(StCode.Msg.c_str());

            bTransfer_State = true;
        }
        else
        {
            Result.Wallet_Address.assign(m_it->second.Wallet_addr.c_str());
            Result.Price = m_it->second.Price;
            Result.Work_Time = m_it->second.Work_Time;
            Result.TransactionHash.assign(StCode.Hex.c_str());
            Result.bState = false;
            Result.ErrorCode.assign(StCode.Code.c_str());
            Result.ErrorMsg.assign(StCode.Msg.c_str());

            bTransfer_State = false;
        }
        Result.Total_Reward_Coin = m_nTotalRewardCoin;
        m_Reward_Result.insert(std::make_pair(nLoop, Result));
        nLoop += 1;

        // 토큰 전송
        m_pUrl->InitCURL();
        m_pUrl->CleanCURL();

        if (nLoop < static_cast<int>(m_pRewardInfo->Reward_List.size()))
        {
            sleep(60);
        }
    }

    std::string TempResultFileName("Reward_Result.json");
    m_pJson->bWrite_RewardResult(m_Reward_Result, TempResultFileName, m_nTotalRewardCoin);

    return bRet;
}

bool cReward::Token_ReTransfer()
{
    bool bRet = false;

    if (false == bTransfer_State)
    {
        StErrorCode StCode;
        StReward_Result Result;

        // Reward_Result.json 파일을 읽어 들인다.
        int nLoop = 0;
        m_Reward_Result.clear();

        for (m_it2 = m_pReRewardInfo->ReReward_List.begin(); m_it2 != m_pReRewardInfo->ReReward_List.end(); ++m_it2)
        {
            if (0 != m_it2->second.TxHash.compare("None"))
            {
                Result.Wallet_Address.assign(m_it2->second.Wallet_Address.c_str());
                Result.Price = atoll(m_it2->second.Price.c_str());
                Result.TransactionHash.assign(m_it2->second.TxHash.c_str());
                Result.bState = true;
                Result.ErrorCode.assign(m_it2->second.ErrorCode.c_str());
                Result.ErrorMsg.assign(m_it2->second.ErrorMsg.c_str());

                bTransfer_State = true;
                m_Reward_Result.insert(std::make_pair(nLoop, Result));
                nLoop += 1;

                std::cout << "Token Send Transaction Hash = [ " << Result.TransactionHash.c_str() << " ]"
                    << ". Msg = [ " << Result.ErrorMsg.c_str() << " ]"
                    << ". Code = [ " << Result.ErrorCode.c_str() << " ]"
                    << std::endl;

                if (nLoop != static_cast<int>(m_pReRewardInfo->ReReward_List.size()))
                {
                    sleep(60);
                }
            }
            else
            {
                m_pUrl->InitCURL();
                m_pUrl->SetCurlHeaderList(m_strContentType);
                m_pUrl->SetCurlHeaderList(m_strContentHeader);
                m_pUrl->SetUrl(m_strURL);

                m_strParam.assign(Network_Type);
                m_strParam2.assign(m_pRewardInfo->Master_Wallet_Addr.c_str());
                m_strParam3.assign(m_it2->second.Wallet_Address.c_str());
                m_strParam4.assign(SmartContract);
                m_strParam5.assign(m_pRewardInfo->Master_Wallet_Privatekey);
                m_strParam6.assign(std::to_string(nGas_Price));

                nGas_Limit = 100000;
                m_strParam7.assign(std::to_string(nGas_Limit));
                m_strParam8.assign(m_it2->second.Price);

                StCode = m_pUrl->Transfer_Reward(m_strParam, m_strParam2, m_strParam3, m_strParam4, m_strParam5, m_strParam6, m_strParam7, m_strParam8, nLoop, m_pJson);

                m_pUrl->CleanCURL();

                if (0 == StCode.Hex.find("0x") || 0 != StCode.Hex.find("None"))
                {
                    //전송 성공
                    Result.Wallet_Address.assign(m_it2->second.Wallet_Address.c_str());
                    Result.Price = atoll(m_it2->second.Price.c_str());
                    Result.TransactionHash.assign(StCode.Hex.c_str());
                    Result.bState = true;
                    Result.ErrorCode.assign(StCode.Code.c_str());
                    Result.ErrorMsg.assign(StCode.Msg.c_str());

                    bTransfer_State = true;
                }
                else
                {
                    //전송 에러
                    Result.Wallet_Address.assign(m_it2->second.Wallet_Address.c_str());
                    Result.Price = atoll(m_it2->second.Price.c_str());
                    Result.TransactionHash.assign(StCode.Hex.c_str());
                    Result.bState = false;
                    Result.ErrorCode.assign(StCode.Code.c_str());
                    Result.ErrorMsg.assign(StCode.Msg.c_str());

                    bTransfer_State = false;
                }
                m_Reward_Result.insert(std::make_pair(nLoop, Result));
                nLoop += 1;

                // 토큰 전송
                m_pUrl->InitCURL();
                m_pUrl->CleanCURL();

                if (nLoop != static_cast<int>(m_pReRewardInfo->ReReward_List.size()))
                {
                    sleep(60);
                }
            }
        }

        std::string TempResultFileName("Reward_Result.json");
        m_pJson->bWrite_RewardResult(m_Reward_Result, TempResultFileName, m_nTotalRewardCoin);
    }

    return bRet;
}