#define CURL_STATICLIB

#include <iostream>
#include <fstream>
#include <curl/curl.h>

#include "API.h"

using namespace std;

/*
* libcurl info
In Project Properties -> VC++ Directories -> Include Directories add C:\curl\builds\libcurl-vc-x86-release-static-ipv6-sspi-winssl\include\
In Project Properties -> VC++ Directories -> Library Directories add C:\curl\builds\libcurl-vc-x86-release-static-ipv6-sspi-winssl\lib\ there
In Project Properties -> Linker -> Input -> Additional Dependencies add libcurl_a.lib, Ws2_32.lib, Crypt32.lib, Wldap32.lib and Normaliz.lib
*/

bool API::getLP()
{
	cout << "Enter mail on MOEX.\n";
	cin >> email;
	cout << "Enter password on MOEX.\n";
	cin >> password;
	return true;
}

bool API::auth(char email, char password)
{
	curl = curl_easy_init();
	if (curl) {
		CURLcode res;

		//set options for the curl
		curl_easy_setopt(curl, CURLOPT_URL, "http://iss.moex.com/iss/history/engines/stock/markets/shares/boards/tqbr/securities.xml?date=2013-12-20");
		//curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
		//curl_easy_setopt(curl, CURLOPT_USERNAME, email);
		//curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		return true;
	}
	return false;
}

bool API::getReq()
{
	getLP();
	auth(*email, *password);
	return true;
}

int main()
{
	API api, *p;
	p = &api;

	p->getReq();
	return 0;
}