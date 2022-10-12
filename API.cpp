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


bool API::auth()
{
	CURLcode res;
	FILE *f_res;
	char outfilename[FILENAME_MAX] = "C:\\Users\\Admin\\Desktop\\MOEXApiTryout.txt";

	curl = curl_easy_init();
	if (curl) {


		fopen_s(&f_res, outfilename, "w");

		//set options for the curl
		curl_easy_setopt(curl, CURLOPT_URL, "http://iss.moex.com/iss/history/engines/stock/markets/shares/boards/tqbr/securities.xml?date=2013-12-20"); //from where
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL); //write funcrion
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, f_res); //to where write
		//curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
		//curl_easy_setopt(curl, CURLOPT_USERNAME, email);
		//curl_easy_setopt(curl, CURLOPT_PASSWORD, password);

		res = curl_easy_perform(curl); // here i get the .xml response
		curl_easy_cleanup(curl);
		fclose(f_res);
		return true;
	}
	return false;
}

bool API::getReq()
{
	auth();
	return true;
}

int main()
{
	API api, *p;
	p = &api;

	p->getReq();
	return 0;
}