/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����

#include "CQHandler.h"

using namespace std;

int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;

std::shared_ptr<I_CQHandler> g_CQHandlerPtr = nullptr;

/* 
* ����Ӧ�õ�ApiVer��Appid������󽫲������
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* ����Ӧ��AuthCode����Q��ȡӦ����Ϣ��������ܸ�Ӧ�ã���������������������AuthCode��
* ��Ҫ�ڱ��������������κδ��룬���ⷢ���쳣���������ִ�г�ʼ����������Startup�¼���ִ�У�Type=1001����
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	g_CQHandlerPtr = CreateDefaultCQHandler(AuthCode);
	return 0;
}


/*
* Type=1001 ��Q����
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q������ִ��һ�Σ���������ִ��Ӧ�ó�ʼ�����롣
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventStartup, 0)() {
	return g_CQHandlerPtr->OnEvent_Startup();
	//return 0;
}


/*
* Type=1002 ��Q�˳�
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q�˳�ǰִ��һ�Σ���������ִ�в���رմ��롣
* ������������Ϻ󣬿�Q���ܿ�رգ��벻Ҫ��ͨ���̵߳ȷ�ʽִ���������롣
*/
CQEVENT(int32_t, __eventExit, 0)() {
	return g_CQHandlerPtr->OnEvent_Exit();
	//return 0;
}

/*
* Type=1003 Ӧ���ѱ�����
* ��Ӧ�ñ����ú󣬽��յ����¼���
* �����Q����ʱӦ���ѱ����ã�����_eventStartup(Type=1001,��Q����)�����ú󣬱�����Ҳ��������һ�Ρ�
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	return g_CQHandlerPtr->OnEvent_Enable();
	//return 0;
}


/*
* Type=1004 Ӧ�ý���ͣ��
* ��Ӧ�ñ�ͣ��ǰ�����յ����¼���
* �����Q����ʱӦ���ѱ�ͣ�ã��򱾺���*����*�����á�
* ���۱�Ӧ���Ƿ����ã���Q�ر�ǰ��������*����*�����á�
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return g_CQHandlerPtr->OnEvent_Disable();
	//return 0;
}


/*
* Type=21 ˽����Ϣ
* subType �����ͣ�11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/����������
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, int32_t font) {

	//���Ҫ�ظ���Ϣ������ÿ�Q�������ͣ��������� return EVENT_BLOCK - �ضϱ�����Ϣ�����ټ�������  ע�⣺Ӧ�����ȼ�����Ϊ"���"(10000)ʱ������ʹ�ñ�����ֵ
	//������ظ���Ϣ������֮���Ӧ��/�������������� return EVENT_IGNORE - ���Ա�����Ϣ
	return g_CQHandlerPtr->OnEvent_PrivateMsg(subType, sendTime, fromQQ, msg, font);
	//return EVENT_IGNORE;
}


/*
* Type=2 Ⱥ��Ϣ
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
	return g_CQHandlerPtr->OnEvent_GroupMsg(subType, sendTime, fromGroup, fromQQ, fromAnonymous, msg, font);
	//return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=4 ��������Ϣ
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t sendTime, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {
	return g_CQHandlerPtr->OnEvent_DiscussMsg(subType, sendTime, fromDiscuss, fromQQ, msg, font);
	//return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=101 Ⱥ�¼�-����Ա�䶯
* subType �����ͣ�1/��ȡ������Ա 2/�����ù���Ա
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {
	return g_CQHandlerPtr->OnEvent_System_GroupAdmin(subType, sendTime, fromGroup, beingOperateQQ);
	//return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=102 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/ȺԱ�뿪 2/ȺԱ���� 3/�Լ�(����¼��)����
* fromQQ ������QQ(��subTypeΪ2��3ʱ����)
* beingOperateQQ ������QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {
	return g_CQHandlerPtr->OnEvent_System_GroupMemberDecrease(subType, sendTime, fromGroup, fromQQ, beingOperateQQ);
	//return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=103 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/����Ա��ͬ�� 2/����Ա����
* fromQQ ������QQ(������ԱQQ)
* beingOperateQQ ������QQ(����Ⱥ��QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {
	return g_CQHandlerPtr->OnEvent_System_GroupMemberIncrease(subType, sendTime, fromGroup, fromQQ, beingOperateQQ);
	//return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=201 �����¼�-���������
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {
	return g_CQHandlerPtr->OnEvent_Friend_Add(subType, sendTime, fromQQ);
	//return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=301 ����-�������
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//return CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");
	return g_CQHandlerPtr->OnEvent_Request_AddFriend(subType, sendTime, fromQQ, msg, responseFlag);
	//return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=302 ����-Ⱥ���
* subType �����ͣ�1/����������Ⱥ 2/�Լ�(����¼��)������Ⱥ
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	return CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	return CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}
	return g_CQHandlerPtr->OnEvent_Request_AddGroup(subType, sendTime, fromGroup, fromQQ, msg, responseFlag);
	//return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}

/*
* �˵������� .json �ļ������ò˵���Ŀ��������
* �����ʹ�ò˵������� .json ���˴�ɾ�����ò˵�
*/
CQEVENT(int32_t, __menuReInit, 0)() {
	if (g_CQHandlerPtr->OnEvent_ReInit())
	{
		CQ_AddLog(CQLOG_INFOSUCCESS, "���³�ʼ��", "���³�ʼ���ɹ�");
	} 
	else
	{
		CQ_AddLog(CQLOG_FATAL, "���³�ʼ��", "���³�ʼ��ʧ��");
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
