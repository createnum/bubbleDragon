package com.skt.sdk.channel.impl;

import org.json.JSONObject;

import skt.board.bubbleDragon.R;
import skt.tools.PhoneUtils;
import zcweb.HttpRequest;
import zcweb.ZCPayIcallback;
import zcweb.ZCWeb;

import com.skt.sdk.channel.Channel;
import com.skt.sdk.main.CplusAndJavaInteraction;
import com.skt.sdk.main.MainActivity;
import com.skt.sdk.tools.ExchangeDict;
import com.skt.sdk.tools.ExchangeDict.ExchangeInfo;
import com.unicom.xiaowo.Pay;
import com.unicom.xiaowo.Pay.UnipayPayResultListener;

import android.app.Activity;
import android.content.Context;
import android.widget.Toast;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;

public class ZCWebChannel extends Channel{
	public static Context context;
	public static PhoneUtils.OperatorName curOperator = null;
	public static String orderid = "";
	protected String appid;
	protected String channelid = "";
	protected String cpid = "";
	protected String data;
	protected String gamename = "";
	protected long lastPayTime;
	protected String productid;
	protected String com;
	protected String url = "http://182.92.149.179/jiekou//wow_pj/order.php";
    public ZCWebChannel(Activity activity, String productid, String appid){
         super(activity, 0);
         context = MainActivity.myMainActivity;
         this.productid = productid;
         this.appid = appid;
    }
    
    @Override
     public void init(){
		this.com = context.getString(R.string.company);
		this.gamename = context.getString(R.string.app_name);
    }


    
     @Override
    public void pay(int targetID){
    	
    	long curTime = System.currentTimeMillis();
    	if(curTime - lastPayTime < 60000){
    		((Activity) context).runOnUiThread(new Runnable(){
                @Override
                public void run(){
                	Toast.makeText(context, "支付太频繁，请一分钟后再支付！", Toast.LENGTH_LONG).show();
                }
    		});
    		return;
    	}
    	/*lastPayTime = curTime;
		lastPayTime = 0;//关闭60秒等待
    	final ExchangeInfo exchangeInfo = ExchangeDict.getInstance().getStoreInfo(channelIndex).getByTargetID(targetID);
        CplusAndJavaInteraction.curPayRmb = exchangeInfo.rmb;
		if(!MainActivity.myMainActivity.getString(R.string.visibleCommodityInfo).equals("true")){
			CplusAndJavaInteraction.sendThirdPartyTjLog("pay", ""+exchangeInfo.rmb, ""+(exchangeInfo.gold+exchangeInfo.presentGold), "23");//23 :电信爱动漫   友盟统计中的支付渠道
			continuePay(exchangeInfo.rmb);
			Toast.makeText(context, "发起请求计费", Toast.LENGTH_SHORT).show();
			return;
		}
        context.runOnUiThread(new Runnable(){
            @Override
            public void run(){
				final PayConfirmDialog dialog = new PayConfirmDialog(MainActivity.myMainActivity);
			   		dialog.init(exchangeInfo.payname, "" + exchangeInfo.rmb*0.01, "010-84543920");
			   		dialog.setCloseListener(new OnClickListener() {
						@Override
						public void onClick(View v) {
							CplusAndJavaInteraction.payFail(CplusAndJavaInteraction.curPayRmb, "取消支付");
							CplusAndJavaInteraction.sendThirdPartyTjLog("payFail", ""+exchangeInfo.rmb, "1", "23");//23 :电信爱动漫   友盟统计中的支付渠道
							dialog.dismiss();
						}
				    });
				    dialog.setConfirmListener(new OnClickListener() {
				        @Override
				        public void onClick(View v) {
				        	continuePay(exchangeInfo.rmb);
							CplusAndJavaInteraction.sendThirdPartyTjLog("pay", ""+exchangeInfo.rmb, ""+(exchangeInfo.gold+exchangeInfo.presentGold), "23");//23 :电信爱动漫   友盟统计中的支付渠道							
				        	dialog.dismiss();
				        }
				    });
				   dialog.show();
				
            }
        });*/
    	
    	final ExchangeInfo info = ExchangeDict.getInstance().getStoreInfo(channelIndex).getByTargetID(targetID);
    	final int rmb=info.rmb;
    	CplusAndJavaInteraction.curPayRmb = info.rmb;
		orderid = getOrderid(rmb);
		String reslutCode = saveOrder(rmb, orderid);
		if ("200000".equals(reslutCode)) {
			((Activity) context).runOnUiThread(new Runnable() {
				public void run() {
					try {
						Log.i("debug", appid + "@@@" + cpid + "@@@" + channelid
								+ "@@@" + gamename + "@@@" + "北京赞成科技有限公司"
								+ "@@@" + info.payCode + "@@@" + info.payname
								+ "@@@" + rmb / 100);
						 Pay.getInstance() .payChannel(context, gamename, com,
								info.payCode, info.payname, rmb / 100 + "",
								orderid, new UnipayPayResultListener() {
							@Override
							public void PayResult(String paramString1, int paramInt,int i,
									String paramString2) {
								// TODO Auto-generated method stub
								System.out.println(paramString1 + paramInt + paramString2);
								Toast.makeText(
										ZCWebChannel.context,
										"arg0:" + paramString1 + ";arg1:" + paramInt + "arg3:"
												+ paramString2, 0).show();
								switch (paramInt) {
								default:
									return;
								case 1:
									CplusAndJavaInteraction.paySuccess(
											CplusAndJavaInteraction.curPayRmb, orderid);
									CplusAndJavaInteraction.sendThirdPartyTjLog("paySuccess", ""
											+ CplusAndJavaInteraction.curPayRmb, "0", "24");// 24
																							// :赞成web联通
																							// 友盟统计中的支付渠道
									Toast.makeText(context, "支付成功", Toast.LENGTH_SHORT).show();
									return;
								case 2:
									CplusAndJavaInteraction.payFail(
											CplusAndJavaInteraction.curPayRmb, paramString1);
									CplusAndJavaInteraction
											.sendThirdPartyTjLog("payFail", ""
													+ CplusAndJavaInteraction.curPayRmb,
													paramString1, "24");// 24
									// :赞成web联通
									// 友盟统计中的支付渠道
									Toast.makeText(context, "支付失败" + paramString1,
											Toast.LENGTH_SHORT).show();
									return;
								}
							}
						});
					} catch (Exception localException) {
						localException.printStackTrace();
					}
				}
			});
		}
        
    }
    
