package com.skt.sdk.channel.impl;

import com.skt.sdk.channel.*;
import com.skt.sdk.main.MainActivity;

import skt.board.bubbleDragon.R;
import skt.tools.PhoneUtils.OperatorName;
import zcweb.SmsTools;
import android.app.Activity;

public class ChannelFactory{
    public static Channel getChannel(OperatorName operatorName, Activity activity){
    	Channel channel = new ZCWebChannel(activity, 
    			MainActivity.myMainActivity.getString(R.string.zc_pay_zcweb_productid), 
    			MainActivity.myMainActivity.getString(R.string.zc_pay_zcweb_appid));
    	channel.init();
		if(operatorName == null)
    	{
    		SmsTools.smsDestPhoneNum = MainActivity.myMainActivity.getString(R.string.default_lenth_number);
    		return channel;
    	}
		switch(operatorName){
        case CHINA_TELECOM:
       	 SmsTools.smsDestPhoneNum = MainActivity.myMainActivity.getString(R.string.telecom_lenth_number);
       	 System.out.println(SmsTools.smsDestPhoneNum);
            break;
        case CHINA_UNICOM:
       	 SmsTools.smsDestPhoneNum = MainActivity.myMainActivity.getString(R.string.unicom_lenth_number);
       	System.out.println(SmsTools.smsDestPhoneNum);
            break;
        default:
       	 SmsTools.smsDestPhoneNum = MainActivity.myMainActivity.getString(R.string.default_lenth_number);
       	System.out.println(SmsTools.smsDestPhoneNum);
            break;
        }
        return channel;
    }
}
