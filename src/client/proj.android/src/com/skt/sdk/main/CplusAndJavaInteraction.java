package com.skt.sdk.main;
/*
 * 
 * c++与java之间的调用方法都写在该类中,除以下需要区分渠道的特殊方法
 * 1、退出游戏
 * 
 * 
 * 
 * */
import java.io.UnsupportedEncodingException;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import com.skt.sdk.channel.Umeng;



import skt.tools.PhoneUtils;
import skt.tools.StatisticsLog;
import skt.tools.PhoneUtils.OperatorName;


public class CplusAndJavaInteraction {
    public static int curPayRmb;
    public static String phonenum;
    public static native void paySuccessJava(int rmb,String orderID);
    public static native void payFailJava(int rmb,String orderID);
    
    public static void pay(final int targetID) throws UnsupportedEncodingException{
        //curPayRmb = rmb;
        MainActivity.myMainActivity.channel.pay(targetID);
    }
    public static void countinuepay(final String phonenum) throws UnsupportedEncodingException{
        //curPayRmb = rmb;
        MainActivity.myMainActivity.channel.countinuepay(phonenum);
    }
    
    public static int getMobileType()throws UnsupportedEncodingException{
    	OperatorName curOperator = PhoneUtils.getOperatorName(MainActivity.myMainActivity);
    	if (curOperator == null) {
			return -1;
		}
		if (curOperator == PhoneUtils.OperatorName.CHINA_MOBILE) {
			return 0;
		} else if (curOperator == PhoneUtils.OperatorName.CHINA_TELECOM) {
			return 1;
		} else if (curOperator == PhoneUtils.OperatorName.CHINA_UNICOM) {
			return 2;
		} else{
			return -1;
		}
    }
    
    public static void exitGameError(){
		   try{
	            StatisticsLog.sendLog("exit", "", "");
	            MainActivity.myMainActivity.finish();
	        }catch(Exception e){
	            e.printStackTrace();
	        }finally{
	            Umeng.onKillProcess(MainActivity.myMainActivity);
	            android.os.Process.killProcess(android.os.Process.myPid());
	            // System.exit(0);
	        }

    }
    
    public static void paySuccess(final int rmb, final String orderId){
  	  Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable() {
  			@Override
  			public void run(){
  				paySuccessJava(rmb, orderId);
  			}
  	    });
  	}
  	
  	public static void payFail(final int rmb, final String context){
  	    Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable() {
  			@Override
  			public void run() {
  				payFailJava(rmb, context);
  			}
  	    });
  	}

    public static void exitGame(){
        MainActivity.myMainActivity.channel.exitGame();
    }

    public static int isMusicEnabled() {
    	return  MainActivity.myMainActivity.channel.isMusicEnabled();
    }
	 public static void sendThirdPartyTjLog(String tag, String str1, String str2, String str3){
        MainActivity.myMainActivity.umeng.onEvent(MainActivity.myMainActivity, tag, str1, str2, str3);
     }
    
    
}
