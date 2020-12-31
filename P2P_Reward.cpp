#include <iostream>

#if defined(_WIN32) && defined(_WIN64)
	#include <corecrt_io.h>
    #include <io.h>
#else
	#include <cstdio>
	#include <sys/stat.h>
	#include <unistd.h>
    #include <dirent.h>
#endif

#include "cReward.h"

static std::string TempWalletAddr;
static std::string TempWalletPrivateKey;
static std::string TempWalletFilePath;

const char* SmartContract = ""; // 토큰의 스마트 컨트랙 주소
const char* APIKey = ""; // https://cryptoapis.io 에서 API key를 발급받아 적어 넣는다.
const char* Network_Type = "ropsten"; // 접속할 네트워크 타입
const char* PrivateKey = "";
const int RESEND_COUNT = 3;

int main(int argc, char* argv[])
{
    std::cout << "P2P_Reward Start!!!!!" << std::endl;

    short nState = 0;
    bool bState = false;

    cReward* pReward = new cReward();

    if (NULL == argv[1]) //
    {
        std::cout << "Program Argument is NULL" << std::endl;
        system("pause");
        exit(1);
    }
    else if (0 == strcmp(argv[1], "--help"))
    {
        std::cout << "" << std::endl;
        std::cout << " P2P_Reward.exe [type] if(type == 1){ [Your_Wallet_Address] [Your_Wallet_privateKey] }" << std::endl;
        std::cout << " if type = 1, need arguments(Your_Wallet_Address and Your_Wallet_privateKey)," << std::endl;
        std::cout << " if type = 2, it will used already inputted data(Your_Wallet_Address and Your_Wallet_privateKey)" << std::endl;
        std::cout << "" << std::endl;
        std::cout << " examples)" << std::endl;
        std::cout << " type is 1 : " << "P2P_Reward.exe 1 Your_Wallet_Address Your_Wallet_privateKey" << std::endl;
        std::cout << " type is 2 : " << "P2P_Reward.exe 2 " << std::endl;
    }
    else
    {
        if (argc < 2)
        {
            std::cout << "Program Argument is Enought" << std::endl;
            system("pause");
            exit(1);
        }
        else if (argc >= 2)
        {
            if (ARG_TYPE::ARGUMENT_TYPE == atoi(argv[1]))
            {
                // 첫번째 Argument 값이 1일 경우에는 직접 Argument를 받는다

                // argv[2] 와 argv[3] 값이 NULL 이 아닌지 체크
                if (NULL == argv[2])
                {
                    //마스터 지갑 주소가 입력이 안되었다.
                    std::cout << "Master Wallet Address Argument is NULL" << std::endl;
                    system("pause");
                    exit(1);
                }
                else if (NULL == argv[3])
                {
                    //마스터 지갑의 Privatekey가 입력이 안되었다.
                    std::cout << "Master Wallet PrivateKey Argument is NULL" << std::endl;
                    system("pause");
                    exit(1);
                }
                else
                {
                    // 둘다 입력이 되었다.
                    std::cout << "둘다 입력이 되어서 " << std::endl;
                    nState = ARG_TYPE::ARGUMENT_TYPE;
                    bState = true;
                }
            }
            else if (ARG_TYPE::FILE_TYPE == atoi(argv[1]))
            {
                // File로 부터 읽는 타입일때
                nState = ARG_TYPE::FILE_TYPE;
                bState = true;
            }
            else
            {
                std::cout << "Program Argument is Worng" << std::endl;
                system("pause");
                exit(1);
            }
        }

        if (nState == ARG_TYPE::ARGUMENT_TYPE)
        {
            std::cout << "Argument Count : " << argc << std::endl;
            std::cout << "Master Wallet Address : " << argv[2] << std::endl;
            std::cout << "Master Wallet Privatekey : " << argv[3] << std::endl;

            TempWalletAddr.assign(argv[2]);
            TempWalletPrivateKey.assign(argv[3]);
        }
        else
        {
            // 지갑 파일을 읽는다.
            TempWalletFilePath.assign(pReward->GetWalletInfoPath());
            
            if (-1 == access(TempWalletFilePath.c_str(), 00))
            {
                // 지갑 파일이 존재하지 않는다.
                std::cout << "Master Wallet Info File Not Exists" << std::endl;
                system("pause");
                exit(1);
            }
            else
            {
                bState = pReward->WalletInfoRead(TempWalletFilePath, TempWalletAddr, TempWalletPrivateKey);
                if (true != bState)
                {
                    std::cout << "Master Wallet Info Not Load" << std::endl;
                    std::cout << "Path = " << TempWalletFilePath.c_str() << std::endl;
                    system("pause");
                    exit(1);
                }
                if (true == pReward->m_pRewardInfo->Master_Wallet_Addr.empty() || true == pReward->m_pRewardInfo->Master_Wallet_Privatekey.empty())
                {
                    std::cout << "Master Wallet Info File is Empty" << std::endl;
                    system("pause");
                    exit(1);
                }
            }
        }

        if (true == bState)
        {
            // for 문 돌려서 resultinfo_*.json 파일의 목록을 Vector에 담는다.
            std::vector<std::string> TempResult = pReward->Get_Files_inDirectory(pReward->GetResultInfoPath(), "result_?????.json");

            for (int i = 0; i < TempResult.size(); ++i)
            {
                std::string ResultFileName(TempResult[i].c_str());
                std::string PathName = pReward->GetResultInfoPath();
                PathName = PathName + ResultFileName;
                std::cout << "Read ResultInfo Json File : [ " << PathName.c_str() << " ] " << std::endl;
                if (true != pReward->ResultInfoRead(PathName, TempWalletAddr, TempWalletPrivateKey, pReward->m_nTotalRewardCoin))
                {
                    std::cout << "Reward Result Info Not Found" << std::endl;
                    system("pause");
                    exit(1);
                }
                else
                {
                    pReward->GetRewardInfo(PathName);
                }
            }
            // 토큰 밸런스 추출
            long double nTotal_Token = pReward->TokenBalance();
            __int64 nGas_Fee, nGas_Limit;

            if (nTotal_Token <= 0 || nTotal_Token < pReward->m_nTotalRewardCoin)
            {
                //토큰이 부족하다 
                std::cout << "Your Token is Not Enoughth, Token Amount = [ " << nTotal_Token << " ]" << std::endl;
                system("pause");
                exit(1);
            }
            else
            {
                std::cout << "Your Token Amount = [ " << static_cast<__int64>(nTotal_Token) << " ]" << std::endl;

                nGas_Fee = pReward->GetGasFee();

                if (nGas_Fee < 0)
                {
                    std::cout << "Gas Fee is Lower, Gas Fee = [ " << nGas_Fee << std::endl;
                    system("pause");
                    exit(1);
                }

                nGas_Limit = pReward->GetGasLimit();

                if (nGas_Limit < 0)
                {
                    std::cout << "Gas Limit is Lower, Gas Limit = [ " << nGas_Limit << std::endl;
                    system("pause");
                    exit(1);
                }

                pReward->Token_Transfer("");

            }

            if (false == pReward->bTransfer_State)
            {
                std::string RewardResultName("Reward_Result.json");
                //std::string RewardResultName("Reward_Result_");
                //ResultFileName = ResultFileName.replace(ResultFileName.begin(), ResultFileName.end() + 11, "");
                //RewardResultName.append(ResultFileName);

                while (false == pReward->bTransfer_State)
                {
                    if (true != pReward->RewardResultInfoRead(pReward->GetRewardResultInfoPath(RewardResultName)))
                    //if (true != pReward->RewardResultInfoRead(pReward->GetRewardResultInfoPath(ResultFileName)))
                    {
                        std::cout << "Master Wallet Info Not Load" << std::endl;
                        system("pause");
                        exit(1);
                    }

                    pReward->GetReRewardInfo(RewardResultName);
                    pReward->Token_ReTransfer(RewardResultName);
                }
            }
            std::cout << "Reward Success" << std::endl;
        }
    }
}