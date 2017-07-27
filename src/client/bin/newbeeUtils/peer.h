/*
����ϵͳ�����еĹ�ϵ��������Ϸ�ĸ��
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
	//��Ϊֻ��Xһͷ�Ƕ�ֵ�ġ����ԣ�totalNumber�ͷ���X�˵ĸ����Ϳ����ˡ�
	long totalNumber()
	{
		return m_x.totalNumber();
	}
	//ɾ��X���е�һ����ע�⣬����Ĭ��relation���һ��ȫΪ0�����ͷ��Լ���
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
			m_log->ERRORLOG("��ͼ�Ƿ��ͷţ�����ֹ��");
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
			m_log->ERRORLOG("��ͼ�Ƿ��ͷţ�����ֹ��");
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
			m_log->ERRORLOG("�Ƿ����ʣ�");
			return 0;
		}
	}
	X* getNextPartner(Y* y)
	{
		if(y == m_y){
			return getNextPartner();
		}else{
			m_log->ERRORLOG("�Ƿ����ʣ�");
			return 0;
		}
	}
	Y* getPartner(X* x)
	{
		if(m_x.searchIfEqual(x))
		{
			return m_y;
		}else{
			m_log->ERRORLOG("�Ƿ����ʣ�");
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
	void allocationOne(X* node){//��ΪY�˾�ֻ��һ��������allocateһ����X.
		X** pNode = m_x.allocateLast();
		if (pNode) {
			*pNode = node;
		}else{
			m_log->ERRORLOG("����Relation X��(Chain)����������X�˿ռ����ˡ�");
		}
	}
	//���ı��һ��������ֵ��ͬʱ��Ҫ�ڶ���������ֵ��
	//void changePartner(X* constX, Y* newY){//���ı�Yָ�롣
	//	//�ȼ��X���ǲ��ǻ����������ӣ���ȷ���ǲ������·���relation.
	//	if (totalNumber()>1) {//�������1������ԭ����relation����ע�⣬��һ�������µ�relation����ΪnewY�п���Ҳ��һ��relation��
	//		release(constX);//������relation��ɾ���������ӵ��µ�relation�С�
	//		Peers<X, MAX_NUM_X, Y>* newY_de_Relation = newY->getRelation(this);
	//		if (newY_de_Relation) {//���y�Ѿ�����һ������ʾy���������Ĺ�ϵ����ô�����X�ӽ�ȥ�Ϳ����ˡ�
	//			newY_de_Relation->allocationOne(constX);
	//			constX->setRelation(newY_de_Relation);
	//		}else{	//һ��ȫ�µķ��䣬��һ�����䶯�����Ĳ���ָ�롱�������ֱ�Ϊ
	//			//��ֵ��ʹX,Y�˵�ָ��relation��ָ�룻��ֵ��ʹrelation��m_x�е�һ�m_y�ֱ�ָ��X,Y�˵㡣
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
	void changePartner(X* newX, X* oldX){//���ı�Xnodeָ�롣
		X** pNode = m_x.searchIfEqual(oldX);
		if (pNode) {
			oldX->cutoffRelation(this);
			*pNode = newX;
			newX->setRelation(this);
		}else{//���û���ҵ�����ʲôҲ������
			m_log->ERRORLOG("û���ҵ���Ӧ��Xֵ��");
		}
	}
protected:
	//����ȫ�ķ��ʣ�û�ж�Y���м��
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
			m_log->ERRORLOG("��ͼ�Ƿ��ͷţ�����ֹ��");
		}
	}
	//��ȫ���ͷ�
	void release()
	{
		if(isSide1Empty() && isSide2Empty())
		{
			World::_relationMM->releaseRelation(this);
		}
	}
	////�����ģ���Ϊһ��Ҫ����ȥ��������
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
			m_log->infolog("Side1","��");

		}
		if (m_side2) {
			m_log->infolog("Side2", m_side1->getProfile());
		}else{
			m_log->infolog("Side2","��");

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
	{//��ԭ����ֵ��ȷ�����Ķˣ�Ȼ�����㡣
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
{//֪��Tָ���Լ��ĳ�Ա�ĵ�ַ��Peer
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
	{//��ԭ����ֵ��ȷ�����Ķˣ�Ȼ�����㡣
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
	{//��ԭ����ֵ��ȷ�����Ķˣ�Ȼ������ֵ��
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
	{//ͬʱ�������е�ֵ
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
