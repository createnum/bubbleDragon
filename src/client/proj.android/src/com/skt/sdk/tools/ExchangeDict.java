package com.skt.sdk.tools;

import java.util.*;

import org.dom4j.*;

public class ExchangeDict{
    public static final String EXCHANGE_CONFIG_XML_PATH = "res/exchange.xml";

    protected static ExchangeDict instance;
    protected List<StoreInfo> storeInfos = new ArrayList<StoreInfo>();

    protected ExchangeDict(){
    }

    public static ExchangeDict getInstance(){
        if(null == instance){
            Element rootNode = CommonFunc.loadDictFile(EXCHANGE_CONFIG_XML_PATH);
            ExchangeDict inst = new ExchangeDict();
            inst.loadInfos(rootNode);
            instance = inst;
        }
        return instance;
    }

    protected void loadInfos(Element rootNode){
        storeInfos.clear();
        try{
        	Iterator<Element> iter = rootNode.elementIterator();
            while(iter.hasNext()){
                Element storeNode = iter.next();
                StoreInfo storeInfo = new StoreInfo();
                @SuppressWarnings("unchecked")
				Iterator<Element> storeIter = storeNode.elementIterator();
                while(storeIter.hasNext()){
                    Element currentNode = storeIter.next();
                    ExchangeInfo info = new ExchangeInfo();
					info.targetID = Integer.parseInt(currentNode.attributeValue("targetID"));
                    info.gold = Integer.parseInt(currentNode.attributeValue("gold"));
                    info.presentGold = Integer.parseInt(currentNode.attributeValue("presentGold"));
                    info.rmb = Integer.parseInt(currentNode.attributeValue("rmb"));
					info.isvisible = currentNode.attributeValue("isVisibledInStore");
                    info.payCode = currentNode.attributeValue("payCode"); 
                    info.productid = currentNode.attributeValue("productid"); 
					info.payname = currentNode.attributeValue("payName");
					info.orderid = currentNode.attributeValue("orderid");
                    storeInfo.exchangeInfos.add(info);
                }
             this.storeInfos.add(storeInfo);
            }
        }catch(Exception e){
            e.printStackTrace();
            System.exit(-1);
        }
    }

    public StoreInfo getStoreInfo(int channelIndex){
        return this.storeInfos.get(channelIndex);
    }

    public static class ExchangeInfo{
        public int targetID;
		public int gold;
        public int presentGold;
        public int rmb;
        public String payCode;
        public String isvisible;
        public String productid;
		public String payname;
		public String orderid;
        public String getShowName(){
            return "" + (gold + presentGold) + "½ð±Ò";
        }
    }

    public static class StoreInfo{

        public ExchangeInfo getByTargetID(int targetID){
            for(ExchangeInfo info : this.exchangeInfos){
                if(targetID == info.targetID){
                    return info;
                }
            }
            return null;
        }

        public ExchangeInfo getByPayCode(String payCode){
            for(ExchangeInfo info : this.exchangeInfos){
                if(payCode.equals(info.payCode)){
                    return info;
                }
            }
            return null;
        }
        
        public ExchangeInfo getByGold(int Gold){
            for(ExchangeInfo info : this.exchangeInfos){
                if(Gold == info.gold){
                    return info;
                }
            }
            return null;
        }
        public ExchangeInfo getByrmb(int rmb){
            for(ExchangeInfo info : this.exchangeInfos){
                if(rmb == info.rmb){
                    return info;
                }
            }
            return null;
        }
        

        protected List<ExchangeInfo> exchangeInfos = new ArrayList<ExchangeInfo>();
    }
}
