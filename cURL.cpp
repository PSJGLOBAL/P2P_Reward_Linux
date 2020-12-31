#include "cURL.h"

template<typename ... Args>
static std::string string_format(const std::string& format, Args ... args)
{
    size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0' 
    if (size <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside 
}

cURL::cURL()
{

}

cURL::~cURL()
{

}

long double cURL::Get_TokenBalance(const std::string _Param, const std::string _Param2, const std::string _Param3, cJSON* Json)
{
    long double nTokenBalance = 0.0f;

    if (m_curl)
    {

        AppendUrl(_Param);
        AppendUrl("/tokens/");
        AppendUrl(_Param2.c_str());	//wallet_address
        AppendUrl("/");
        AppendUrl(_Param3.c_str());	//contract address
        AppendUrl("/balance");	//method	


        curl_easy_setopt(m_curl, CURLOPT_URL, m_strTargetURL.c_str());
        curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headerlist);
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, false);

        // 결과 기록
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_str_buffer);

        m_res = curl_easy_perform(m_curl);

        if (m_res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(m_res));
            return false;
        }
        else
        {
            stBalanceInfo Info;
            Info = Json->Get_BalanceInfo(m_str_buffer);

            if (0 == Info.Balance)
            {
                nTokenBalance = 0;
            }
            else
            {
                nTokenBalance = Info.Balance;
                std::cout << "Balance = [ " << static_cast<__int64>(nTokenBalance) << " ]" << std::endl;
            }
        }
    }

    return nTokenBalance;
}

__int64 cURL::Get_GasFee(const std::string _Param, cJSON* Json)
{
    __int64 GasPrice = 0;

    if (m_curl)
    {
        AppendUrl(_Param);
        AppendUrl("/txs/fee");

        curl_easy_setopt(m_curl, CURLOPT_URL, m_strTargetURL.c_str());
        curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headerlist);
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, false);

        // 결과 기록
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_str_buffer);

        m_res = curl_easy_perform(m_curl);

        if (m_res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(m_res));
            return false;
        }
        else
        {
            GasPrice = Json->Get_GasPrice(m_str_buffer);
            std::cout << "Gas Price = [ " << GasPrice << " Wei ]" << std::endl;
        }
    }
    return GasPrice;
}

__int64 cURL::Get_GasLimit(std::string _Param, std::string _Param2, std::string _Param3, cJSON* Json)
{
    __int64 GasLimit = 0;

    if (m_curl)
    {
        AppendUrl(_Param);
        AppendUrl("/txs/gas");

        m_strResourceJSON.assign("{ \"fromAddress\" : \"");
        m_strResourceJSON.append(_Param2.c_str());
        m_strResourceJSON.append("\", \"toAddress\" : \"");
        m_strResourceJSON.append(_Param3.c_str());
        m_strResourceJSON.append("\"}");

        curl_easy_setopt(m_curl, CURLOPT_URL, m_strTargetURL.c_str());
        curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headerlist);
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, false);
        curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
        curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, m_strResourceJSON.c_str());
        // 결과 기록
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_str_buffer);

        m_res = curl_easy_perform(m_curl);

        if (m_res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(m_res));
            return false;
        }
        else
        {
            GasLimit = Json->Get_GasLimit(m_str_buffer);
            std::cout << "Gas Limit = [ " << GasLimit << " ]" << std::endl;
        }
    }
    return GasLimit;
}

StErrorCode cURL::Transfer_Reward(const std::string _Param, const std::string _Param2, const std::string _Param3, const std::string _Param4, const std::string _Param5,
    const std::string _Param6, const std::string _Param7, const std::string _Param8, const int ncount, cJSON* Json)
{
    StErrorCode StCode;

    if (m_curl)
    {
        AppendUrl(_Param);
        AppendUrl("/tokens/transfer");

        //string to Hex
        m_strResourceJSON.assign("{ \"fromAddress\" : \"");
        m_strResourceJSON.append(_Param2.c_str());
        m_strResourceJSON.append("\", \"toAddress\" : \"");
        m_strResourceJSON.append(_Param3.c_str());
        m_strResourceJSON.append("\", \"contract\" : \"");
        m_strResourceJSON.append(_Param4.c_str());
        m_strResourceJSON.append("\", \"privateKey\" : \"");
        m_strResourceJSON.append(_Param5.c_str());
        m_strResourceJSON.append("\", \"gasPrice\" : \"");
        m_strResourceJSON.append(_Param6.c_str());
        m_strResourceJSON.append("\", \"gasLimit\" : \"");
        m_strResourceJSON.append(_Param7.c_str());
        m_strResourceJSON.append("\", \"token\" : \"");
        m_strResourceJSON.append(_Param8.c_str());
        m_strResourceJSON.append("\"}");

        curl_easy_setopt(m_curl, CURLOPT_URL, m_strTargetURL.c_str());
        curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headerlist);
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, false);
        curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
        curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, m_strResourceJSON.c_str());
        // 결과 기록
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_str_buffer);

        m_res = curl_easy_perform(m_curl);

        if (m_res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(m_res));
            StCode.Msg.assign("None");
            StCode.Hex.assign("None");
            StCode.Code.assign(curl_easy_strerror(m_res));
        }
        else
        {
            StCode = Json->Get_TokenSendTransactionHash(m_str_buffer);

            switch (atoi(StCode.Code.c_str()))
            {
            case 3022:
            {
                std::cout << "Not enough tokens"
                    << ". Msg = [ " << StCode.Msg.c_str() << " ]"
                    << ". Code = [ " << StCode.Code.c_str() << " ]"
                    << std::endl;
            }break;

            case 3023:
            {
                std::cout << "Balance is not enough"
                    << ". Msg = [ " << StCode.Msg.c_str() << " ]"
                    << ". Code = [ " << StCode.Code.c_str() << " ]"
                    << std::endl;
            }break;

            case 3024:
            {
                std::cout << "contract cannot be null or empty"
                    << ". Msg = [ " << StCode.Msg.c_str() << " ]"
                    << ". Code = [ " << StCode.Code.c_str() << " ]"
                    << std::endl;
            }break;

            case 3025:
            {
                std::cout << "Check input Data no reference fount to Privided contract and from Address"
                    << ". Msg = [ " << StCode.Msg.c_str() << " ]"
                    << ". Code = [ " << StCode.Code.c_str() << " ]"
                    << std::endl;
            }break;

            default:
            {
                std::cout << "Token Send Transaction Hash = [ " << StCode.Hex.c_str() << " ]"
                    << ". Msg = [ " << StCode.Msg.c_str() << " ]"
                    << ". Code = [ " << StCode.Code.c_str() << " ]"
                    << std::endl;
            }
            }
        }
    }
    return StCode;
}