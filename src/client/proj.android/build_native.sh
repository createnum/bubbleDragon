APPNAME="bubbleDragon"

# options
buildexternalsfromsource=
export channelName=
export skinName=
packageName=
usage(){
cat << EOF
usage: $0 [options]

Build C/C++ code for $APPNAME using Android NDK

OPTIONS:
-s	Build externals from source
[-t channelName] pack channel apk
	channelName values:
		ag		andGame
		uuc		uucun
		nor3	normal with 3 operator
		wo3		3 operator have woStore
		tp		telecomPlatform
		nor3wb	weibu 3
		mm      MM3.1.7
		wo      woStore
		yddm	yidongdongman
		zc		zctelecom
		ap		alippay
		es		Esurfing
		sp		selfPay
		lt		letu
		zc_web         zc_web
		sms_adm	sms_aidongman
[-p skinName] 
		skinName values:
		ppl		paopaolong
		ppl2	paopaolong2
-h	this help
EOF
}

while getopts "sht:p:" OPTION; do
case "$OPTION" in
s)
buildexternalsfromsource=1
;;
t)
channelName=$OPTARG
;;
p)
skinName=$OPTARG
;;
h)
usage
exit 0
;;
esac
done

# paths

if [ -z "${NDK_ROOT+aaa}" ];then
echo "please define NDK_ROOT"
exit 1
fi

export DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# ... use paths relative to current directory
COCOS2DX_ROOT="$DIR/../../../../../backers/cocos2d-x-2.2.2"
APP_ROOT="$DIR/.."
APP_ANDROID_ROOT="$DIR"

echo "NDK_ROOT = $NDK_ROOT"
echo "COCOS2DX_ROOT = $COCOS2DX_ROOT"
echo "APP_ROOT = $APP_ROOT"
echo "APP_ANDROID_ROOT = $APP_ANDROID_ROOT"

# make sure res is exist
if [ -d "$APP_ANDROID_ROOT"/res ]; then
    rm -rf "$APP_ANDROID_ROOT"/res
fi
mkdir "$APP_ANDROID_ROOT"/res

# make sure libs is exist
if [ -d "$APP_ANDROID_ROOT"/libs ]; then
    rm -rf "$APP_ANDROID_ROOT"/libs
fi
mkdir "$APP_ANDROID_ROOT"/libs

# make sure assets is exist
if [ -d "$APP_ANDROID_ROOT"/assets ]; then
    rm -rf "$APP_ANDROID_ROOT"/assets
fi
mkdir "$APP_ANDROID_ROOT"/assets

# make sure res is exist
if [ -d "$APP_ANDROID_ROOT"/src ]; then
    rm -rf "$APP_ANDROID_ROOT"/src
fi
mkdir "$APP_ANDROID_ROOT"/src

