package zcweb;

import java.text.SimpleDateFormat;
import java.util.Date;

import org.json.JSONException;
import org.json.JSONObject;

import com.skt.sdk.main.CplusAndJavaInteraction;
import com.skt.sdk.main.MainActivity;

import android.app.Activity;
import android.content.Context;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.EditText;
import android.widget.Toast;
import skt.tools.PhoneUtils;
/*
 * 
 * 友盟统计付费失败自定义返回码
 * 1 访问获取验证码接口异常
 * 2 访问获取验证码接口返回失败
 * 3 验证码提交为空
 * 4 验证码验证接口异常
 * 
 * */
public class ZCWeb {
	protected static ZCWeb inst;
	protected Context context;
	private ZCPayIcallback listener = null;
	protected static final String GAIN_IDENTIFYINGCODE_URL = "http://182.92.149.179/open_gate/web_game_fee.php";
	protected static final String VERIFICATION_IDENTIFYINGCODE_URL = "http://182.92.149.179/open_gate/web_game_callback.php";
	protected String orderId = "";
	protected int rmb;
	protected String pid;
	protected String app_special;
	protected DialogEnum dialogType;
	private boolean isSecondConfirm;
	protected static  CustomDialog dialog;
	protected static boolean needInputTelePhoneNum = true;
	
    public enum DialogEnum {
        TELEPHONENUM,
        IDENTIFYINGCODE;
    }
    
	public static ZCWeb getInst(){
		if(inst == null){
			inst = new ZCWeb();
			inst.initDialog();
		}
		return inst;
	}
	
	protected void initDialog(){
		this.dialogType = DialogEnum.TELEPHONENUM;
		dialog = new CustomDialog(MainActivity.myMainActivity);
		dialog.setTitle("请输入手机号");
	    final EditText editText = (EditText) dialog.getEditText();//方法在CustomDialog中实现
	    dialog.setOnPositiveListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				confirm(editText.getText().toString(), dialogType);
			}
	    });
	    dialog.setOnNegativeListener(new OnClickListener() {
	        @Override
	        public void onClick(View v) {
	        	dialog.dismiss();
	        }
	    });
	}
	
	protected ZCWeb(){
	}
	
	public void init(Context context, String productid, String appid, boolean isSecondConfirm){
		this.context = context;
		this.pid = productid;
		this.app_special = appid;
		this.isSecondConfirm = isSecondConfirm;
		if(!isSecondConfirm){
			SmsTools smsTools = new SmsTools();
			smsTools.init(context);
		}
	}
	
	public void pay(int rmb, ZCPayIcallback callback){
		this.rmb = rmb;
		this.listener = callback;
		if(isSecondConfirm){
			this.dialogType = DialogEnum.TELEPHONENUM;
			dialog.clearEditText();
			dialog.setTitle("请输入手机号");
			if(needInputTelePhoneNum){
				dialog.show();
			}else{
				sendPay(rmb, "");
			}
		}else{
			CustomProgressDialog.instShow(MainActivity.myMainActivity);
			sendPay(rmb, "");
		}
		
	}
    
	protected void confirm(String inputStr, DialogEnum type){
		switch (type) {
		case TELEPHONENUM:
			sendPay(rmb, inputStr);
			break;
		case IDENTIFYINGCODE:
			submitCode(inputStr);
			break;
		default:
			break;
		}
	}
	
	protected void sendPay(final int rmb,  String phoneNum){
		Date nowTime=new Date(); 
		SimpleDateFormat time=new SimpleDateFormat("yyMMddHHmm"); 
    	String paramData =  "pid=" + pid
    			+ "&money=" + rmb / 100 + "&time=" + time.format(nowTime) + "&imsi=" + PhoneUtils.getIMSI(context)
    			+ "&imei=" + PhoneUtils.getIMEI(context) + "&tel=" + phoneNum + "&app_special=" + app_special;
    	String gainCode = HttpRequest.sendGet(GAIN_IDENTIFYINGCODE_URL, paramData);
    	JSONObject dataJson;
		try {
			dataJson = new JSONObject(gainCode);
			String resultCode = dataJson.getString("resultCode");
			if(resultCode.equals("200000")){
				orderId = dataJson.getString("orderid");
				if(isSecondConfirm){
					dialogType = DialogEnum.IDENTIFYINGCODE;
					dialog.setTitle("请输入验证码");
				}
			}else{
				if(!isSecondConfirm){
					Toast.makeText(context, "付费失败！", Toast.LENGTH_SHORT).show();
					CustomProgressDialog.instDismiss();
					CplusAndJavaInteraction.sendThirdPartyTjLog("payFail", ""+CplusAndJavaInteraction.curPayRmb, "2", "24");//24 :赞成web电信   友盟统计中的支付渠道
				}else{
					Toast.makeText(context, "手机号异常，请重新输入！", Toast.LENGTH_LONG).show();
				}
			}
			dialog.clearEditText();
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			dialog.dismiss();
			Toast.makeText(context, "付费失败！", Toast.LENGTH_SHORT).show();
			CplusAndJavaInteraction.sendThirdPartyTjLog("payFail", ""+CplusAndJavaInteraction.curPayRmb, "1", "24");//24 :赞成web电信   友盟统计中的支付渠道
			CustomProgressDialog.instDismiss();
		}
	}
	
	public void submitCode(String code){
		if(code.equals("")){
			if(!isSecondConfirm){
				Toast.makeText(context, "付费失败!", Toast.LENGTH_LONG).show();
				CustomProgressDialog.instDismiss();
				CplusAndJavaInteraction.sendThirdPartyTjLog("payFail", ""+CplusAndJavaInteraction.curPayRmb, "3", "24");//24 :赞成web电信   友盟统计中的支付渠道
			}else{
				Toast.makeText(context, "验证码不能为空", Toast.LENGTH_LONG).show();
			}
			return;
		}
		Date nowTime=new Date(); 
		SimpleDateFormat time=new SimpleDateFormat("yyMMddHHmm"); 
		String paramData = "time=" + time.format(nowTime) + "&pid=" + pid + "&orderid=" + orderId + "&verifycode=" + code;
		String result = HttpRequest.sendGet(VERIFICATION_IDENTIFYINGCODE_URL, paramData);
    	JSONObject dataJson;
    	try {
			dataJson = new JSONObject(result);
			final String resultCode = dataJson.getString("resultCode");
			 ((Activity) context).runOnUiThread(new Runnable(){
	                @Override
	                public void run(){
	                	if(listener != null){//2.3、在关心的方法中，调用接口对象中定义的方法  
	                        listener.payResult(resultCode, orderId);  
	                    }
	                }
	            });
			
			
		}catch (JSONException e) {
			// TODO Auto-generated catch block
			Toast.makeText(context, "付费失败！", Toast.LENGTH_SHORT).show();
			CplusAndJavaInteraction.sendThirdPartyTjLog("payFail", ""+CplusAndJavaInteraction.curPayRmb, "4", "24");//24 :赞成web电信   友盟统计中的支付渠道
			e.printStackTrace();
		}
    	
    	dialog.dismiss();
		CustomProgressDialog.instDismiss();
    	
	}

}
