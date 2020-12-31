#pragma once

#include "Common.h"
#include "cJSON.h"

static size_t WriteMemoryCallback(char* contents, size_t size, size_t nmemb, std::string* out)
{
	std::size_t total_size = size * nmemb;
	if (total_size)
	{
		out->append(contents, total_size);
		return total_size;
	}
	return 0;
}

class cURL
{
public:
	cURL();
	~cURL();

	inline void InitCURL()
	{
		m_headerlist = nullptr;
		curl_global_init(CURL_GLOBAL_ALL);
		m_curl = curl_easy_init();
	}

	inline void CleanCURL()
	{
		m_strResourceJSON.clear();
		m_str_buffer.clear();

		curl_easy_cleanup(m_curl);
		curl_slist_free_all(m_headerlist);
		curl_global_cleanup();
	}

	inline void SetCurlHeaderList(const std::string _Content_Type) 
	{
		m_headerlist = curl_slist_append(m_headerlist, _Content_Type.c_str());		
	};

	inline void SetUrl(const std::string _Url)
	{
		m_strTargetURL.assign(_Url.c_str());
	};

	inline void AppendUrl(const std::string _Url)
	{
		m_strTargetURL.append(_Url.c_str());
	}

	long double Get_TokenBalance(const std::string _Param, const std::string _Param2, const std::string _Param3, cJSON* Json);
	
	__int64 Get_GasFee(const std::string _Param, cJSON* Json);
	__int64 Get_GasLimit(std::string _Param, std::string _Param2, std::string _Param3, cJSON* Json);

	StErrorCode Transfer_Reward(const std::string _Param, const std::string _Param2, const std::string _Param3, const std::string _Param4, const std::string _Param5, const std::string _Param6, const std::string _Param7, const std::string _Param8, const int _ncount, cJSON* Json);

private:
	CURL* m_curl;
	CURLcode m_res;

	std::string m_strTargetURL;
	std::map<int, std::string>	m_ResourceMap;
	std::string m_strResourceJSON;
	std::string m_str_buffer;
	struct curl_slist* m_headerlist;
};

