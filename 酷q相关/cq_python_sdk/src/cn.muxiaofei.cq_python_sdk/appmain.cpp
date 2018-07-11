/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载

#include "CQHandler.h"

using namespace std;

int ac = -1; //AuthCode 调用酷Q的方法时需要用到
bool enabled = false;

std::shared_ptr<I_CQHandler> g_CQHandlerPtr = nullptr;

/* 
* 返回应用的ApiVer、Appid，打包后将不会调用
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* 接收应用AuthCode，酷Q读取应用信息后，如果接受该应用，将会调用这个函数并传递AuthCode。
* 不要在本函数处理其他任何代码，以免发生异常情况。如需执行初始化代码请在Startup事件中执行（Type=1001）。
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	g_CQHandlerPtr = CreateDefaultCQHandler(AuthCode);
	return 0;
}


/*
* Type=1001 酷Q启动
* 无论本应用是否被启用，本函数都会在酷Q启动后执行一次，请在这里执行应用初始化代码。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventStartup, 0)() {
	return g_CQHandlerPtr->OnEvent_Startup();
	//return 0;
}


/*
* Type=1002 酷Q退出
* 无论本应用是否被启用，本函数都会在酷Q退出前执行一次，请在这里执行插件关闭代码。
* 本函数调用完毕后，酷Q将很快关闭，请不要再通过线程等方式执行其他代码。
*/
CQEVENT(int32_t, __eventExit, 0)() {
	return g_CQHandlerPtr->OnEvent_Exit();
	//return 0;
}

/*
* Type=1003 应用已被启用
* 当应用被启用后，将收到此事件。
* 如果酷Q载入时应用已被启用，则在_eventStartup(Type=1001,酷Q启动)被调用后，本函数也将被调用一次。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	return g_CQHandlerPtr->OnEvent_Enable();
	//return 0;
}


/*
* Type=1004 应用将被停用
* 当应用被停用前，将收到此事件。
* 如果酷Q载入时应用已被停用，则本函数*不会*被调用。
* 无论本应用是否被启用，酷Q关闭前本函数都*不会*被调用。
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return g_CQHandlerPtr->OnEvent_Disable();
	//return 0;
}


/*
* Type=21 私聊消息
* subType 子类型，11/来自好友 1/来自在线状态 2/来自群 3/来自讨论组
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, int32_t font) {

	//如果要回复消息，请调用酷Q方法发送，并且这里 return EVENT_BLOCK - 截断本条消息，不再继续处理  注意：应用优先级设置为"最高"(10000)时，不得使用本返回值
	//如果不回复消息，交由之后的应用/过滤器处理，这里 return EVENT_IGNORE - 忽略本条消息
	return g_CQHandlerPtr->OnEvent_PrivateMsg(subType, sendTime, fromQQ, msg, font);
	//return EVENT_IGNORE;
}


/*
* Type=2 群消息
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
	return g_CQHandlerPtr->OnEvent_GroupMsg(subType, sendTime, fromGroup, fromQQ, fromAnonymous, msg, font);
	//return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=4 讨论组消息
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t sendTime, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {
	return g_CQHandlerPtr->OnEvent_DiscussMsg(subType, sendTime, fromDiscuss, fromQQ, msg, font);
	//return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=101 群事件-管理员变动
* subType 子类型，1/被取消管理员 2/被设置管理员
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {
	return g_CQHandlerPtr->OnEvent_System_GroupAdmin(subType, sendTime, fromGroup, beingOperateQQ);
	//return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=102 群事件-群成员减少
* subType 子类型，1/群员离开 2/群员被踢 3/自己(即登录号)被踢
* fromQQ 操作者QQ(仅subType为2、3时存在)
* beingOperateQQ 被操作QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {
	return g_CQHandlerPtr->OnEvent_System_GroupMemberDecrease(subType, sendTime, fromGroup, fromQQ, beingOperateQQ);
	//return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=103 群事件-群成员增加
* subType 子类型，1/管理员已同意 2/管理员邀请
* fromQQ 操作者QQ(即管理员QQ)
* beingOperateQQ 被操作QQ(即加群的QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {
	return g_CQHandlerPtr->OnEvent_System_GroupMemberIncrease(subType, sendTime, fromGroup, fromQQ, beingOperateQQ);
	//return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=201 好友事件-好友已添加
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {
	return g_CQHandlerPtr->OnEvent_Friend_Add(subType, sendTime, fromQQ);
	//return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=301 请求-好友添加
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//return CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");
	return g_CQHandlerPtr->OnEvent_Request_AddFriend(subType, sendTime, fromQQ, msg, responseFlag);
	//return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=302 请求-群添加
* subType 子类型，1/他人申请入群 2/自己(即登录号)受邀入群
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	return CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	return CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}
	return g_CQHandlerPtr->OnEvent_Request_AddGroup(subType, sendTime, fromGroup, fromQQ, msg, responseFlag);
	//return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}

/*
* 菜单，可在 .json 文件中设置菜单数目、函数名
* 如果不使用菜单，请在 .json 及此处删除无用菜单
*/
CQEVENT(int32_t, __menuReInit, 0)() {
	if (g_CQHandlerPtr->OnEvent_ReInit())
	{
		CQ_AddLog(CQLOG_INFOSUCCESS, "重新初始化", "重新初始化成功");
	} 
	else
	{
		CQ_AddLog(CQLOG_FATAL, "重新初始化", "重新初始化失败");
	}
	return 0;
}

CQEVENT(int32_t, __menu01, 0)() {
	return g_CQHandlerPtr->OnEvent_Menu("01");
	//return 0;
}

