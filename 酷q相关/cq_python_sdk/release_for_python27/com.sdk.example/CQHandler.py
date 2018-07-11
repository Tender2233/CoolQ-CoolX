# -*- coding:gbk -*-

import sys
import requests
reload(sys)
sys.setdefaultencoding('gbk')

import os
import logging
logging.basicConfig(
    level       = logging.INFO,
    format      = '%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
    datefmt     = '%Y-%m-%d %H:%M:%S',
    filename    = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'CQHanlder.log'),
    filemode    = 'w+'
)

#���������غ���

import json
import re
welcome_text = "��ӭ��Ӻ��ѣ�����������ɲ鿴���ʹ��"
fail_to_connect_mysql_text = "�������ݿ�ʧ�ܣ�����ϵ����Ա��"
help_text = "�״�ʹ�����Ȱ��ֻ�\n������ֱ�������ֻ���\n����13012345678\n�󶨺�ֱ�ӷ��ͻ�������Ӽ���"
error_phone_text = "�ֻ��������������飡"
error_url_text = "��������ȷ�����ӣ�"
share_error_url_text = "�������������"
get_help_text = "��������鿴ָ�"
no_support_type_text = "��֧�ֵ���Ϣ��ʽ����������鿴ָ�"
doing_hongbao_text = "������ȡ�������ĵȴ�10s��"
check_point_error_text = "��ѯ����ʧ��,������������ʽ���ֻ���"
fail_to_connect_server_text = "���ӷ�����ʧ�ܣ�"
not_bind_id_text = "�㻹û�а��ֻ��ţ�ֱ�������ֻ��žͿ��԰�"
log_error_text="��ļ�¼��ѯʧ��"


def check_points(str):
    try:
        str = str.strip("��ѯ")
        res = re.search('(1[0-9]{10})', str).group(1)
        return res
    except:
        return False

def check_url(url):
    try:
        url = url.strip()
        res = re.search('(https://h5.ele.me/hongbao/.*)', url).group(1)
        return res
    except:
        return False

def check_phone(phone):
    try:
        phone = phone.strip()
        res = re.search('(1[0-9]{10})', phone).group(1)
        return res
    except:
        return False

def change_url(url):
    try:
        url = url.strip()
        res = re.search('(https*://.*)', url).group(1)
        result = requests.get(res)
        url = result.url
        return url
    except:
        return False

def get_phone(id):
    try:
        code, res = get_bind_id(id, 1)
    except:
        return -1
    if code != 1:
        phone = None
    else:
        phone = res

    return phone

def points_back(phone):
    data = {
        "phone": phone,
    }
    r = requests.post("http://hb-user-api.newitd.com/get_user_info", data, timeout=30)
    if r.status_code == 200:
        js_res = json.loads(r.text)
        return js_res["code"], js_res["info"]
    else:
        return -500, fail_to_connect_server_text

def take_bind_id(phone, id, type):
    data = {
        "phone": phone,
        "id": id,
        #"type": type
    }
    r = requests.post("http://hb-user-api.newitd.com/bind_qq_id", data, timeout=30)
    if r.status_code == 200:
        js_res = json.loads(r.text)
        return js_res["code"], js_res["info"]
    else:
        return -500, fail_to_connect_server_text

def get_bind_id(id, type):
    data = {
        "id": id,
        #"type": type
    }
    r = requests.post("http://hb-user-api.newitd.com/get_qq_id", data, timeout=30)
    if r.status_code == 200:
        js_res = json.loads(r.text)
        if js_res["code"] != 1:
            return js_res["code"], js_res["info"]
        else:
            return js_res["code"], js_res["phone"]
    else:
        return -500, fail_to_connect_server_text

def bot_get_hongbao(id, type, url,agenct):
    code, phone = get_bind_id(id, type)
    if code != 1:
        return code, not_bind_id_text

    data = {
        "phone": phone,
        "url": url,
        "agency":agenct,
    }
    r = requests.post("http://hb-api.newitd.com/hongbao", data, timeout=30)
    if r.status_code == 200:
        json_result = r.text[5:-1]
        js_res = json.loads(r.text)
        return js_res["code"], js_res["info"]
    else:
        return -500, fail_to_connect_server_text

