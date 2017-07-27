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
 * ����ͳ�Ƹ���ʧ���Զ��巵����
 * 1 ���ʻ�ȡ��֤��ӿ��쳣
 * 2 ���ʻ�ȡ��֤��ӿڷ���ʧ��
 * 3 ��֤���ύΪ��
 * 4 ��֤����֤�ӿ��쳣
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
		dialog.setTitle("�������ֻ���");
	    final EditText editText = (EditText) dialog.getEditText();//������CustomDialog��ʵ��
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
			dialog.setTitle("�������ֻ���");
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
					dialog.setTitle("��������֤��");
				}
			}else{
				if(!isSecondConfirm){
					Toast.makeText(context, "����ʧ�ܣ�", Toast.LENGTH_SHORT).show();
					CustomProgressDialog.instDismiss();
					CplusAndJavaInteraction.sendThirdPartyTjLog("payFail", ""+CplusAndJavaInteraction.curPayRmb, "2", "24");//24 :�޳�web����   ����ͳ���е�֧������
				}else{
					Toast.makeText(context, "�ֻ����쳣�����������룡", Toast.LENGTH_LONG).show();
				}
			}
			dialog.clearEditText();
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			dialog.dismiss();
			Toast.makeText(context, "����ʧ�ܣ�", Toast.LENGTH_SHORT).show();
			CplusAndJavaInteraction.sendThirdPartyTjLog("payFail", ""+CplusAndJavaInteraction.curPayRmb, "1", "24");//24 :�޳�web����   ����ͳ���е�֧������
			CustomProgressDialog.instDismiss();
		}
	}
	
	public void submitCode(String code){
		if(code.equals("")){
			if(!isSecondConfirm){
				Toast.makeText(context, "����ʧ��!", Toast.LENGTH_LONG).show();
				CustomProgressDialog.instDismiss();
				CplusAndJavaInteraction.sendThirdPartyTjLog("payFail", ""+CplusAndJavaInteraction.curPayRmb, "3", "24");//24 :�޳�web����   ����ͳ���е�֧������
			}else{
				Toast.makeText(context, "��֤�벻��Ϊ��", Toast.LENGTH_LONG).show();
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
	                	if(listener != null){//2.3���ڹ��ĵķ����У����ýӿڶ����ж���ķ���  
	                        listener.payResult(resultCode, orderId);  
	                    }
	                }
	            });
			
			
		}catch (JSONException e) {
			// TODO Auto-generated catch block
			Toast.makeText(context, "����ʧ�ܣ�", Toast.LENGTH_SHORT).show();
			CplusAndJavaInteraction.sendThirdPartyTjLog("payFail", ""+CplusAndJavaInteraction.curPayRmb, "4", "24");//24 :�޳�web����   ����ͳ���е�֧������
			e.printStackTrace();
		}
    	
    	dialog.dismiss();
		CustomProgressDialog.instDismiss();
    	
	}

}
