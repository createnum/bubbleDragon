/*
描述系统各类中的关系，而非游戏的概念。
*/
#ifndef PEERSS_H
#define PEERSS_H
#include "engineObject.h"
#include "target.h"
#include "troop.h"
#include "defense.h"
#include "message.h"
#include "event.h"
#include "plot.h"
#include "eventMilitary.h"
#include "intelligence.h"
template<class X, int MAX_NUM_X, class Y>
class Peers : public CObject
{
public:
	//
	//void releaseOne0X(X* chainXNode){
	//	X** pNode = m_x.searchIfEqual(chainXNode);
	//	if (pNode) {
	//		m_x.deleteOnewithCount(pNode);
	//		*pNode = 0;
	//	}
	//}
	//因为只有X一头是多值的。所以，totalNumber就返回X端的个数就可以了。
	long totalNumber()
	{
		return m_x.totalNumber();
	}
	//删除X其中的一个，注意，这里默认relation如果一端全为0，就释放自己。
	void release(X* chainXNode)
	{
		X** pNode = m_x.searchIfEqual(chainXNode);
		if(pNode)
		{
			cutoffSideX(pNode);
			if(0 == m_y){
				World::_relationMM->releaseRelation(this);
			}
		}else{
			m_log->ERRORLOG("试图非法释放，被阻止！");
		}
	}
	void release(Y* y)
	{
		if(y == m_y)
		{
			if(isXEmpty()){
				World::_relationMM->releaseOne(this);
			}
		}
		else{
			m_log->ERRORLOG("试图非法释放，被阻止！");
		}
	}
	void printSelf()
	{
		int count = 0;
		m_log->enableIndent();
		X** px = m_x.first();
		if (0 == px) {
			m_log->ERRORLOG("X is empty!");
		}else do{
			count++;
			m_log->infolog("%d",count);
			m_log->infolog("X", *px);
			px = m_x.next();
		}while(0 != px);
		if (m_y) {
			m_log->infolog("Y", m_y);
		}
		m_log->infolog(_T("\n"));
	}
	Peers()
	{
		m_y = 0;
	}
	virtual ~Peers(){}
	X* getFirstPartner(Y* y)
	{
		if(y == m_y){
			getFirstPartner();
		}else{
			m_log->ERRORLOG("非法访问！");
			return 0;
		}
	}
	X* getNextPartner(Y* y)
	{
		if(y == m_y){
			return getNextPartner();
		}else{
			m_log->ERRORLOG("非法访问！");
			return 0;
		}
	}
	Y* getPartner(X* x)
	{
		if(m_x.searchIfEqual(x))
		{
			return m_y;
		}else{
			m_log->ERRORLOG("非法访问！");
			return 0;
		}
	}
	void addPartner(X* x)
	{
		X** ppx = m_x.allocateLast();
		*ppx = x;
		//		x->setRelation(this);
	}
	void setPartner(Y* y){ m_y = y;}
	void allocationOne(X* node){//因为Y端就只有一个，所以allocate一定是X.
		X** pNode = m_x.allocateLast();
		if (pNode) {
			*pNode = node;
		}else{
			m_log->ERRORLOG("分配Relation X端(Chain)出错！可能是X端空间满了。");
		}
	}
	//欲改变第一个参数的值，同时需要第二个参数的值。
	//void changePartner(X* constX, Y* newY){//欲改变Y指针。
	//	//先检查X端是不是还有其他连接，以确定是不是重新分配relation.
	//	if (totalNumber()>1) {//如果大于1，则保留原来的relation。但注意，不一定分配新的relation。因为newY中可能也有一个relation！
	//		release(constX);//从已有relation中删除，把它加到新的relation中。
	//		Peers<X, MAX_NUM_X, Y>* newY_de_Relation = newY->getRelation(this);
	//		if (newY_de_Relation) {//如果y已经有了一个，表示y现有这样的关系，那么把这个X加进去就可以了。
	//			newY_de_Relation->allocationOne(constX);
	//			constX->setRelation(newY_de_Relation);
	//		}else{	//一个全新的分配，共一个分配动作，四步“指针”操作，分别为
	//			//赋值，使X,Y端点指向relation的指针；赋值，使relation的m_x中的一项，m_y分别指向X,Y端点。
	//			Peers<X, MAX_NUM_X, Y>* newRelation = World::_relationMM->allocateRelation(this);
	//			newY->setRelation(newRelation);
	//			constX->setRelation(newRelation);
	//			newRelation->allocationOne(constX);
	//			newRelation->setPartner(newY);
	//		}
	//	}else{
	//		m_y->cutoffRelation(this);
	//		setPartner(newY);
	//		newY->setRelation(this);
	//	}
	//}
	void changePartner(X* newX, X* oldX){//欲改变Xnode指针。
		X** pNode = m_x.searchIfEqual(oldX);
		if (pNode) {
			oldX->cutoffRelation(this);
			*pNode = newX;
			newX->setRelation(this);
		}else{//如果没有找到，就什么也不做。
			m_log->ERRORLOG("没有找到相应的X值！");
		}
	}
protected:
	//不安全的访问，没有对Y进行检查
	X* getFirstPartner()
	{
		X** ppX = m_x.first();
		if (ppX) {
			return *ppX;
		}else
			return 0;
	}
	X* getNextPartner()
	{
		X** ppX = m_x.next();
		if (ppX) {
			return *ppX;
		}else
			return 0;
	}
	inline void cutoffSideX(X** ppChainXNode)
	{
		m_x.deleteOnewithCount(ppChainXNode);
	}
	bool isXEmpty()
	{
		return m_x.isEmpty();
	}
	NEWBEE_UTILS::Chain<X*, MAX_NUM_X> m_x;
	Y* m_y;
};

