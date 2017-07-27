package com.skt.sdk.channel.impl;


import skt.board.bubbleDragon.R;
import android.app.Dialog;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
/**
 * @Function: �Զ���Ի���
 * @Date: 2013-10-28
 * @Time: ����12:37:43
 * @author Tom.Cai
 */
public class PayConfirmDialog extends Dialog {
    private Button closeButton, confirmButton;
    private TextView goodsName;
    private TextView goodsCost;
	private TextView payConfirmDesView;
    
    public PayConfirmDialog(Context context) {
        super(context, R.style.dialog);
        setCustomDialog();
    }
 
    private void setCustomDialog() {
        View mView = LayoutInflater.from(getContext()).inflate(R.layout.pay_confirm, null);
        mView.setBackgroundResource(R.drawable.tanchuangda);
        goodsName = (TextView) mView.findViewById(R.id.goodsName);
        goodsCost = (TextView) mView.findViewById(R.id.goodsCost);
        closeButton = (Button) mView.findViewById(R.id.payConfirmClose);
        confirmButton = (Button) mView.findViewById(R.id.payConfirmConfirm);
        payConfirmDesView = ((TextView) mView.findViewById(R.id.pay_confirm_des));
        super.setContentView(mView);
    }
    
    public void init(String name, String costRmb, String servicephone){
        this.goodsName.setText(name);;
        this.goodsCost.setText(costRmb);
        String pay_confirm_des_bak = getContext().getResources().getString(R.string.pay_confirm_des);   
        String pay_confirm_des=String.format(pay_confirm_des_bak,costRmb,costRmb,servicephone);
        payConfirmDesView.setText(pay_confirm_des);
    }
    
    /**
     * ȷ����������
     * @param listener
     */ 
    public void setCloseListener(View.OnClickListener listener){ 
        closeButton.setOnClickListener(listener); 

    } 
    /**
     * ȡ����������
     * @param listener
     */ 
    public void setConfirmListener(View.OnClickListener listener){ 
        confirmButton.setOnClickListener(listener); 
        this.dismiss();
    }
}