    public void continuePay(int rmb)
    {
    	final ExchangeInfo exchangeInfo = ExchangeDict.getInstance().getStoreInfo(channelIndex).getByrmb(rmb);
    	 ((Activity) context).runOnUiThread(new Runnable(){
             @Override
             public void run(){
 				try {
 					CplusAndJavaInteraction.sendThirdPartyTjLog("pay", ""+exchangeInfo.rmb, ""+(exchangeInfo.gold+exchangeInfo.presentGold), "24");//24 :赞成web联通  友盟统计中的支付渠道
 					ZCWeb.getInst().pay(CplusAndJavaInteraction.curPayRmb, new ZCPayIcallback(){  
 			            public void payResult(String resultCode, String orderId) {  
 			                if(resultCode.equals("200000")){
 				            		 CplusAndJavaInteraction.paySuccess(CplusAndJavaInteraction.curPayRmb, orderId);
 				            		 CplusAndJavaInteraction.sendThirdPartyTjLog("paySuccess", ""+CplusAndJavaInteraction.curPayRmb, "0", "24");//24 :赞成web联通  友盟统计中的支付渠道
 				            		 Toast.makeText(context, "支付成功", Toast.LENGTH_SHORT).show();
 			            		}else{
 			            			 CplusAndJavaInteraction.payFail(CplusAndJavaInteraction.curPayRmb, resultCode);
 			            			 CplusAndJavaInteraction.sendThirdPartyTjLog("payFail", ""+CplusAndJavaInteraction.curPayRmb, resultCode, "24");//24 :赞成web联通   友盟统计中的支付渠道
 			            			 Toast.makeText(context, "支付失败" + resultCode, Toast.LENGTH_SHORT).show();
 			            		}
 								CplusAndJavaInteraction.curPayRmb = 0;
 			            }  
 			        });
 				} catch (Exception e) {
 					// TODO Auto-generated catch block
 					e.printStackTrace();
 				}
             }
         });
    	
    }


    @Override
    public void onResume(){
    }

    @Override
    public void onDestroy(){
        // TODO Auto-generated method stub

    }

    @Override
    public void onPause(){
    }
    public String getOrderid(int paramInt) {
		int str = (int) (Math.random() * 900) + 100;
		switch (paramInt) {
		default:
			return str + "905141492020150617163143668400010";
		case 100:
			return str + "905141492020150617163143668400010";
		case 200:
			return str + "905141492020150617163143668400012";
		case 400:
			return str + "905141492020150617163143668400014";
		case 600:
			return str + "905141492020150617163143668400016";
		case 800:
			return str + "905141492020150617163143668400018";
		case 1000:
			return str + "905141492020150617163143668400020";
		case 1200:
			return str + "905141492020150617163143668400026";
		case 1500:
			return str + "905141492020150617163143668400022";
		case 2000:
			return str + "905141492020150617163143668400024";
		}
	}

	public String saveOrder(int paramInt, String paramString) {
		String str1 = PhoneUtils.getPhoneNumber(context);
		String str2 = PhoneUtils.getIMSI(context);
		String str3 = PhoneUtils.getIMEI(context);
		String app=context.getString(R.string.zc_appid);
		final ExchangeInfo info = ExchangeDict.getInstance()
				.getStoreInfo(this.channelIndex).getByrmb(paramInt);
		try {
			this.data = ("app="+app+"&pid=ZCZY&tel=" + str1
					+ "&consumecode=" + info.payCode + "&salechannelid=&money="
					+ paramInt / 100 + "&time=&sign=&imsi=" + str2 + "&imei="
					+ str3 + "&cpparam=&gate=&mac=&appname=&feename="
					+ "&orderid=" + paramString);
			this.data = new String(this.data.getBytes(), "UTF-8");
			Log.i("test", this.data);
			paramString = new JSONObject(HttpRequest.sendGet(this.url,
					this.data)).getString("resultCode");
			return paramString;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return "";
	}

}
