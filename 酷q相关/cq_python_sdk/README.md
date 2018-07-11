#cn.muxiaofei.cq_python_sdk

#使用方法：
1. 将文件cn.muxiaofei.cq_pythonxx_sdk.dll, cn.muxiaofei.cq_pythonxx_sdk.json以及目录cn.muxiaofei.cq_pythonxx_sdk复制到酷Q的app目录下 (pythonxx表示python27或者python35, 目前只提供了python27版本)
2. 将以上三个文件（夹）的名称统一改为你自己的appid，例如：com.sdk.example.dll, com.sdk.example.json以及com.sdk.example
3. 编辑cn.muxiaofei.cq_pythonxx_sdk目录下的CQHandler.py，实现自己的处理逻辑
4. 程序基于VC++ 2010开发，所以需要在使用此SDK的机器上安装 vcredist_2010_x86.exe

#开发教程：
1. cn.muxiaofei.cq_pythonxx_sdk目录下的Python27目录是基于官方Python 2.7.12提取的绿色版Python运行时，包含官方安装包的全部功能，如果需要添加额外的第三方包请放置于Python27\Lib目录下
2. CQSDK.py是对于酷Q SDK的封装，示例（发送群消息）：

    import CQSDK
    CQSDK.SendGroupMsg(fromGroup, msg)

3. CQHandler.py是程序的入口点，里面封装了酷Q支持的所有事件，请在此源文件中实现自己的事件处理逻辑（里面已经自带了示例DEMO代码，可自动回复私聊和群聊）

#注意事项：
1. 由于Python27对于中文的支持问题，请务必将源文件按照 ANSI 编码保存，并在源文件的第一行加入：# -*- coding:gbk -*- 否则可能会出现中文乱码及其它异常情况
2. 默认支持最多9个菜单项（_menu01 ～ _menu09），请根据需要更改或者删减.json以及CQHandler.py中的相关代码
3. 为了方便开发者，在更改了python代码之后，只需点击菜单中的“重新初始化”即可使代码立即生效，（此功能只作为开发调试使用，建议发布时请在json文件中删除_menuReInit菜单）无需重新启动酷Q客户端
4. Python SDK技术支持问题请加QQ群：575465519