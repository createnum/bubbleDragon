package com.skt.sdk.tools;

import org.dom4j.*;

public class GlobalDict {
	public static final String VERSION_XML_PATH = "res/channel.xml";
	protected static GlobalDict instance ;
	public String version;
	public boolean uucunChannel;
	public boolean mobileChannel;
	public boolean telecomChannel;
	public boolean unicomChannel;
	public boolean andgameChannel;
	public boolean wostoreChannel;
	
	protected GlobalDict() {

	}

	public static GlobalDict getInstance() {
		if (null == instance) {
			Element rootNode = CommonFunc.loadDictFile(VERSION_XML_PATH);
			GlobalDict inst = new GlobalDict();
			inst.loadInfos(rootNode);
			instance = inst;
		}
		return instance;
	}

	protected void loadInfos(Element rootNode) {
		try {
			version = rootNode.elementTextTrim("version");
			uucunChannel = Boolean.parseBoolean(rootNode
					.elementTextTrim("uucunChannel"));
			mobileChannel = Boolean.parseBoolean(rootNode
					.elementTextTrim("mobileChannel"));
			telecomChannel = Boolean.parseBoolean(rootNode
					.elementTextTrim("telecomChannel"));
			unicomChannel = Boolean.parseBoolean(rootNode
					.elementTextTrim("unicomChannel"));
			andgameChannel = Boolean.parseBoolean(rootNode
					.elementTextTrim("andgameChannel"));
			wostoreChannel = Boolean.parseBoolean(rootNode
					.elementTextTrim("wostoreChannel"));
		} catch (Exception e) {
			e.printStackTrace();
			System.exit(-1);
		}
	}
}