#pragma once

#define CQAPPID (__CQ_APPID__.c_str())
#define CQAPPINFO (__CQ_APPINFO__.c_str())

void CQ_MessageBox(const char* msg, const char* title);
const char* CQ_GetAppID();

int32_t CQ_SendPrivateMsg(int64_t QQID, const char *msg);
int32_t CQ_SendGroupMsg(int64_t groupid, const char *msg);
int32_t CQ_SendDiscussMsg(int64_t discussid, const char *msg);
int32_t CQ_SendLike(int64_t QQID);
int32_t CQ_SetGroupKick(int64_t groupid, int64_t QQID, CQBOOL rejectaddrequest);
int32_t CQ_SetGroupBan(int64_t groupid, int64_t QQID, int64_t duration);
int32_t CQ_SetGroupAdmin(int64_t groupid, int64_t QQID, CQBOOL setadmin);
int32_t CQ_SetGroupWholeBan(int64_t groupid, CQBOOL enableban);
int32_t CQ_SetGroupAnonymousBan(int64_t groupid, const char *anomymous, int64_t duration);
int32_t CQ_SetGroupAnonymous(int64_t groupid, CQBOOL enableanomymous);
int32_t CQ_SetGroupCard(int64_t groupid, int64_t QQID, const char *newcard);
int32_t CQ_SetGroupLeave(int64_t groupid, CQBOOL isdismiss);
int32_t CQ_SetGroupSpecialTitle(int64_t groupid, int64_t QQID, const char *newspecialtitle, int64_t duration);
int32_t CQ_SetDiscussLeave(int64_t discussid);
int32_t CQ_SetFriendAddRequest(const char *responseflag, int32_t responseoperation, const char *remark);
int32_t CQ_SetGroupAddRequestV2(const char *responseflag, int32_t requesttype, int32_t responseoperation, const char *reason);
const char * CQ_GetGroupMemberInfoV2(int64_t groupid, int64_t QQID, CQBOOL nocache);
const char * CQ_GetGroupMemberList(int64_t groupid);
const char * CQ_GetStrangerInfo(int64_t QQID, CQBOOL nocache);
int32_t CQ_AddLog(int32_t priority, const char *category, const char *content);
const char * CQ_GetCookies();
int32_t CQ_GetCsrfToken();
int64_t CQ_GetLoginQQ();
const char * CQ_GetLoginNick();
const char * CQ_GetAppDirectory();
int32_t CQ_SetFatal(const char *errorinfo);
