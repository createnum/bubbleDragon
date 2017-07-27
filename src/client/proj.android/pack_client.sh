ANDROID_SDK_ROOT=D:/cocos2dx/android_sdk/android_sdk/
ANDROID_SDK_VERSION_PATH=platforms/android-14/android.jar

DIR=E:/sktGame/games/bubbleDragon/src/client/proj.android
RELEASE_ROOT="$DIR/../../../../../apk"
APP_ANDROID_ROOT="$DIR"
CLIENT_NAME=bubbleDragon.apk

"$ANDROID_SDK_ROOT"/platform-tools/aapt.exe p -f -m -J gen -S res -I "$ANDROID_SDK_ROOT"/"$ANDROID_SDK_VERSION_PATH" -M AndroidManifest.xml

#javac -encoding GBK -target 1.6 -bootclasspath "$ANDROID_SDK_ROOT"/"$ANDROID_SDK_VERSION_PATH" -d bin/classes src/*.java gen/*.java -classpath libs/*
echo ----------------------
echo ---pack classes.dex--
echo ----------------------
"$ANDROID_SDK_ROOT"/platform-tools/dx.bat --dex --output="$APP_ANDROID_ROOT"/bin/classes.dex "$APP_ANDROID_ROOT"/bin/classes "$APP_ANDROID_ROOT"/libs
echo ----------------------
echo ----pack resources----
echo ----------------------
"$ANDROID_SDK_ROOT"/platform-tools/aapt.exe package -f -M "$APP_ANDROID_ROOT"/AndroidManifest.xml -S "$APP_ANDROID_ROOT"/res -A "$APP_ANDROID_ROOT"/assets -I "$ANDROID_SDK_ROOT"/"$ANDROID_SDK_VERSION_PATH" -F "$APP_ANDROID_ROOT"/bin/resources.ap_
echo ----------------------
echo --pack original.apk--
echo ----------------------
"$ANDROID_SDK_ROOT"/tools/apkbuilder.bat "$APP_ANDROID_ROOT"/bin/original.apk -v -u -z "$APP_ANDROID_ROOT"/bin/resources.ap_ -f "$APP_ANDROID_ROOT"/bin/classes.dex -rf "$APP_ANDROID_ROOT"/src -rj "$APP_ANDROID_ROOT"/libs -nf "$APP_ANDROID_ROOT"/libs
echo ----------------------
echo ---signed apk---------
echo ----------------------
jarsigner -verbose -keystore "$RELEASE_ROOT"/zhanxia.keystore -storepass zancheng -signedjar "$APP_ANDROID_ROOT"/bin/signed.apk "$APP_ANDROID_ROOT"/bin/original.apk zhanxia.keystore
echo ----------------------
echo ---zip align apk---------
echo ----------------------
"$ANDROID_SDK_ROOT"/tools/zipalign -v 4 "$APP_ANDROID_ROOT"/bin/signed.apk "$APP_ANDROID_ROOT"/bin/"$CLIENT_NAME"
#cd $RELEASE_ROOT

rm "$APP_ANDROID_ROOT"/bin/classes.dex "$APP_ANDROID_ROOT"/bin/original.apk "$APP_ANDROID_ROOT"/bin/signed.apk "$APP_ANDROID_ROOT"/bin/resources.ap_