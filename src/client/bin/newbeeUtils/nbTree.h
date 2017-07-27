#ifndef NEWBEE_TREE_H
#define NEWBEE_TREE_H

namespace NEWBEE_UTILS{
template<class T>
class NBTreeNode{
public:
	NBTreeNode* parent()
	{
		return m_parent; 
	}
	const NBTreeNode* parent() const				
	{
		return m_parent;
	}
	const NBTreeNode* firstChild() const {
		return m_firstChild;
	}
	NBTreeNode* firstChild(){
		return m_firstChild;
	}
	NBTreeNode* next(){
		return m_nextSibling;
	}
	int insertChild(NBTreeNode* newChild){ // �ݲ������
		newChild->m_nextSibling = m_firstChild;
		m_firstChild = newChild;
		newChild->m_parent = this;
		//newChild->m_level = this->m_level +1;
		return 0;
	}
	//int pickOffChild(NBTreeNode* newChild){ // ������ժ��ĳ�ڵ�
	//	newChild->m_nextSibling = m_firstChild->m_nextSibling;
	//	m_firstChild->m_nextSibling = newChild;
	//	return 0;
	//}
	NBTreeNode()
	{
		NBTreeNode_();
	}
	virtual ~NBTreeNode()
	{
	}
	void clear()//ɾ�������ӽڵ㡣
	{
		release();
		//NBTreeNode__();
	}
	const NBTreeNode* nextSibling() const{
		return m_nextSibling;
	}
	NBTreeNode* nextSibling(){
		return m_nextSibling; 
	}
	bool hasChild() const
	{
		return 0 != m_firstChild;
	}
	T* getData()
	{
		return &m_data;
	}
	virtual void setData(T data)
	{
		m_data = data;
	}
	virtual void setData_with_value(T data)
	{
		m_data = data;
	}
	virtual void setData_with_ref(T& data)
	{
		m_data = data;
	}
	void releaseTree(){ // �ͷ�������
		NBTreeNode *node = m_firstChild, *temp = 0;
		while ( node )
		{
			temp = node;
			node = node->nextSibling();
			temp->release();
		}
	}
	int getId(){
		return m_id;
	}
	void setId(int id){
		m_id = id;
	}
protected:
	virtual void release(){  // �ͷű��ڵ���麯��
		delete this;
		//return -1; // ���������п��ܻ�����ڴ�й¶�����ⲿ���ڴ������ƶ�����
	}
	void NBTreeNode_(){
		m_firstChild = 0;
		m_parent = 0;
		m_id = -1;
		//m_level = 1; // Ĭ���Ǹ��ڵ�һ��ġ�
	}
	//void NBTreeNode__(){
	//	NBTreeNode_();
	//}
	NBTreeNode*	m_parent;
	NBTreeNode*	m_firstChild; // ��һ���ӽڵ�
	NBTreeNode*	m_nextSibling; // ��һ���ֵܽڵ�
	//int m_level;// ���Ĳ㼶��root�ǵ�һ�㣬ֵ��1�����ĺ��ӣ�Ҷ�ӣ��ڵ��ǵڶ��㡣�Դ�����
	T m_data;
	int m_id;
private:
	NBTreeNode( const NBTreeNode& );				// not implemented.
	void operator=( const NBTreeNode& base );	// not allowed.

};
template <class T>
class HookTree // @@@@ ��湳��������ν���ӣ����Լ��������ڴ棬��Ҫ������ȥ��
{
public:
protected:
};
}
#endif //NEWBEE_TREE_H