CQEVENT(int32_t, __menu02, 0)() {
	return g_CQHandlerPtr->OnEvent_Menu("02");
	//return 0;
}

CQEVENT(int32_t, __menu03, 0)() {
	return g_CQHandlerPtr->OnEvent_Menu("03");
	//return 0;
}

CQEVENT(int32_t, __menu04, 0)() {
	return g_CQHandlerPtr->OnEvent_Menu("04");
	//return 0;
}

CQEVENT(int32_t, __menu05, 0)() {
	return g_CQHandlerPtr->OnEvent_Menu("05");
	//return 0;
}

CQEVENT(int32_t, __menu06, 0)() {
	return g_CQHandlerPtr->OnEvent_Menu("06");
	//return 0;
}

CQEVENT(int32_t, __menu07, 0)() {
	return g_CQHandlerPtr->OnEvent_Menu("07");
	//return 0;
}

CQEVENT(int32_t, __menu08, 0)() {
	return g_CQHandlerPtr->OnEvent_Menu("08");
	//return 0;
}

CQEVENT(int32_t, __menu09, 0)() {
	return g_CQHandlerPtr->OnEvent_Menu("09");
	//return 0;
}

////////////////////////////////////////////////////////////////////////

void CQ_MessageBox(const char* msg, const char* title)
{
	::MessageBox(NULL, msg, title, MB_OK);
}

const char* CQ_GetAppID()
{
	return __CQ_APPID__.c_str();
}

int32_t CQ_SendPrivateMsg(int64_t QQID, const char *msg)
{
	return CQ_sendPrivateMsg(ac, QQID, msg);
}

int32_t CQ_SendGroupMsg(int64_t groupid, const char *msg)
{
	return CQ_sendGroupMsg(ac, groupid, msg);
}

int32_t CQ_SendDiscussMsg(int64_t discussid, const char *msg)
{
	return CQ_sendDiscussMsg(ac, discussid, msg);
}

int32_t CQ_SendLike(int64_t QQID)
{
	return CQ_sendLike(ac, QQID);
}

int32_t CQ_SetGroupKick(int64_t groupid, int64_t QQID, CQBOOL rejectaddrequest)
{
	return CQ_setGroupKick(ac, groupid, QQID, rejectaddrequest);
}

int32_t CQ_SetGroupBan(int64_t groupid, int64_t QQID, int64_t duration)
{
	return CQ_setGroupBan(ac, groupid, QQID, duration);
}

int32_t CQ_SetGroupAdmin(int64_t groupid, int64_t QQID, CQBOOL setadmin)
{
	return CQ_setGroupAdmin(ac, groupid, QQID, setadmin);
}

int32_t CQ_SetGroupWholeBan(int64_t groupid, CQBOOL enableban)
{
	return CQ_setGroupWholeBan(ac, groupid, enableban);
}

int32_t CQ_SetGroupAnonymousBan(int64_t groupid, const char *anomymous, int64_t duration)
{
	return CQ_setGroupAnonymousBan(ac, groupid, anomymous, duration);
}

int32_t CQ_SetGroupAnonymous(int64_t groupid, CQBOOL enableanomymous)
{
	return CQ_setGroupAnonymous(ac, groupid, enableanomymous);
}

int32_t CQ_SetGroupCard(int64_t groupid, int64_t QQID, const char *newcard)
{
	return CQ_setGroupCard(ac, groupid, QQID, newcard);
}

int32_t CQ_SetGroupLeave(int64_t groupid, CQBOOL isdismiss)
{
	return CQ_setGroupLeave(ac, groupid, isdismiss);
}

int32_t CQ_SetGroupSpecialTitle(int64_t groupid, int64_t QQID, const char *newspecialtitle, int64_t duration)
{
	return CQ_setGroupSpecialTitle(ac, groupid, QQID, newspecialtitle, duration);
}

int32_t CQ_SetDiscussLeave(int64_t discussid)
{
	return CQ_setDiscussLeave(ac, discussid);
}

int32_t CQ_SetFriendAddRequest(const char *responseflag, int32_t responseoperation, const char *remark)
{
	return CQ_setFriendAddRequest(ac, responseflag, responseoperation, remark);
}

int32_t CQ_SetGroupAddRequestV2(const char *responseflag, int32_t requesttype, int32_t responseoperation, const char *reason)
{
	return CQ_setGroupAddRequestV2(ac, responseflag, requesttype, responseoperation, reason);
}

const char * CQ_GetGroupMemberInfoV2(int64_t groupid, int64_t QQID, CQBOOL nocache)
{
	return CQ_getGroupMemberInfoV2(ac, groupid, QQID, nocache);
}

const char * CQ_GetGroupMemberList(int64_t groupid)
{
	return CQ_getGroupMemberList(ac, groupid);
}

const char * CQ_GetStrangerInfo(int64_t QQID, CQBOOL nocache)
{
	return CQ_getStrangerInfo(ac, QQID, nocache);
}

int32_t CQ_AddLog(int32_t priority, const char *category, const char *content)
{
	return CQ_addLog(ac, priority, category, content);
}

const char * CQ_GetCookies()
{
	return CQ_getCookies(ac);
}

int32_t CQ_GetCsrfToken()
{
	return CQ_getCsrfToken(ac);
}

int64_t CQ_GetLoginQQ()
{
	return CQ_getLoginQQ(ac);
}

const char * CQ_GetLoginNick()
{
	return CQ_getLoginNick(ac);
}

const char * CQ_GetAppDirectory()
{
	return CQ_getAppDirectory(ac);
}

int32_t CQ_SetFatal(const char *errorinfo)
{
	return CQ_setFatal(ac, errorinfo);
}
