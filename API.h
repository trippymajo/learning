#define CURL_STATICLIB
#include <curl/curl.h>

class API
{
public:
	char email[128];
	char password[128];

public:
	CURL* curl;

	bool auth();
	bool getReq();
};
