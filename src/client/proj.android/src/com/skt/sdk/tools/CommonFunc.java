package com.skt.sdk.tools;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;

import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;

import com.skt.sdk.main.MainActivity;

import skt.tools.EncryptUtils;
import skt.tools.PhoneUtils;
import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;

public class CommonFunc{
    public static String getSdcardRootPath(){
        File sdDir = null;
        boolean sdCardExist = Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED); // �ж�sd���Ƿ����
        if(sdCardExist){
            sdDir = Environment.getExternalStorageDirectory();// ��ȡ��Ŀ¼
        }
        return null == sdDir ? null : sdDir.toString();
    }

    public static String getSdcardPath(String relativePath){
        String rootPath = getSdcardRootPath();
        if(null == rootPath){
            return null;
        }
        String path = rootPath + File.separator + relativePath;
        File file = new File(path);
        File parentFile = file.getParentFile();
        if(!parentFile.exists()){
            parentFile.mkdirs();
        }
        return path;
    }
    public static Bitmap compress(Bitmap image, float newWidth, float newHeight, int newByteSizeK) {  //byteSize��λk
        
        ByteArrayOutputStream baos = new ByteArrayOutputStream();         
        image.compress(Bitmap.CompressFormat.JPEG, 100, baos);  
        if( baos.toByteArray().length / 1024>1024) {//�ж����ͼƬ����1M,����ѹ������������ͼƬ��BitmapFactory.decodeStream��ʱ���    
            baos.reset();//����baos�����baos  
            image.compress(Bitmap.CompressFormat.JPEG, 50, baos);//����ѹ��50%����ѹ��������ݴ�ŵ�baos��  
        }  
        ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());  
        BitmapFactory.Options newOpts = new BitmapFactory.Options();  
        //��ʼ����ͼƬ����ʱ��options.inJustDecodeBounds ���true��  
        newOpts.inJustDecodeBounds = true;  
        Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, newOpts);  
        newOpts.inJustDecodeBounds = false;  
        int w = newOpts.outWidth;  
        int h = newOpts.outHeight;  
        //���������ֻ��Ƚ϶���800*480�ֱ��ʣ����ԸߺͿ���������Ϊ  
        //���űȡ������ǹ̶��������ţ�ֻ�ø߻��߿�����һ�����ݽ��м��㼴��  
        int be = 1;//be=1��ʾ������  
        if (w > h && w > newWidth) {//�����ȴ�Ļ����ݿ�ȹ̶���С����  
            be = (int) (newOpts.outWidth / newWidth);  
        } else if (w < h && h > newHeight) {//����߶ȸߵĻ����ݿ�ȹ̶���С����  
            be = (int) (newOpts.outHeight / newHeight);  
        }  
        if (be <= 0)  
            be = 1;  
        newOpts.inSampleSize = be;//�������ű���  
        //���¶���ͼƬ��ע���ʱ�Ѿ���options.inJustDecodeBounds ���false��  
        isBm = new ByteArrayInputStream(baos.toByteArray());  
        bitmap = BitmapFactory.decodeStream(isBm, null, newOpts);  
        return compressBySize(bitmap, newByteSizeK);//ѹ���ñ�����С���ٽ�������ѹ��  
    }  
    
    public static Bitmap compressBySize(Bitmap image, int newByteSizeK) {  
        
        ByteArrayOutputStream baos = new ByteArrayOutputStream();  
        image.compress(Bitmap.CompressFormat.JPEG, 100, baos);//����ѹ������������100��ʾ��ѹ������ѹ��������ݴ�ŵ�baos��  
        int options = 100;  
        while ( baos.toByteArray().length / 1024> newByteSizeK) {  //ѭ���ж����ѹ����ͼƬ�Ƿ���ڹ涨�Ĵ�С,���ڼ���ѹ��         
            baos.reset();//����baos�����baos  
            image.compress(Bitmap.CompressFormat.JPEG, options, baos);//����ѹ��options%����ѹ��������ݴ�ŵ�baos��  
            options -= 10;//ÿ�ζ�����10  
        }  
        ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());//��ѹ���������baos��ŵ�ByteArrayInputStream��  
        Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, null);//��ByteArrayInputStream��������ͼƬ  
        return bitmap;  
    } 
    
    public static String getDeviceId(Context context) {
        String id = PhoneUtils.getMacAddress(context) + 
                    PhoneUtils.getIMEI(context) + 
                    PhoneUtils.getBluetoothAddress(context) +
                    PhoneUtils.getSerial();
        return EncryptUtils.MD5Encrypt(id);
    }
    public static String getResFileContent(String filename, Context context) {
        InputStream is = context.getClass().getClassLoader()
                .getResourceAsStream(filename);
        if (is == null) {
            return null;
        }
        StringBuilder builder = new StringBuilder();
        String content = "";
        BufferedReader bufferedReader = new BufferedReader(
                new InputStreamReader(is));
        try {
            while (bufferedReader.ready()) {
                content = bufferedReader.readLine();
                builder.append(content);
            }
            bufferedReader.close();
        } catch (IOException e) {
            return null;
        }
        return builder.toString();
    }
    
    public static Element loadDictFile(String filePath){
        InputStream is = null;
        try{
            AssetManager assetManager = MainActivity.myMainActivity.getAssets();
            is = assetManager.open(filePath);
            
            SAXReader reader = new SAXReader();
            Document document = reader.read(is);
            if(null == document){
                return null;
            }
            return document.getRootElement();
        }catch(DocumentException e){
            e.printStackTrace();
        }catch(IOException e){
            e.printStackTrace();
        }finally{
            if(is != null){
                try{
                    is.close();
                }catch(IOException e){
                    e.printStackTrace();
                }
            }
        }
        return null;
    }

    public static OutputStream getNativeFileOutputStream(Context activity, String name){
        try{
            return activity.openFileOutput(name, Context.MODE_PRIVATE);
        }catch(FileNotFoundException e){
            e.printStackTrace();
            return null;
        }
    }

    public static boolean writeToNativePlace(Activity activity, String name, byte[] bts){
        OutputStream outStream = null;
        try{
            outStream = getNativeFileOutputStream(activity, name);
            if(null != outStream){
                outStream.write(bts);
                return true;
            }else{
                return false;
            }
        }catch(IOException e){
            e.printStackTrace();
            return false;
        }finally{
            if(null != outStream){
                try{
                    outStream.close();
                }catch(IOException e){
                    e.printStackTrace();
                }
            }
        }
    }
    
    public static InputStream readFileFromNativePlace(Context activity, String name){
        try{
            File file = activity.getFileStreamPath(name);
            if(file.exists()){
                return activity.openFileInput(name);
            }
            return null;            
        }catch(FileNotFoundException e){
            e.printStackTrace();
            return null;
        }
    }
}