# copy general resources
for file in "$APP_ANDROID_ROOT"/packConf/generalRes/*
do
if [ -d "$file" ]; then
    cp -rf "$file" "$APP_ANDROID_ROOT"
fi

if [ -f "$file" ]; then
    cp "$file" "$APP_ANDROID_ROOT"
fi
done

# copy resources
for file in "$APP_ROOT"/Resources/*
do
if [ -d "$file" ]; then
    cp -rf "$file" "$APP_ANDROID_ROOT"/assets
fi

if [ -f "$file" ]; then
    cp "$file" "$APP_ANDROID_ROOT"/assets
fi
done

# run ndk-build
if [[ "$buildexternalsfromsource" ]]; then
    echo "Building external dependencies from source"
	
	"$NDK_ROOT"/ndk-build -C "$APP_ANDROID_ROOT" \
    "NDK_MODULE_PATH=${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/source"
else
    echo "Using prebuilt externals"
	"$NDK_ROOT"/ndk-build -C "$APP_ANDROID_ROOT" \
    "NDK_MODULE_PATH=${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/prebuilt"
fi


#拷贝sdk所需资源
if [ "$channelName" = "" ]; then  #基础资源
    echo "pack general"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
elif [ "$channelName" = "ag" ]; then	#和游戏 （游戏基地）  ok
    echo "pack andgame"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/andGame/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
elif [ "$channelName" = "mm" ]; then	#移动mm	ok
	echo "pack operator mm"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/chinaMobile/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
elif [ "$channelName" = "wo" ]; then	#沃商店（联通） 支持三网	ok
    echo "pack operator with woStore"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/woStore/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
elif [ "$channelName" = "tp" ]; then  	#电信	ok
    echo "pack telecom Platform"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/chinaTelecom/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
elif [ "$channelName" = "zc" ]; then  	#赞成		ok
	echo "pack zctelecom"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/zc/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
elif [ "$channelName" = "xt" ]; then 	#翔通		ok
	echo "pack xiangtong"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/xiangtong/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
elif [ "$channelName" = "yj" ]; then 	#易接		ok
	echo "pack yijie"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/yijie/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
elif [ "$channelName" = "lt" ]; then	#乐图
	echo "pack letu"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/letu/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
elif [ "$channelName" = "zc_web" ]; then	#乐图
	echo "pack zc_web"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/zc_web/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
elif [ "$channelName" = "sms_adm" ]; then	#爱动漫
	echo "pack sms_aidongman"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/sms_adm/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
elif [ "$channelName" = "uuc" ]; then 	#	（未完成）
    echo "pack uucun"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/uucun/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
elif [ "$channelName" = "yddm" ]; then 	#移动动漫（未完成）
	echo "pack yidongdongman"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/yidongdongman/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/channels/operatorNormal/" "$APP_ANDROID_ROOT"
elif [ "$channelName" = "nor3wb" ]; then #？？3网（未完成）
    echo "pack operator normal with 3 weibu"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/chinaMobile/" "$APP_ANDROID_ROOT"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/chinaTelecom/" "$APP_ANDROID_ROOT"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/chinaUnicom/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/umeng/" "$APP_ANDROID_ROOT"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/channels/operatorNormalWB/" "$APP_ANDROID_ROOT"
elif [ "$channelName" = "nor3" ]; then  #3网 （3网）（未完成）
	echo "pack operator normal with 3"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/chinaMobile/" "$APP_ANDROID_ROOT"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/chinaTelecom/" "$APP_ANDROID_ROOT"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/chinaUnicom/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/umeng/" "$APP_ANDROID_ROOT"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/channels/operatorNormal/" "$APP_ANDROID_ROOT"
elif [ "$channelName" = "wo3" ]; then  #沃商店（3网）（未完成）
    echo "pack operator with woStore"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/chinaMobile/" "$APP_ANDROID_ROOT"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/chinaTelecom/" "$APP_ANDROID_ROOT"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/woStore/" "$APP_ANDROID_ROOT"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/sdks/testin/" "$APP_ANDROID_ROOT"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/channels/operatorWoStore/" "$APP_ANDROID_ROOT"
else
    usage
    exit 0
fi


echo "   skinName :  $skinName";
if [ "$skinName" = "" ]; then
    echo "skin ======= null"
	packageName="skt.board.bubbleDragon"
	sh copyRes.sh "$APP_ANDROID_ROOT/packConf/skins/ppl/general" "$APP_ANDROID_ROOT"
elif [ "$skinName" = "ppl" ]; then
    echo "skin ======= ppl"
	packageName="skt.board.bubbleDragon"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/skins/ppl/general" "$APP_ANDROID_ROOT"
elif [ "$skinName" = "ppl2" ]; then
    echo "skin ======= ppl2"
	packageName="skt.board.bubbleDragon2"
    sh copyRes.sh "$APP_ANDROID_ROOT/packConf/skins/ppl2/general" "$APP_ANDROID_ROOT"
else
    usage
    exit 0
fi
#替换包名
sed -i "s/skt.eliminate.billiards.xmnzq.R/$packageName".R"/g" `grep "skt.eliminate.billiards.xmnzq.R" -rl "$APP_ANDROID_ROOT"/src`
sed -i "s/skt.eliminate.billiards.xmnzq/$packageName""/g" "$APP_ANDROID_ROOT"/AndroidManifest.xml

#替换icon （要求-p不为空） 添加sdk计费代码等文件（要求-t -p  都不为空）
if [ "$skinName" != "" ]; then 
	cp "$APP_ANDROID_ROOT"/packConf/skins/$skinName/general/icon.png "$APP_ANDROID_ROOT"/res/drawable-hdpi/icon.png
	cp "$APP_ANDROID_ROOT"/packConf/skins/$skinName/general/icon.png "$APP_ANDROID_ROOT"/res/drawable-ldpi/icon.png
	cp "$APP_ANDROID_ROOT"/packConf/skins/$skinName/general/icon.png "$APP_ANDROID_ROOT"/res/drawable-mdpi/icon.png
	if [ "$channelName" != "" ]; then 
		sh copyRes.sh "$APP_ANDROID_ROOT/packConf/skins/$skinName/channel_res/$channelName" "$APP_ANDROID_ROOT"
	fi
fi
