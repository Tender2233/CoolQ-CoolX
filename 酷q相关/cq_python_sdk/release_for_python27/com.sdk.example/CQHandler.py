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

#这里放置相关函数

import json
import re
welcome_text = "欢迎添加好友，输入帮助即可查看如何使用"
fail_to_connect_mysql_text = "连接数据库失败！请联系管理员！"
help_text = "首次使用请先绑定手机\n方法：直接输入手机号\n例：13012345678\n绑定后直接发送或分享链接即可"
error_phone_text = "手机号输入有误，请检查！"
error_url_text = "请输入正确的链接！"
share_error_url_text = "分享的链接有误！"
get_help_text = "输入帮助查看指令！"
no_support_type_text = "不支持的消息格式！输入帮助查看指令！"
doing_hongbao_text = "正在领取，请耐心等待10s！"
check_point_error_text = "查询点数失败,请检查你的命令格式和手机号"
fail_to_connect_server_text = "连接服务器失败！"
not_bind_id_text = "你还没有绑定手机号，直接输入手机号就可以绑定"
log_error_text="你的记录查询失败"


def check_points(str):
    try:
        str = str.strip("查询")
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
    str1 = "以下是你的点数日志，"
    str1 += phone
    str1 += "\n"
    for log in res:
        if log[2] < 0:
            str1 += "消耗"
        else:
            str1 += "增加"
        str1 += str(abs(log[2]))
        str1 += "点---"
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
    str1 = "以下是你的任务日志，"
    str1 += phone
    str1 += "\n"
    for log in res:
        str1 += "时间："
        str1 += log[6][5:]
        str1 += "，领取"
        if log[7]:
            str1 += "成功"
            str1 += "，领取到"
            str1 += str(log[3])
            str1 += "元红包"
        else:
            str1 += "失败"
        str1 += "\n"
    return str1

#函数结束

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
            CQSDK.SendPrivateMsg(fromQQ, msg+"回复成功")
        except Exception as e:
            logging.exception(e)

    def OnEvent_GroupMsg(self, subType, sendTime, fromGroup, fromQQ, fromAnonymous, msg, font):
        logging.info('OnEvent_GroupMsg: subType={0}, sendTime={1}, fromGroup={2}, fromQQ={3}, fromAnonymous={4}, msg={5}, font={6}'.format(subType, sendTime, fromGroup, fromQQ, fromAnonymous, msg, font))
        agenct=0
        if "测试" in msg:
            CQSDK.SendGroupMsg(fromGroup,"测试requests开始")
            testre=requests.get("http://www.baidu.com",timeout=30)
            CQSDK.SendGroupMsg(fromGroup,testre.status_code)
            return
        if fromGroup=="一键领取饿了么大红包" :#大群
            agenct="13968487277"
        elif fromGroup=="一键抢饿了么大红包群" :#三群
            agenct="18503801949"
        elif fromGroup=="饿了么帮抢红包群" :#二群
            agenct="18758582255"
        elif fromGroup=="一键领饿了么大红包群":
            agenct="13588041694"
        elif fromGroup=="学习资源赚钱平台":
            agenct="13182971295"
        phone = check_phone(msg)
        CQSDK.SendGroupMsg(fromGroup,phone)
        if not phone:
            phone = get_phone(fromQQ)
            if(phone==-1):
                CQSDK.SendGroupMsg(fromGroup,msg+"你还没有绑定手机，请输入手机号进行绑定。")
                CQSDK.SendGroupMsg(fromGroup,"phone=-1")#正式版删除
                return
        else:
            code, res = take_bind_id(phone,fromQQ, 1)
            CQSDK.SendGroupMsg(fromGroup,msg+"这里调用了take_bind_id")#正式版删除
            CQSDK.SendGroupMsg(fromGroup,res)
            return
        url = change_url(msg)
        if "查询" in msg:
            if not phone:
                phone = get_phone(fromQQ)
                if(phone==-1):
                    CQSDK.SendGroupMsg(fromGroup,not_bind_id_text)
                    return
            code, res = points_back(phone)
            CQSDK.SendGroupMsg(fromGroup,fromQQ + "：" + res)
            return
        if "充值" in content or"点数"in content and "查询" not in content:
            CQSDK.SendGroupMsg(fromGroup,"请前往http://hb.newitd.com/pay.html进行充值")
            return

        if url and "newitd" not in url:
            if (phone == -1):
                CQSDK.SendGroupMsg(fromGroup,fromQQ + "：" + not_bind_id_text)
                return
            if not phone:
                CQSDK.SendGroupMsg(fromGroup,fromQQ + "：" + not_bind_id_text)
                return
            url = check_url(url)
            if url:
                CQSDK.SendGroupMsg(fromGroup,fromQQ + "：" + doing_hongbao_text)
                code, res = bot_get_hongbao(fromQQ, 1, url,agenct)
                CQSDK.SendGroupMsg(fromGroup,fromQQ + "：" + res)
            else:
                CQSDK.SendGroupMsg(fromGroup,fromQQ + "：" + share_error_url_text)
            return

        try:
            CQSDK.SendGroupMsg(fromGroup, msg+"回复成功")
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
