
#import "MyUtilsConfig.h"

MyUtilsConfig* MyUtilsConfig::m_inst=0;

void MyUtilsConfig::init(){
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    std::string path = [documentsDirectory UTF8String];
    path.append("/");
    
    m_mainLogPath = path + "main.log";
    m_timerLogPath = path + "timerThread.log";
}