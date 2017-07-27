//testCases.xml内的字符串常量
#ifndef CONST_H
#define CONST_H

#include "newbeeUtils/commonFunc.h"
static const NEWBEE_UTILS::OS_ENDIAN PEER_ENDIAN = NEWBEE_UTILS::NEWBEE_BIG_ENDIAN;// 服务器解析报文的字节序
static const NEWBEE_UTILS::OS_ENDIAN HOST_ENDIAN = NEWBEE_UTILS::endian();// 服务器解析报文的字节序

static const int PACKAGE_SIZE_LEN = 4;
static const int PACKAGE_HEAD_LEN = PACKAGE_SIZE_LEN+1+4+1;
static const char *ATTRNAME_CMD_NAME = "packetName";
static const char *ATTRNAME_SUBCMD_NAME = "subPacketName";

//static const char *CONF_FILE_PATH="../../../tradeWar_doc/testCases.xml";]
static const char *CONF_FILE_PATH="";
static const char *CONF_FILE_NAME="testCases.xml";

static const char *NODE_NAME_ROOT="Root";
static const char *NODE_NAME_CMDINSTANCES="CmdInstances";
static const char *NODE_NAME_VARIABLES="Variables";
static const char *NODE_NAME_CASES="TestCases";
static const char *NODE_NAME_CMDSTREAMS="CmdStreams";
static const char *NODE_NAME_TASKS="Tasks";
static const char *NODE_NAME_ACTORS="Actors";

static const char *NODE_NAME_SYNER="syner";
static const char *ATTRNAME_SYNERNAME = "synerName";

static const char *NODE_NAME_CONFIGRATIONS="Conf";
static const char *ATTRNAME_PEERNAME = "peerName";
static const char *ATTRNAME_TIMES = "times";
static const int ATTR_VALUE_NO_TIMES = -1;
static const int ATTR_VALUE_TIMES_DEFAULT = 1;

static const char *ATTR_NAME_CMD_FORMAT_FILENAME="cmdFormatFileName";

static const char *NODE_NAME_CONF_STARTUP="startup";
//日志相关
static const char *ERRORLOG_DEFAULT_NAME="defaultStressTest.log";

static const char *ATTRNAME_HOME_PATH="homePath";

static const char *NODE_NAME_CONF_LOG_HANDLER="handlerLog";

static const char *NODE_NAME_CONF_LOG_MAIN="mainLog";

static const char *ATTRNAME_LOG_LEVEL="logLevel";
static const char *ATTR_VALUE_LOG_LEVEL_DEBUG="debug";
static const char *ATTR_VALUE_LOG_LEVEL_INFO="info";
static const char *ATTR_VALUE_LOG_LEVEL_WARN="warn";
static const char *ATTR_VALUE_LOG_LEVEL_ERROR="error";
static const char *ATTRNAME_LOG_BUFFER_MODE="bufferMode";
static const char *ATTR_VALUE_LOG_NONE="none";
static const char *ATTR_VALUE_LOG_AUTO="auto";

static const char *NODE_NAME_CMD_DELAY_TIME="cmdDefualtDelayTime";
static const char *ATTRNAME_CMD_DELAY_TIME="seconds";

static const char *ATTRNAME_COMMENTS = "comments";

static const char *ATTR_VALUE_NO_PROMPTINFO = "comments: none";
static const char *ATTR_VALUE_NO_NAME = "no_name";
static const int ATTR_VALUE_NO_ID = -1;
static const int ATTR_VALUE_NO_PORT = -1;
static const char *ATTR_VALUE_NO_IP = "no_ip";
static const char *ATTR_VALUE_NULL = "";

static const char PRINT_SEPERATOR = ' ';
static const char *STARTUP_STRING = "runTest";

static const char SEPARATOR_CMD_HEAD = ' ';
static const char SEPARATOR_PARAM = '\x1c';
static const char SEPARATOR_PARAM_VISIBLE = '\x07';
static const char SEPARATOR_CMD = '\x0d';

static const char *STRING_FALSE = "false";
static const char *STRING_TRUE = "true";
static const char XML_VARIABLE_TAG = '@';

#endif //CONST_H