template<class X, int MAX_NUM_X, class Y, int MAX_NUM_Y>
class Peerss : public CObject
{
public:

	//	void release(X* px){
	//		m_y[0]->cutoffRelation(this);
	//		World::_relationMM->releaseRelation(this);
	////		memset(this, 0, sizeof(Peer<X,MAX_NUM_X,Y,MAX_NUM_Y>));
	//	}
	void printSelf()
	{
		int count = 0;
		X** px = m_x.first();
		if (0 == px) {
			m_log->ERRORLOG("X_pointer = 0 !");
		}else do{
			count++;
			m_log->infolog("%d",count);
			m_log->infolog("X_ptr", *px);
			px = m_x.next();
		}while(0 != px);
		Y** py = m_y.first();
		count = 0;
		if (0 == py) {
			m_log->ERRORLOG("Y_pointer = 0 !");
		}else do{
			count++;
			m_log->infolog("%d",count);
			m_log->infolog("Y_ptr", *py);
			py = m_y.next();
		}while(0 != py);
	}

	Peerss(){
	}
	virtual ~Peerss(){};
	X* getFirstPeer(X* ){
		if (!m_x.isEmpty()) {
			return *(m_x.first());
		}else {
			return 0;
		}
	}
	Y* getFirstPartner(X*){
		if (!m_y.isEmpty()) {
			return *(m_y.first());
		}else {
			return 0;
		}
	}
	void setPartner(X* x){ m_x[0] = x;}
	void setPartner(Y* y){ m_y[0] = y;}

	X* getPartner(Y* ){ return getFirstPartner(Y*);}
	//	Y* getPartner(X* ){ return  getFirstPartner(X*);}
	//	void setPartner(X* x){ m_x[0] = x;}
	//	void setPartner(Y* y){ m_y[0] = y;}

protected:
	//	RELATION_KIND m_kind;
	NEWBEE_UTILS::Chain<X*, MAX_NUM_X> m_x;
	NEWBEE_UTILS::Chain<Y*, MAX_NUM_Y> m_y;
};

