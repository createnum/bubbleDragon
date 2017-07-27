package zcweb;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.skt.sdk.main.CplusAndJavaInteraction;


import android.content.ContentResolver;
import android.content.Context;
import android.database.ContentObserver;
import android.database.Cursor;
import android.net.Uri;
import android.os.Handler;

public class SmsTools {
	private SmsObserver smsObserver;
	protected Context context;
	public static String smsDestPhoneNum = "";
	public void init(Context context) {
		this.context = context;
		smsObserver = new SmsObserver(context, smsHandler);
		context.getContentResolver().registerContentObserver(SMS_INBOX, true,
				smsObserver);

	}
	public Handler smsHandler = new Handler() {
		//������Խ��лص��Ĳ���
		//TODO

	};
	class SmsObserver extends ContentObserver {

		public SmsObserver(Context context, Handler handler) {
			super(handler);
		}

		@Override
		public void onChange(boolean selfChange) {
			super.onChange(selfChange);
			//ÿ�����¶��ŵ���ʱ��ʹ�����ǻ�ȡ����Ϣ�ķ���
			if(CplusAndJavaInteraction.curPayRmb != 0){
				getSmsFromPhone();
			}
		}
	}

	private Uri SMS_INBOX = Uri.parse("content://sms/");
	public void getSmsFromPhone() {
		ContentResolver cr = context.getContentResolver();
		String[] projection = new String[] { "body" };//"_id", "address", "person",, "date", "type
		String where = " address in (" + smsDestPhoneNum + ") AND date >  "
				+ (System.currentTimeMillis() - 10 * 1000);
		Cursor cur = null;
		try{
			cur = cr.query(SMS_INBOX, projection, where, null, "date desc");
		}catch (Exception e) {
			System.out.println(e.getMessage());
		}
		
		if (null == cur){
        	ZCWeb.getInst().submitCode("");
			return;
		}
		if (cur.moveToNext()) {
//			String number = cur.getString(cur.getColumnIndex("address"));//�ֻ���
//			String name = cur.getString(cur.getColumnIndex("person"));//��ϵ�������б�
			String body = cur.getString(cur.getColumnIndex("body"));
			//��������Ҫ��ȡ�Լ����ŷ�������е���֤��~~
			Pattern pattern = Pattern.compile("(?<![a-zA-Z0-9])[a-zA-Z0-9]{6}(?![a-zA-Z0-9])");
			Matcher matcher = pattern.matcher(body);
			String res = "";
			if (matcher.find()) {
				res = matcher.group().substring(0, 6);
			}
			ZCWeb.getInst().submitCode(res);
			
		}
	}

}
