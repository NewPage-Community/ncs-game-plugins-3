#if defined _NCS_Include_
	#endinput
#endif
#define _NCS_Include_

#pragma newdecls required

// Pluing info
#define P_AUTHOR 		"NewPage Dev-Team"
#define P_URLS	 		"https://new-page.xyz"
#define P_PRE 			"NewPage Core System (NCS)"
#if defined DEV
	#define P_VERSION "v3.0.dev"
#else
    #define P_VERSION "v3.0.<pipeline_iid>"
#endif

// Core Include
#include <ncs/api>
#include <ncs/color>
#include <ncs/client>
#include <ncs/log>
#include <ncs/menu>
#include <ncs/utils>