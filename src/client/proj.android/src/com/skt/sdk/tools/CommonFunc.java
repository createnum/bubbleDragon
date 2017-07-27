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
        boolean sdCardExist = Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED); // 判断sd卡是否存在
        if(sdCardExist){
            sdDir = Environment.getExternalStorageDirectory();// 获取跟目录
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
    public static Bitmap compress(Bitmap image, float newWidth, float newHeight, int newByteSizeK) {  //byteSize单位k
        
        ByteArrayOutputStream baos = new ByteArrayOutputStream();         
        image.compress(Bitmap.CompressFormat.JPEG, 100, baos);  
        if( baos.toByteArray().length / 1024>1024) {//判断如果图片大于1M,进行压缩避免在生成图片（BitmapFactory.decodeStream）时溢出    
            baos.reset();//重置baos即清空baos  
            image.compress(Bitmap.CompressFormat.JPEG, 50, baos);//这里压缩50%，把压缩后的数据存放到baos中  
        }  
        ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());  
        BitmapFactory.Options newOpts = new BitmapFactory.Options();  
        //开始读入图片，此时把options.inJustDecodeBounds 设回true了  
        newOpts.inJustDecodeBounds = true;  
        Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, newOpts);  
        newOpts.inJustDecodeBounds = false;  
        int w = newOpts.outWidth;  
        int h = newOpts.outHeight;  
        //现在主流手机比较多是800*480分辨率，所以高和宽我们设置为  
        //缩放比。由于是固定比例缩放，只用高或者宽其中一个数据进行计算即可  
        int be = 1;//be=1表示不缩放  
        if (w > h && w > newWidth) {//如果宽度大的话根据宽度固定大小缩放  
            be = (int) (newOpts.outWidth / newWidth);  
        } else if (w < h && h > newHeight) {//如果高度高的话根据宽度固定大小缩放  
            be = (int) (newOpts.outHeight / newHeight);  
        }  
        if (be <= 0)  
            be = 1;  
        newOpts.inSampleSize = be;//设置缩放比例  
        //重新读入图片，注意此时已经把options.inJustDecodeBounds 设回false了  
        isBm = new ByteArrayInputStream(baos.toByteArray());  
        bitmap = BitmapFactory.decodeStream(isBm, null, newOpts);  
        return compressBySize(bitmap, newByteSizeK);//压缩好比例大小后再进行质量压缩  
    }  
    
    public static Bitmap compressBySize(Bitmap image, int newByteSizeK) {  
        
        ByteArrayOutputStream baos = new ByteArrayOutputStream();  
        image.compress(Bitmap.CompressFormat.JPEG, 100, baos);//质量压缩方法，这里100表示不压缩，把压缩后的数据存放到baos中  
        int options = 100;  
        while ( baos.toByteArray().length / 1024> newByteSizeK) {  //循环判断如果压缩后图片是否大于规定的大小,大于继续压缩         
            baos.reset();//重置baos即清空baos  
            image.compress(Bitmap.CompressFormat.JPEG, options, baos);//这里压缩options%，把压缩后的数据存放到baos中  
            options -= 10;//每次都减少10  
        }  
        ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());//把压缩后的数据baos存放到ByteArrayInputStream中  
        Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, null);//把ByteArrayInputStream数据生成图片  
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
