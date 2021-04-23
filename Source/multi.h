/**
 * @file multi.h
 *
 * Interface of functions for keeping multiplayer games in sync.
 */
#pragma once

#include <stdint.h>

#include "msg.h"

namespace devilution {

// must be unsigned to generate unsigned comparisons with pnum
#define MAX_PLRS 4

enum event_type : uint8_t {
	EVENT_TYPE_PLAYER_CREATE_GAME,
	EVENT_TYPE_PLAYER_LEAVE_GAME,
	EVENT_TYPE_PLAYER_MESSAGE,
};

#pragma pack(push, 1)
struct GameData {
	Sint32_NET size;
	Sint32_NET dwSeed;
	Uint32_NET programid;
	Uint8 versionMajor;
	Uint8 versionMinor;
	Uint8 versionPatch;
	_difficulty nDifficulty;
	Uint8 nTickRate;
	Uint8 bRunInTown;
	Uint8 bTheoQuest;
	Uint8 bCowQuest;
	Uint8 bFriendlyFire;
};
#pragma pack(pop)

extern bool gbSomebodyWonGameKludge;
extern char szPlayerDescript[128];
extern WORD sgwPackPlrOffsetTbl[MAX_PLRS];
extern BYTE gbActivePlayers;
extern bool gbGameDestroyed;
extern GameData sgGameInitInfo;
extern bool gbSelectProvider;
extern bool gbIsMultiplayer;
extern char szPlayerName[128];
extern BYTE gbDeltaSender;
extern int player_state[MAX_PLRS];

void multi_msg_add(BYTE *pbMsg, BYTE bLen);
void NetSendLoPri(int playerId, BYTE *pbMsg, BYTE bLen);
void NetSendHiPri(int playerId, BYTE *pbMsg, BYTE bLen);
void multi_send_msg_packet(uint32_t pmask, BYTE *src, BYTE len);
void multi_msg_countdown();
void multi_player_left(int pnum, int reason);
void multi_net_ping();
bool multi_handle_delta();
void multi_process_network_packets();
void multi_send_zero_packet(int pnum, _cmd_id bCmd, BYTE *pbSrc, DWORD dwLen);
void NetClose();
bool NetInit(bool bSinglePlayer);
bool multi_init_single(GameData *gameData);
bool multi_init_multi(GameData *gameData);
void recv_plrinfo(int pnum, TCmdPlrInfoHdr *p, bool recv);

} // namespace devilution
