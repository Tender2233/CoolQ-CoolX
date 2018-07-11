#pragma once

#include <memory>

struct I_CQHandler
{
	virtual int32_t OnEvent_Startup() = 0;
	virtual int32_t OnEvent_Exit() = 0;
	virtual int32_t OnEvent_Enable() = 0;
	virtual int32_t OnEvent_Disable() = 0;
	virtual int32_t OnEvent_PrivateMsg(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, int32_t font) = 0;
	virtual int32_t OnEvent_GroupMsg(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) = 0;
	virtual int32_t OnEvent_DiscussMsg(int32_t subType, int32_t sendTime, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) = 0;
	virtual int32_t OnEvent_System_GroupAdmin(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) = 0;
	virtual int32_t OnEvent_System_GroupMemberDecrease(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) = 0;
	virtual int32_t OnEvent_System_GroupMemberIncrease(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) = 0;
	virtual int32_t OnEvent_Friend_Add(int32_t subType, int32_t sendTime, int64_t fromQQ) = 0;
	virtual int32_t OnEvent_Request_AddFriend(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) = 0;
	virtual int32_t OnEvent_Request_AddGroup(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) = 0;
	virtual int32_t OnEvent_Menu(const char* index = "") = 0;
	virtual bool    OnEvent_ReInit() = 0;
};

std::shared_ptr<I_CQHandler> CreateDefaultCQHandler(int32_t AuthCode);
