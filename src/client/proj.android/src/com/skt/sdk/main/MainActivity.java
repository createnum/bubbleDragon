package com.skt.sdk.main;


/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/

import java.io.UnsupportedEncodingException;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;


import com.skt.sdk.channel.Channel;
import com.skt.sdk.channel.Umeng;
import com.skt.sdk.channel.impl.ChannelFactory;

import skt.board.bubbleDragon.R;
import skt.tools.PhoneUtils;
import skt.tools.StatisticsLog;
import skt.tools.PhoneUtils.OperatorName;
//import android.app.Activity;
//import android.app.AlertDialog;
//import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;

import android.net.Uri;
import android.os.Bundle;

public class MainActivity extends Cocos2dxActivity {

	public static MainActivity myMainActivity = null;
	static final String RECIVE_LOG_URL = "http://42.96.170.133:10017/fishLog/newLog";
	public Channel channel;
	public static Umeng umeng;
	public static OperatorName staticcurOperator;
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		myMainActivity = this;
        OperatorName curOperator = PhoneUtils.getOperatorName(this);
        staticcurOperator = curOperator;
        String channelId = Channel.getSonChannelID(this);
		StatisticsLog.init(this, RECIVE_LOG_URL, channelId);
		
		channel = ChannelFactory.getChannel(curOperator, this);
//		// 初始化各个支付平台
//		PublicChannel.getInstance().initPlatformListener(this);
//		//添加统计点
//		String channelId = PublicChannel.LoadMMSonChannelID(this);
//		StatisticsLog.init(this, RECIVE_LOG_URL, channelId);
		String umChannelId = "";
		try {
			 ApplicationInfo appInfo = this.getPackageManager()
			        .getApplicationInfo(getPackageName(),PackageManager.GET_META_DATA);
			umChannelId = appInfo.metaData.getString("com.snowfish.channel");
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		umeng = new Umeng();
		umeng.onCreate(myMainActivity, umChannelId);
	}

	public Cocos2dxGLSurfaceView onCreateView() {
		Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
		// bubbleDragon should create stencil buffer
		glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

		return glSurfaceView;
	}
	static {
		System.loadLibrary("cocos2dcpp");
	}	
	public static String getAndroidData() throws UnsupportedEncodingException {
	    return staticcurOperator +","+ myMainActivity.getString(R.string.version_name)
	    +","+myMainActivity.getString(R.string.newbie_gift_gold)+","+myMainActivity.getString(R.string.app_name)
	    +","+PhoneUtils.getIMEI(myMainActivity)+","+PhoneUtils.getIMSI(myMainActivity);
	}
	public static native void setCMusic(int isOpen);
    public static void setMusic(){
    }
	public static void openURL(){
        Intent intent= new Intent();        
        intent.setAction("android.intent.action.VIEW");    
        Uri content_url = Uri.parse("http://www.play.cn");   
        intent.setData(content_url);  
        myMainActivity.startActivity(intent);
    }
	@Override
    public void onResume(){
    	 
        // TODO Auto-generated method stub
        
        /* 自定义IntentFilter为SENT_SMS_ACTIOIN Receiver */

		channel.onResume();
        super.onResume();
    }

    @Override
    public void onDestroy(){
        // TODO Auto-generated method stub

    }

    @Override
    public void onPause(){
    	channel.onPause();
    	super.onPause();
    }
}
