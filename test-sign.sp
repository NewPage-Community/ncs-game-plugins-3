#include <ncs>
#include <ncs/sign>
#include <ncs/money>
#include <ncs/chat>

#define P_NAME P_PRE ... " - Sign reward"
#define P_DESC "Sign reward plugin"

public Plugin myinfo = 
{
	name        = P_NAME,
	author      = P_AUTHOR,
	description = P_DESC,
	version     = P_VERSION,
	url         = P_URLS
};

public void NCS_Sign_OnUserSigned(int client)
{
    NCS_Money_Give(client, 10000, "签到奖励");
    NCS_Chat(client, _, "{blue}感谢参与测试，奖励你 {green}10000软妹币");
}