template<class T>
class Peer : public CObject
{
public:
	void release(T* side)
	{
		if(side == m_side1)
		{
			m_side1 = 0;
			if(isSide2Empty())
			{
				World::_relationMM->releaseRelation(this);
			}
		}else if(side == m_side2)
		{
			m_side2 =0;
			if(isSide1Empty())
			{
				World::_relationMM->releaseRelation(this);
			}
		}else{
			m_log->ERRORLOG("试图非法释放，被阻止！");
		}
	}
	//安全的释放
	void release()
	{
		if(isSide1Empty() && isSide2Empty())
		{
			World::_relationMM->releaseRelation(this);
		}
	}
	////最弱的，因为一定要传进去两个参数
	//void release(T* side1,T* side2)
	//{
	//	if((m_side1 == side1 && m_side2==side2) || (m_side1 == side2 && m_side2==side1))
	//	{
	//		m_side1 = 0;
	//		m_side2 = 0;
	//		World::_relationMM->releaseRelation(this);
	//	}
	//}
	bool isSide1Empty()
	{
		return 0 == m_side1;
	}
	bool isSide2Empty()
	{
		return 0 == m_side2;
	}
	void printSelf()
	{
		m_log->enableIndent();
		if (m_side1) {
			m_log->infolog("Side1", m_side1->getProfile());
		}else{
			m_log->infolog("Side1","无");

		}
		if (m_side2) {
			m_log->infolog("Side2", m_side1->getProfile());
		}else{
			m_log->infolog("Side2","无");

		}
		m_log->infolog(_T("\n"));
	}
	Peer(){
		m_side1 = 0;
		m_side2 = 0;
	};
	virtual ~Peer(){};
	T* getPeer(T* side){
		if(m_side1 == side)
		{
			return m_side2;
		} if (m_side2 == side)	{
			return m_side1;
		}else{
			m_log->warnlogs("%s no end can be found!", CLASS_NAME);
			return 0;
		}
	}
	void resetPeer(T* oldSideValue)
	{//由原来的值，确定是哪端，然后置零。
		if(oldSideValue == m_side1)
		{
			m_side1 = 0;
		}else if (oldSideValue == m_side2){
			m_side2 = 0;
		}else{
			m_log->warnlogs("%s no end can be found!", CLASS_NAME);
		}
	}
	void setPeers(T* side1,T* side2){
		m_side1 = side1;
		m_side2 = side2;
	}
protected:
	//	RELATION_KIND m_kind;
	T* m_side1;
	T* m_side2;
};

template<class T>
class PeerX : public Peer<T>
{//知道T指向自己的成员的地址的Peer
public:
	void release(){
		//m_side2->cutoffPeer(m_ppPeerX2);
		//m_side1->cutoffPeer(m_ppPeerX1);
		World::_relationMM->releaseRelation(this);
		m_side1 = 0;
		m_side2 = 0;
		m_ppPeerX1 = 0;
		m_ppPeerX2 = 0;
	}
	void printSelf()
	{
		m_log->enableIndent();
		if (m_side1) {
			m_log->infolog("m_side1", m_side1);
		}else{
			m_log->ERRORLOG("m_side1 = 0 !");
		}
		if (m_side2) {
			m_log->infolog("m_side2", m_side2);
		}else{
			m_log->ERRORLOG("m_side2 = 0 !");
		}
		m_log->infolog(_T("\n"));
	}
	PeerX()
	{
		m_side1 = 0;
		m_side2 = 0;
		m_ppPeerX1 = 0;
		m_ppPeerX2 = 0;
	};
	virtual ~PeerX(){};
	T* getPeer(T* side){
		if(m_side1 == side)
		{
			return m_side2;
		}else
		{
			return m_side1;
		}
	}
	void resetPeer(T* oldSideValue)
	{//由原来的值，确定是哪端，然后置零。
		if(oldSideValue == m_side1)
		{
			m_side1 = 0;
			m_ppPeerX1 = 0;
		}else if (oldSideValue == m_side2){
			m_side2 = 0;
			m_ppPeerX2 = 0;
		}else{
			m_log->warnLogs("%s no end can be found!", CLASS_NAME);
		}
	}
	void changePeer(T* oldSideValue, T* newSideValue, PeerX<T>** newppPeerX)
	{//由原来的值，确定是哪端，然后赋予新值。
		if(oldSideValue == m_side1)
		{
			m_side1 = newSideValue;
			m_ppPeerX1 = newppPeerX;
		}else{
			m_side1 = newSideValue;
			m_ppPeerX1 = newppPeerX;
		}
	}
	void setPeers(T* side1, PeerX<T>** ppPeerX1, T* side2, PeerX<T>** ppPeerX2)
	{//同时设置所有的值
		m_side1 = side1;
		m_ppPeerX1 = ppPeerX1;
		m_ppPeerX2 = ppPeerX2;
		m_side2 = side2;
	}
protected:
	PeerX<T>** m_ppPeerX1;
	PeerX<T>** m_ppPeerX2;
};
#endif // !defined(PEERSS_H)