def get_log(phone):
    data = {
        "phone": phone,
        "limit": 20
    }
    r = requests.get("http://hb-api.newitd.com/get_user_log", data, timeout=30)
    if r.status_code == 200:
        json_result = r.text[5:-1]
        js_res = json.loads(json_result)
        return js_res["code"], js_res["res"]
    else:
        return -500, fail_to_connect_server_text

def format_log(res, phone):
    str1 = "��������ĵ�����־��"
    str1 += phone
    str1 += "\n"
    for log in res:
        if log[2] < 0:
            str1 += "����"
        else:
            str1 += "����"
        str1 += str(abs(log[2]))
        str1 += "��---"
        str1 += log[4][5:]
        str1 += "\n"
    return str1

def get_task(phone):
    data = {
        "phone": phone,
        "limit": 10
    }
    r = requests.get("http://hb-api.newitd.com/get_user_task", data, timeout=30)

    if r.status_code == 200:
        json_result = r.text[5:-1]
        js_res = json.loads(json_result)
        return js_res["code"], js_res["res"]
    else:
        return -500, fail_to_connect_server_text

def format_task(res, phone):
    str1 = "���������������־��"
    str1 += phone
    str1 += "\n"
    for log in res:
        str1 += "ʱ�䣺"
        str1 += log[6][5:]
        str1 += "����ȡ"
        if log[7]:
            str1 += "�ɹ�"
            str1 += "����ȡ��"
            str1 += str(log[3])
            str1 += "Ԫ���"
        else:
            str1 += "ʧ��"
        str1 += "\n"
    return str1

#��������

import CQSDK

