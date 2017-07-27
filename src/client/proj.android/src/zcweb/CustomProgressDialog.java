package zcweb;


import skt.board.bubbleDragon.R;
import android.app.ProgressDialog;
import android.content.Context;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class CustomProgressDialog extends ProgressDialog{
	
	public CustomProgressDialog(Context context) {
		super(context);
	}

	static CustomProgressDialog inst;
	

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.pay_progress_dialog);
	}
	
	public static void instShow(Context context){
		if(null == inst){
			inst = new CustomProgressDialog(context);
		}
		inst.show();
	}
	
	public static void instDismiss() {
		if(null != inst){
			inst.dismiss();
			inst = null;
		}
		
	}
	
	@Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
		boolean result = super.onKeyDown(keyCode, event);
		 if (keyCode == KeyEvent.KEYCODE_BACK){
			 inst = null;
            }
        return result;
    }
	
	@Override
	 public boolean onTouchEvent(MotionEvent event){
		return true;
	}
	
}