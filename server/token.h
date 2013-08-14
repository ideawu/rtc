#ifndef RTC_SERVER_TOKEN_H
#define RTC_SERVER_TOKEN_H

#include <inttypes.h>
#include <string>
#include <map>

class TokenSvc
{
private:
	struct TokenItem
	{
		int room_id;
		uint32_t expired_at;
	};
	uint32_t timer_ticks;
	uint32_t last_clean_ticks;
	std::map<std::string, TokenItem> tokens;
	
public:
	TokenSvc();
	~TokenSvc();
	
	int tick(uint32_t timer_ticks);
	std::string create(int room_id, int ttl_sec);
	void destroy(const std::string &token);
	int check_and_destroy(int room_id, const std::string &token);
};

#endif