class CQHandler(object):
    def __init__(self):
        logging.info('__init__')
        
    def __del__(self):
        logging.info('__del__')
        
    def OnEvent_Enable(self):
        logging.info('OnEvent_Enable')

    def OnEvent_Disable(self):
        logging.info('OnEvent_Disable')

    def OnEvent_PrivateMsg(self, subType, sendTime, fromQQ, msg, font):
        logging.info('OnEvent_PrivateMsg: subType={0}, sendTime={1}, fromQQ={2}, msg={3}, font={4}'.format(subType, sendTime, fromQQ, msg, font))

        try:
            CQSDK.SendPrivateMsg(fromQQ, msg+"�ظ��ɹ�")
        except Exception as e:
            logging.exception(e)

    def OnEvent_GroupMsg(self, subType, sendTime, fromGroup, fromQQ, fromAnonymous, msg, font):
        logging.info('OnEvent_GroupMsg: subType={0}, sendTime={1}, fromGroup={2}, fromQQ={3}, fromAnonymous={4}, msg={5}, font={6}'.format(subType, sendTime, fromGroup, fromQQ, fromAnonymous, msg, font))
        agenct=0
        if "����" in msg:
            CQSDK.SendGroupMsg(fromGroup,"����requests��ʼ")
            testre=requests.get("http://www.baidu.com",timeout=30)
            CQSDK.SendGroupMsg(fromGroup,testre.status_code)
            return
        if fromGroup=="һ����ȡ����ô����" :#��Ⱥ
            agenct="13968487277"
        elif fromGroup=="һ��������ô����Ⱥ" :#��Ⱥ
            agenct="18503801949"
        elif fromGroup=="����ô�������Ⱥ" :#��Ⱥ
            agenct="18758582255"
        elif fromGroup=="һ�������ô����Ⱥ":
            agenct="13588041694"
        elif fromGroup=="ѧϰ��Դ׬Ǯƽ̨":
            agenct="13182971295"
        phone = check_phone(msg)
        CQSDK.SendGroupMsg(fromGroup,phone)
        if not phone:
            phone = get_phone(fromQQ)
            if(phone==-1):
                CQSDK.SendGroupMsg(fromGroup,msg+"�㻹û�а��ֻ����������ֻ��Ž��а󶨡�")
                CQSDK.SendGroupMsg(fromGroup,"phone=-1")#��ʽ��ɾ��
                return
        else:
            code, res = take_bind_id(phone,fromQQ, 1)
            CQSDK.SendGroupMsg(fromGroup,msg+"���������take_bind_id")#��ʽ��ɾ��
            CQSDK.SendGroupMsg(fromGroup,res)
            return
        url = change_url(msg)
        if "��ѯ" in msg:
            if not phone:
                phone = get_phone(fromQQ)
                if(phone==-1):
                    CQSDK.SendGroupMsg(fromGroup,not_bind_id_text)
                    return
            code, res = points_back(phone)
            CQSDK.SendGroupMsg(fromGroup,fromQQ + "��" + res)
            return
        if "��ֵ" in content or"����"in content and "��ѯ" not in content:
            CQSDK.SendGroupMsg(fromGroup,"��ǰ��http://hb.newitd.com/pay.html���г�ֵ")
            return

        if url and "newitd" not in url:
            if (phone == -1):
                CQSDK.SendGroupMsg(fromGroup,fromQQ + "��" + not_bind_id_text)
                return
            if not phone:
                CQSDK.SendGroupMsg(fromGroup,fromQQ + "��" + not_bind_id_text)
                return
            url = check_url(url)
            if url:
                CQSDK.SendGroupMsg(fromGroup,fromQQ + "��" + doing_hongbao_text)
                code, res = bot_get_hongbao(fromQQ, 1, url,agenct)
                CQSDK.SendGroupMsg(fromGroup,fromQQ + "��" + res)
            else:
                CQSDK.SendGroupMsg(fromGroup,fromQQ + "��" + share_error_url_text)
            return

        try:
            CQSDK.SendGroupMsg(fromGroup, msg+"�ظ��ɹ�")
        except Exception as e:
            logging.exception(e)

    def OnEvent_DiscussMsg(self, subType, sendTime, fromDiscuss, fromQQ, msg, font):
        logging.info('OnEvent_DiscussMsg: subType={0}, sendTime={1}, fromDiscuss={2}, fromQQ={3}, msg={4}, font={5}'.format(subType, sendTime, fromDiscuss, fromQQ, msg, font))

    def OnEvent_System_GroupAdmin(self, subType, sendTime, fromGroup, beingOperateQQ):
        logging.info('OnEvent_System_GroupAdmin: subType={0}, sendTime={1}, fromGroup={2}, beingOperateQQ={3}'.format(subType, sendTime, fromGroup, beingOperateQQ))

    def OnEvent_System_GroupMemberDecrease(self, subType, sendTime, fromGroup, fromQQ, beingOperateQQ):
        logging.info('OnEvent_System_GroupMemberDecrease: subType={0}, sendTime={1}, fromGroup={2}, fromQQ={3}, beingOperateQQ={4}'.format(subType, sendTime, fromGroup, fromQQ, beingOperateQQ))

    def OnEvent_System_GroupMemberIncrease(self, subType, sendTime, fromGroup, fromQQ, beingOperateQQ):
        logging.info('OnEvent_System_GroupMemberIncrease: subType={0}, sendTime={1}, fromGroup={2}, fromQQ={3}, beingOperateQQ={4}'.format(subType, sendTime, fromGroup, fromQQ, beingOperateQQ))

    def OnEvent_Friend_Add(self, subType, sendTime, fromQQ):
        logging.info('OnEvent_Friend_Add: subType={0}, sendTime={1}, fromQQ={2}'.format(subType, sendTime, fromQQ))

    def OnEvent_Request_AddFriend(self, subType, sendTime, fromQQ, msg, responseFlag):
        logging.info('OnEvent_Request_AddFriend: subType={0}, sendTime={1}, fromQQ={2}, msg={3}, responseFlag={4}'.format(subType, sendTime, fromQQ, msg, responseFlag))

    def OnEvent_Request_AddGroup(self, subType, sendTime, fromGroup, fromQQ, msg, responseFlag):
        logging.info('OnEvent_Request_AddGroup: subType={0}, sendTime={1}, fromGroup={2}, fromQQ={3}, msg={4}, responseFlag={5}'.format(subType, sendTime, fromGroup, fromQQ, msg, responseFlag))

    def OnEvent_Menu01(self):
        logging.info('OnEvent_Menu01')

    def OnEvent_Menu02(self):
        logging.info('OnEvent_Menu02')

    def OnEvent_Menu03(self):
        logging.info('OnEvent_Menu03')

    def OnEvent_Menu04(self):
        logging.info('OnEvent_Menu04')

    def OnEvent_Menu05(self):
        logging.info('OnEvent_Menu05')

    def OnEvent_Menu06(self):
        logging.info('OnEvent_Menu06')

    def OnEvent_Menu07(self):
        logging.info('OnEvent_Menu07')

    def OnEvent_Menu08(self):
        logging.info('OnEvent_Menu08')

    def OnEvent_Menu09(self):
        logging.info('OnEvent_Menu09')
