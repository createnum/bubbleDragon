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
	int insertChild(NBTreeNode* newChild){ // 暂不会出错
		newChild->m_nextSibling = m_firstChild;
		m_firstChild = newChild;
		newChild->m_parent = this;
		//newChild->m_level = this->m_level +1;
		return 0;
	}
	//int pickOffChild(NBTreeNode* newChild){ // 从树上摘下某节点
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
	void clear()//删除所有子节点。
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
	void releaseTree(){ // 释放整棵树
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
	virtual void release(){  // 释放本节点的虚函数
		delete this;
		//return -1; // 调用这里有可能会产生内存泄露，由外部的内存分配机制而定。
	}
	void NBTreeNode_(){
		m_firstChild = 0;
		m_parent = 0;
		m_id = -1;
		//m_level = 1; // 默认是根节点一层的。
	}
	//void NBTreeNode__(){
	//	NBTreeNode_();
	//}
	NBTreeNode*	m_parent;
	NBTreeNode*	m_firstChild; // 第一个子节点
	NBTreeNode*	m_nextSibling; // 下一个兄弟节点
	//int m_level;// 树的层级，root是第一层，值是1，它的孩子（叶子）节点是第二层。以此类推
	T m_data;
	int m_id;
private:
	NBTreeNode( const NBTreeNode& );				// not implemented.
	void operator=( const NBTreeNode& base );	// not allowed.

};
template <class T>
class HookTree // @@@@ 多叉钩子树（所谓钩子，即自己不分配内存，需要外界挂上去。
{
public:
protected:
};
}
#endif //NEWBEE_TREE_H



